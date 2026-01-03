/**
 * @file Wolf.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration du loup : prédateur rapide et agressif
 * @details Héritage d'Entity, vitesse élevée, IA de chasse avancée
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */


#ifndef WOLF_HPP
#define WOLF_HPP

#include "Entity.hpp"
#include <vector>

// Déclaration anticipée (pour éviter inclusion circulaire)
struct Sheep;

// Structure représentant un loup (hérite d’Entity)
struct Wolf : public Entity {
    float speed = 90;    // Vitesse de déplacement
    float hunger = 15;   // Taux de perte d’énergie
    float view = 200;    // Rayon de vision (détection des moutons)
    
    // Constructeur : position de départ + paramètres de base
    Wolf(sf::Vector2f p) : Entity(p, 120, sf::Color(139, 69, 19), 15.f) {}
    
    void update(float dt);               // Gestion énergie + apparence
    void hunt(std::vector<Sheep>& sheeps); // Recherche et poursuite
    void eat(std::vector<Sheep>& sheeps);  // Nourrissage
};

#endif
