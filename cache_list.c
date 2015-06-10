#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "cache_list.h"
#include "cache.h"
#include "low_cache.h"
/*! Test de liste vide */
bool Cache_List_Is_Empty(struct Cache_List *list){
	if(list->next == NULL && list->pheader == NULL)
		return true;

	return false;
}

/*! Création d'une liste de blocs */
struct Cache_List *Cache_List_Create(){
printf("Cache_List_Create\n");
	struct Cache_List *liste = malloc(sizeof(struct Cache_List));

	liste->prev=NULL;
	liste->next=NULL;
	liste->pheader=NULL;

	return liste;
}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list){
printf("Cache_List_Delete\n");
	struct Cache_List *elem = list;
	struct Cache_List *del;
	while(elem != NULL){
		del = elem;
		elem = elem->next;
		del->pheader = NULL;
		free(del);
	}


	/*struct Cache_List *elementActu = list;
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
	free(elementActu);*/
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
printf("Cache_List_Append\n");
	struct Cache_List *chainon=list;	

	if(Cache_List_Is_Empty(list)){	
		list->pheader=pbh;
	}else{
	//positionnement sur le dernier element
	
		while(chainon != NULL){
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
printf("Cache_List_Prepend\n");
	struct Cache_List *chainon=list;

	if(Cache_List_Is_Empty(list)){
		list->pheader=pbh;
	}else{

		//dans le doute on essaye de remonter jusqu'au premier element de la liste
		
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
printf("Cache_List_Remove_First\n");
	struct Cache_Block_Header *block = NULL;

<<<<<<< HEAD
	if(!Cache_List_Is_Empty(list)){
		block = Cache_List_Remove(list, list->pheader);
=======
	struct Cache_List *elementActu=list;
	struct Cache_List *next;
	if(elementActu-> next == NULL && elementActu -> prev == NULL){
		struct Cache_Block_Header* temp=&(list->pheader);
		Cache_List_Clear(list);
		return temp;
	}
	next=elementActu->next;
	next->prev=NULL;
	elementActu -> next = NULL;
	elementActu->prev=NULL;
	struct Cache_Block_Header* temp=&(elementActu->pheader);
		elementActu->pheader=NULL;
		Cache_List_Delete(elementActu);
		return temp;
>>>>>>> 4cb23d99d8e25ad1ebabfd8d980c9fff85ce932a
	}
	
	return block;
}
/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
printf("Cache_List_Remove_Last");
	struct Cache_Block_Header *block = NULL;
	struct Cache_List *elem = list;

	if(!Cache_List_Is_Empty(list)){
<<<<<<< HEAD
		while(elem->next != NULL)elem = elem->next;
		block = Cache_List_Remove(list, elem->pheader);
	}

	return block;
}
/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,struct Cache_Block_Header *pbh){
printf("Cache_List_Remove\n");
	struct Cache_Block_Header *block = NULL;
	struct Cache_List *elem = list;
	struct Cache_List *del;

	if(!Cache_List_Is_Empty(list)){
		while(elem->pheader != pbh && elem != NULL)elem = elem->next;
		if(elem != NULL){
			block = elem->pheader;
			del = elem;
			if(elem->next == NULL && elem->prev != NULL){
				elem = elem->prev;
				elem->next = NULL;
			}else if(elem->next != NULL && elem->prev == NULL){
				elem = elem->next;
				elem->prev = NULL;
			}else if(elem->next != NULL && elem->prev != NULL){
				elem = elem->next;
				elem->prev = del->prev;
				elem->prev->next = elem;
			}
			
			del->prev = NULL;
			del->next = NULL;
			del->pheader = NULL;
			Cache_List_Delete(del);
		}
	}

	return block;	
=======
		struct Cache_List *elementActu=list;
		struct Cache_List *avantDer;
		if(elementActu-> next == NULL && elementActu -> prev == NULL){
		struct Cache_Block_Header* temp=&(list->pheader);
		Cache_List_Clear(list);
		return temp;
	}
	for(elementActu; elementActu->next != NULL; elementActu = elementActu->next){
		avantDer = elementActu;
	}
	//si plusieurs elements
	avantDer->next=NULL;
	elementActu -> prev = NULL;
	elementActu->next=NULL;
struct Cache_Block_Header* temp=&(elementActu->pheader);
		elementActu->pheader=NULL;
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
		struct Cache_Block_Header* temp=&(list->pheader);
		Cache_List_Clear(list);
		return temp;
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
		elementActu->pheader=NULL;
		Cache_List_Delete(elementActu);
		return temp;
	}	
	}
	return NULL;
>>>>>>> 4cb23d99d8e25ad1ebabfd8d980c9fff85ce932a
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
printf("Cache_List_Clear\n");
	if(!Cache_List_Is_Empty(list)){
		Cache_List_Delete(list->next);
		list->pheader=NULL;
		list->next = NULL;
	}
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,struct Cache_Block_Header *pbh){
 printf("Cache_List_Move_To_End\n");
 	Cache_List_Remove(list,pbh);
 	Cache_List_Append(list,pbh);

}
/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,struct Cache_Block_Header *pbh){
printf("Cache_List_Move_To_Begin\n");
	if(list->pheader!=pbh){
	 	Cache_List_Remove(list,pbh);
		Cache_List_Prepend(list,pbh);
	}
}
