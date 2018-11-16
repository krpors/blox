#include <cassert>
#include <memory>
#include <iostream>

#include "player.hpp"

Player::Player(const std::shared_ptr<Map>& map) {
	this->map = map;

	if (!texture.loadFromFile("player.png")) {
		throw "unable to load player texture";
	}

	this->bounds = { 37, 68, 16, 24 };

	this->animationRest.setFrameTime(sf::milliseconds(100));
	this->animationRest.addFrame({ 0,  0, 16, 16});
	this->animationRest.addFrame({ 16, 0, 16, 16});
	this->animationRest.addFrame({ 32, 0, 16, 16});
	this->animationRest.addFrame({ 48, 0, 16, 16});

	this->animationWalk.setFrameTime(sf::milliseconds(50));
	this->animationWalk.addFrame({ 0,  16, 16, 16 });
	this->animationWalk.addFrame({ 16, 16, 16, 16 });
	this->animationWalk.addFrame({ 32, 16, 16, 16 });
	this->animationWalk.addFrame({ 48, 16, 16, 16 });
	this->animationWalk.addFrame({ 64, 16, 16, 16 });
	this->animationWalk.addFrame({ 80, 16, 16, 16 });

	this->playerSprite.setAnimation(animationWalk);
	this->playerSprite.setTexture(this->texture);
	this->playerSprite.setOrigin({ 8, 8 });
	this->playerSprite.setScale({ 2, 2 });
	this->playerSprite.setPlaying(true);
}

Player::~Player() {
}

void Player::handleEvent(const sf::Event& event) {
	// refactor using sf::Keyboard::is....?
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = true; break;
		case sf::Keyboard::Right: this->moveRight = true; break;
		case sf::Keyboard::Down:  this->moveDown  = true; break;
		case sf::Keyboard::Up:    this->moveUp    = true; break;
		case sf::Keyboard::F:     this->playerSprite.flipTexture();
		default: break;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft  = false; break;
		case sf::Keyboard::Right: this->moveRight = false; break;
		case sf::Keyboard::Down:  this->moveDown  = false; break;
		case sf::Keyboard::Up:    this->moveUp    = false; break;
		default: break;
		}
	}
}

bool Player::isPlayerColliding(const sf::FloatRect& bounds) const {
	return this->map->isColliding(bounds);
}

void Player::update(const sf::Time& dt) {
	sf::FloatRect newBounds = this->bounds;

	if (this->moveLeft) {
		newBounds.left -= 0.2f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveRight) {
		newBounds.left += 0.2f * (dt.asMicroseconds() / 1000.0f);
	}

	if (this->moveLeft || this->moveRight) {
		this->playerSprite.setAnimation(this->animationWalk);
		// If we are colliding over the x-axis, reset the newBounds.
		if (this->map->isColliding(newBounds)) {
			newBounds = this->bounds;
		}
	} else {
		this->playerSprite.setAnimation(this->animationRest);
	}

	newBounds.top += 0.09f * (dt.asMicroseconds() / 1000.0f);

	if (this->map->isColliding(newBounds)) {
		newBounds = this->bounds;
		newBounds.top -= 1;
		std::cout << "Colliding y axis " << (newBounds.top + newBounds.height) << std::endl;
	}

	this->playerSprite.update(dt);

	// if (!this->map->isColliding(newBounds)) {
		this->bounds = newBounds;
		this->playerSprite.setPosition({ bounds.left + 8, bounds.top + 10 });
	// }
}

const sf::FloatRect& Player::getBounds() const {
	return this->bounds;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape collisionBounds;
	collisionBounds.setSize({ this->bounds.width, this->bounds.height});
	collisionBounds.setFillColor(sf::Color::Transparent);
	collisionBounds.setOutlineColor(sf::Color::Red);
	collisionBounds.setOutlineThickness(1.0f);
	collisionBounds.setPosition( { this->bounds.left, this->bounds.top });

	sf::RectangleShape globBoundsRect;
	globBoundsRect.setSize({ this->playerSprite.getGlobalBounds().width, this->playerSprite.getGlobalBounds().height});
	globBoundsRect.setFillColor(sf::Color::Transparent);
	globBoundsRect.setOutlineColor(sf::Color::Magenta);
	globBoundsRect.setOutlineThickness(1.0f);
	globBoundsRect.setPosition({ this->playerSprite.getGlobalBounds().left, this->playerSprite.getGlobalBounds().top});

	target.draw(this->playerSprite, states);
	target.draw(collisionBounds, states);
	target.draw(globBoundsRect, states);
}