#include <cache.h>

struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,size_t recordsz, unsigned nderef){
	Cache *c = malloc(sizeof(Cache));
	c->fp = fopen(fic, "r+");
	c->file = fic;
	c->nblocks = nblocks;
	c->nrecords = nrecords;
	c->recordsz = recordsz;
	c->blocksz = nrecords * recordsz;
	c->nderef = nderef;
	c->pstrategy = NULL;
	//c->instrument
	//c->pfree = malloc(sizeof(Cache_Block_Header));
	c->headers = malloc(sizeof(Cache_Block_Header)*nblocks);

	return c;	
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	if(Cache_Sync(pcache) == CACHE_KO)
		return CACHE_KO;
	if(fclose(pcache->fp) != 0)
		return CACHE_KO;
	free(c->headers);
	free(pcache);

	return CACHE_OK;
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){
	int i;
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

	return CACHE_OK;
}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord){

}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord){

}
