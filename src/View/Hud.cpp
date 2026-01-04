/**
 * @file Hud.cpp
 * @author Sasha Marie te Rehorst (sasha.marieterehorst@gmail.com)
 * @brief Implémentation de la classe Hud : Gestion de l'interface utilisateur.
 * @details Gère l'affichage du menu (Fond, Titre, Statistiques, FPS).
 * @version 0.2
 * @date 2026-01-03
 *
 * @copyright Copyright (c) 2026
 *
 */

// Inclusion de la bibliothèque std.
#include <iostream> 

// Inclusion de la bibliothèque string.
#include <string>

// Inclusion de notre bibliothèque Hud.
#include "../../include/View/Hud.hpp"

// -------------------------------------------------------------------------
// CONSTRUCTEUR
// -------------------------------------------------------------------------

/**
 * @brief Constructeur de l'objet Hud.
 * Initialise les variables et définit la largeur du menu.
 */
Hud::Hud() :
    m_textFps(m_font),
    m_textInfo(m_font),
    m_textTitle(m_font)
{
    // On définit une largeur pour notre menu (250 pixels).
    m_width = 250.f; 
}

// -------------------------------------------------------------------------
// MÉTHODES PUBLIQUES
// -------------------------------------------------------------------------

/**
 * @brief Initialise toutes les ressources (Textes, Formes, Couleurs).
 * @param windowSize La taille actuelle de la fenêtre (pour la hauteur du fond).
 * @return true si tout s'est bien passé, false sinon.
 */
bool Hud::init(sf::Vector2u windowSize) {
    
    // Chargement de la police d'écriture.
    // C'est le fichier .ttf qui définit le style des lettres.
    if (!m_font.openFromFile("../assets/font.ttf")) {
        std::cerr << "[ERREUR] Hud: Impossible de charger 'assets/font.ttf'." << std::endl;
        return false;
    }

    // -----------------------------------------------------------
    // LE FOND DU MENU
    // -----------------------------------------------------------
    
    // On donne au fond la largeur du menu et la hauteur de la fenêtre.
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(windowSize.y)));
    
    // On met une couleur noir transparent (Alpha = 150 sur 255) pour voir le jeu derrière.
    m_background.setFillColor(sf::Color(0, 0, 0, 150)); 
    
    // On ajoute une bordure pour délimiter la zone.
    m_background.setOutlineThickness(-1.f); 
    m_background.setOutlineColor(sf::Color(255, 255, 255, 30));

    // -----------------------------------------------------------
    // FPS 
    // -----------------------------------------------------------
    
    m_textFps.setCharacterSize(14);                 // Taille du texte petite
    m_textFps.setFillColor(sf::Color::Green);       // Couleur verte
    m_textFps.setPosition(sf::Vector2f(10.f, 10.f));// En haut à gauche avec une petite marge
    m_textFps.setString("FPS: --");

    // -----------------------------------------------------------
    // STATISTIQUES
    // -----------------------------------------------------------

    m_textTitle.setString("STATISTIQUES");
    m_textTitle.setCharacterSize(18);               // Taille moyenne
    m_textTitle.setFillColor(sf::Color::White);     // Blanc
    m_textTitle.setStyle(sf::Text::Bold);           // En gras

    // Calcul pour centrer le texte :
    // On récupère les dimensions du texte.
    sf::FloatRect bounds = m_textTitle.getLocalBounds();
    
    // On définit le "point d'origine" du texte pile en son milieu.
    m_textTitle.setOrigin(sf::Vector2f(
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    ));
    
    // On place ce point au milieu du menu et à 65px de hauteur.
    m_textTitle.setPosition(sf::Vector2f(m_width / 2.0f, 65.f)); 

    // -----------------------------------------------------------
    // LA BARRE DE SÉPARATION
    // -----------------------------------------------------------
    
    // Une ligne blanche horizontale.
    m_separator.setSize(sf::Vector2f(m_width - 40.f, 2.f)); // Largeur du menu moins les marges
    m_separator.setFillColor(sf::Color::White);
    
    // On centre aussi l'origine de la barre.
    m_separator.setOrigin(sf::Vector2f(m_separator.getSize().x / 2.0f, m_separator.getSize().y / 2.0f));
    
    // On la place juste en dessous du titre.
    m_separator.setPosition(sf::Vector2f(m_width / 2.0f, 85.f)); 

    // -----------------------------------------------------------
    // LES INFORMATIONS
    // -----------------------------------------------------------
    
    m_textInfo.setCharacterSize(16);
    m_textInfo.setFillColor(sf::Color(220, 220, 220)); // Blanc
    m_textInfo.setPosition(sf::Vector2f(20.f, 105.f)); // Alignées à gauche, sous la barre
    m_textInfo.setLineSpacing(1.5f);                   // On espace les lignes pour que ce soit aéré
    m_textInfo.setString("Chargement...");

    return true; // Tout est bon
}


/**
 * @brief Met à jour l'ensemble des textes du HUD (FPS et Statistiques).
 * @details Cette méthode est appelée à chaque frame pour rafraîchir les nombres affichés à l'écran.
 * Elle construit une longue chaîne de caractères contenant toutes les infos (Vivants, Morts, Naissances)
 * et l'envoie à l'objet sf::Text.
 * * @param fps Le nombre d'images par seconde actuel (float).
 * @param grass Nombre d'entités "Herbe" vivantes.
 * @param sheep Nombre d'entités "Mouton" vivantes.
 * @param wolves Nombre d'entités "Loup" vivantes.
 * @param deadS Nombre total de moutons morts depuis le début.
 * @param deadW Nombre total de loups morts depuis le début.
 * @param bornS Nombre total de naissances de moutons.
 * @param bornW Nombre total de naissances de loups.
 */
void Hud::update(float fps, int grass, int sheep, int wolves, int deadS, int deadW, int bornS, int bornW) {
    
    // -----------------------------------------------------------
    // 1. MISE À JOUR DES FPS (Frames Per Second)
    // -----------------------------------------------------------
    // On convertit le float (ex: 59.943) en int (ex: 59) pour la lisibilité.
    // std::to_string transforme le nombre en texte affichable.
    m_textFps.setString("FPS: " + std::to_string(static_cast<int>(fps)));


    // -----------------------------------------------------------
    // 2. CONSTRUCTION DU BLOC DE TEXTE (STATISTIQUES)
    // -----------------------------------------------------------
    // On utilise une chaîne std::string vide qu'on remplit ligne par ligne.
    std::string info = "";
    
    // --- SECTION VIVANTS ---
    // Affiche la population actuelle présente sur l'écran.
    info += "Herbe:   " + std::to_string(grass) + "\n";
    info += "Moutons: " + std::to_string(sheep) + "\n";
    info += "Loups:   " + std::to_string(wolves) + "\n\n"; // \n\n saute une ligne pour aérer

    // --- SECTION MORTS (CIMETIERE) ---
    // Affiche le cumul des pertes depuis le lancement.
    info += "--- MORTS ---\n";
    info += "Moutons: " + std::to_string(deadS) + "\n";
    info += "Loups:   " + std::to_string(deadW) + "\n\n";

    // --- SECTION NAISSANCES ---
    // Affiche le cumul des reproduction (bébés nés) depuis le lancement.
    info += "--- NAISSANCES ---\n";
    info += "Moutons: " + std::to_string(bornS) + "\n";
    info += "Loups:   " + std::to_string(bornW);


    // -----------------------------------------------------------
    // 3. APPLICATION AU RENDU
    // -----------------------------------------------------------
    // On envoie la chaîne complète à l'objet graphique SFML pour qu'elle soit dessinée.
    m_textInfo.setString(info);
}
/**
 * @brief Dessine tous les éléments sur l'écran.
 * @param window La fenêtre où dessiner.
 */
void Hud::draw(sf::RenderWindow& window) {
    // L'ordre est important : ce qu'on dessine en premier est au fond.
    
    window.draw(m_background); // Le fond gris
    window.draw(m_separator);  // La barre blanche
    window.draw(m_textTitle);  // Le titre
    window.draw(m_textFps);    // Les FPS
    window.draw(m_textInfo);   // Les stats
}

/**
 * @brief Appelé quand l'utilisateur change la taille de la fenêtre.
 * Permet d'étirer le fond du menu jusqu'en bas.
 */
void Hud::onResize(sf::Vector2u newSize) {
    // On garde la largeur (m_width), mais on change la hauteur.
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(newSize.y)));
}