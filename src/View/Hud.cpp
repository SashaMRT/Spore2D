/**
 * @file Hud.cpp
 * @brief Implémentation de la classe Hud : Gestion de l'interface utilisateur.
 */

#include <iostream> 
#include <string>
#include <iomanip> // Pour std::fixed et std::setprecision
#include "../../include/View/Hud.hpp"

// ... (Constructeur inchangé) ...
Hud::Hud() :
    m_textFps(m_font),
    m_textInfo(m_font),
    m_textTitle(m_font)
{
    m_width = 250.f; 
}

// ... (Init modifié pour le path de la font) ...
bool Hud::init(sf::Vector2u windowSize) {
    
    // TENTATIVE DE CHARGEMENT ROBUSTE
    // On essaye plusieurs chemins possibles selon d'où on lance le programme
    bool loaded = false;
    if (m_font.openFromFile("assets/font.ttf")) loaded = true;
    else if (m_font.openFromFile("../assets/font.ttf")) loaded = true;
    else if (m_font.openFromFile("../../assets/font.ttf")) loaded = true;
    // Si c'est du Linux standard, parfois c'est dans /usr/share/fonts...
    // Mais ici on se contente des dossiers locaux.

    if (!loaded) {
        std::cerr << "[ERREUR CRITIQUE] Impossible de charger la police !" << std::endl;
        std::cerr << "Assurez-vous que le dossier 'assets' est a cote de l'executable." << std::endl;
        // On ne retourne pas false pour ne pas crash, mais le texte sera invisible ou carré.
        // return false; 
    }

    // LE FOND DU MENU
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(windowSize.y)));
    m_background.setFillColor(sf::Color(0, 0, 0, 150)); 
    m_background.setOutlineThickness(-1.f); 
    m_background.setOutlineColor(sf::Color(255, 255, 255, 30));

    // FPS 
    m_textFps.setCharacterSize(14);
    m_textFps.setFillColor(sf::Color::Green);
    m_textFps.setPosition(sf::Vector2f(10.f, 10.f));
    m_textFps.setString("FPS: --");

    // TITRE
    m_textTitle.setString("STATISTIQUES");
    m_textTitle.setCharacterSize(18);
    m_textTitle.setFillColor(sf::Color::White);
    m_textTitle.setStyle(sf::Text::Bold);
    
    sf::FloatRect bounds = m_textTitle.getLocalBounds();
    m_textTitle.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    m_textTitle.setPosition(sf::Vector2f(m_width / 2.0f, 65.f)); 

    // BARRE DE SÉPARATION
    m_separator.setSize(sf::Vector2f(m_width - 40.f, 2.f));
    m_separator.setFillColor(sf::Color::White);
    m_separator.setOrigin(sf::Vector2f(m_separator.getSize().x / 2.0f, m_separator.getSize().y / 2.0f));
    m_separator.setPosition(sf::Vector2f(m_width / 2.0f, 85.f)); 

    // INFORMATIONS
    m_textInfo.setCharacterSize(16);
    m_textInfo.setFillColor(sf::Color(220, 220, 220));
    m_textInfo.setPosition(sf::Vector2f(20.f, 105.f));
    m_textInfo.setLineSpacing(1.4f);
    m_textInfo.setString("Chargement...");

    return true;
}

// ... (Update modifié pour afficher le temps et le mode Pause) ...
void Hud::update(float fps, int grass, int sheep, int wolves, int deadS, int deadW, int bornS, int bornW) {
    
    m_textFps.setString("FPS: " + std::to_string(static_cast<int>(fps)));

    std::string info = "";
    
    // TEMPS SIMULÉ (Récupéré via une variable globale ou passée en paramètre, 
    // ici on suppose qu'on l'ajoute plus tard, mais préparons l'affichage)
    // Pour l'instant on ne l'a pas dans les arguments, on va le rajouter dans Simulation.hpp
    
    info += "Herbe:   " + std::to_string(grass) + "\n";
    info += "Moutons: " + std::to_string(sheep) + "\n";
    info += "Loups:   " + std::to_string(wolves) + "\n\n";

    info += "--- MORTS ---\n";
    info += "Moutons: " + std::to_string(deadS) + "\n";
    info += "Loups:   " + std::to_string(deadW) + "\n\n";

    info += "--- NAISSANCES ---\n";
    info += "Moutons: " + std::to_string(bornS) + "\n";
    info += "Loups:   " + std::to_string(bornW) + "\n\n";
    
    // Commandes
    info += "[P]ause / [R]eset";

    m_textInfo.setString(info);
}

// ... (Draw et onResize inchangés) ...
void Hud::draw(sf::RenderWindow& window) {
    window.draw(m_background);
    window.draw(m_separator);
    window.draw(m_textTitle);
    window.draw(m_textFps);
    window.draw(m_textInfo);
}

void Hud::onResize(sf::Vector2u newSize) {
    m_background.setSize(sf::Vector2f(m_width, static_cast<float>(newSize.y)));
}