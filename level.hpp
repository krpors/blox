#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tmxparser/Tmx.h>
#include <SFML/Graphics.hpp>

class Map : public sf::Drawable {
private:
	Tmx::Map tmxMap;

	sf::Texture texture;
	sf::VertexArray varray;

	const sf::FloatRect getTextureRectForTile(const Tmx::Tileset* const tileset, const Tmx::MapTile& tile) const;
	void addTileQuad(int x, int y, const Tmx::TileLayer* const tileLayer, const Tmx::Tileset* const tileset, const Tmx::MapTile& tile);

	void swap(sf::Vector2f& a, sf::Vector2f& b) const;
public:
	Map();
	~Map();

	void load(const std::string& file);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // LEVEL_HPP