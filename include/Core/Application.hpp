/**
 * @file Application.hpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Déclaration de la classe Application (Moteur principal).
 * @details Définit la classe principale qui orchestre la boucle de jeu,
 * la fenêtre SFML, et la communication entre le Modèle, la Vue (Renderer) et le HUD.
 * @version 0.5
 * @date 2026-01-05
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "../View/Hud.hpp"
#include "../View/Renderer.hpp"

/**
 * @class Application
 * @brief Classe principale (Contrôleur) de l'application.
 */
class Application {
public:
    // -------------------------------------------------------------------------
    // CONSTRUCTEUR
    // -------------------------------------------------------------------------

    /**
     * @brief Constructeur de l'application.
     * @details Prépare la fenêtre, initialise le HUD et le Renderer.
     * @param testMode Si true, l'application se fermera toute seule après 30 frames (pour les tests auto). 
     * Par défaut, c'est false (Mode normal).
     */
    Application(bool testMode = false);

    // -------------------------------------------------------------------------
    // BOUCLE DE JEU
    // -------------------------------------------------------------------------

    /**
     * @brief Lance la boucle principale (Run Loop).
     * @details Contient la boucle while(isOpen) qui gère :
     * 1. Les événements (Clavier/Souris).
     * 2. La mise à jour logique (Update).
     * 3. Le rendu graphique (Draw).
     */
    void run();

private:
    // -------------------------------------------------------------------------
    // MEMBRES PRIVÉS
    // -------------------------------------------------------------------------

    sf::RenderWindow m_window; ///< La fenêtre graphique SFML.
    
    // --- Sous-systèmes ---
    Hud m_hud;           ///< Gestionnaire de l'interface utilisateur (Menu gauche).
    Renderer m_renderer; ///< Gestionnaire du rendu de la simulation (Zone de jeu).

    // --- Drapeaux d'état ---
    bool m_isTestMode;   ///< Indique si on est en mode test (fermeture auto).
    bool m_isPaused;     ///< Indique si le jeu est en pause (Touche P ou Game Over).
};