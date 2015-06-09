#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "cache_list.h"
#include "cache.h"
#include "low_cache.h"

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){

	struct Cache_List *liste =(struct Cache_List *)malloc(sizeof(struct Cache_List));
	liste->prev=NULL;
	liste->next=NULL;
	liste->pheader=NULL;
	return liste;

}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list){

	free(list);
	printf("coucou Delete");


}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
	if(Cache_List_Is_Empty(list)){
		list->pheader=pbh;
	}else{
	//positionnement sur le dernier element
	struct Cache_List *chainon=list;
	while(chainon->next!=NULL){
		chainon=chainon->next;
	}
	struct Cache_List *newChainon= Cache_List_Create();
	newChainon->pheader=pbh;
	chainon->next=newChainon;
	}

}
/*! Insertion d'un élément au début*/
void Cache_List_Prepend(struct Cache_List *list, struct Cache_Block_Header *pbh){
	if(Cache_List_Is_Empty(list)){
		list->pheader=pbh;
	}else{

	//dans le doute on essaye de remonter jusqu'au premier element de la liste
	struct Cache_List *chainon=list;
	while(chainon->prev){
		chainon=chainon->prev;
	}
	struct Cache_List *newChainon= Cache_List_Create();
	newChainon->pheader=pbh;
	chainon->prev=newChainon;
	}
}

/*! Retrait du premier élément */
/*struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list){
	if(!Cache_List_Is_Empty(list)){

	struct Cache_List *elementActu=list;
	struct Cache_List *next;
	while(elementActu->prev){
		elementActu=elementActu->prev;
	}
	if(elementActu->next){
	next=elementActu->next;
	next->prev=NULL;
	return next->pheader;
	}else{
	Cache_List_Clear(list);
	return NULL;
}
}
	return NULL;
}
*/

struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list){
	if(!Cache_List_Is_Empty(list)){

	struct Cache_List *elementActu=list;
	struct Cache_List *next;
	if(elementActu-> next == NULL && elementActu -> prev == NULL){
		list = Cache_List_Create();
		return elementActu->pheader;
	}
	next=elementActu->next;
	next->prev=NULL;
	elementActu -> next = NULL;
	return elementActu->pheader;
	}
	return NULL;
}
/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
	struct Cache_List *elementActu=list;
	struct Cache_List *avantDer;
	if(!Cache_List_Is_Empty(list)){
		if(elementActu-> next == NULL && elementActu -> prev == NULL){
		list = Cache_List_Create();
		return elementActu->pheader;
	}
	for(elementActu; elementActu->next != NULL; elementActu = elementActu->next){
	}
	//si plusieurs elements
	avantDer=elementActu->prev;
	avantDer->next=NULL;
	elementActu -> prev = NULL;
	return elementActu->pheader;
	}
	return NULL;
}
/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,struct Cache_Block_Header *pbh){
	struct Cache_List *elementActu=list;
	if(!Cache_List_Is_Empty(list)){
		if(elementActu-> next == NULL && elementActu -> prev == NULL){
		list = Cache_List_Create();
		return elementActu->pheader;
	}
	// si on arrive en fin de liste ou qu'on trouve pbh
	
	for(elementActu; elementActu->next != NULL || elementActu->pheader != pbh; elementActu = elementActu->next){

	}
	//si la liste ne contient pas pbh
	if(elementActu==NULL){
		return NULL;
	}else {

		struct Cache_List *previous=elementActu->prev;
		previous->next=elementActu->next;
		elementActu->next=NULL;
		elementActu->prev=NULL;
		return elementActu->pheader;
	}
	}
	return NULL;
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
	if(!Cache_List_Is_Empty(list)){
	while(list->next!=NULL){
		Cache_List_Remove_Last(list);
	}

	list->pheader=NULL;
	}
}

/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
	if(list->next==NULL){
		if(list->pheader==NULL){
		return true;
	}
		return false;
	}
	else{ 
		return false;
	}
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,struct Cache_Block_Header *pbh){
 	if(list->pheader!=pbh){
 	Cache_List_Remove(list,pbh);
 	Cache_List_Append(list,pbh);
 	}


}
/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,struct Cache_Block_Header *pbh){
	if(list->pheader!=pbh){
	Cache_List_Remove(list,pbh);
	Cache_List_Prepend(list,pbh);
	}
}
