#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tmxparser/Tmx.h>
#include <SFML/Graphics.hpp>

class Map : public sf::Drawable {
private:
	Tmx::Map tmxMap;

	sf::Texture texture;
	sf::VertexArray varray;

	const sf::IntRect getTextureRectForTileId(const Tmx::Tileset* const tileset, const int tileid) const;
	void addTileQuad(const sf::IntRect& positionRect, const sf::IntRect& textureRect);
public:
	Map();
	~Map();

	void load(const std::string& file);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // LEVEL_HPP