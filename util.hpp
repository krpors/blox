#ifndef UTIL_HPP
#define UTIL_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "player.hpp"

class Camera : public sf::View {
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<Map> map;
public:
	Camera();
	~Camera();

	void setPlayer(const std::shared_ptr<Player>& player);
	void setMap(const std::shared_ptr<Map>& map);
	void handleEvent(const sf::Event& event);
	void update();
};

#endif // UTIL_HPP
