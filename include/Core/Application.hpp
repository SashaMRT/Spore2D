/**
 * @file Application.hpp
 * @brief Déclaration de la classe Application
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "../View/Hud.hpp"
#include "../View/Renderer.hpp"

class Application {
public:
    Application();
    Application(bool testMode); // Surcharge pour le mode test
    void run();

private:
    sf::RenderWindow m_window;
    Hud m_hud;
    Renderer m_renderer;
    bool m_isTestMode;
    
    // --- CORRECTION : Variable nécessaire pour la pause ---
    bool m_isPaused; 
};