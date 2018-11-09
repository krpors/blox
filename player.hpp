#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "level.hpp"

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:
	const Map* map;

	sf::FloatRect position;

	sf::Texture texture;
	sf::Sprite playerSprite;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;
public:
	Player(const Map* const map);
	~Player();

	void handleEvent(const sf::Event& event);
	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PLAYER_HPP