#include "low_cache.h"
#include "strategy.h"

// Compteur d'accès (lecture/écriture)
int nacces;

struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,size_t recordsz, unsigned nderef){
	Cache *c = malloc(sizeof(struct Cache));
	c->fp = fopen(fic, "r+");
	c->file = fic;
	c->nblocks = nblocks;
	c->nrecords = nrecords;
	c->recordsz = recordsz;
	c->blocksz = nrecords * recordsz;
	c->nderef = nderef;
	c->pstrategy = Strategy_Create(c);
	//c->instrument = malloc(sizeof(struct Cache_Instrument));
	Cache_Get_Instrument(c);
	//c->pfree = malloc(sizeof(Cache_Block_Header));
	c->headers = malloc(sizeof(struct Cache_Block_Header)*nblocks);

	nacces = 0;

	return c;	
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	if(Cache_Sync(pcache) == CACHE_KO)
		return CACHE_KO;
	if(fclose(pcache->fp) != 0)
		return CACHE_KO;

	Strategy_Close(pcache);
	
	free(pcache->instrument);
	free(pcache->headers);
	free(pcache);

	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){
	int i;

	// Réinitialisation du compteur d'accès (lecture/écriture)
	nacces = 0;

	// Incrémentation du nombre de synchronisations
	pcache->instrument.n_sync++;

	// On parcourt tous les blocs du cache
	for(i = 0 ; i < pcache->nblocks ; i++){
		// Si le bit M du bloc est à 1
		if((pcache->headers[i].flags & MODIF) > 0){
			// On se positionne au niveau du bloc dans le fichier
			if(fseek(pcache->fp, i * pcache->blocksz, SEEK_SET) != 0)
				return CACHE_KO;
			// On enregistre le bloc à sa position dans le fichier
			if(fputs(pcache->headers[i].data, fp) == EOF)
				return CACHE_KO;
			// Le bit M est passé à 0
			pcache->headers[i].flags &= ~MODIF;
		}
	}

	return CACHE_OK;
}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache){
	int i;
	for(i = 0 ; i < pcache->nblocks ; i++)
		pcache->headers[i].flags &= ~VALID;

	Strategy_Invalidate(pcache);	

	return CACHE_OK;
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord){
	char *buff = (char*)precord;
	int ib = irfile / pcache->nrecords; // Indice du bloc contenant l'enregistrement
	struct Cache_Block_Header *block = pcache->headers[ib]; // Bloc contenant l'enregistrement

	// Si le bloc contenant l'enregistrement est valide (V = 1)
	if(block->flags & VALID > 0){
		// Addresse de l'enregistrement dans le cache
		char *data = ADDR(pcache, irfile, block);

		// Ecriture de l'enregistrement dans le buffer
		if(snprintf(buff, pcache->recordsz, data) < 0)
			return CACHE_KO;

		// L'enregistrement est dans le cache, on augment n_hits
		pcache->instrument.n_hits++;

	// Si le bloc n'est pas valide, utilise la stratégie de remplacement
	}else{
		block = Strategy_Replace_Block(pcache);
	}

	// Incrémentation du nombre de lectures
	pcache->instrument.n_read++;
	
	// Gestion de la synchronisation
	if(++nacces == NSYNC)
		Cache_Sync(pcache);

	// Fonction "réflexe" lors de la lecture.
	Strategy_Read(pcache, block);

	return CACHE_OK;
}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord){
	char *buff = (char*)precord;
	int ib = irfile / pcache->nrecords; // Indice du bloc contenant l'enregistrement
	struct Cache_Block_Header *block = pcache->headers[ib]; // Bloc contenant l'enregistrement

	// Si le bloc contenant l'enregistrement est valide (V = 1)
	if(block->flags & VALID > 0){
		// Addresse de l'enregistrement dans le cache
		char *data = ADDR(pcache, irfile, block);

		// Ecriture de l'enregistrement dans le buffer
		if(snprintf(data, pcache->recordsz, buff) < 0)
			return CACHE_KO;
		
		// Un enregistrement a été modifié, le flag MODIF du bloc passe à 1
		block->flags |= MODIF;
		
		// L'enregistrement est dans le cache, on augment n_hits
		pcache->instrument.n_hits++;

	// Si le bloc n'est pas valide, utilise la stratégie de remplacement
	}else{
		block = Strategy_Replace_Block(pcache);
	}

	// Incrémentation du nombre d'écritures
	pcache->instrument.n_write++;

	// Gestion de la synchronisation
	if(++nacces == NSYNC)
		Cache_Sync(pcache);

	// Fonction "réflexe" lors de l'écriture.
	Strategy_Write(pcache, block);

	return CACHE_OK;
}

struct Cache_Instrument *Cache_Get_Instrument(struct Cache *pcache){
	struct Cache_Instrument instr = pcache->instrument;

	pcache->instrument.n_reads = 0;
	pcache->instrument.n_writes = 0;
	pcache->instrument.n_hits = 0;
	pcache->instrument.n_syncs = 0;
	pcache->instrument.n_deref = 0;

	return &instr;
}
