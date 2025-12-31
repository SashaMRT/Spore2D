/**
 * @file Application.cpp
 * @author sasha.marieterehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Renderer : Représentation de l'affichage du monde du jeu.
 * @details Place la zone de jeu à droite du menu (HUD) et dessine les bordures.
 * @version 0.1
 * @date 2025-12-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// Inclusion de notre bibliothèque Renderer.
// Elle permet l'utilisation de la classe Renderer et de ces méthodes.
#include "../../include/View/Renderer.hpp"

#include <cstdlib>
#include <ctime>

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Renderer.
 * C'est ici que l'on initialise les variables.
 */
Renderer::Renderer() {
    // Vide pour l'instant
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Initialise la zone de jeu.
 * @details Calcule la taille et la position de la zone de jeu en fonction de la taille
 * de la fenêtre et de la largeur du menu.
 * @param windowSize La taille totale de la fenêtre.
 * @param hudWidth La largeur du menu en pixels.
 */
void Renderer::init(sf::Vector2u windowSize, float hudWidth) {
    // Calcul de la place restante pour le jeu
    // On convertit les unsigned int en float pour les calculs de position (sinon erreurs).
    float gameWidth = static_cast<float>(windowSize.x) - hudWidth;
    float gameHeight = static_cast<float>(windowSize.y);

    // Configuration de la zone de jeu
    // On retire 20 pixels en largeur et hauteur pour laisser une marge (sinon colle le menu)
    m_gameArea.setSize(sf::Vector2f(gameWidth - 20.f, gameHeight - 20.f));
    
    // Positionnement du rectangle
    // X = On commence après le menu (+10px de marge)
    // Y = On commence un peu en bas (+10px de marge)
    m_gameArea.setPosition(sf::Vector2f(hudWidth + 10.f, 10.f));
    
    // Style
    m_gameArea.setFillColor(sf::Color::Black);            // Fond noir
    m_gameArea.setOutlineThickness(2.f);                  // Épaisseur de la bordure
    m_gameArea.setOutlineColor(sf::Color(100, 100, 100)); // Gris clair pour la bordure
}
// ==== Déclarations ====
static void ecosystemUpdate(float dt);
static void ecosystemDraw(sf::RenderWindow& window);

// ---- Rendu principal ----
void Renderer::draw(sf::RenderWindow& window) {
    window.draw(m_gameArea);              // Fond de la zone de jeu

    static sf::Clock ecoClock;           // Chrono pour mesurer le temps entre frames
    float dt = ecoClock.restart().asSeconds(); // Temps écoulé depuis la dernière frame

    ecosystemUpdate(dt);                 // Mise à jour de la logique
    ecosystemDraw(window);               // Dessin des entités (herbes, moutons, loups)
}

/**
 * @brief Recalcul de la zone de jeu.
 */
void Renderer::onResize(sf::Vector2u newSize, float hudWidth) {
    // Calcul des nouvelles dimensions
    // Largeur = Largeur fenêtre - Largeur Menu
    float gameWidth = static_cast<float>(newSize.x) - hudWidth;
    float gameHeight = static_cast<float>(newSize.y);

    // Mise à jour de la zone de jeu
    // On garde toujours la petite marge de 20 pixels (-20.f)
    m_gameArea.setSize(sf::Vector2f(gameWidth - 20.f, gameHeight - 20.f));
}


// ===== ÉCOSYSTÈME =====
// Gère la simulation : herbe, moutons, loups (spawn, logique, affichage)

#include "../../include/Model/Entity.hpp"
#include "../../include/Model/Grass.hpp"
#include "../../include/Model/Sheep.hpp"
#include "../../include/Model/Wolf.hpp"
#include <algorithm>

// Groupes d’entités
static std::vector<Grass> ecosystem_grass;
static std::vector<Sheep> ecosystem_sheeps;
static std::vector<Wolf> ecosystem_wolves;

// Fonctions internes
static void initEcosystem();
static void ecosystemUpdate(float dt);
static void ecosystemDraw(sf::RenderWindow& window);

// Création initiale aléatoire
static void initEcosystem() {
    ecosystem_grass.clear();
    ecosystem_sheeps.clear();
    ecosystem_wolves.clear();
    
    // Herbes
    for (int i = 0; i < 8; i++) {
        float x = 400 + (rand() % 600);
        float y = 100 + (rand() % 500);
        ecosystem_grass.emplace_back(Grass({x, y}));
    }
    // Moutons
    for (int i = 0; i < 5; i++) {
        float x = 450 + (rand() % 500);
        float y = 150 + (rand() % 400);
        ecosystem_sheeps.emplace_back(Sheep({x, y}));
    }
    // Loups
    for (int i = 0; i < 3; i++) {
        float x = 700 + (rand() % 300);
        float y = 200 + (rand() % 300);
        ecosystem_wolves.emplace_back(Wolf({x, y}));
    }
}

// Mise à jour de l’écosystème
static void ecosystemUpdate(float dt) {
    static bool randInit = false;
    if (!randInit) { srand(time(NULL)); initEcosystem(); randInit = true; }

    for (auto& g : ecosystem_grass) g.update(dt);
    for (auto& w : ecosystem_wolves) { w.update(dt); w.hunt(ecosystem_sheeps); w.eat(ecosystem_sheeps); }

    // IA moutons
    for (auto& s : ecosystem_sheeps) {
        if (!s.alive) continue;
        s.update(dt);
        sf::Vector2f moveDir = {0.f, 0.f};

        // Recherche d’herbe proche
        Grass* bestGrass = nullptr;
        float bestDist = 999.f;
        for (const auto& g : ecosystem_grass)
            if (g.alive) {
                float d = s.dist(g.pos);
                if (d < bestDist && d < 200.f) { bestDist = d; bestGrass = const_cast<Grass*>(&g); }
            }

        if (bestGrass && bestDist < 150.f) {
            sf::Vector2f dir = bestGrass->pos - s.pos;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            moveDir = dir / len;
        }

        // Fuite si un loup est proche
        for (const auto& w : ecosystem_wolves)
            if (w.alive && s.dist(w.pos) < 100.f) {
                sf::Vector2f flee = s.pos - w.pos;
                float len = std::sqrt(flee.x*flee.x + flee.y*flee.y);
                moveDir = (flee / len) * 1.5f;
                break;
            }

        // Déplacement + limites + mange
        s.pos += moveDir * s.speed * dt;
        s.pos.x = std::clamp(s.pos.x, 400.f, 1200.f);
        s.pos.y = std::clamp(s.pos.y, 100.f, 800.f);
        s.eat(ecosystem_grass);
    }

    // Suppression des entités mortes
    ecosystem_grass.erase(std::remove_if(ecosystem_grass.begin(), ecosystem_grass.end(), [](auto& g){return !g.alive;}), ecosystem_grass.end());
    ecosystem_sheeps.erase(std::remove_if(ecosystem_sheeps.begin(), ecosystem_sheeps.end(), [](auto& s){return !s.alive;}), ecosystem_sheeps.end());
    ecosystem_wolves.erase(std::remove_if(ecosystem_wolves.begin(), ecosystem_wolves.end(), [](auto& w){return !w.alive;}), ecosystem_wolves.end());

    // Respawn si tous les moutons sont morts
    if (ecosystem_sheeps.empty()) initEcosystem();
}

// Dessin de tous les éléments
static void ecosystemDraw(sf::RenderWindow& window) {
    for (auto& g : ecosystem_grass) { g.shape.setPosition(g.pos); window.draw(g.shape); }
    for (auto& s : ecosystem_sheeps) if (s.alive) { s.shape.setPosition(s.pos); window.draw(s.shape); }
    for (auto& w : ecosystem_wolves) if (w.alive) { w.shape.setPosition(w.pos); window.draw(w.shape); }
}
