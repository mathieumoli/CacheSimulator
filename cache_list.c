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
	struct Cache_List *elementActu = list;
	list->prev=NULL;
	struct Cache_List *precedent;
	// je me mets sur le dernier element
	for(elementActu; elementActu->next != NULL; elementActu = elementActu->next){
	}
	
	for(elementActu; elementActu->prev != NULL; elementActu = elementActu->prev){
		precedent=elementActu->prev;
		free(elementActu);
	}


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