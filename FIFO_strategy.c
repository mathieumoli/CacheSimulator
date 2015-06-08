#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"

int pointeurVersListe;
int pointeurVersCache;


//! Creation et initialisation de la stratégie (invoqué par la création de cache).
void *Strategy_Create(struct Cache *pcache){

	pointeurVersCache= pcache;
	pointeurVersListe= Cache_List_Create();	

}

//! Fermeture de la stratégie.
void Strategy_Close(struct Cache *pcache){

	Cache_Liste_Delete(pointeurversListe);

}

//! Fonction "réflexe" lors de l'invalidation du cache.
void Strategy_Invalidate(struct Cache *pcache){

}

//! Algorithme de remplacement de bloc.
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache){
return null;
}

//! Fonction "réflexe" lors de la lecture.
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pb){

}

//! Fonction "réflexe" lors de l'écriture.
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pb){

}

//! Identification de la stratégie.
char *Strategy_Name(){
	return "FIFO Strategy";
}