[![Stories in Ready](https://badge.waffle.io/mathieumoli/CacheSimulator.png?label=ready&title=Ready)](https://waffle.io/mathieumoli/CacheSimulator)
# CacheSimulator
Un simulateur de cache pour le projet de fin d'année en Si3.

Info :

Un bloc : recordsz
irfile : indice (index) du bloc (enregistrement) visé dans le fichier
Cache:
Entete
	V : si = 0, il y a un bloc libre.
	M : si = 1, il y a eu une modification et il faut "envoyer" cette modif sur le fichier avant de le remettre a 0
	il contient les infos de nrecords
il y a nblocks entete dans le cache, soit nblocks*nrecords bloc.

Algo:

1. utilisateur fournit irfile
2. if V == 0 (si enregistrement dans un bloc valide du cache)
	if lecture
		cache -> buffer
	if ecriture
		buffer -> cache (M = 1)
3. 
if(irfile pas contenu dans le cache)
int i;
for(i = 0; i < nblocks; i++)
{
	memcpy(irfile) vers le bloc libre
	soit on passe en ecriture (buff vers le cache) (M = 1)
	Soit on passe en lecture (cache ver le buff)
	Si on trouve, boolean trouve = true;
}
if (!trouve)
On trouve un bloc, Si M = 1
	On enregistre
	puis V = 1 et M = 0

if(nbecriture >= NSYNC)
for(i = 0; i < nbblocks; i++)
	if(block.M == 1)
		transfert vers fichier;

Ce projet est developpé par : Etienne Laskar, [Pierre Massanès](https://github.com/PierreMassanes), [Mathieu Molinengo](https://github.com/mathieumoli), [Florian Muller](https://github.com/JuIngong).

Graphe de metric du projet : 

[![Throughput Graph](https://graphs.waffle.io/mathieumoli/CacheSimulator/throughput.svg)](https://waffle.io/mathieumoli/CacheSimulator/metrics) 

