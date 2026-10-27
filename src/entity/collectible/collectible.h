#pragma once

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <SFML/Graphics.hpp>

class Collectible
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Color couleur = sf::Color::White;
	bool isCollected = false;
	sf::Clock anim;

public:
	Collectible();
	void configurer(sf::Vector2f position, sf::Color newColor);
	void update(float deltaTime);
	sf::FloatRect getBounds() const;
	void collecter();
	sf::Color getColor() const;
	bool estCollecte() const;
	void recommencer();
	void draw(sf::RenderWindow& window);
	void drawInterface(sf::RenderWindow& window, sf::Vector2f position) const;
};

#endif