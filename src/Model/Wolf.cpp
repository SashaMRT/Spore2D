/**
 * @file Wolf.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation optimisée du Loup.
 * @version 0.7
 * @date 2026-01-06
 */

#include "../../include/Model/Wolf.hpp"
#include "../../include/Model/Sheep.hpp"
#include <cmath>
#include <algorithm> // Pour std::min

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(200, 50, 50), 9.f) // Rouge
{
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 0, 0)); 
    
    speed = 95.f;     
    energy = 60.f;    
    m_reproCooldown = 5.0f;
}

// -------------------------------------------------------------------------
// LOGIQUE
// -------------------------------------------------------------------------

void Wolf::update(float dt) {
    if (!alive) return;

    // Métabolisme
    energy -= 3.0f * dt;
    if (energy <= 0) alive = false;

    // Gestion du temps
    if (m_reproCooldown > 0) m_reproCooldown -= dt;
}

void Wolf::moveAI(float dt, const std::vector<Sheep>& sheeps, float simTime) {
    if (!alive) return;

    // 1. RECHERCHE DE CIBLE (Mouton le plus proche)
    const Sheep* target = nullptr;
    float minDist = 400.f; // Portée de vision

    for (const auto& s : sheeps) {
        if (!s.alive) continue; // On ignore les morts

        float d = dist(s.pos);
        if (d < minDist) {
            minDist = d;
            target = &s;
        }
    }

    // 2. DÉPLACEMENT
    sf::Vector2f moveDir(0.f, 0.f);

    if (target) {
        // CHASSE : Vecteur normalisé vers la cible
        sf::Vector2f diff = target->pos - pos;
        if (minDist > 0.1f) moveDir = diff / minDist;
    } 
    else {
        // ERRANCE : Mouvement sinusoïdal pseudo-aléatoire
        // On utilise l'adresse mémoire (this) comme graine unique pour chaque loup
        long long seed = (long long)this % 100; 
        float angle = std::sin(simTime * 0.5f + seed) * 6.28f; // 2*PI approx
        
        moveDir = {std::cos(angle), std::sin(angle)};
        speed *= 0.6f; // Ralentit quand il erre (temporairement pour ce calcul)
    }

    pos += moveDir * speed * dt;
    
    // On remet la vitesse normale si elle a été modifiée par l'errance
    if (!target) speed /= 0.6f; 
}

// -------------------------------------------------------------------------
// ACTIONS
// -------------------------------------------------------------------------

void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;

    for (auto& s : sheeps) {
        // Collision simple (rayon loup + rayon mouton approx 20px)
        if (s.alive && dist(s.pos) < 20.f) { 
            s.alive = false;       // Miam
            energy += 50.f;        // Gain d'énergie
            if (energy > maxEnergy) energy = maxEnergy;
            
            break; // OPTIMISATION : Un loup ne mange qu'un mouton à la fois par frame.
                   // On sort de la boucle immédiatement.
        }
    }
}

bool Wolf::canReproduce() const {
    return alive && energy > 80.f && m_reproCooldown <= 0.f;
}

void Wolf::resetReproduction() {
    energy -= 40.f;         
    m_reproCooldown = 10.f; 
}

void Wolf::draw(sf::RenderWindow& window) {
    shape.setPosition(pos); 
    window.draw(shape);     
}