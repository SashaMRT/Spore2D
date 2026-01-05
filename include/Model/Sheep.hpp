/**
 * @file Sheep.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Sheep (La Proie).
 * @details Définit les attributs et méthodes spécifiques aux moutons :
 * gestion de l'énergie, interaction avec l'herbe et reproduction.
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"

// -------------------------------------------------------------------------
// FORWARD DECLARATION
// -------------------------------------------------------------------------
// On annonce au compilateur que la classe "Grass" existe.
// Cela évite d'inclure "Grass.hpp" ici et réduit les dépendances circulaires.
class Grass;

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
     * @brief Met à jour les statistiques internes (Faim, Cooldown).
     * @param dt Temps écoulé (Delta Time).
     */
    void update(float dt);
    
    /**
     * @brief Affiche le sprite du mouton.
     * @param window La fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);
    
    // -------------------------------------------------------------------------
    // ALIMENTATION
    // -------------------------------------------------------------------------

    /**
     * @brief Logique de recherche de nourriture.
     * @details (Note : La logique principale est gérée dans Simulation.cpp pour l'instant).
     * @param grasses Liste des herbes disponibles.
     */
    void eat(std::vector<Grass>& grasses);

    /**
     * @brief Augmente l'énergie du mouton.
     * @param amount Quantité d'énergie gagnée.
     */
    void gainEnergy(float amount);

    // -------------------------------------------------------------------------
    // REPRODUCTION
    // -------------------------------------------------------------------------

    /**
     * @brief Vérifie les conditions pour la naissance d'un bébé.
     * @return true si assez d'énergie et cooldown terminé.
     */
    bool canReproduce() const; 

    /**
     * @brief Applique le coût énergétique après une naissance.
     */
    void resetReproduction();  

public:
    // Attributs publics
    float speed; ///< Vitesse de déplacement du mouton.

private:
    // Attributs privés
    float m_reproCooldown; ///< Temps d'attente avant la prochaine reproduction.
};