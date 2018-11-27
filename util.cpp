#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>

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

void Camera::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseWheelScrolled) {
		int multiplier = 60;
		if (event.mouseWheelScroll.delta > 0) {
			this->setSize(this->getSize().x - multiplier * 1.33f, this->getSize().y - multiplier);
		} else if (event.mouseWheelScroll.delta < 0) {
			this->setSize(this->getSize().x + multiplier * 1.33f, this->getSize().y + multiplier);
		}
	}
}

void Camera::update() {
	const sf::FloatRect& pbounds = this->player->getBounds();
	const sf::FloatRect& mbounds = this->map->getBounds();

	// The minimum x and y camera positions are calculated based on the map's
	// edges. That will prevent the camera from scrolling 'over the edges' of
	// the map. When we aren't clamped to the edge of the map, the player is
	// centered.
	float xmin = this->getSize().x / 2.0f;
	float xmax = mbounds.width - xmin;

	float ymin = this->getSize().y / 2.0f;
	float ymax = mbounds.height - ymin;

	float cx = std::min(xmax, std::max(xmin, pbounds.left));
	float cy = std::min(ymax, std::max(ymin, pbounds.top));

	setCenter(cx, cy);
}