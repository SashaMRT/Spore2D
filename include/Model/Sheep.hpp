/**
 * @file Sheep.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration du mouton : proie pacifique et brouteuse
 * @details Héritage d'Entity, vitesse modérée, IA de pâturage et fuite
 * @version 0.1
 * @date 2026-01-03
 * 
 * @copyright Copyright (c) 2026
 */


#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"
#include "Grass.hpp"
#include <vector>

// Déclaration anticipée du loup (évite inclusion circulaire)
struct Wolf;

// Représente un mouton, dérivé d’Entity
struct Sheep : public Entity {
    float speed = 45;                     // Vitesse de déplacement
    float hunger = 10;                    // Consommation d’énergie
    float view = 100;                     // Rayon de vision
    sf::Vector2f wanderDirection = {1.f, 0.f}; // Direction de base (errance)

    // Constructeur : position + paramètres par défaut
    Sheep(sf::Vector2f p) : Entity(p, 100, sf::Color::White, 12.f) {}

    void update(float dt);                // Gère énergie et couleur
    void eat(std::vector<Grass>& grass);  // Mange l’herbe proche
};

#endif
