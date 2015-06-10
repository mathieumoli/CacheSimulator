#include "low_cache.h"
#include "strategy.h"

struct Cache_Block_Header *Cache_Contains_Valid_Record(struct Cache *pcache, int irfile);

// Compteur d'accès (lecture/écriture)
int nacces;

struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,size_t recordsz, unsigned nderef){
	int i;	
	struct Cache *c = malloc(sizeof(struct Cache));
	c->fp = fopen(fic, "wb+");
	c->file = fic;
	c->nblocks = nblocks;
	c->nrecords = nrecords;
	c->recordsz = recordsz;
	c->blocksz = nrecords * recordsz;
	c->nderef = nderef;
	c->pstrategy = Strategy_Create(c);
	Cache_Get_Instrument(c);
	c->headers = malloc(sizeof(struct Cache_Block_Header)*nblocks);
	
	for(i = 0 ; i < nblocks ; i++){
		c->headers[i].data = malloc(recordsz * nrecords);
		c->headers[i].ibcache = i;
		c->headers[i].flags &= 0x0;
	}

	c->pfree = &(c->headers[0]);

	nacces = 0;

	return c;	
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	int i;	
	if(Cache_Sync(pcache) == CACHE_KO)
		return CACHE_KO;
	if(fclose(pcache->fp) != 0)
		return CACHE_KO;

	Strategy_Close(pcache);
	
	for(i = 0 ; i < pcache->nblocks ; i++){
		free(pcache->headers[i].data);
	}
	
	//free(pcache->instrument);
	free(pcache->headers);
	free(pcache);

	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){
	int i;
									//printf("Cache_Sync\n");
	// Réinitialisation du compteur d'accès (lecture/écriture)
	nacces = 0;

	// Incrémentation du nombre de synchronisations
	pcache->instrument.n_syncs++;

	// On parcourt tous les blocs du cache
	for(i = 0 ; i < pcache->nblocks ; i++){
		// Si le bit M du bloc est à 1
		if((pcache->headers[i].flags & MODIF) > 0){
			// On se positionne au niveau du bloc dans le fichier
			if(fseek(pcache->fp, pcache->headers[i].ibfile * pcache->blocksz, SEEK_SET) != 0)
				return CACHE_KO;
			// On enregistre le bloc à sa position dans le fichier
			if(fputs(pcache->headers[i].data, pcache->fp) == EOF)
				return CACHE_KO;
			// Le bit M est passé à 0
			pcache->headers[i].flags &= ~MODIF;
		}
	}

	return CACHE_OK;
}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache){
	int i;								//printf("Cache_Invalidate\n");
	for(i = 0 ; i < pcache->nblocks ; i++)
		pcache->headers[i].flags &= ~VALID;

	pcache->pfree = &(pcache->headers[0]);

	Strategy_Invalidate(pcache);	

	return CACHE_OK;
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord){
	char *buff = (char*)precord;						//printf("Cache_Read\n");
	struct Cache_Block_Header *block;

	// Si l'enregistrement est présent dans un bloc valide du cache
	if((block = Cache_Contains_Valid_Record(pcache, irfile)) != NULL){

		// L'enregistrement est dans le cache, on augment n_hits
		pcache->instrument.n_hits++;

	// Si l'enregistrement n'est pas dans un bloc valide, utilise la stratégie de remplacement
	}else{
		// Bloc récupéré par la stratégie (peut être un bloc libre)
		block = Strategy_Replace_Block(pcache);

		// pfree a pu être modifié, on le réinitialise
		pcache->pfree = Get_Free_Block(pcache);

		// Si le bloc a été modifié, il est réécrit sur le disque
		if((block->flags & MODIF) > 0){
			if(fseek(pcache->fp, DADDR(pcache, block->ibfile), SEEK_SET) != 0)
				return CACHE_KO;

			if(fputs(block->data, pcache->fp) == EOF)
				return CACHE_KO;
		}

		// Ensuite on récupère le bloc depuis le fichier
		int ibrec = irfile / pcache->nrecords;
		if(fseek(pcache->fp, DADDR(pcache, ibrec), SEEK_SET) != 0)
				return CACHE_KO;

		fgets(block->data, pcache->blocksz, pcache->fp);

		// Les flags sont mis à jour (V = 1, M = 0)
		block->flags |= VALID;
		block->flags &= ~MODIF;

		// L'indice du bloc dans le fichier est mis à jour
		block->ibfile = ibrec;
	}

	// Ecriture de l'enregistrement dans le buffer
	if(snprintf(buff, pcache->recordsz, "%s", ADDR(pcache, irfile, block)) < 0)
		return CACHE_KO;

	// Incrémentation du nombre de lectures
	pcache->instrument.n_reads++;
	
	// Gestion de la synchronisation
	if(++nacces == NSYNC)
		Cache_Sync(pcache);

	// Fonction "réflexe" lors de la lecture.
	Strategy_Read(pcache, block);

	return CACHE_OK;
}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord){

	char *buff = (char*)precord;					//printf("Cache_Write\n");
	struct Cache_Block_Header *block;

	// Si l'enregistrement est présent dans un bloc valide du cache
	if((block = Cache_Contains_Valid_Record(pcache, irfile)) != NULL){

		// L'enregistrement est dans le cache, on augment n_hits
		pcache->instrument.n_hits++;

	// Si l'enregistrement n'est pas dans un bloc valide, utilise la stratégie de remplacement
	}else{
		// Bloc récupéré par la stratégie (peut être un bloc libre)
		block = Strategy_Replace_Block(pcache);

		// pfree a pu être modifié, on le réinitialise
		pcache->pfree = Get_Free_Block(pcache);

		// Si le bloc a été modifié, il est réécrit sur le disque
		if((block->flags & MODIF) > 0){
			if(fseek(pcache->fp, DADDR(pcache, block->ibfile), SEEK_SET) != 0)
				return CACHE_KO;

			if(fputs(block->data, pcache->fp) == EOF)
				return CACHE_KO;
		}

		// Ensuite on récupère le bloc depuis le fichier
		int ibrec = irfile / pcache->nrecords;
		if(fseek(pcache->fp, DADDR(pcache, ibrec), SEEK_SET) != 0)
				return CACHE_KO;

		fgets(block->data, pcache->blocksz, pcache->fp);

		// Les flags sont mis à jour (V = 1, M = 0)
		block->flags |= VALID;
		block->flags &= ~MODIF;

		// L'indice du bloc dans le fichier est mis à jour
		block->ibfile = ibrec;
	}

	// Ecriture de l'enregistrement depuis le buffer dans le cache
	if(snprintf(ADDR(pcache, irfile, block), pcache->recordsz, "%s", buff) < 0)
		return CACHE_KO;

	// Un enregistrement a été modifié, le flag MODIF du bloc passe à 1
	block->flags |= MODIF;

	// Incrémentation du nombre d'écritures
	pcache->instrument.n_writes++;
	
	// Gestion de la synchronisation
	if(++nacces == NSYNC)
		Cache_Sync(pcache);

	// Fonction "réflexe" lors de l'écriture.
	Strategy_Write(pcache, block);

	return CACHE_OK;
}

//! Résultat de l'instrumentation.
struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache){
	struct Cache_Instrument *instr = malloc(sizeof(struct Cache_Instrument));	printf("Cache_Get_Instrument\n");
	*instr = pcache->instrument;

	pcache->instrument.n_reads = 0;
	pcache->instrument.n_writes = 0;
	pcache->instrument.n_hits = 0;
	pcache->instrument.n_syncs = 0;
	pcache->instrument.n_deref = 0;

	return instr;
}

// Retourne le bloc VALIDE du cache qui contient l'enregistrement, si ce bloc n'existe pas, retourne NULL
struct Cache_Block_Header *Cache_Contains_Valid_Record(struct Cache *pcache, int irfile){
	int ib = irfile / pcache->nrecords;
	int i;
	for(i = 0 ; i < pcache->nblocks ; i++){
		if(pcache->headers[i].ibfile == ib){
			if((pcache->headers[i].flags & VALID) == 0)
				return NULL;
			return &(pcache->headers[i]);
		}
	}

	return NULL;
}



