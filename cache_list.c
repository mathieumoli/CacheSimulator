#include <stdbool.h>
#include "cache_list.h"

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){

	struct Cache_List *liste =(struct Cache_List *)malloc(sizeof(struct Cache_List));
	return liste;

}
/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list){

	free(list);

}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){

	//positionnement sur le dernier element
	struct Cache_list *chainon=list;
	while(chainon->next){
		chainon=chainon->next;
	}
	struct Cache_list *newChainon= Cache_List_Create();
	newChainon->phheader=pbh;
	chainon->next=newChainon;

}
/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){

	//dans le doute on essaye de remonter jusqu'au premier element de la liste
	struct Cache_list *chainon=list;
	while(chainon->prev){
		chainon=chainon->prev;
	}
	struct Cache_list *newChainon= Cache_List_Create();
	newChainon->phheader=pbh;
	chainon->prev=newChainon;
}

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list){
	

	struct Cache_List *elementActu=list;
	struct Cache_List *next;
	while(elementActu->prev){
		elementActu=elementActu->prev;
	}

	next=elementActu->next;
	next->prev=NULL;
	Cache_List_Remove(elementActu);
	return next->pheader;
}
/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
	struct Cache_list *elementActu=list;
	struct Cache_list *avantDer;
	while(elementActu->next){
		elementActu=elementActu->next;
	}

	avantDer=elementActu->prev;
	avantDer->next=NULL;
	Cache_List_Remove(elementActu);
	return avantDer->pheader;
}
/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,
                                             struct Cache_Block_Header *pbh){
	struct Cache_list *elementActu=list;
	struct Cache_list *blocRecherche;

	// si on arrive en fin de liste ou qu'on trouve pbh
	while(premier->prev || blocRecherche!=NULL)
	{
		if(premier->pheader==pbh){
			 blocRecherche=premier;
		}else
		premier=premier->next;
	}
	//si la liste ne contient pas pbh
	if(blocRecherche==NULL){
		return NULL;
	}else {
			Cache_List_Remove(elementActu);

		struct Cache_list *previous=blocRecherche->prev;
		struct Cache_list *next=blocRecherche->next;
		previous->next=next;
		Cache_List_Remove(blocRecherche);
		return pbh;
	}

}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
	while(list->next){
		Cache_List_Remove_Last();
	}

	list->pheader=NULL;
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
	if(!list->next && list->header==NULL){
		return true;
	}
	else{ 
		return false;
	}
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,
                            struct Cache_Block_Header *pbh);
/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,
                              struct Cache_Block_Header *pbh);