#include "strategy.h"
#include "low_cache.h"
#include "cache_list.h"

#define FIFO(pcache) ((struct Cache_List *)((pcache)->pstrategy))

//! Creation et initialisation de la stratégie (invoqué par la création de cache).
void *Strategy_Create(struct Cache *pcache){

	return Cache_List_Create();	

}

//! Fermeture de la stratégie.
void Strategy_Close(struct Cache *pcache){

	Cache_List_Delete(FIFO(pcache));

}

//! Fonction "réflexe" lors de l'invalidation du cache.
void Strategy_Invalidate(struct Cache *pcache){

	Cache_List_Clear(FIFO(pcache));

}

//! Algorithme de remplacement de bloc.
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache){	
    struct Cache_List *list = FIFO(pcache);
    struct Cache_Block_Header *blocInvalide;

     /* je cherche un bloc invalide */
    if ((blocInvalide = Get_Free_Block(pcache)) != NULL)
    {
        /* on ajoute le bloc invalide*/
        Cache_List_Append(list, blocInvalide);
        return blocInvalide;
    }

    /* sinon j'enleve le premier*/
    blocInvalide = Cache_List_Remove_First(list);
    Cache_List_Append(list, blocInvalide);

    return blocInvalide;    
}

//! Fonction "réflexe" lors de la lecture.
// Inutile pour le FIFO il se fait en fonction de la strategie FIFO
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pb){

}

//! Fonction "réflexe" lors de l'écriture.
// Inutile pour le FIFO il se fait en fonction de la strategie FIFO
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pb){

}

//! Identification de la stratégie.
char *Strategy_Name(){
	return "FIFO Strategy";
}