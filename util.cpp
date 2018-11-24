#include "util.hpp"

#include <SFML/Graphics.hpp>

Camera::Camera() {
	this->setSize(320, 240);
}

Camera::~Camera() {

}

void Camera::setPlayer(const std::shared_ptr<Player>& player) {
	this->player = player;
}
void Camera::setMap(const std::shared_ptr<Map>& map) {
	this->map = map;
}

void Camera::update() {
	const sf::FloatRect& pbounds = this->player->getBounds();

	float cx = pbounds.left;
	float cy = pbounds.top;

	if (cx < (320 / 2)) {
		cx = 320 / 2;
	} else if (cx > 800 / 2) {
		cx = 800 / 2;
	}

	if (cy < 240 / 2) {
		cy = 240 / 2;
	}

	setCenter(cx, cy);
}