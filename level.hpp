#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <tmxparser/Tmx.h>
#include <SFML/Graphics.hpp>

class Map : public sf::Drawable {
private:
	Tmx::Map tmxMap;

	const Tmx::TileLayer* collisionLayer;

	sf::Texture texture;
	// sf::VertexArray varray;

	std::vector<sf::VertexArray> layersToDraw;

	const sf::FloatRect getTextureRectForTile(const Tmx::Tileset* const tileset, const Tmx::MapTile& tile) const;
	void addTileQuad(int x, int y, sf::VertexArray& target, const Tmx::TileLayer* const tileLayer, const Tmx::Tileset* const tileset, const Tmx::MapTile& tile);

	/**
	 * Swap two vectors (used for tile rendering).
	 */
	void swap(sf::Vector2f& a, sf::Vector2f& b) const;

public:
	Map();
	~Map();

	void load(const std::string& file);
	bool isTileCollidable(int tilex, int tiley) const;
	/**
	 * Checks whether the given rectangle (which should denote a collision
	 * bounding box) will collide with a tile in the collision layer.
	 */
	bool isColliding(const sf::FloatRect& objectBounds) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void drawBackgrounds(sf::RenderTarget& target) const;
	void drawForegrounds(sf::RenderTarget& target) const;
};

#endif // LEVEL_HPP