#include <cassert>
#include <memory>
#include <iostream>

#include "player.hpp"

Player::Player(const Map* const map) {
	assert(map != nullptr);

	this->map = map;

	if (!texture.loadFromFile("player.png")) {
		throw "unable to load player texture";
	}

	this->position.top = 33;
	this->playerSprite.setTexture(this->texture);
	this->playerSprite.setTextureRect({ 0, 0, 16, 16 });
	this->playerSprite.setScale({ 2, 2 });
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

void Player::update(const sf::Time& dt) {
	sf::FloatRect pos = this->position;

	if (this->moveLeft) {
		pos.left -= 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveRight) {
		pos.left += 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveDown) {
		pos.top += 0.5f * (dt.asMicroseconds() / 1000.0f);
	}
	if (this->moveUp) {
		pos.top -= 0.5f * (dt.asMicroseconds() / 1000.0f);
	}

	if (this->map->isTileCollidable(pos.left, pos.top)) {
		std::cout << "BOOP!!!" << std::endl;
	} else {
		this->position = pos;
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setSize({ 32, 32});
	rect.setOutlineColor(sf::Color::Red);
	rect.setPosition( { this->position.left, this->position.top });
	target.draw(rect, states);
}