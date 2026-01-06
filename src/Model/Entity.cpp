/**
 * @file Entity.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation optimisée de la classe Entity.
 * @version 0.9
 * @date 2026-01-06
 */

#include "../../include/Model/Entity.hpp"
#include <cmath>

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Entity::Entity(sf::Vector2f p, float maxE, sf::Color col, float r) 
    : pos(p), maxEnergy(maxE), energy(maxE), alive(true), radius(r)
{
    shape.setRadius(r);
    shape.setFillColor(col);
    
    // CORRECTION SFML 3 : Utilisation d'accolades pour le Vector2f
    shape.setOrigin({r, r}); 
}

// -------------------------------------------------------------------------
// MÉTHODES
// -------------------------------------------------------------------------

float Entity::dist(const sf::Vector2f& o) const {
    float dx = pos.x - o.x;
    float dy = pos.y - o.y;
    return std::sqrt(dx*dx + dy*dy);
}

void Entity::checkBounds(float xMin, float xMax, float yMin, float yMax) {
    if (!alive) return;

    float padding = radius + 2.0f; 
    float killThreshold = 50.0f;

    // GAUCHE / DROITE
    if (pos.x < xMin + padding) {
        if (pos.x < xMin - killThreshold) alive = false; 
        else pos.x = xMin + padding;
    }
    else if (pos.x > xMax - padding) {
        if (pos.x > xMax + killThreshold) alive = false;
        else pos.x = xMax - padding;
    }

    // HAUT / BAS
    if (pos.y < yMin + padding) {
        if (pos.y < yMin - killThreshold) alive = false;
        else pos.y = yMin + padding;
    }
    else if (pos.y > yMax - padding) {
        if (pos.y > yMax + killThreshold) alive = false;
        else pos.y = yMax - padding;
    }
}

void Entity::resolveCollision(Entity& other) {
    if (!alive || !other.alive) return;

    float dx = pos.x - other.pos.x;
    float dy = pos.y - other.pos.y;
    float distSq = dx*dx + dy*dy; // Distance au carré (très rapide)

    float minDist = radius + other.radius;
    float minDistSq = minDist * minDist;

    // OPTIMISATION MAJEURE :
    // On évite la racine carrée (sqrt) tant qu'on n'est pas sûr qu'il y a collision.
    // sqrt est une opération coûteuse pour le processeur.
    if (distSq < minDistSq && distSq > 0.0001f) {
        
        // Maintenant qu'on sait qu'on se touche, on fait le calcul précis
        float d = std::sqrt(distSq);
        
        // Vecteur direction normalisé
        sf::Vector2f push = {dx / d, dy / d}; 
        float overlap = minDist - d;
        
        // Réponse physique : on s'écarte
        sf::Vector2f separation = push * (overlap * 0.5f);
        pos += separation;
        other.pos -= separation;
    }
}