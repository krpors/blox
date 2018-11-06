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
		for (int y = 0; y < layer->GetHeight(); y++) {
			for (int x = 0; x < layer->GetWidth(); x++) {
				const int bla = layer->GetTileTilesetIndex(x, y);
				if (bla != -1) {
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = this->tmxMap.GetTileset(tile.tilesetId);
					if (tileset != nullptr) {
						this->addTileQuad(x, y, tileset, tile);
					}
				}
			}
		}
	}
}

void Map::addTileQuad(int x, int y, const Tmx::Tileset* tileset, const Tmx::MapTile& tile) {
	float tw = tileset->GetTileWidth();
	float th = tileset->GetTileHeight();

	sf::Vertex topleft     ( { x * tw     , y * th      } );
	sf::Vertex topright    ( { x * tw + tw, y * th      } );
	sf::Vertex bottomright ( { x * tw + tw, y * th + th } );
	sf::Vertex bottomleft  ( { x * tw     , y * th + th } );

	const sf::FloatRect& bleh = this->getTextureRectForTile(tileset, tile);
	topleft.texCoords     = { bleh.left,      bleh.top };
	topright.texCoords    = { bleh.left + tw, bleh.top };
	bottomright.texCoords = { bleh.left + tw, bleh.top + th };
	bottomleft.texCoords  = { bleh.left,      bleh.top + th };

	if (tile.flippedHorizontally) {
		topleft.texCoords.x += tw;
		bottomleft.texCoords.x += tw;
		topright.texCoords.x -= tw;
		bottomright.texCoords.x -= tw;
	}
	if (tile.flippedVertically) {
		topleft.texCoords.y += th;
		bottomleft.texCoords.y -= th;
		topright.texCoords.y += th;
		bottomright.texCoords.y -= th;
	}
	if (tile.flippedDiagonally) {
		std::clog
			<< "Tile " << tile.gid
			<< " is flipped diagonally, and "
			<< std::boolalpha
			<< " horiz: " << tile.flippedHorizontally
			<< ", vert: " << tile.flippedVertically
			<< std::endl;
	}

	this->varray.append(topleft);
	this->varray.append(topright);
	this->varray.append(bottomright);
	this->varray.append(bottomleft);
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