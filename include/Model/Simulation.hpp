/**
 * @file Simulation.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Interface globale de gestion de l'écosystème.
 * @details Ce fichier agit comme le "contrôleur" du modèle. Il expose les fonctions
 * permettant de mettre à jour la logique du jeu, de dessiner les entités et de récupérer les stats.
 * Il ne contient pas de classe, mais des fonctions libres pour simplifier l'accès.
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <SFML/Graphics.hpp>

// -------------------------------------------------------------------------
// FONCTIONS DE CONTRÔLE
// -------------------------------------------------------------------------

/**
 * @brief Initialise ou Réinitialise l'écosystème.
 * @details Vide les listes existantes (Moutons, Loups, Herbe) et génère une nouvelle
 * population aléatoire. Appelé au démarrage et lors d'un Reset [R].
 */
void initEcosystem();

/**
 * @brief Définit les murs invisibles du monde.
 * @details Permet à la simulation de savoir quand un animal touche le bord de la fenêtre.
 * @param xMin Bordure gauche.
 * @param xMax Bordure droite.
 * @param yMin Bordure haute.
 * @param yMax Bordure basse.
 */
void setWorldBounds(float xMin, float xMax, float yMin, float yMax);

/**
 * @brief Met à jour toute la logique du jeu (Cerveau).
 * @details Gère l'IA, les déplacements, la faim, la reproduction et la mort.
 * @param dt Le temps écoulé depuis la dernière image (Delta Time) pour fluidifier les mouvements.
 */
void ecosystemUpdate(float dt);

/**
 * @brief Dessine toutes les entités vivantes sur la fenêtre.
 * @param window La fenêtre SFML cible.
 */
void ecosystemDraw(sf::RenderWindow& window);

// -------------------------------------------------------------------------
// STATISTIQUES
// -------------------------------------------------------------------------

/**
 * @struct EcosystemStats
 * @brief Structure conteneur pour transférer les données de jeu vers le HUD.
 * @details Regroupe toutes les variables importantes en un seul paquet.
 */
struct EcosystemStats {
    // Populations actuelles (Vivants)
    int grass;  ///< Nombre d'herbes.
    int sheep;  ///< Nombre de moutons.
    int wolves; ///< Nombre de loups.
    
    // Statistiques cumulées (Historique)
    int deadSheep;  ///< Total des moutons morts.
    int deadWolves; ///< Total des loups morts.
    int bornSheep;  ///< Total des naissances de moutons.
    int bornWolves; ///< Total des naissances de loups.
    
    // Temps
    float timeElapsed; ///< Temps total simulé en secondes.
};

/**
 * @brief Récupère l'état actuel des statistiques.
 * @return Une structure EcosystemStats remplie avec les dernières valeurs.
 */
EcosystemStats getEcosystemStats();

#endif