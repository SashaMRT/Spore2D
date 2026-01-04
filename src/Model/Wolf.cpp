/**
 * @file Wolf.cpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Wolf.
 * @details Gestion de l'IA de chasse et du cycle de vie du loup.
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Bibliothèque utilisées
#include "../../include/Model/Wolf.hpp"
#include <cmath> // Pour les calculs

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Wolf::Wolf(sf::Vector2f position) : pos(position), alive(true) {
    shape.setRadius(8.f);
    shape.setOrigin(sf::Vector2f(8.f, 8.f)); 
    speed = 95.f; // Rapide

    m_energy = 50.f;       
    m_reproCooldown = 5.0f;
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

void Wolf::update(float dt) {
    if (!alive) return;

    // Le loup perd de l'énergie plus vite (3.0f)
    m_energy -= 3.0f * dt; 
    
    if (m_energy <= 0) alive = false;

    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

// -------------------------------------------------------------------------
// CHASSE (IA)
// -------------------------------------------------------------------------

void Wolf::hunt(const std::vector<Sheep>& sheeps) {
    if (!alive) return;
    
    const Sheep* target = nullptr;
    float minDist = 300.f; // Rayon de vision

    // On cherche la proie la plus proche
    for (const auto& s : sheeps) {
        if (s.alive) {
            float d = dist(s.pos);
            if (d < minDist) {
                minDist = d;
                target = &s;
            }
        }
    }

    // Déplacement vers la cible
    if (target) {
        sf::Vector2f dir = target->pos - pos;
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        // Normalisation et mouvement
        pos += (dir / len) * speed * 0.016f; 
    }
}

void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;

    for (auto& s : sheeps) {
        // Si contact (< 20px)
        if (s.alive && dist(s.pos) < 20.f) {
            s.alive = false;  // Mouton mangé
            m_energy += 40.f; // Gros gain d'énergie
            if (m_energy > 100.f) m_energy = 100.f;
        }
    }
}

float Wolf::dist(sf::Vector2f otherPos) const {
    float dx = pos.x - otherPos.x;
    float dy = pos.y - otherPos.y;
    return std::sqrt(dx*dx + dy*dy);
}

// -------------------------------------------------------------------------
// REPRODUCTION
// -------------------------------------------------------------------------

bool Wolf::canReproduce() const {
    // Seuil d'énergie plus élevé pour le loup (70)
    return alive && m_energy > 70.f && m_reproCooldown <= 0.f;
}

void Wolf::resetReproduction() {
    m_energy -= 40.f;       
    m_reproCooldown = 10.f; // Cycle plus lent
}

// -------------------------------------------------------------------------
// AFFICHAGE
// -------------------------------------------------------------------------

void Wolf::draw(sf::RenderWindow& window) {
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 100, 100)); // Gris
    
    // Contour rouge
    shape.setOutlineThickness(1);
    shape.setOutlineColor(sf::Color::Red); 
    window.draw(shape);
}