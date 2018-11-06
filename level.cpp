#include <iostream>

#include <tmxparser/Tmx.h>

#include "level.hpp"

Map::Map() {
	std::cout << "Creating map" << std::endl;

	varray.setPrimitiveType(sf::Quads);
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

	for (int i = 0; i < this->tmxMap.GetNumTileLayers(); i++) {
		const Tmx::TileLayer* layer = this->tmxMap.GetTileLayer(i);

		std::cout << "Drawing layer " << layer->GetName() << std::endl;
		if (layer->GetName() == "Collision") {
			// for now, don't draw the collision layer.
			continue;
		}
		for (int y = 0; y < layer->GetHeight(); y++) {
			for (int x = 0; x < layer->GetWidth(); x++) {
				const int bla = layer->GetTileTilesetIndex(x, y);
				if (bla != -1) {
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = this->tmxMap.GetTileset(tile.tilesetId);
					if (tileset != nullptr) {
						this->addTileQuad(x, y, layer, tileset, tile);
					}
				}
			}
		}
	}
}

void Map::addTileQuad(int x, int y, const Tmx::TileLayer* const layer, const Tmx::Tileset* const tileset, const Tmx::MapTile& tile) {
	float tw = tileset->GetTileWidth();
	float th = tileset->GetTileHeight();

	unsigned int opacity = layer->GetOpacity() * 255;
	sf::Color color = sf::Color(0xff, 0xff, 0xff, opacity);
	std::cout << layer->GetOpacity() << std::endl;

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

	this->varray.append(topleft);
	this->varray.append(topright);
	this->varray.append(bottomright);
	this->varray.append(bottomleft);
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

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = &this->texture;
	target.draw(this->varray, states);
}