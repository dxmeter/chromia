#include "npc_fixe.h"

NPCFixe::NPCFixe() : sprite(texture)
{
    if (!texture.loadFromFile(RESOURCES_DIR "/player/npc_fixe_sprite_sheet.png"))
    {
        return;
    }

    sprite.setTexture(texture, true);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 512, 512 }));
    sprite.setScale({ 0.20f, 0.20f });
}

void NPCFixe::configurer(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void NPCFixe::update(float deltaTime)
{
    sf::IntRect sub_npc({ 0, 0 }, { 512, 512 });
    sub_npc.position.x = (static_cast<int>(anim.getElapsedTime().asSeconds() / 0.15f) % 4) * 512;
    sprite.setTextureRect(sub_npc);
}

sf::FloatRect NPCFixe::getBounds() const
{
    sf::FloatRect rectangle = sprite.getGlobalBounds();

    rectangle.position.x += rectangle.size.x * 0.36f;
    rectangle.position.y += rectangle.size.y * 0.40f;

    rectangle.size.x *= 0.28f;
    rectangle.size.y *= 0.38f;

    return rectangle;
}

void NPCFixe::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}