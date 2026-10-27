#include "collectible.h"
#include <iostream>

Collectible::Collectible() :sprite(texture)
{
    if (!texture.loadFromFile(RESOURCES_DIR "/collectible/collectible_sprite_sheet.png"))
    {
        return;
    }

    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    sprite.setScale({ 0.12f, 0.12f });
    sprite.setColor(couleur);
}

void Collectible::configurer(sf::Vector2f position, sf::Color newColor)
{
    sprite.setPosition(position);
    couleur = newColor;
    sprite.setColor(couleur);
}

void Collectible::update(float deltaTime)
{
    sf::IntRect sub_collectible({ 0, 0 }, { 512, 512 });
    int image = static_cast<int>(anim.getElapsedTime().asSeconds() / 0.15f) % 4;

    sub_collectible.position.x = image * 512;
    sprite.setTextureRect(sub_collectible);
}

sf::FloatRect Collectible::getBounds() const
{
    sf::FloatRect rectangle = sprite.getGlobalBounds();

    rectangle.position.x += rectangle.size.x * 0.20f;
    rectangle.position.y += rectangle.size.y * 0.20f;

    rectangle.size.x *= 0.60f;
    rectangle.size.y *= 0.60f;

    return rectangle;
}

void Collectible::collecter()
{
    isCollected = true;
}

sf::Color Collectible::getColor() const
{
    return couleur;
}

bool Collectible::estCollecte() const
{
    return isCollected;
}

void Collectible::recommencer()
{
    isCollected = false;
    anim.restart();
}

void Collectible::draw(sf::RenderWindow& window)
{
    if (isCollected)
        return;

    sf::FloatRect rectangle = sprite.getGlobalBounds();

    sf::Vector2f centre = { rectangle.position.x + rectangle.size.x / 2.f,rectangle.position.y + rectangle.size.y / 2.f };

    float rayonAnneau = rectangle.size.x / 2.f;

    for (int i = 4; i >= 1; i--)
    {
        float rayon = rayonAnneau + i * 3.f;

        sf::CircleShape halo(rayon);
        halo.setOrigin({ rayon, rayon });
        halo.setPosition(centre);

        sf::Color couleurHalo = couleur;
        couleurHalo.a = 15;
        halo.setFillColor(couleurHalo);

        window.draw(halo);
    }

    window.draw(sprite);
}
void Collectible::drawInterface(sf::RenderWindow& window, sf::Vector2f position) const
{
    sf::Sprite icone(texture);
    icone.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    icone.setScale({ 0.10f, 0.10f });
    icone.setPosition(position);
    if (isCollected)
        icone.setColor(couleur);
    else
        icone.setColor(sf::Color(65, 65, 70));
    window.draw(icone);
}
