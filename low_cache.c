#include "low_cache.h"


// Recherche d'un bloc libre.
struct Cache_Block_Header *Get_Free_Block(struct Cache *pcache){

	int i ;
	for(i = 0; i<pcache->nblocks; i++){
		if(pcache->headers[i].flags &= ~VALID)
			return pcache->headers[i];
	}

	return NULL;
}