# Compression Lempel-Ziv-Welch (LZW)

## Département d’Informatique
**2ème année de Licence**  
**Projet de Structure de Données II**  
**Auteurs**: MOHAMED YOUNIS Ahmed & MAHAMAT Ouagal  
**Date**: Décembre 2021  
**Enseignants**: HETROY WHEELER Franck, HAAS Antoine, MORGENTHALER Sébastien

---

## Sommaire
- [Introduction](#introduction)
- [Gestion du dictionnaire](#gestion-du-dictionnaire)
- [Mise en application](#mise-en-application)
- [Programmation modulaire](#programmation-modulaire)
- [Conclusion](#conclusion)
- [Annexes](#annexes)

---

## Introduction
La compression Lempel-Ziv-Welch (LZW) est une méthode de compression de données non destructive. Ce projet vise à appliquer l'algorithme LZW, qui évite les répétitions dans les chaînes de caractères pour économiser de l'espace. L'algorithme utilise un dictionnaire construit dynamiquement pour la compression et la décompression.

## Gestion du dictionnaire
Nous avons implémenté le dictionnaire sous trois formes différentes : 
- Liste chaînée
- Trie informatique
- Table de hachage

Avant de créer le dictionnaire, nous récupérons la taille du fichier à compresser pour allouer le dictionnaire de manière appropriée, en ne considérant que les caractères Extended ASCII (0 à 255).

## Mise en application
La mise en application des algorithmes de compression et décompression est disponible dans le fichier `main.c`. Ce fichier prend en paramètre trois fichiers :
1. Le fichier texte à compresser
2. Un fichier au format `.lzw` pour le code binaire de la compression
3. Un fichier texte pour le texte décompressé

Nous avons également effectué des tests de performance sur les différentes structures pour déterminer leur efficacité.

### Résultats des tests
- Liste: 0.000044 ms
- Trie: 0.000009 ms
- Hashmap: 0.000177 ms

## Programmation modulaire
Le projet est divisé en plusieurs modules, chacun ayant son propre fichier d'en-tête et fichier source :
- **Liste**: gestion des listes chaînées
- **Trie**: gestion des tries
- **Hashmap**: gestion des tables de hachage
- **Main**: application de la compression et décompression

La compilation se fait à l'aide d'un Makefile. Utilisez la commande `make` pour compiler et `make clean` pour nettoyer les fichiers générés.

## Conclusion
Ce projet a permis d'approfondir notre compréhension des algorithmes de compression et de gestion des structures de données. La répartition des tâches a facilité notre collaboration et l'efficacité de l'algorithme LZW a été confirmée.

## Annexes
- Figure 1: Temps d'insertion
- Figure 2: Temps de recherche

