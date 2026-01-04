/**
 * @file Sheep.hpp
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Définition de la classe Sheep (Mouton).
 * @details Contient les attributs (énergie, position) et les méthodes du mouton.
 * @version 0.2
 * @date 2026-01-04
 *
 * @copyright Copyright (c) 2026
 *
 */

// Comme ifndef mais plus moderne
#pragma once

// Bibliothèque utilisées*
#include <SFML/Graphics.hpp>
#include <vector>
#include "Grass.hpp"

class Sheep {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------
    
    /**
     * @brief Constructeur du mouton.
     * @param position La position de départ (X, Y).
     */
    Sheep(sf::Vector2f position);

    // -------------------------------------------------------------------------
    // MÉTHODES DE MISE À JOUR & DESSIN
    // -------------------------------------------------------------------------

    /**
     * @brief Mise à jour logique (Faim, Vieillissement, Reproduction).
     * @param dt Temps écoulé depuis la dernière image (Delta Time).
     */
    void update(float dt);

    /**
     * @brief Affiche le mouton sur la fenêtre.
     */
    void draw(sf::RenderWindow& window);
    
    // -------------------------------------------------------------------------
    // ACTIONS SPÉCIFIQUES
    // -------------------------------------------------------------------------

    /**
     * @brief Permet au mouton de manger de l'herbe pour gagner de l'énergie.
     * @param grasses La liste de toute l'herbe disponible.
     */
    void eat(std::vector<Grass>& grasses);

    /**
     * @brief Calcule la distance entre ce mouton et une autre position.
     */
    float dist(sf::Vector2f otherPos) const;

    // -------------------------------------------------------------------------
    // REPRODUCTION
    // -------------------------------------------------------------------------

    /**
     * @brief Vérifie si le mouton peut faire un bébé.
     * @return true si vivant, assez d'énergie et pas de délai d'attente.
     */
    bool canReproduce() const; 
    
    /**
     * @brief Réinitialise le statut après une naissance (baisse d'énergie).
     */
    void resetReproduction();  

    // -------------------------------------------------------------------------
    // ATTRIBUTS PUBLICS
    // -------------------------------------------------------------------------
    sf::Vector2f pos;       // Position X, Y
    sf::CircleShape shape;  // Apparence
    bool alive;             // Etats
    float speed;            // Vitesse de déplacement

private:
    // -------------------------------------------------------------------------
    // ATTRIBUTS PRIVÉS (INTERNES)
    // -------------------------------------------------------------------------
    float m_energy;          // Barre d'énergie (0 = mort)
    float m_reproCooldown;   // Temps d'attente avant le prochain bébé
};