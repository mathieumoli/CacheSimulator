#include <cache.h>

struct Cache *Cache_Create(const char *fic, unsigned nblocks, unsigned nrecords,size_t recordsz, unsigned nderef){
	Cache c = malloc(sizeOf(Cache));
	c->*file = *fic;
	c->nblocks = nblocks;
	c->nrecords = nrecords;
	c->recordsz = recordsz;
	c->nderef = nderef;
}

//! Fermeture (destruction) du cache.
Cache_Error Cache_Close(struct Cache *pcache){
	free(pcache);
}

//! Synchronisation du cache.
Cache_Error Cache_Sync(struct Cache *pcache){

}

//! Invalidation du cache.
Cache_Error Cache_Invalidate(struct Cache *pcache){

}

//! Lecture  (à travers le cache).
Cache_Error Cache_Read(struct Cache *pcache, int irfile, void *precord){

}

//! Écriture (à travers le cache).
Cache_Error Cache_Write(struct Cache *pcache, int irfile, const void *precord){

}