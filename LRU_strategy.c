/*
*
* Strategie Least Recently Used (LRU)
*
*/

#include "cache_list.h"
#include "strategy.h"
#include "low_cache.h"

#define LRU(pcache) ((struct Cache_List *)((pcache)->pstrategy))

//! Creation et initialisation de la stratégie (invoqué par la création de cache).
void *Strategy_Create(struct Cache *pcache){
	return Cache_List_Create();
}

//! Fermeture de la stratégie.
void Strategy_Close(struct Cache *pcache){
	Cache_Liste_Delete(LRU(pcache));
}

//! Fonction "réflexe" lors de l'invalidation du cache.
void Strategy_Invalidate(struct Cache *pcache){
	Cache_List_Clear(LRU(pcache));
}

//! Algorithme de remplacement de bloc.
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache){
	struct Cache_List *list = LRU(pcache);
    struct Cache_Block_Header *blocInvalide;
²
     /* je cherche un bloc invalide */
    if ((blocinvalide = Get_Free_Block(pcache)) != NULL)
    {
        /* on ajoute le bloc invalide*/
        Cache_List_Append(list, blocInvalide);
        return blocInvalide;
    }

    /* sinon j'enleve le premier*/
    blocInvalide = Cache_List_Remove_Last(list);
    Cache_List_Append(list, blocInvalide);

    return blocInvalide; 
}

//! Fonction "réflexe" lors de la lecture.
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pb){
	Cache_Liste_Move_To_End(LRU(pcache), pb);
}

//! Fonction "réflexe" lors de l'écriture.
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pb){
	Cache_Liste_Move_To_End(LRU(pcache), pb);
}

//! Identification de la stratégie.
char *Strategy_Name(){
	return "LRU Strategy";
}