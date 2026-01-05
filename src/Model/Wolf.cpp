/**
 * @file Wolf.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Wolf (Prédateur).
 * @details Gère la logique spécifique aux loups : chasse, alimentation, perte d'énergie et reproduction.
 * @version 0.4
 * @date 2026-01-05
 */

#include "../../include/Model/Wolf.hpp"
#include "../../include/Model/Sheep.hpp" // Nécessaire pour interagir avec les moutons
#include <cmath> // Pour std::sqrt (calcul de distance)

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Construit un nouveau Loup.
 * @param position La position de départ (x, y) dans le monde.
 */
Wolf::Wolf(sf::Vector2f position) 
    : Entity(position, 100.f, sf::Color(200, 50, 50), 9.f) // Appel au constructeur parent (Rouge)
{
    // Personnalisation visuelle (Cercle avec bordure foncée)
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 0, 0)); 
    
    // Attributs spécifiques
    speed = 95.f;     // Un peu plus rapide que les moutons
    energy = 60.f;    // Énergie de départ moyenne
    m_reproCooldown = 5.0f; // Pas de reproduction immédiate à la naissance
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Met à jour l'état interne du loup (Métabolisme).
 * @param dt Temps écoulé depuis la dernière image (Delta Time).
 */
void Wolf::update(float dt) {
    if (!alive) return; // Si mort, on ne fait rien

    // 1. Perte d'énergie (Métabolisme)
    // Le loup perd 3 points d'énergie par seconde juste en vivant.
    energy -= 3.0f * dt; 
    
    // Vérification de la mort (Famine)
    if (energy <= 0) {
        alive = false;
        energy = 0; // Pour éviter les valeurs négatives
    }

    // Mise à jour du temps avant prochaine reproduction
    if (m_reproCooldown > 0) {
        m_reproCooldown -= dt;
    }
}

// -------------------------------------------------------------------------
// COMPORTEMENT : CHASSE
// -------------------------------------------------------------------------

/**
 * @brief Cherche le mouton le plus proche et se déplace vers lui.
 * @param sheeps La liste de tous les moutons présents dans la simulation.
 */
void Wolf::hunt(const std::vector<Sheep>& sheeps) {
    if (!alive) return;
    
    const Sheep* target = nullptr; // Pointeur vers la cible trouvée
    float minDist = 350.f;         // Rayon de vision du loup (350 pixels)

    // Algorithme du "Plus Proche Voisin"
    for (const auto& s : sheeps) {
        if (s.alive) {
            float d = dist(s.pos); // Calcul distance
            // Si ce mouton est plus proche que le précédent trouvé
            if (d < minDist) {
                minDist = d;
                target = &s;
            }
        }
    }

    // Si une cible a été trouvée, on se déplace vers elle
    if (target) {
        // Calcul du vecteur direction (Cible - Soi-même)
        sf::Vector2f dir = target->pos - pos;
        
        // Normalisation (On ramène la longueur du vecteur à 1)
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        
        if (len > 0) {
            // Déplacement : Direction * Vitesse * Facteur Temps (0.016 ~= 60 FPS)
            pos += (dir / len) * speed * 0.016f; 
        }
    }
}

// -------------------------------------------------------------------------
// COMPORTEMENT : MANGER
// -------------------------------------------------------------------------

/**
 * @brief Vérifie les collisions avec les moutons pour les manger.
 * @param sheeps La liste des moutons (modifiable car on va changer leur état 'alive').
 */
void Wolf::eat(std::vector<Sheep>& sheeps) {
    if (!alive) return;

    for (auto& s : sheeps) {
        // Si le mouton est vivant et qu'on le touche (< 20px)
        if (s.alive && dist(s.pos) < 20.f) { 
            s.alive = false;  // Le mouton meurt
            
            // Gain d'énergie pour le loup
            energy += 50.f; 
            
            // On ne peut pas dépasser le maximum (100)
            if (energy > maxEnergy) energy = maxEnergy;
        }
    }
}

// -------------------------------------------------------------------------
// REPRODUCTION
// -------------------------------------------------------------------------

/**
 * @brief Vérifie si le loup remplit les conditions pour avoir un bébé.
 * @return true si Énergie > 80 et Cooldown terminé.
 */
bool Wolf::canReproduce() const {
    // Il faut être vivant, bien nourri (> 80) et reposé.
    return alive && energy > 80.f && m_reproCooldown <= 0.f;
}

/**
 * @brief Applique les coûts de la reproduction après une naissance.
 */
void Wolf::resetReproduction() {
    energy -= 40.f;         // Faire un bébé fatigue beaucoup (Coût en énergie)
    m_reproCooldown = 10.f; // Pause de 10 secondes avant le prochain
}

// -------------------------------------------------------------------------
// RENDU
// -------------------------------------------------------------------------

/**
 * @brief Affiche le loup à l'écran.
 */
void Wolf::draw(sf::RenderWindow& window) {
    shape.setPosition(pos); // Met à jour la position du sprite
    window.draw(shape);     // Dessine
}