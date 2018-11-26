#include <cmath>
#include <iostream>

#include <tmxparser/Tmx.h>

#include "level.hpp"

Layer::Layer(const std::string& name)
		: name(name) {
	vertices.setPrimitiveType(sf::Quads);
}

Layer::~Layer() {
}

//==============================================================================

Map::Map() {
	std::cout << "Creating map" << std::endl;
}

Map::~Map() {
	std::cout << "Destroying map" << std::endl;
}

void Map::load(const std::string& file) {
	//https://en.sfml-dev.org/forums/index.php?topic=12052.msg83668#msg83668
	//https://gamedev.stackexchange.com/questions/21238/tiled-tmx-c-library-parser

	std::cout << "Loading!" << std::endl;
	tmxMap.ParseFile(file);
	if (tmxMap.HasError()) {
		std::clog
			<< "Failed to parse TMX file " << file << ":"
			<< tmxMap.GetErrorCode() << " (" << tmxMap.GetErrorText() << ")"
			<< std::endl;
		throw "failed to load tmx map";
	}

	if (tmxMap.GetNumTilesets() != 1) {
		throw "expected only one tileset for now";
	}

	const Tmx::Tileset* ts = tmxMap.GetTileset(0);
	const std::string& src = ts->GetImage()->GetSource();
	std::clog << "Image source is " << src << std::endl;
	if (!this->texture.loadFromFile(src)) {
		throw "failed to load texture from tilemap";
	}
	texture.setSmooth(false);

	for (int i = 0; i < this->tmxMap.GetNumTileLayers(); i++) {
		const Tmx::TileLayer* layer = this->tmxMap.GetTileLayer(i);

		std::cout << "Drawing layer " << layer->GetName() << std::endl;

		// Every layer has its own vertex array so we can draw certain layers
		// first, and other layers later. That way we can create the illusion
		// that certain layers are 'behind' the player or other entities, and
		// other layers are 'in front of' them.
		if (layer->GetName() == "Collision") {
			this->collisionLayer = layer;
			// for now, don't draw the collision layer.
			continue;
		}

		Layer l(layer->GetName());

		for (int y = 0; y < layer->GetHeight(); y++) {
			for (int x = 0; x < layer->GetWidth(); x++) {
				const int bla = layer->GetTileTilesetIndex(x, y);
				if (bla != -1) {
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = this->tmxMap.GetTileset(tile.tilesetId);

					if (tileset != nullptr) {
						this->addTileQuad(x, y, l.vertices, layer, tileset, tile);
					}
				}
			}
		}

		this->layersToDraw.push_back(l);
	}
}

void Map::addTileQuad(int x, int y, sf::VertexArray& target, const Tmx::TileLayer* const layer, const Tmx::Tileset* const tileset, const Tmx::MapTile& tile) {
	float tw = tileset->GetTileWidth();
	float th = tileset->GetTileHeight();

	unsigned int opacity = layer->GetOpacity() * 255;
	sf::Color color = sf::Color(0xff, 0xff, 0xff, opacity);

	sf::Vertex topleft;
	sf::Vertex topright;
	sf::Vertex bottomright;
	sf::Vertex bottomleft;

	topleft.position     = { x * tw     , y * th      };
	topright.position    = { x * tw + tw, y * th      };
	bottomright.position = { x * tw + tw, y * th + th };
	bottomleft.position  = { x * tw     , y * th + th };

	topleft.color = color;
	topright.color = color;
	bottomright.color = color;
	bottomleft.color = color;

	const sf::FloatRect& bleh = this->getTextureRectForTile(tileset, tile);
	topleft.texCoords     = { bleh.left,      bleh.top };
	topright.texCoords    = { bleh.left + tw, bleh.top };
	bottomright.texCoords = { bleh.left + tw, bleh.top + th };
	bottomleft.texCoords  = { bleh.left,      bleh.top + th };

	// Note: must do diagonal flip first before flipping horizontally or
	// vertically, per the documentation of the TMX spec. See
	// https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#tile-flipping
	// for more information on that.
	if (tile.flippedDiagonally) {
		this->swap(topright.texCoords, bottomleft.texCoords);
	}
	if (tile.flippedHorizontally) {
		this->swap(topleft.texCoords, topright.texCoords);
		this->swap(bottomleft.texCoords, bottomright.texCoords);
	}
	if (tile.flippedVertically) {
		this->swap(topleft.texCoords, bottomleft.texCoords);
		this->swap(topright.texCoords, bottomright.texCoords);
	}

	target.append(topleft);
	target.append(topright);
	target.append(bottomright);
	target.append(bottomleft);
}

void Map::swap(sf::Vector2f& a, sf::Vector2f& b) const {
	sf::Vector2f temp = a;
	a = b;
	b = temp;
}

const sf::FloatRect Map::getTextureRectForTile(const Tmx::Tileset* const tileset, const Tmx::MapTile& tile) const {
	// following the `tileid = y * width + x` basics, we actually rewrite
	// the formula to get the y and x.
	const int tileWidth = tileset->GetTileWidth();
	const int tilesPerRow = tileset->GetImage()->GetWidth() / tileWidth;
	int tiley = tile.id / tilesPerRow;
	int tilex = tile.id - (tiley * tilesPerRow);

	sf::FloatRect textureRect(tilex * tileWidth, tiley * tileWidth, tileWidth, tileWidth);
	return textureRect;
}

bool Map::isColliding(const sf::FloatRect& objectBounds) const {
	// what tile is occupied at the top left?
	int tileTopX1 = std::floor(objectBounds.left / static_cast<float>(this->tmxMap.GetTileWidth() ));
	int tileTopY1 = std::floor(objectBounds.top  / static_cast<float>(this->tmxMap.GetTileHeight() ));

	// what tile is occupied at the bottom right?
	int tileBottomX1 = std::floor((objectBounds.left + objectBounds.width) / static_cast<float>(this->tmxMap.GetTileWidth() ));
	int tileBottomY1 = std::floor((objectBounds.top + objectBounds.height) / static_cast<float>(this->tmxMap.GetTileHeight() ));

	// If we go out of bounds of the map, return true.
	if (   tileTopX1 < 0
		|| tileTopY1 < 0
		|| tileBottomX1 > this->tmxMap.GetWidth()
		|| tileBottomY1 > this->tmxMap.GetHeight() ) {
		return true;
	}

	// now iterate over the occupied tiles given the objectBounds.
	for (int x = tileTopX1; x <= tileBottomX1; x++) {
		for (int y = tileTopY1; y <= tileBottomY1; y++) {
			if (this->isTileCollidable(x, y)) {
				return true;
			}
		}
	}

	return false;
}

const sf::FloatRect Map::getBounds() const {
	// TODO: this is rather "const" data and only needs to be calculated once.
	sf::FloatRect r;
	r.left = 0;
	r.top = 0;
	r.width = this->tmxMap.GetWidth() * this->tmxMap.GetTileWidth();
	r.height = this->tmxMap.GetHeight() * this->tmxMap.GetTileHeight();
	return r;
}

bool Map::isTileCollidable(int tilex, int tiley) const {
	return this->collisionLayer->GetTileGid(tilex, tiley) > 0;
}


void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// states.texture = &this->texture;
	// for (auto layer : this->layersToDraw) {
	// 	target.draw(layer.vertices, states);
	// }
}

void Map::drawBackgrounds(sf::RenderTarget& target) const {
	sf::RenderStates states;
	states.texture = &this->texture;
	for (auto layer : this->layersToDraw) {
		if (layer.name == "Main") {
			break;
		}
		target.draw(layer.vertices, states);
	}
}

void Map::drawForegrounds(sf::RenderTarget& target) const {
	sf::RenderStates states;
	states.texture = &this->texture;
	bool startDrawing = false;
	for (auto layer : this->layersToDraw) {
		if (layer.name == "Main") {
			startDrawing = true;
		}
		if (startDrawing) {
			target.draw(layer.vertices, states);
		}
	}
}