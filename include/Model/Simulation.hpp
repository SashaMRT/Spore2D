/**
 * @file Simulation.hpp
 * @author Sasha Marie te Rehorst
 * @author Gael Guinaliu
 * @brief Interface de la simulation avec gestion stricte des bordures.
 * @version 1.1
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
    int grass;
    int sheep;
    int lambs;
    int wolves;
    int pups;
    int deadSheep;
    int deadWolves;
    int bornSheep;
    int bornWolves;
    float simulationTime;
};

enum class EntityType { Grass, Sheep, Wolf };

// Fonctions globales
void initEcosystem();
void setWorldBounds(float xMin, float xMax, float yMin, float yMax);
void ecosystemUpdate(float dt);
void ecosystemDraw(sf::RenderWindow& window);
EcosystemStats getEcosystemStats();
void spawnEntity(EntityType type, float x, float y);

#endif