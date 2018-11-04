#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tmxparser/Tmx.h>
#include <SFML/Graphics.hpp>

class Map {
private:
	Tmx::Map tmxMap;

	sf::IntRect bleh(const Tmx::Tileset* const tileset, const int tileid);
public:
	Map();
	~Map();

	void load(const std::string& file);
};

#endif // LEVEL_HPP