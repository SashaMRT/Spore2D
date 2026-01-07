/**
 * @file Simulation.hpp
 * @author Sasha Marie te Rehorst
 * @author Gael Guinaliu
 * @brief Interface de la simulation marine.
 * @version 2.0
 */

#pragma once

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <SFML/Graphics.hpp>

/**
 * @struct EcosystemStats
 * @brief Statistiques pour le HUD.
 */
struct EcosystemStats {
    int plants;
    int preyTotal;
    int bacteria;
    int fish;
    int sharks;
    int deadPrey;
    int deadSharks;
    int bornPrey;
    int bornSharks;
    float simulationTime;
};

/**
 * @enum EntityType
 * @brief Types d'entités créables.
 */
enum class EntityType { Plant, Bacteria, Fish, Shark };

// --- PROTOTYPES DES FONCTIONS GLOBALES ---
// Ces fonctions doivent être déclarées ici pour être visibles par les autres fichiers .cpp
void initEcosystem();
void setWorldBounds(float xMin, float xMax, float yMin, float yMax);
void ecosystemUpdate(float dt);
void ecosystemDraw(sf::RenderWindow& window);
EcosystemStats getEcosystemStats();
void spawnEntity(EntityType type, float x, float y);

#endif