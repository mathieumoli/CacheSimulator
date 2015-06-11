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
//printf("Cache_List_Create\n");
	struct Cache_List *liste = malloc(sizeof(struct Cache_List));

	liste->prev=NULL;
	liste->next=NULL;
	liste->pheader=NULL;

	return liste;
}

/*! Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list){
 //printf("Cache_List_Delete\n");

	Cache_List_Clear(list);
	free(list);
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(struct Cache_List *list, struct Cache_Block_Header *pbh){
//printf("Cache_List_Append\n");
	struct Cache_List *chainon=list;	

	if(Cache_List_Is_Empty(list)){	
		list->pheader=pbh;
	}else{
	//positionnement sur le dernier element
	
		while(chainon->next != NULL){
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
//printf("Cache_List_Prepend\n");
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
//printf("Cache_List_Remove_First\n");
	struct Cache_Block_Header *block = NULL;

	if(!Cache_List_Is_Empty(list)){
		block = Cache_List_Remove(list, list->pheader);
	}
	
	return block;
}
/*! Retrait du dernier élément */
struct Cache_Block_Header *Cache_List_Remove_Last(struct Cache_List *list){
//printf("Cache_List_Remove_Last");
	struct Cache_Block_Header *block = NULL;
	struct Cache_List *elem = list;

	if(!Cache_List_Is_Empty(list)){
		while(elem->next != NULL)elem = elem->next;
		block = Cache_List_Remove(list, elem->pheader);
	}

	return block;
}
/*! Retrait d'un élément quelconque */
struct Cache_Block_Header *Cache_List_Remove(struct Cache_List *list,struct Cache_Block_Header *pbh){
//printf("Cache_List_Remove\n");
	struct Cache_Block_Header *block = NULL;
	struct Cache_List *elem = list;
	struct Cache_List *del;

	if(!Cache_List_Is_Empty(list)){
		while(elem != NULL && elem->pheader != pbh)elem = elem->next;
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
					(elem->prev)->next = elem;
				}
			
				del->prev = NULL;
				del->next = NULL;
				del->pheader = NULL;
				Cache_List_Clear(del);
			}
	}

	return block;	
}

/*! Remise en l'état de liste vide */
void Cache_List_Clear(struct Cache_List *list){
//printf("Cache_List_Clear\n");
 	struct Cache_List *del;
	
	if(!Cache_List_Is_Empty(list)){
	 	while(list->next != NULL){
			del = list;
			list = list->next;			
	 		del->pheader = NULL;
	 		free(del);
		}

		list->pheader=NULL;
		list->next = NULL;
	}
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(struct Cache_List *list,struct Cache_Block_Header *pbh){
 //printf("Cache_List_Move_To_End\n");
 	Cache_List_Remove(list,pbh);
 	Cache_List_Append(list,pbh);

}
/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(struct Cache_List *list,struct Cache_Block_Header *pbh){
//printf("Cache_List_Move_To_Begin\n");
	if(list->pheader!=pbh){
	 	Cache_List_Remove(list,pbh);
		Cache_List_Prepend(list,pbh);
	}
}
