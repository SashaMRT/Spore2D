/**
 * @file Entity.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Classe de base commune.
 * @version 0.8
 * @date 2026-01-06
 */

#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

/**
 * @struct Entity
 * @brief Structure de base représentant un objet vivant.
 */
struct Entity {
    // -------------------------------------------------------------------------
    // ATTRIBUTS
    // -------------------------------------------------------------------------
    sf::Vector2f pos;           ///< Position (x, y).
    float energy;               ///< Énergie actuelle.
    float maxEnergy;            ///< Énergie max.
    bool alive;                 ///< État de vie.
    float radius;               ///< Rayon de l'entité (taille physique).
    sf::CircleShape shape;      ///< Apparence (Cercle).
    
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------
    Entity(sf::Vector2f p, float maxE, sf::Color col, float r=10.f);
    
    // -------------------------------------------------------------------------
    // MÉTHODES
    // -------------------------------------------------------------------------

    // Calcule la distance vers un point
    float dist(const sf::Vector2f& o) const;

    /**
     * @brief Vérifie si l'entité touche les bords du monde.
     * @details Bloque l'entité si elle dépasse un peu, ou la tue si le mur l'écrase.
     * @param xMin Limite gauche.
     * @param xMax Limite droite.
     * @param yMin Limite haute.
     * @param yMax Limite basse.
     */
    void checkBounds(float xMin, float xMax, float yMin, float yMax);

    /**
     * @brief Gère la collision physique avec une autre entité.
     * @details Si les deux entités se chevauchent, elles se repoussent mutuellement.
     * @param other L'autre entité avec qui on entre en collision.
     */
    void resolveCollision(Entity& other);
};

#endif