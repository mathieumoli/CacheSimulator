#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "cache_list.h"
#include "cache.h"
#include "low_cache.h"

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

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){

	struct Cache_List *liste =malloc(sizeof(struct Cache_List));
	liste->prev=NULL;
	liste->next=NULL;
	liste->pheader=NULL;
	return liste;

}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list){
	struct Cache_List *elementActu = list;
	list->prev=NULL;
	struct Cache_List *precedent;
	// je me mets sur le dernier element
	for(elementActu; elementActu->next != NULL; elementActu = elementActu->next){
	}
	while(elementActu->prev != NULL){
		precedent=elementActu->prev;
		free(elementActu);
		elementActu=precedent;
	}
	free(elementActu);


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
	newChainon->prev=chainon;
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
	newChainon->next=chainon;
	}
}

/*! Retrait du premier élément */
struct Cache_Block_Header *Cache_List_Remove_First(struct Cache_List *list){
		/* data */
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
	elementActu->prev=NULL;
	struct Cache_Block_Header* temp=&(elementActu->pheader);
		Cache_List_Delete(elementActu);
		return temp;
	}
	return NULL;
}
/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
	if(!Cache_List_Is_Empty(list)){
		struct Cache_List *elementActu=list;
		struct Cache_List *avantDer;
		if(elementActu-> next == NULL && elementActu -> prev == NULL){
		list = Cache_List_Create();
		return elementActu->pheader;
	}
	for(elementActu; elementActu->next != NULL; elementActu = elementActu->next){
		avantDer = elementActu;
	}
	//si plusieurs elements
	avantDer->next=NULL;
	elementActu -> prev = NULL;
	elementActu->next=NULL;
struct Cache_Block_Header* temp=&(elementActu->pheader);
		Cache_List_Delete(elementActu);
		return temp;
	}
	return NULL;
}
/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,struct Cache_Block_Header *pbh){
	if(!Cache_List_Is_Empty(list)){
		struct Cache_List *elementActu=list;
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
		struct Cache_List *next=elementActu->next;

		previous->next=elementActu->next;
		next->prev=elementActu->prev;
		elementActu->next=NULL;
		elementActu->prev=NULL;
struct Cache_Block_Header* temp=&(elementActu->pheader);
		Cache_List_Delete(elementActu);
		return temp;
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

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,struct Cache_Block_Header *pbh){
 	
 	Cache_List_Remove(list,pbh);
 	Cache_List_Append(list,pbh);
 	
 	


}
/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,struct Cache_Block_Header *pbh){
	if(list->pheader!=pbh){
 	Cache_List_Remove(list,pbh);
	Cache_List_Prepend(list,pbh);
	}
}