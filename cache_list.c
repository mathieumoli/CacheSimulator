#include "cache_list.h"
#include "cache.h"
#include "low_cache.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Cache_Block_Header Cache_Block_Header;
typedef struct Cache_List Cache_List;

/*! Test de liste vide */
bool Cache_List_Is_Empty(Cache_List *list)
{
    if (list->next == NULL)
        return true;
    else
        return false;
}

//Creation de la sentinelle de la liste
// cette sentinelle sera tout le temps vide (pheader=NULL)
struct Cache_List *Cache_List_Create()
{
 // reservation espace memoire pour le premier chainon/
 //sentinelle qui sera toujours vide
 Cache_List *liste = malloc(sizeof(struct Cache_List));

 liste->prev=NULL;
 liste->next=NULL;
 liste->pheader=NULL;

 return liste;
}

 /* Destruction d'une liste de blocs */
void Cache_List_Delete(struct Cache_List *list)
{
    //free tous les elements sauf le premier
    Cache_List_Clear(list);
    //free du premier
    free(list);
}

/*! Insertion d'un élément à la fin */
void Cache_List_Append(Cache_List *list, Cache_Block_Header *pbh)
{
    Cache_List *chainon=list;
    //deplacement en fin de liste
    while(chainon->next != NULL){
        chainon=chainon->next;
    }

    //creation du nouveau chainon et ajout des donnees
    //sur le chainon precedent et sur le nouveau chainon
    Cache_List *newChainon= Cache_List_Create();
    newChainon->pheader=pbh;
    chainon->next=newChainon;
    newChainon->prev=chainon;
    

}

/*! Insertion d'un élément au début*/
void Cache_List_Prepend(Cache_List *list, Cache_Block_Header *pbh)
{
    Cache_List *chainon=list;
    //creation du nouveau chainon
    Cache_List *newChainon= Cache_List_Create();

    //ajout des donnees
    //sur le chainon et sur la sentinelle
    newChainon->pheader=pbh;
    newChainon->prev=chainon;
    newChainon->next=chainon->next;
    chainon->next=newChainon;

}

/*! Retrait du premier élément */
Cache_Block_Header *Cache_List_Remove_First(Cache_List *list)
{

    Cache_Block_Header *block = NULL;
    Cache_List *first;

    //si vide return NULL
    if(!Cache_List_Is_Empty(list)){
        first = list->next;
        list->next=first->next;
        //s'il n'y a pas qu'un seul element
        // on modifie le suivant
        if(first->next!=NULL){
            (first->next)->prev=list;
        }
        
        // attribution du pheader a retourner
        block = first->pheader;
        free(first);
    }
    
    return block;

}

/*! Retrait du dernier élément */
Cache_Block_Header *Cache_List_Remove_Last(Cache_List *list)
{

    Cache_Block_Header *block = NULL;
    Cache_List *elem = list;
    Cache_List *prec;

    //si vide return NULL
    if(!Cache_List_Is_Empty(list)){
        //deplacement sur le dernier element
        while(elem->next != NULL)elem = elem->next;
        prec=elem->prev;
        prec->next=NULL;
        block = elem->pheader;

        free(elem);
    }

    return block;

}

/*! Retrait d'un élément quelconque */
Cache_Block_Header *Cache_List_Remove(Cache_List *list, Cache_Block_Header *pbh)
{

    Cache_Block_Header * block;
    Cache_List* elem= list;

    //si vide return NULL
    if(!Cache_List_Is_Empty(list)){
        while (elem!= NULL && elem->pheader != pbh) elem= elem->next;
        //si pas contenu return NULL
        if(elem!=NULL)
        {
            //il est a la fin appel de Cache_List_Remove_Last
            if (elem->next == NULL)
            {
                block=Cache_List_Remove_Last(list);
            }
            else
            {
                //modification des donnees
                //sur le chainon precedent et 
                //sur le chainon suivant
                block = elem->pheader;
                (elem->next)->prev = elem->prev;
                (elem->prev)->next = elem->next;
                free(elem);
            }
        }
    }

    return block;     
}


/*! Remise en l'état de liste vide */
void Cache_List_Clear(Cache_List *list)
{
    Cache_List *del;
    //si la liste est vide elle est deja clear
    if(!Cache_List_Is_Empty(list)){
        while(list->next != NULL){
        //on supprime les elements un a un
            del = list->next;
            list->next=del->next;          
            del->pheader = NULL;
            free(del);
        }
    }
}

/*! Transférer un élément à la fin */
void Cache_List_Move_To_End(Cache_List *list,Cache_Block_Header *pbh)
{

    if(!Cache_List_Is_Empty(list)){
        struct Cache_List *elem = list;
        while(elem->next != NULL && elem->pheader != pbh){elem=elem->next;}
    // si l'element est deja a la fin on ne fait rien
        if(elem->next!=NULL){
    //pour deplacer on l'enleve et on le met a la fin
            Cache_List_Remove(list,pbh);
            Cache_List_Append(list,pbh);
        }
    }else{
    //si vide juste a ajouter
        Cache_List_Append(list,pbh);  
    }
}

/*! Transférer un élément  au début */
void Cache_List_Move_To_Begin(Cache_List *list,Cache_Block_Header *pbh)
{
    // si la liste n'est pas vide 
    if(!Cache_List_Is_Empty(list)){
        //si pbh est deja en premiere place
        // on ne fait rien
        if((list->next)->pheader!=pbh){
            Cache_List_Remove(list,pbh);
            Cache_List_Prepend(list,pbh);
        }
    }else{
        //a la fin ou au debut peu importe car vide
        Cache_List_Prepend(list,pbh);

    }
}
