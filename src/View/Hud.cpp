/**
 * @file Hud.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Hud : Gestion de l'interface utilisateur.
 * @details Gère l'affichage du menu latéral (Fond, Titre, Statistiques, FPS).
 * @version 0.4
 * @date 2026-01-05
 */

// Bibliothèques standard
#include <iostream> 
#include <string>
#include <iomanip> // Pour std::fixed et std::setprecision (si besoin de formatage avancé)

// Inclusion de notre bibliothèque Hud
#include "../../include/View/Hud.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Hud.
 * @details Initialise les objets textes avec la police (requis par SFML 3) 
 * et définit la largeur par défaut du menu.
 */
Hud::Hud() :
    m_textFps(m_font),
    m_textInfo(m_font),
    m_textTitle(m_font)
{
    // On définit une largeur fixe pour notre menu (250 pixels).
    m_width = 250.f; 
}

// -------------------------------------------------------------------------
// INITIALISATION
// -------------------------------------------------------------------------

/**
 * @brief Initialise toutes les ressources (Textes, Formes, Couleurs).
 * @details Tente de charger la police depuis plusieurs dossiers différents pour éviter les erreurs
 * selon l'endroit d'où le programme est lancé.
 * * @param windowSize La taille actuelle de la fenêtre (pour la hauteur du fond).
 * @return true si tout s'est bien passé (même si la police a échoué, on renvoie true pour ne pas crash).
 */
bool Hud::init(sf::Vector2u windowSize) {
    
    // -----------------------------------------------------------
    // 1. CHARGEMENT ROBUSTE DE LA POLICE
    // -----------------------------------------------------------
    // On essaye plusieurs chemins possibles. Si on lance depuis "build/", depuis "src/",
    // ou depuis la racine, le chemin vers "assets" peut changer.
    bool loaded = false;
    
    if (m_font.openFromFile("assets/font.ttf")) loaded = true;          // Cas idéal
    else if (m_font.openFromFile("../assets/font.ttf")) loaded = true;  // Un dossier au dessus
    else if (m_font.openFromFile("../../assets/font.ttf")) loaded = true;// Deux dossiers au dessus
    else if (m_font.openFromFile("arial.ttf")) loaded = true;           // Fallback sur arial à la racine

    if (!loaded) {
        std::cerr << "[ERREUR CRITIQUE] Impossible de charger la police !" << std::endl;
        std::cerr << "Veuillez placer 'font.ttf' dans un dossier 'assets' ou 'arial.ttf' a cote de l'executable." << std::endl;
        // On continue quand même, le texte sera invisible ou affichera des carrés, mais le jeu tournera.
    }

    // -----------------------------------------------------------
    // LE FOND DU MENU
    // -----------------------------------------------------------
    // Rectangle vertical qui prend toute la hauteur de la fenêtre.
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(windowSize.y)));
    
    // Noir transparent (Alpha = 150/255) pour voir le jeu derrière.
    m_background.setFillColor(sf::Color(0, 0, 0, 150)); 
    
    // Bordure blanche très fine et transparente pour délimiter.
    m_background.setOutlineThickness(-1.f); 
    m_background.setOutlineColor(sf::Color(255, 255, 255, 30));

    // -----------------------------------------------------------
    // FPS (Frames Per Second)
    // -----------------------------------------------------------
    m_textFps.setCharacterSize(14);                 // Petit texte
    m_textFps.setFillColor(sf::Color::Green);       // Vert (classique pour les FPS)
    m_textFps.setPosition(sf::Vector2f(10.f, 10.f));// Coin haut-gauche
    m_textFps.setString("FPS: --");

    // -----------------------------------------------------------
    // TITRE
    // -----------------------------------------------------------
    m_textTitle.setString("STATISTIQUES");
    m_textTitle.setCharacterSize(18);
    m_textTitle.setFillColor(sf::Color::White);
    m_textTitle.setStyle(sf::Text::Bold);           // Gras
    
    // Centrage du titre :
    sf::FloatRect bounds = m_textTitle.getLocalBounds();
    // On place l'origine du texte (le point pivot) pile en son centre.
    m_textTitle.setOrigin(sf::Vector2f(
        bounds.position.x + bounds.size.x / 2.0f, 
        bounds.position.y + bounds.size.y / 2.0f
    ));
    // On place ce point au milieu du menu (m_width / 2) à 65px de hauteur.
    m_textTitle.setPosition(sf::Vector2f(m_width / 2.0f, 65.f)); 

    // -----------------------------------------------------------
    // BARRE DE SÉPARATION
    // -----------------------------------------------------------
    m_separator.setSize(sf::Vector2f(m_width - 40.f, 2.f)); // Ligne horizontale
    m_separator.setFillColor(sf::Color::White);
    
    // Centrage de la barre
    m_separator.setOrigin(sf::Vector2f(m_separator.getSize().x / 2.0f, m_separator.getSize().y / 2.0f));
    m_separator.setPosition(sf::Vector2f(m_width / 2.0f, 85.f)); 

    // -----------------------------------------------------------
    // TEXTE DES INFORMATIONS
    // -----------------------------------------------------------
    m_textInfo.setCharacterSize(16);
    m_textInfo.setFillColor(sf::Color(220, 220, 220)); // Gris très clair
    m_textInfo.setPosition(sf::Vector2f(20.f, 105.f)); // Alignement gauche
    m_textInfo.setLineSpacing(1.4f);                   // Espacement des lignes pour la lisibilité
    m_textInfo.setString("Chargement...");

    return true;
}

// -------------------------------------------------------------------------
// MISE À JOUR (UPDATE)
// -------------------------------------------------------------------------

/**
 * @brief Met à jour les textes avec les données actuelles de la simulation.
 * @details Construit la chaîne de caractères affichant les populations, les morts et les naissances.
 */
void Hud::update(float fps, int grass, int sheep, int wolves, int deadS, int deadW, int bornS, int bornW) {
    
    // Mise à jour des FPS
    m_textFps.setString("FPS: " + std::to_string(static_cast<int>(fps)));

    // Construction du texte principal
    std::string info = "";
    
    // --- SECTION VIVANTS ---
    info += "Herbe:   " + std::to_string(grass) + "\n";
    info += "Moutons: " + std::to_string(sheep) + "\n";
    info += "Loups:   " + std::to_string(wolves) + "\n\n";

    // --- SECTION MORTS ---
    info += "--- MORTS ---\n";
    info += "Moutons: " + std::to_string(deadS) + "\n";
    info += "Loups:   " + std::to_string(deadW) + "\n\n";

    // --- SECTION NAISSANCES ---
    info += "--- NAISSANCES ---\n";
    info += "Moutons: " + std::to_string(bornS) + "\n";
    info += "Loups:   " + std::to_string(bornW) + "\n\n";
    
    // --- SECTION COMMANDES (Aide Mémoire) ---
    info += "[Echap] Quitter";

    // Application du texte
    m_textInfo.setString(info);
}

// -------------------------------------------------------------------------
// RENDU (DRAW)
// -------------------------------------------------------------------------

/**
 * @brief Affiche les éléments du HUD sur la fenêtre.
 */
void Hud::draw(sf::RenderWindow& window) {
    // L'ordre est important (couches superposées)
    window.draw(m_background); // Fond
    window.draw(m_separator);  // Ligne
    window.draw(m_textTitle);  // Titre
    window.draw(m_textFps);    // FPS
    window.draw(m_textInfo);   // Stats
}

// -------------------------------------------------------------------------
// REDIMENSIONNEMENT
// -------------------------------------------------------------------------

/**
 * @brief Adapte la hauteur du fond du menu quand la fenêtre change de taille.
 */
void Hud::onResize(sf::Vector2u newSize) {
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(newSize.y)));
}