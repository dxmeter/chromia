#include "player.h"
#include <iostream>
#include <cmath>

Player::Player() : sprite(texture)
{
    if (!texture.loadFromFile(RESOURCES_DIR "/player/player_sprite_sheet.png"))
    {
        return;
    }

    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    sprite.setScale({ 0.25f, 0.25f });
    sprite.setPosition({ 50.f, 1500.f });
    positionDepart = sprite.getPosition();
    positionDebutNiveau = sprite.getPosition();

    sonSautCharge = bufferSaut.loadFromFile(RESOURCES_DIR "/sound/jump.wav");
    sonSaut.setVolume(60.f);
    sonDegatCharge = bufferDegat.loadFromFile(RESOURCES_DIR "/sound/impact.wav");
    sonDegat.setVolume(20.f);
    sonMarcheCharge = bufferMarche.loadFromFile(RESOURCES_DIR "/sound/walk.wav");
    sonMarche.setVolume(75.f);
}

void Player::update(float deltaTime, bool isMoving, const Level& level)
{
    if (vie == 0) {
        return;
    }

    if (bonusVitesseActif && chronoBonusVitesse.getElapsedTime().asSeconds() >= 5.f) {
        bonusVitesseActif = false;
    }

    if (isInvicible && chronoDegats.getElapsedTime().asSeconds() >= 1.f)
        isInvicible = false;

    sf::FloatRect pieds = getBounds();
    pieds.position.y += 1.f;
    onFloor = vitesseY >= 0.f && level.toucheDecor(pieds);

    if (onFloor) {
        vitesseX = 100.f;
    }
    else {
        vitesseX = 150.f;
    }
    if (bonusVitesseActif)
    {
        vitesseX = vitesseX * 2.f;
    }

    isMoving = false;
    bool toLeft = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        moveWithCollision(-vitesseX * deltaTime, true, level);
        isMoving = true;
        toLeft = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        moveWithCollision(vitesseX * deltaTime, true, level);
        isMoving = true;
        toLeft = false;
    }

    bool space = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (space && !spaceAvant && onFloor)
    {
        vitesseY = -forceSaut;
        onFloor = false;
        sonSaut.play();
    }
    spaceAvant = space;
    
    vitesseY += gravite * deltaTime;
    onFloor = false;
    moveWithCollision(vitesseY * deltaTime, false, level);

    if (isMoving && onFloor)
    {
        if (sonMarche.getStatus() != sf::Sound::Status::Playing)
        {
            sonMarche.play();
        }
    }
    else
    {
        sonMarche.stop();
    }

    if (isMoving)
    {
        sf::IntRect sub_player({ 0, 0 }, { 512, 512 });
        int image = static_cast<int>(anim.getElapsedTime().asSeconds() / 0.15f) % 4;

        sf::Vector2f echelle = sprite.getScale();
        float tailleX = std::abs(echelle.x);

        if (toLeft)
        {
            sprite.setOrigin({ 512.f, 0.f });
            sprite.setScale({ -tailleX, echelle.y });
        }
        else
        {
            sprite.setOrigin({ 0.f, 0.f });
            sprite.setScale({ tailleX, echelle.y });
        }
        sub_player.position.x = image * 512;
        sprite.setTextureRect(sub_player);
    }
}

bool Player::touchePlateforme(const Level& level, float direction, bool horizontal) const
{
    sf::FloatRect rectangle = getBounds();
    int gauche = static_cast<int>(std::floor(rectangle.position.x));
    int droite = static_cast<int>(std::floor(rectangle.position.x + rectangle.size.x - 0.01f));
    int haut = static_cast<int>(std::floor(rectangle.position.y));
    int bas = static_cast<int>(std::floor(rectangle.position.y + rectangle.size.y - 0.01f));

    if (horizontal)
    {
        int bord = gauche;
        if (direction > 0.f)
            bord = droite;

        for (int y = haut; y <= bas; y++)
        {
            if (level.isColliding(bord, y))
                return true;
        }
    }
    else
    {
        int bord = haut;
        if (direction > 0.f)
            bord = bas;

        for (int x = gauche; x <= droite; x++)
        {
            if (level.isColliding(x, bord))
                return true;
        }
    }
    return false;
}

void Player::moveWithCollision(float distance, bool horizontal, const Level& level)
{
    while (distance != 0.f)
    {
        float pas = distance;

        if (pas > 1.f)
            pas = 1.f;
        else if (pas < -1.f)
            pas = -1.f;

        sf::Vector2f avant = sprite.getPosition();

        if (horizontal)
            sprite.move({ pas, 0.f });
        else
            sprite.move({ 0.f, pas });

        if (touchePlateforme(level, pas, horizontal))
        {
            sprite.setPosition(avant);

            if (!horizontal)
            {
                if (pas > 0.f)
                    onFloor = true;
                vitesseY = 0.f;
            }
            break;
        }
        distance -= pas;
    }
}

sf::Vector2f Player::getCameraTarget() const
{
    sf::Vector2f position = sprite.getPosition();
    return { position.x + 64.f, position.y + 68.f };
}

sf::FloatRect Player::getBounds() const
{
    sf::Vector2f position = sprite.getPosition();

    return sf::FloatRect({ position.x + 44.f, position.y + 39.f },{ 40.f, 58.f }
    );
}

void Player::perdreVie()
{

    if (vie > 0 && !isInvicible)
    {
        
        vie = vie - 1;
        if (sonDegatCharge)
        {
            sonDegat.play();
        }
        sprite.setPosition(positionDepart);
        vitesseY = 0.f;
        onFloor = false;
        isInvicible = true;
        chronoDegats.restart();
    }
}

void Player::memoPosition(const Level& level)
{
    sf::FloatRect rectangle = getBounds();
    int y = static_cast<int>(std::floor(rectangle.position.y + rectangle.size.y + 1.f));
    int gauche = static_cast<int>(rectangle.position.x + 2.f);
    int droite = static_cast<int>(rectangle.position.x + rectangle.size.x - 2.f);

    if (onFloor && !isInvicible && level.positionSure(rectangle)
        && level.isColliding(gauche, y) && level.isColliding(droite, y))
    {
        positionDepart = sprite.getPosition();
    }
}

void Player::gagnerVie()
{
    vie = vie + 1;
}

int Player::getVies() const
{
    return vie;
}

void Player::activerBonusVitesse()
{
    bonusVitesseActif = true;
    chronoBonusVitesse.restart();
}

void Player::recommencer()
{
    vie = 3;
    sprite.setPosition(positionDebutNiveau);
    positionDepart = positionDebutNiveau;
    vitesseY = 0.f;
    onFloor = false;
    spaceAvant = false;
    isInvicible = false;
    bonusVitesseActif = false;
    sprite.setOrigin({ 0.f, 0.f });
    sprite.setScale({ 0.25f, 0.25f });
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    anim.restart();
}

void Player::draw(sf::RenderWindow& window)
{
    if (isInvicible && static_cast<int>(chronoDegats.getElapsedTime().asSeconds() / 0.1f) % 2 == 0)
        return;
    window.draw(sprite);
}

void Player::definirDepart(sf::Vector2f position)
{
    positionDebutNiveau = position;
}
