#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:
	sf::IntRect prect;
	sf::Texture texture;
	sf::Sprite playerSprite;

	bool moveLeft = false;
	bool moveRight = false;
	bool moveUp = false;
	bool moveDown = false;

	float x;
	float y;
public:
	Player();
	~Player();

	void handleEvent(const sf::Event& event);
	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // PLAYER_HPP