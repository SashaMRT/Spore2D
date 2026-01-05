/**
 * @file Sheep.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Sheep (Proie).
 * @details Gère la logique spécifique aux moutons : métabolisme (faim), reproduction et affichage.
 * L'intelligence artificielle de déplacement est gérée dans Simulation.cpp.
 * @version 0.4
 * @date 2026-01-05
 */

#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Grass.hpp"
#include <cmath> // Pour les calculs mathématiques si besoin

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Construit un nouveau Mouton.
 * @details Initialise un mouton blanc, de taille 7, avec une vitesse moyenne.
 * @param position La position de départ (x, y) dans le monde.
 */
Sheep::Sheep(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(240, 240, 240), 7.f) // BLANC (Proche du blanc pur)
{
    // Attributs spécifiques
    speed = 60.f;        // Vitesse de déplacement (plus lent que le loup)
    energy = 50.f;       // Énergie de départ
    m_reproCooldown = 2.0f; // Petit délai à la naissance avant de pouvoir se reproduire
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Met à jour l'état interne du mouton.
 * @param dt Temps écoulé depuis la dernière image (Delta Time).
 */
void Sheep::update(float dt) {
    if (!alive) return; // Si mort, on ne fait rien

    // Perte d'énergie (Métabolisme de base)
    // Le mouton perd 2 points d'énergie par seconde en existant.
    energy -= 2.0f * dt; 
    
    // Vérification de la mort (Famine)
    if (energy <= 0) {
        alive = false; // Meurt de faim
    }

    // Mise à jour du temps avant prochaine reproduction
    if (m_reproCooldown > 0) {
        m_reproCooldown -= dt;
    }
}

// -------------------------------------------------------------------------
// ALIMENTATION
// -------------------------------------------------------------------------

/**
 * @brief Gestion de l'alimentation (Placeholder).
 * @details Cette fonction est vide car la logique de collision et de suppression de l'herbe
 * est gérée directement dans Simulation.cpp pour des raisons de performance et de gestion de mémoire.
 */
void Sheep::eat(std::vector<Grass>& grasses) {
    // Géré par la simulation principalement
}

/**
 * @brief Permet au mouton de regagner de l'énergie (quand il mange de l'herbe).
 * @param amount Quantité d'énergie gagnée.
 */
void Sheep::gainEnergy(float amount) {
    energy += amount;
    
    // On ne peut pas dépasser le maximum (Capacité de l'estomac)
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }
}

// -------------------------------------------------------------------------
// REPRODUCTION
// -------------------------------------------------------------------------

/**
 * @brief Vérifie si le mouton remplit les conditions pour avoir un bébé.
 * @return true si Vivant, Énergie > 60 et Cooldown terminé.
 */
bool Sheep::canReproduce() const {
    // Les moutons se reproduisent plus facilement que les loups (seuil 60 vs 80)
    return alive && energy > 60.f && m_reproCooldown <= 0.f;
}

/**
 * @brief Applique les coûts de la reproduction après une naissance.
 */
void Sheep::resetReproduction() {
    energy -= 30.f;      // Coût énergétique de la naissance
    m_reproCooldown = 5.f; // Pause de 5 secondes avant le prochain bébé
}

// -------------------------------------------------------------------------
// RENDU
// -------------------------------------------------------------------------

/**
 * @brief Affiche le mouton à l'écran.
 */
void Sheep::draw(sf::RenderWindow& window) {
    // Mise à jour de la position du cercle SFML
    shape.setPosition(pos);
    window.draw(shape);
}