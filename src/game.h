#ifndef SFMLPROJECT_GAME_H
#define SFMLPROJECT_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>

#include "entity/player/player.h"
#include "level/level.h"

class Game
{
public:
    Game();

    void initialize();
    void loop();

    void updateThings();
    void drawThings();

    void onResize(sf::Vector2u size);

private:
    // Paramètre fenêtre
    sf::RenderWindow window;
    sf::View view;
    sf::View vueInterface;
    sf::Clock clock;

    // Joueur et contenu du niveau
    Player player;
    Level level;


    // Interface et couleur de fond
    int ecran = 0;
    sf::Texture textureMenu;
    sf::Texture textureHistoire;
    sf::Texture textureCoeur;
    sf::Texture textureGameOver;
    sf::Texture textureVictoire;
    sf::Texture textureChoixNiveaux;
    sf::Color backgroundColor = sf::Color::Black;

    //Condition victoire
    bool victoire = false;

    // Audio
    sf::Music musique;

    sf::Font policeInterface;
    bool policeChargee = false;
    int numeroNiveau = 1;
    int niveauChoisi = 1; 
    void drawInterface();
    void recommencerPartie();
    bool changerNiveau(int numero);
    bool erreurNiveau = false;
};

#endif
