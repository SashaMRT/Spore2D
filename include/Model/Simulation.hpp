/**
 * @file Simulation.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Interface globale de gestion de l'écosystème.
 * @details Ce fichier agit comme le "contrôleur" du modèle.
 * @version 0.6
 * @date 2026-01-06
 */

#pragma once

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <SFML/Graphics.hpp>

// -------------------------------------------------------------------------
// DÉFINITIONS DES TYPES (C'est ici que manquait l'erreur !)
// -------------------------------------------------------------------------

/**
 * @enum EntityType
 * @brief Liste des types d'entités que le joueur peut faire apparaître.
 */
enum class EntityType {
    Grass,  ///< Herbe
    Sheep,  ///< Mouton
    Wolf    ///< Loup
};

// -------------------------------------------------------------------------
// FONCTIONS DE CONTRÔLE
// -------------------------------------------------------------------------

/**
 * @brief Initialise ou Réinitialise l'écosystème.
 */
void initEcosystem();

/**
 * @brief Définit les murs invisibles du monde.
 * @param xMin Bordure gauche.
 * @param xMax Bordure droite.
 * @param yMin Bordure haute.
 * @param yMax Bordure basse.
 */
void setWorldBounds(float xMin, float xMax, float yMin, float yMax);

/**
 * @brief Met à jour toute la logique du jeu (Cerveau).
 * @param dt Le temps écoulé depuis la dernière image (Delta Time).
 */
void ecosystemUpdate(float dt);

/**
 * @brief Dessine toutes les entités vivantes sur la fenêtre.
 * @param window La fenêtre SFML cible.
 */
void ecosystemDraw(sf::RenderWindow& window);

/**
 * @brief Fait apparaître une entité manuellement (Mode Dieu).
 * @param type Le type d'entité (Herbe, Mouton, Loup).
 * @param x Position X.
 * @param y Position Y.
 */
void spawnEntity(EntityType type, float x, float y);

// -------------------------------------------------------------------------
// STATISTIQUES
// -------------------------------------------------------------------------

/**
 * @struct EcosystemStats
 * @brief Structure conteneur pour transférer les données de jeu vers le HUD.
 */
struct EcosystemStats {
    int grass;
    int sheep;
    int wolves;
    
    int deadSheep;
    int deadWolves;
    int bornSheep;
    int bornWolves;
    
    float timeElapsed;
};

/**
 * @brief Récupère l'état actuel des statistiques.
 * @return Une structure EcosystemStats remplie avec les dernières valeurs.
 */
EcosystemStats getEcosystemStats();

#endif