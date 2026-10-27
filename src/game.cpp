#include "game.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

Game::Game()
{
    window.create(sf::VideoMode({ 1280, 720 }), "Game");

    vueInterface = window.getDefaultView();
    view = window.getDefaultView();
    view.setSize({ 960.f, 960.f * 624.f / 1280.f });

    // Paremètre ne dessous qui permet la vérification de l'ensemble du niveau
    //view.setSize({ 1680.f * 1280.f / 624.f, 1680.f });
    onResize(window.getSize());
    view.setCenter({ 480.f, 1680.f - view.getSize().y / 2.f });

    window.setFramerateLimit(60);

    window.setKeyRepeatEnabled(false);
}

void Game::initialize()
{
    textureMenu.loadFromFile(RESOURCES_DIR "/interface/menu_screen_1280x720.png");
    textureChoixNiveaux.loadFromFile(RESOURCES_DIR "/interface/choix_screen_1280x720.png");
    textureHistoire.loadFromFile(RESOURCES_DIR "/interface/histoire_screen_1280x720.png");
    textureGameOver.loadFromFile(RESOURCES_DIR "/interface/gameover_screen_1280x720.png");
    textureVictoire.loadFromFile(RESOURCES_DIR "/interface/victoire_screen_1280x720.png");
    textureCoeur.loadFromFile(RESOURCES_DIR "/interface/coeur_vie.png");

    policeChargee = policeInterface.openFromFile(RESOURCES_DIR "/police/minecraft.ttf");

    bool themeMusic = musique.openFromFile(RESOURCES_DIR "/music/theme_music.mp3");

    if (themeMusic)
    {
        musique.setVolume(10.f);
        musique.setLooping(true);
        musique.play();
    }
    else
    {
        std::cout << "Impossible de charger la musique\n";
    }

    // 1er niveau
    changerNiveau(1);

    while (window.isOpen())
    {
        loop();
    }
}

void Game::loop()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* resized =event->getIf<sf::Event::Resized>())
        {
            onResize(resized->size);
        }
        else if (const auto* clic =event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (clic->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f point =window.mapPixelToCoords(clic->position, vueInterface);

                int x = static_cast<int>(point.x);
                int y = static_cast<int>(point.y);
                std::cout << "Ecran : " << ecran
                    << " ; clic : " << x << ", " << y << '\n';

                // ACCUEIL
                if (ecran == 0)
                {
                    if (x >= 550 && x <= 730 && y >= 460 && y <= 510)
                    {
                        ecran = 1;
                    }
                    else if (x >= 515 && x <= 765 && y >= 525 && y <= 570)
                    {
                        window.close();
                    }
                }
                // HISTOIRE
                else if (ecran == 1)
                {
                    if (x >= 485 && x <= 795 && y >= 605 && y <= 655)
                    { 
                        ecran = 3;
                    }
                }
                // CHOIX DU NIVEAU
                else if (ecran == 3)
                {
                    if (x >= 495 && x <= 785 && y >= 392 && y <= 436)
                    {
                        if (changerNiveau(1))
                            ecran = 2;
                    }
                    else if (x >= 495 && x <= 785 && y >= 462 && y <= 506)
                    {
                        if (changerNiveau(2))
                            ecran = 2;
                    }
                    else if (x >= 550 && x <= 730 && y >= 546 && y <= 582)
                    {
                        ecran = 1;
                    }
                }
                // GAME OVER ET VICTOIRE
                else if (ecran == 2)
                {
                    bool clicGameOver =player.getVies() == 0 &&x >= 530 && x <= 750 &&y >= 460 && y <= 515;
                    bool clicVictoire =victoire &&x >= 525 && x <= 755 &&y >= 495 && y <= 535;

                    bool clicMenuGameOver =player.getVies() == 0 &&x >= 560 && x <= 720 &&y >= 530 && y <= 580;
                    bool clicMenuVictoire =victoire &&x >= 560 && x <= 720 &&y >= 550 && y <= 600;

                    if (clicMenuGameOver || clicMenuVictoire)
                    {
                        changerNiveau(1);
                        ecran = 0;
                    }
                    else if (clicGameOver)
                    {
                        recommencerPartie();
                    }
                    else if (clicVictoire)
                    {
                        if (numeroNiveau == 1)
                        {
                            changerNiveau(2);
                        }
                        else
                            changerNiveau(1);
                        
                    }
                }
            }
        }
        else if (const auto* touche =event->getIf<sf::Event::KeyPressed>())
        {
            if (touche->code == sf::Keyboard::Key::Enter)
            {
                if (ecran == 0)
                {
                    ecran = 1;
                }
                else if (ecran == 1)
                {
                    ecran = 3;
                }
                else if (ecran == 2)
                {
                    if (victoire)
                    {
                        if (numeroNiveau == 1)
                            changerNiveau(2);
                        else
                            changerNiveau(1);
                    }
                }
            }

            if (touche->code == sf::Keyboard::Key::Escape)
            {
                if (ecran == 0)
                {
                    window.close();
                }
                else if (ecran == 1)
                {
                    ecran = 0;
                }
                else if (ecran == 3)
                {
                    ecran = 1;
                }
                else
                {
                    changerNiveau(1);
                    ecran = 0;
                }
            }
        }
    }

    if (!window.isOpen())
        return;

    updateThings();

    window.clear(backgroundColor);
    drawThings();
    window.display();
}

void Game::updateThings()
{
    float deltaTime = clock.restart().asSeconds();

    if (deltaTime > 0.05f)
        deltaTime = 0.05f;

    if (ecran != 2 || victoire || player.getVies() == 0)
        return;

    player.update(deltaTime, false, level);
    level.updateFixes(deltaTime);
    level.updateMobiles(deltaTime);

    bool contactFixe = level.toucheFixe(player.getBounds());
    bool contactMobile = level.toucheMobile(player.getBounds());
    if (contactFixe || contactMobile)
        player.perdreVie(); 

    if (player.getVies() == 0)
        return;

    player.memoPosition(level);
    level.updateCollectibles(deltaTime);
    backgroundColor = level.recupererCollectibles(player.getBounds(), backgroundColor);

    if (level.recupererBonusVie(player.getBounds()))
        player.gagnerVie();
    if (level.recupererBonusVitesse(player.getBounds()))
        player.activerBonusVitesse();

    // Déplacement caméra rapporté au joueur
    sf::Vector2f camera = player.getCameraTarget();
    float demiHauteur = view.getSize().y / 2.f;
    if (camera.y < demiHauteur)
        camera.y = demiHauteur;
    if (camera.y > 1680.f - demiHauteur)
        camera.y = 1680.f - demiHauteur;
    view.setCenter({ 480.f, camera.y });

    bool contactPorte = player.getBounds().findIntersection(level.getPorteBounds()).has_value();
    if (level.tousCollectiblesRecuperes() && contactPorte)
    {
            victoire = true;
    }
}

void Game::drawThings()
{
    // ACCUEIL
    if (ecran == 0)
    {
        window.setView(vueInterface);

        sf::Sprite menu(textureMenu);
        window.draw(menu);
        return;
    }

    // HISTOIRE
    if (ecran == 1)
    {
        window.setView(vueInterface);

        sf::Sprite histoire(textureHistoire);
        window.draw(histoire);
        return;
    }

    // CHOIX DU NIVEAU
    if (ecran == 3)
    {
        window.setView(vueInterface);

        sf::Sprite choix(textureChoixNiveaux);
        window.draw(choix);
        return;
    }

    // JEU
    window.setView(view);
    level.draw(window);

    if (player.getVies() > 0)
    {
        level.drawFixes(window);
        level.drawMobiles(window);
        level.drawCollectibles(window);
        player.draw(window);
    }

    // INTERFACE
    window.setView(vueInterface);
    drawInterface();

    if (player.getVies() == 0)
    {
        sf::Sprite gameOver(textureGameOver);
        window.draw(gameOver);
    }

    if (victoire)
    {
        sf::Sprite ecranVictoire(textureVictoire);
        window.draw(ecranVictoire);
    }

    window.setView(view);
}

void Game::drawInterface()
{
    window.setView(vueInterface);
    sf::RectangleShape bande({ 1260.f, 96.f });
    bande.setFillColor(sf::Color(255, 246, 234));
    window.draw(bande);

    sf::Sprite coeur(textureCoeur);
    coeur.setScale({ 1.1f, 1.1f });
    coeur.setColor(sf::Color(backgroundColor));

    for (int i = 0; i < player.getVies() && i < 6; i++)
    {
        coeur.setPosition({ 225.f + i * 38.f, 46.f });
        window.draw(coeur);
    }
    level.drawAnneauxInterface(window);

    if (policeChargee)
    {
        sf::Text texte(policeInterface, "NIVEAU " + std::to_string(numeroNiveau), 23);
        texte.setFillColor(sf::Color(backgroundColor));
        texte.setPosition({ 24.f, 18.f });
        window.draw(texte);
        texte.setCharacterSize(14);
        texte.setString("ECHAP : MENU");
        texte.setPosition({ 24.f, 57.f });
        window.draw(texte);

        texte.setCharacterSize(20);
        texte.setString("VIES : " + std::to_string(player.getVies()));
        texte.setPosition({ 225.f, 14.f });
        window.draw(texte);

        texte.setString("ANNEAUX : " + std::to_string(level.getNombreCollectibles()) + " / 7");
        texte.setPosition({ 785.f, 10.f });
        window.draw(texte);
    }
}

void Game::recommencerPartie()
{
    player.recommencer();
    level.recommencerCollectibles();
    level.recommencerMobiles();
    backgroundColor = sf::Color::Black;
    victoire = false;
    erreurNiveau = false;
    clock.restart();
    sf::Vector2f camera = player.getCameraTarget();
    float demiHauteur = view.getSize().y / 2.f;
    if (camera.y > 1680.f - demiHauteur)
        camera.y = 1680.f - demiHauteur;
    if (camera.y < demiHauteur)
        camera.y = demiHauteur;
    view.setCenter({ 480.f, camera.y });
}


bool Game::changerNiveau(int numero)
{
    if (!level.charger(numero))
    {
        std::cout << "Chargement impossible du niveau " << numero << '\n';
        return false;
    }
    numeroNiveau = numero;
    if (numero == 1)
        player.definirDepart({ 250.f, 1500.f }); // Map 1
    else
        player.definirDepart({ 50.f, 1500.f });  // Map 2
    recommencerPartie();
    return true;
}

void Game::onResize(sf::Vector2u size)
{
    // Pour conserver les proportions
    if (size.x == 0 || size.y == 0)
        return;

    float largeur = 1.f;
    float hauteur = 1.f;
    float rapport = static_cast<float>(size.x) / size.y;
    if (rapport > 1280.f / 720.f)
        largeur = (1280.f / 720.f) / rapport;
    else
        hauteur = rapport / (1280.f / 720.f);

    float gauche = (1.f - largeur) / 2.f;
    float haut = (1.f - hauteur) / 2.f;
    vueInterface.setViewport(sf::FloatRect({ gauche, haut }, { largeur, hauteur }));
    view.setViewport(sf::FloatRect(
        { gauche, haut + hauteur * 96.f / 720.f },
        { largeur, hauteur * 624.f / 720.f }));
}

int main() {
    Game game;
    game.initialize();
}