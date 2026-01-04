/**
 * @file Entity.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Classe de base commune : position, énergie, affichage pour toutes les entités
 * @details Fournit les propriétés essentielles (pos, energy, shape), méthode dist() 
 * et IA de déplacement pour herbe/moutons/loups
 * @version 0.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 */

// Empêche d'inclure ce fichier deux fois.
#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Classe de base commune aux entités (Herbe, Mouton, Loup)
struct Entity {
    sf::Vector2f pos;           // Position sur la carte
    float energy, maxEnergy;    // Énergie actuelle ,Énergie maximale
    bool alive;                 // Indique si l’entité est en vie
    sf::CircleShape shape;      // Forme affichée à l’écran
    
     // Constructeur : initialise position, couleur, rayon et énergie
    Entity(sf::Vector2f p, float maxE, sf::Color col, float r=10.f) 
        : pos(p), maxEnergy(maxE), energy(maxE), alive(true) {
        shape.setRadius(r);
        shape.setFillColor(col);
        shape.setOrigin(sf::Vector2f(r/2, r/2)); // Centre le cercle
    }
    
    // Calcule la distance entre l’entité et une autre position
    float dist(const sf::Vector2f& o) const {
        float dx = pos.x - o.x;
        float dy = pos.y - o.y;
        return std::sqrt(dx*dx + dy*dy);
    }
};

#endif
