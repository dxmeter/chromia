#include "level.h"
#include <iostream>
#include <cmath>
#include <string>
#include <utility>

Level::Level() : background(backgroundTexture), platforms(platformsTexture), porte(porteTexture)
{
    charger(1);
    sonCollecteCharge = bufferCollecte.loadFromFile(RESOURCES_DIR "/sound/collecting.wav");
    sonCollecte.setVolume(50.f);
}

bool Level::charger(int numero)
{
    if (numero != 1 && numero != 2)
        return false;

    // J'ai inversé les maps dans l'ordre de difficulté
    int numeroMap;
    if (numero == 1)
        numeroMap = 2;
    else
        numeroMap = 1;

    std::string dossier = std::string(RESOURCES_DIR) + "/level" + std::to_string(numeroMap);
    std::string nomMap = "/map_" + std::to_string(numeroMap);

    sf::Texture nouvellesPlateformes;
    sf::Image nouveauMasque;
    if (!nouvellesPlateformes.loadFromFile(dossier + nomMap + ".png") ||
        !nouveauMasque.loadFromFile(dossier + nomMap + "_collision.png"))
    {
        return false;
    }
    if (nouvellesPlateformes.getSize() != sf::Vector2u(960, 1680) ||
        nouveauMasque.getSize() != sf::Vector2u(960, 1680))
    {
        return false;
    }
    if (porteTexture.getSize().x == 0 &&
        !porteTexture.loadFromFile(RESOURCES_DIR "/level1/door.png"))
        return false;

    // Transfert des données de la map 1 à la map 2 
    platformsTexture = std::move(nouvellesPlateformes);
    collisionMask = std::move(nouveauMasque);
    platforms.setTexture(platformsTexture, true);
    platforms.setPosition({ 0.f, 0.f });
    porte.setTexture(porteTexture, true);
    porte.setScale({ 0.15f, 0.15f });

    // Arbres background rajoutés plus tard
    //backgroundCharge = false;
    //if (std::filesystem::exists(dossier + nomMap + "_background.png"))
    //{
    //    backgroundCharge = backgroundTexture.loadFromFile(dossier + nomMap + "_background.png");
    //    if (backgroundCharge)
    //        background.setTexture(backgroundTexture, true);
    //}

    if (numeroMap == 1)
    {
        // Map 2
        nombreFixes = 10;
        nombreMobiles = 4;
        porte.setPosition({ 750.f, 105.f });

        // Ennemis fixes
        ennemisFixes[0].configurer({ 525.f, 1570.f });
        ennemisFixes[1].configurer({ 585.f, 1570.f });
        ennemisFixes[2].configurer({ 790.f, 1570.f });
        ennemisFixes[3].configurer({ 850.f, 1570.f });
        ennemisFixes[4].configurer({ 850.f, 1570.f });
        ennemisFixes[5].configurer({ 510.f, 1270.f });
        ennemisFixes[6].configurer({ 200.f, 875.f });
        ennemisFixes[7].configurer({ 750.f, 555.f });
        ennemisFixes[8].configurer({ 550.f, 370.f });
        ennemisFixes[9].configurer({ 610.f, 370.f });

        // Ennemis mobiles 
        ennemisMobiles[0].configurer({ 540.f, 1380.f }, 500.f, 775.f);
        ennemisMobiles[1].configurer({ 350.f, 990.f }, 40.f, 350.f);
        ennemisMobiles[2].configurer({ 650.f, 700.f }, 500.f, 775.f);
        ennemisMobiles[3].configurer({ 450.f, 225.f }, 450.f, 750.f);

        // Collectibles 
        collectibles[0].configurer({ 340.f, 1370.f }, sf::Color(40, 75, 130)); //Bleu
        collectibles[1].configurer({ 780.f, 1270.f }, sf::Color(130, 112, 32)); //Jaune
        collectibles[2].configurer({ 100.f, 1125.f }, sf::Color(140, 40, 0)); //Rouge
        collectibles[3].configurer({ 50.f, 815.f }, sf::Color(40, 100, 70)); //Vert
        collectibles[4].configurer({ 330.f, 465.f }, sf::Color(150, 80, 32)); //Orange
        collectibles[5].configurer({ 80.f, 185.f }, sf::Color(90, 40, 160)); //Violet
        collectibles[6].configurer({ 750.f, 340.f }, sf::Color(112, 60, 130)); //Rose

        //Bonus 
        bonusVie.configurer({ 825.f, 555.f }, 1);
        bonusVitesse[0].configurer({ 300.f, 650.f }, 2);
        bonusVitesse[1].configurer({ 650.f, 950.f }, 2);

    }
    else
    {
        // Map 1
        nombreFixes = 5;
        nombreMobiles = 4;
        porte.setPosition({ 800.f, 182.f - porte.getGlobalBounds().size.y });

        ennemisFixes[0].configurer({ 650.f, 1275.f }); 
        ennemisFixes[1].configurer({ 500.f, 1020.f }); 
        ennemisFixes[2].configurer({ 600.f, 550.f });
        ennemisFixes[3].configurer({ 600.f, 1275.f });
        ennemisFixes[4].configurer({ 100.f, 186.f });

        ennemisMobiles[0].configurer({ 700.f, 975.f }, 30.f, 400.f);
        ennemisMobiles[1].configurer({ 320.f, 450.f }, 600.f, 700.f);
        ennemisMobiles[2].configurer({ 100.f, 20.f }, 200.f, 700.f);
        ennemisMobiles[3].configurer({ 10.f, 1250.f }, 50.f, 400.f);

        collectibles[0].configurer({ 450.f, 1420.f }, sf::Color(40, 75, 130));
        collectibles[1].configurer({ 805.f, 1205.f }, sf::Color(130, 112, 32));
        collectibles[2].configurer({ 725.f, 1020.f }, sf::Color(140, 40, 0));
        collectibles[3].configurer({ 230.f, 890.f }, sf::Color(40, 100, 70));
        collectibles[4].configurer({ 200.f, 700.f }, sf::Color(150, 80, 32));
        collectibles[5].configurer({ 750.f, 525.f }, sf::Color(90, 40, 160));
        collectibles[6].configurer({ 340.f, 105.f }, sf::Color(112, 60, 130));

        bonusVie.configurer({ 70.f, 835.f }, 1);
        bonusVitesse[0].configurer({ 200.f, 1190.f }, 2);
        bonusVitesse[1].configurer({ 390.f, 520.f }, 2);
    }
    recommencerCollectibles();
    recommencerMobiles();
    return true;
}

// Collisions avec plateformes
bool Level::isColliding(int x, int y) const
{
    sf::Vector2u taille = collisionMask.getSize();

    if (x < 0 || y < 0 ||
        x >= static_cast<int>(taille.x) || y >= static_cast<int>(taille.y))
    {
        return true;
    }
    return collisionMask.getPixel({ static_cast<unsigned int>(x),static_cast<unsigned int>(y) }).a > 128;
}

// Ennemis fixes
void Level::updateFixes(float deltaTime)
{
    for (int i = 0; i < nombreFixes; i++)
    {
        ennemisFixes[i].update(deltaTime);
    }
}

void Level::drawFixes(sf::RenderWindow& window)
{
    for (int i = 0; i < nombreFixes; i++)
    {
        ennemisFixes[i].draw(window);
    }
}

bool Level::toucheFixe(sf::FloatRect rectangle) const
{
    for (int i = 0; i < nombreFixes; i++)
    {
        if (rectangle.findIntersection(ennemisFixes[i].getBounds()))
        {
            return true;
        }
    }

    return false;
}

sf::FloatRect Level::getFixeBounds(int indice) const
{
    return ennemisFixes[indice].getBounds();
}

// Ennemis mobiles
void Level::updateMobiles(float deltaTime)
{
    for (int i = 0; i < nombreMobiles; i++)
    {
        ennemisMobiles[i].update(deltaTime);
    }
}

void Level::drawMobiles(sf::RenderWindow& window)
{
    for (int i = 0; i < nombreMobiles; i++)
    {
        ennemisMobiles[i].draw(window);
    }
}

bool Level::toucheMobile(sf::FloatRect rectangle) const
{
    for (int i = 0; i < nombreMobiles;i++)
    {
        if (rectangle.findIntersection(ennemisMobiles[i].getBounds()))
        {
            return true;
        }
    }

    return false;
}

sf::FloatRect Level::getMobileBounds(int indice) const
{
    return ennemisMobiles[indice].getBounds();
}

// Collectibles et bonus
void Level::updateCollectibles(float deltaTime)
{
    for (int i = 0; i < 7; i++)
    {
        collectibles[i].update(deltaTime);
    }

    for (int i = 0; i < 2; i++)
    {
        bonusVitesse[i].update(deltaTime);
    }

    bonusVie.update(deltaTime);

}

void Level::drawCollectibles(sf::RenderWindow& window)
{
    for (int i = 0; i < 7; i++)
    {
        collectibles[i].draw(window);
    }

    for (int i = 0; i < 2; i++)
    {
        bonusVitesse[i].draw(window);
    }

    bonusVie.draw(window);
}

sf::Color Level::recupererCollectibles(sf::FloatRect joueur, sf::Color couleurActuelle)
{
    for (int i = 0; i < 7; i++)
    {
        if (!collectibles[i].estCollecte())
        {
            if (joueur.findIntersection(collectibles[i].getBounds()))
            {
                collectibles[i].collecter();
                if (sonCollecteCharge)
                {
                    sonCollecte.play();
                }
                couleurActuelle = collectibles[i].getColor();
            }
        }
    }

    return couleurActuelle;
}

void Level::recommencerCollectibles()
{
    for (int i = 0; i < 7; i++)
    {
        collectibles[i].recommencer();
    }

    for (int i = 0; i < 2; i++)
    {
        bonusVitesse[i].recommencer();
    }

    bonusVie.recommencer();

}

bool Level::tousCollectiblesRecuperes() const
{
    for (int i = 0; i < 7; i++)
    {
        if (!collectibles[i].estCollecte())
            return false;
    }
    return true;
}

// Bonus
bool Level::recupererBonusVie(sf::FloatRect joueur)
{
    if (!bonusVie.estCollecte())
    {
        if (joueur.findIntersection(bonusVie.getBounds()))
        {
            bonusVie.collecter();
            sonCollecte.play();
            return true;
        }
    }
    return false;
}

bool Level::recupererBonusVitesse(sf::FloatRect joueur)
{
    for (int i = 0; i < 2; i++)
    {
        if (!bonusVitesse[i].estCollecte() && joueur.findIntersection(bonusVitesse[i].getBounds()))
        {
            bonusVitesse[i].collecter();
            sonCollecte.play();
            return true;
        }
    }

    return false;
}

sf::FloatRect Level::getPorteBounds() const
{
    return porte.getGlobalBounds();
}

// Décor du niveau
void Level::draw(sf::RenderWindow& window)
{
    if (backgroundCharge)
        window.draw(background);
    window.draw(platforms);
    window.draw(porte);
}

// Condition victoire
bool Level::toucheDecor(sf::FloatRect rectangle) const
{
    // Délimitation de la porte
    int gauche = static_cast<int>(std::floor(rectangle.position.x));
    int droite = static_cast<int>(std::floor(rectangle.position.x + rectangle.size.x - 0.01f));
    int haut = static_cast<int>(std::floor(rectangle.position.y));
    int bas = static_cast<int>(std::floor(rectangle.position.y + rectangle.size.y - 0.01f));

    // Lorsque le joueur franchit cette zone
    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (isColliding(x, y))
                return true;
        }
    }
    return false;
}

bool Level::positionSure(sf::FloatRect rectangle) const
{
    if (toucheDecor(rectangle))
        return false;

    rectangle.position.x -= 70.f;
    rectangle.position.y -= 70.f;
    rectangle.size.x += 140.f;
    rectangle.size.y += 140.f;
    if (toucheFixe(rectangle))
        return false;
    for (int i = 0; i < nombreMobiles; i++)
    {
        if (rectangle.findIntersection(ennemisMobiles[i].getZonePatrouille()))
            return false;
    }
    return true;
}

int Level::getNombreCollectibles() const
{
    int nombre = 0;
    for (int i = 0; i < 7; i++)
    {
        if (collectibles[i].estCollecte())
            nombre++;
    }
    return nombre;
}

void Level::drawAnneauxInterface(sf::RenderWindow& window)
{
    for (int i = 0; i < 7; i++)
        collectibles[i].drawInterface(window, { 775.f + i * 66.f, 34.f });
}

void Level::recommencerMobiles()
{
    for (int i = 0; i < nombreMobiles; i++)
        ennemisMobiles[i].recommencer();
}
