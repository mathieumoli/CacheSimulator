#include "strategy.h"
#include "low_cache.h"

struct pstrategy{
	int nderef;
	int cpt;
};

void initialize_flag_R(struct Cache *pcache);
int compute_RM(struct Cache_Block_Header *block);

//! Creation et initialisation de la stratégie (invoqué par la création de cache).
void *Strategy_Create(struct Cache *pcache){
	// Allocation de mémoire pour la structure utilisée par la stratégie
	struct pstrategy *strategy = malloc(sizeof(struct pstrategy));

	// Initialisation des champs de la structure
    	strategy->nderef = pcache->nderef;
    	strategy->cpt = 0;

   	return strategy;
}

//! Fermeture de la stratégie.
void Strategy_Close(struct Cache *pcache){
	// On libère la mémoire allouée à la structure
	free(pcache->pstrategy);
}

//! Fonction "réflexe" lors de l'invalidation du cache.
void Strategy_Invalidate(struct Cache *pcache){
	// Réinitialisation du compteur et du bit REFER de chaque bloc du cache
	initialize_flag_R(pcache);
}

//! Algorithme de remplacement de bloc.
struct Cache_Block_Header *Strategy_Replace_Block(struct Cache *pcache){
	int i, min_rm = 4;
	struct Cache_Block_Header *block_NUR = NULL;
	struct Cache_Block_Header *block;

	// On commence par chercher un bloc libre (VALID = 0)
	if ((block = Get_Free_Block(pcache)) != NULL)
		return block;

	// Si aucun bloc libre n'a été trouvé, on récupère le bloc NUR 
	// (recherche du bloc dont la pondération rm est minimale)
    	for (i = 0 ; i < pcache->nblocks ; i++){
		block = &pcache->headers[i];
	
		// Calcul de rm, rm = 2 * R + M
		int rm = compute_RM(block);

		// rm = 0, le bloc n'a pas été utilisé ni modifié depuis le dernier intervalle
		// Sinon on compare la pondération du bloc à la pondération minimale trouvée.
		if (rm == 0)
			return block;
		else if (rm < min_rm){
	    		min_rm = rm;
	    		block_NUR = block;
		}	
    	}

	return block_NUR; 
}

//! Fonction "réflexe" lors de la lecture.
void Strategy_Read(struct Cache *pcache, struct Cache_Block_Header *pb){
	struct pstrategy *strategy = (struct pstrategy*)(pcache)->pstrategy;

	// Si le compteur de déréférencements est égal à nderef, on réinitialise
	if((++strategy->cpt) == strategy->nderef)
		initialize_flag_R(pcache);
	// On met le flag REFER à 1 (car accès en lecture)
	pb->flags |= REFER;
}

//! Fonction "réflexe" lors de l'écriture.
void Strategy_Write(struct Cache *pcache, struct Cache_Block_Header *pb){
	struct pstrategy *strategy = (struct pstrategy*)(pcache)->pstrategy;

	// Si le compteur de déréférencements est égal à nderef, on réinitialise
	if((++strategy->cpt) == strategy->nderef)
		initialize_flag_R(pcache);
	// On met le flag REFER à 1 (car accès en écriture)
	pb->flags |= REFER;

}

//! Identification de la stratégie.
char *Strategy_Name(){
	return "NUR Strategy";
}

void initialize_flag_R(struct Cache *pcache){
    	struct pstrategy *strategy = (struct pstrategy*)(pcache)->pstrategy;
    	int i;

	// Si l'on souhaite déréférencer toutes les nderef fois (donc nderef != 0)
	if(strategy->nderef != 0){
		// On parcourt tous les blocs du cache pour remettre le flag REFER à 0.
		for (i = 0 ; i < pcache->nblocks ; i++)
			pcache->headers[i].flags &= 0x3;
		// Le compteur est réinitialisé
		strategy->cpt = 0;
		// On augmente le nombre de déréférencements effectués
	    	pcache->instrument.n_deref++;
	}
}

int compute_RM(struct Cache_Block_Header *block){
	int rm = 0;
	if((block->flags & REFER) > 0)
		rm += 2;
	if((block->flags & MODIF) > 0)
		rm += 1;
	
	return rm;
}
