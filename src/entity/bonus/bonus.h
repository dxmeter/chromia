#pragma once

#ifndef BONUS_H
#define BONUS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Bonus
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

	bool isCollected = false;
	int type = 1;

	sf::Clock anim;

public:
	Bonus();

	void configurer(sf::Vector2f position, int type);
	void update(float deltaTime);

	sf::FloatRect getBounds() const;
	int getType();
	void collecter();
	bool estCollecte() const;

	void recommencer();

	void draw(sf::RenderWindow& window);
};

#endif
