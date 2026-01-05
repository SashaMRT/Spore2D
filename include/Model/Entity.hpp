/**
 * @file Entity.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Classe de base pour toutes les entités vivantes.
 * @details Cette structure sert de parent commun aux Moutons et aux Loups. 
 * Elle contient les données partagées (Position, Énergie, Apparence) et des outils mathématiques (Distance).
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

/**
 * @struct Entity
 * @brief Structure de base représentant un objet vivant dans la simulation.
 */
struct Entity {
    // -------------------------------------------------------------------------
    // ATTRIBUTS PUBLICS
    // -------------------------------------------------------------------------
    
    sf::Vector2f pos;           ///< Position actuelle (x, y) sur la carte.
    float energy;               ///< Énergie actuelle (Vie). Si <= 0, l'entité meurt.
    float maxEnergy;            ///< Capacité maximale de l'estomac.
    bool alive;                 ///< État de vie (true = vivant, false = mort/à supprimer).
    sf::CircleShape shape;      ///< L'objet graphique SFML (Cercle).
    
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------

    /**
     * @brief Construit une entité générique.
     * @param p Position de départ.
     * @param maxE Énergie maximale (et initiale).
     * @param col Couleur de l'entité.
     * @param r Rayon du cercle (Taille).
     */
    Entity(sf::Vector2f p, float maxE, sf::Color col, float r=10.f) 
        : pos(p), maxEnergy(maxE), energy(maxE), alive(true) {
        
        shape.setRadius(r);
        shape.setFillColor(col);
        
        // CORRECTION : Pour centrer parfaitement le cercle sur sa position (pos),
        // l'origine doit être égale au rayon.
        shape.setOrigin(sf::Vector2f(r, r)); 
    }
    
    // -------------------------------------------------------------------------
    // OUTILS MATHÉMATIQUES
    // -------------------------------------------------------------------------

    /**
     * @brief Calcule la distance euclidienne vers une cible.
     * @param o La position de l'autre objet (Target).
     * @return float La distance en pixels.
     */
    float dist(const sf::Vector2f& o) const {
        float dx = pos.x - o.x;
        float dy = pos.y - o.y;
        // Théorème de Pythagore : a² + b² = c²
        return std::sqrt(dx*dx + dy*dy);
    }
};

#endif