/**
 * @file Sheep.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Sheep (La Proie).
 * @version 0.6
 * @date 2026-01-06
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <list> // [AJOUT] Nécessaire pour std::list<Grass>
#include "Entity.hpp"

// -------------------------------------------------------------------------
// FORWARD DECLARATION
// -------------------------------------------------------------------------
class Grass;
class Wolf; // [AJOUT] Le mouton doit connaître l'existence du loup pour le fuir

/**
 * @class Sheep
 * @brief Représente un mouton dans la simulation.
 */
class Sheep : public Entity {
public:
    /**
     * @brief Constructeur.
     * @param position Position initiale (x, y).
     */
    Sheep(sf::Vector2f position);

    /**
     * @brief Met à jour les statistiques internes.
     * @param dt Temps écoulé.
     */
    void update(float dt);
    
    /**
     * @brief [NOUVEAU] Cerveau du Mouton (Intelligence Artificielle).
     * @details Gère la fuite (priorité 1), la faim (priorité 2) et l'errance (priorité 3).
     * @param dt Temps écoulé.
     * @param wolves Liste des prédateurs à éviter.
     * @param grass Liste de la nourriture à manger.
     * @param simTime Temps global.
     */
    void moveAI(float dt, const std::vector<Wolf>& wolves, const std::list<Grass>& grass, float simTime);

    /**
     * @brief Affiche le sprite du mouton.
     * @param window La fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);
    
    // Gestion nourriture (partielle)
    void eat(std::vector<Grass>& grasses);
    void gainEnergy(float amount);

    // Reproduction
    bool canReproduce() const; 
    void resetReproduction();  

public:
    // Attributs publics
    float speed; ///< Vitesse de déplacement du mouton.

private:
    // Attributs privés
    float m_reproCooldown; ///< Temps d'attente avant la prochaine reproduction.
};