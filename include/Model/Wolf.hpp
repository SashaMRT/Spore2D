/**
 * @file Wolf.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Wolf (Le Prédateur).
 * @details Définit les attributs et méthodes spécifiques aux loups :
 * vitesse de course, logique de chasse, et conditions de reproduction.
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
// On dit au compilateur : "La classe Sheep existe, fais-moi confiance".
// Cela permet d'utiliser "Sheep" dans les arguments de fonction sans inclure "Sheep.hpp" ici.
// C'est INDISPENSABLE car Sheep.hpp inclut déjà Wolf.hpp (boucle infinie sinon).
class Sheep; 

/**
 * @class Wolf
 * @brief Représente un loup dans la simulation.
 */
class Wolf : public Entity {
public:
    /**
     * @brief Constructeur.
     * @param position Position initiale (x, y).
     */
    Wolf(sf::Vector2f position);

    /**
     * @brief Met à jour les statistiques internes (Faim, Cooldown).
     * @param dt Temps écoulé (Delta Time).
     */
    void update(float dt);
    
    /**
     * @brief Affiche le sprite du loup.
     * @param window La fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);
    
    /**
     * @brief Logique de déplacement vers la proie.
     * @details Cherche le mouton le plus proche et avance vers lui.
     * @param sheeps Liste des moutons (cibles potentielles).
     */
    void hunt(const std::vector<Sheep>& sheeps);

    /**
     * @brief Logique d'alimentation.
     * @details Vérifie la collision avec les moutons et les tue si contact.
     * @param sheeps Liste des moutons (modifiable car ils peuvent mourir).
     */
    void eat(std::vector<Sheep>& sheeps);

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
    float speed; ///< Vitesse de déplacement du loup (en pixels/seconde).

private:
    // Attributs privés
    float m_reproCooldown; ///< Temps d'attente restant avant la prochaine reproduction.
};