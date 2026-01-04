/**
 * @file Sheep.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Sheep.
 * @details Gestion du comportement du mouton (manger, se reproduire, mourir).
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Bibliothèque utilisées
#include "../../include/Model/Sheep.hpp"
#include <cmath> // Pour les calculs

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Initialise le mouton avec ses stats de départ.
 */
Sheep::Sheep(sf::Vector2f position) : pos(position), alive(true) {
    // Configuration graphique
    shape.setRadius(6.f);
    shape.setOrigin(sf::Vector2f(6.f, 6.f)); // Centre le point de pivot
    speed = 60.f;

    // Début de vie : un peu d'énergie, mais pas prêt à se reproduire tout de suite
    m_energy = 40.f; 
    m_reproCooldown = 2.0f; 
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Logique appelée à chaque image.
 */
void Sheep::update(float dt) {
    if (!alive) return;

    // Perte d'énergie naturelle
    m_energy -= 2.0f * dt; 

    // Si plus d'énergie, le mouton meurt
    if (m_energy <= 0) alive = false; 

    // Gestion du délai de reproduction
    if (m_reproCooldown > 0) {
        m_reproCooldown -= dt;
    }
}

// -------------------------------------------------------------------------
// ACTIONS (MANGER / DISTANCE)
// -------------------------------------------------------------------------

/**
 * @brief Manger l'herbe pour survivre.
 */
void Sheep::eat(std::vector<Grass>& grasses) {
    if (!alive) return;

    for (auto& g : grasses) {
        // Si l'herbe existe et qu'on est dessus (distance < 15px)
        if (g.alive && dist(g.pos) < 15.f) { 
            g.alive = false;  // L'herbe disparaît
            m_energy += 20.f; // Gain d'énergie
            
            // On ne dépasse pas 100% d'énergie
            if (m_energy > 100.f) m_energy = 100.f; 
        }
    }
}

/**
 * @brief Calcul de distance (Pythagore).
 */
float Sheep::dist(sf::Vector2f otherPos) const {
    float dx = pos.x - otherPos.x;
    float dy = pos.y - otherPos.y;
    return std::sqrt(dx*dx + dy*dy);
}

// -------------------------------------------------------------------------
// REPRODUCTION
// -------------------------------------------------------------------------

/**
 * @brief Vérifie les conditions de naissance.
 */
bool Sheep::canReproduce() const {
    // Il faut être vivant, en forme (>60 énergie) et que le délai soit passé.
    return alive && m_energy > 60.f && m_reproCooldown <= 0.f;
}

/**
 * @brief Conséquences de la naissance.
 */
void Sheep::resetReproduction() {
    m_energy -= 30.f;      // Faire un bébé fatigue
    m_reproCooldown = 5.f; // On doit attendre 5 secondes
}

// -------------------------------------------------------------------------
// AFFICHAGE
// -------------------------------------------------------------------------

/**
 * @brief Dessine le mouton.
 */
void Sheep::draw(sf::RenderWindow& window) {
    // Corps blanc
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(240, 240, 240)); 
    window.draw(shape);
    
    // Tête noire
    sf::CircleShape head(shape.getRadius() * 0.5f);
    head.setOrigin(sf::Vector2f(head.getRadius(), head.getRadius())); 
    head.setPosition(sf::Vector2f(pos.x, pos.y - 5)); 
    head.setFillColor(sf::Color(20, 20, 20)); 
    window.draw(head);
}