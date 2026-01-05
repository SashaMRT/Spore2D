/**
 * @file Simulation.hpp
 * @brief Interface publique de l'écosystème
 */

#pragma once

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <SFML/Graphics.hpp>

// Initialise l'écosystème
void initEcosystem();

// Définit les limites du monde (A appeler au début ou au resize)
void setWorldBounds(float xMin, float xMax, float yMin, float yMax);

// Met à jour toute la logique
void ecosystemUpdate(float dt);

// Dessine toutes les entités
void ecosystemDraw(sf::RenderWindow& window);

// Structure pour transporter les stats
struct EcosystemStats {
    int grass;
    int sheep;
    int wolves;
    
    // Stats cumulées
    int deadSheep;
    int deadWolves;
    int bornSheep;
    int bornWolves;
    
    // Temps
    float timeElapsed;
};

// Fonction pour récupérer les statistiques
EcosystemStats getEcosystemStats();

#endif