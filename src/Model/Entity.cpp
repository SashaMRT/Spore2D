/**
 * @file Entity.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe de base Entity.
 * @version 0.8
 * @date 2026-01-06
 */

#include "../../include/Model/Entity.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Entity::Entity(sf::Vector2f p, float maxE, sf::Color col, float r) 
    : pos(p), maxEnergy(maxE), energy(maxE), alive(true), radius(r)
{
    shape.setRadius(r);
    shape.setFillColor(col);
    shape.setOrigin(sf::Vector2f(r, r)); 
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

    float padding = radius + 2.0f; // Marge de sécurité
    float killThreshold = 50.0f;   // Tolérance mur qui écrase

    // GAUCHE
    if (pos.x < xMin + padding) {
        if (pos.x < xMin - killThreshold) alive = false; 
        else pos.x = xMin + padding;
    }
    // DROITE
    else if (pos.x > xMax - padding) {
        if (pos.x > xMax + killThreshold) alive = false;
        else pos.x = xMax - padding;
    }

    // HAUT
    if (pos.y < yMin + padding) {
        if (pos.y < yMin - killThreshold) alive = false;
        else pos.y = yMin + padding;
    }
    // BAS
    else if (pos.y > yMax - padding) {
        if (pos.y > yMax + killThreshold) alive = false;
        else pos.y = yMax - padding;
    }
}

void Entity::resolveCollision(Entity& other) {
    if (!alive || !other.alive) return;

    // La distance minimale pour ne pas se toucher est la somme des rayons
    float minDist = radius + other.radius;
    float d = dist(other.pos);

    // Si on est trop proches (mais pas superposés parfaitement pour éviter division par zéro)
    if (d < minDist && d > 0.01f) {
        sf::Vector2f push = (pos - other.pos) / d; // Vecteur direction
        float overlap = minDist - d;               // De combien on se chevauche
        
        // On se pousse chacun de la moitié du chevauchement
        pos += push * (overlap * 0.5f);
        other.pos -= push * (overlap * 0.5f);
    }
}