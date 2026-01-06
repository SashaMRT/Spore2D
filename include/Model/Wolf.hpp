/**
 * @file Wolf.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Wolf (Le Prédateur).
 * @details Définit les attributs et méthodes spécifiques aux loups.
 * @version 0.6
 * @date 2026-01-06
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.hpp"

// -------------------------------------------------------------------------
// FORWARD DECLARATION
// -------------------------------------------------------------------------
// Indispensable pour éviter l'erreur "Wolf a besoin de Sheep qui a besoin de Wolf..."
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
     * @brief [NOUVEAU] Cerveau du Loup (Intelligence Artificielle).
     * @details Décide où aller : soit chasser le mouton le plus proche, soit errer.
     * @param dt Temps écoulé.
     * @param sheeps La liste des moutons (cibles potentielles).
     * @param simTime Temps global de la simulation (pour l'errance aléatoire).
     */
    void moveAI(float dt, const std::vector<Sheep>& sheeps, float simTime);

    /**
     * @brief Affiche le sprite du loup.
     * @param window La fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);
    
    // (L'ancienne méthode hunt est intégrée dans moveAI maintenant, mais on peut la garder ou l'enlever)
    // Pour simplifier, moveAI remplace la logique de déplacement de hunt.
    
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