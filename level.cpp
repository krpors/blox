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
					const int tileid  = layer->GetTileId(x, y);
					// const int tilegid = layer->GetTileGid(x, y);
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = this->tmxMap.GetTileset(tile.tilesetId);

					if (tileset != nullptr) {
						int tw = tileset->GetTileWidth();
						int th = tileset->GetTileHeight();

						sf::IntRect texRect = this->getTextureRectForTileId(tileset, tileid);
						sf::IntRect tileRect(x * tw, y * th, tw, th);

						this->addTileQuad(tileRect, texRect);

						sf::Vertex vertex;
						vertex.position = sf::Vector2f(x * tw, y * th);
					}
				}
			}
		}
	}


}

void Map::addTileQuad(const sf::IntRect& positionRect, const sf::IntRect& textureRect) {
	sf::Vertex topleft;
	topleft.position = sf::Vector2f(positionRect.left, positionRect.top);
	topleft.texCoords = sf::Vector2f(textureRect.left, textureRect.top);

	sf::Vertex topright;
	topright.position = sf::Vector2f(positionRect.left + positionRect.width, positionRect.top);
	topright.texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top);

	sf::Vertex bottomright;
	bottomright.position = sf::Vector2f(positionRect.left + positionRect.width, positionRect.top + positionRect.height);
	bottomright.texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height);

	sf::Vertex bottomleft;
	bottomleft.position = sf::Vector2f(positionRect.left, positionRect.top + positionRect.height);
	bottomleft.texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height);

	this->varray.append(topleft);
	this->varray.append(topright);
	this->varray.append(bottomright);
	this->varray.append(bottomleft);
}

const sf::IntRect Map::getTextureRectForTileId(const Tmx::Tileset* const tileset, const int tileid) const {
	// following the `tileid = y * width + x` basics, we actually rewrite 
	// the formula to get the y and x.
	const int tileWidth = tileset->GetTileWidth();
	const int tilesPerRow = tileset->GetImage()->GetWidth() / tileWidth;
	int tiley = tileid / tilesPerRow;
	int tilex = tileid - (tiley * tilesPerRow);
	return sf::IntRect(tilex * tileWidth, tiley * tileWidth, tileWidth, tileWidth);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = &this->texture;

	// sf::VertexArray varr(sf::Quads);
	// sf::Vertex tl(sf::Vector2f(100, 100), sf::Vector2f(0, 0));
	// // tl.color = sf::Color::Red;
	// sf::Vertex tr(sf::Vector2f(200, 100), sf::Vector2f(32, 0));
	// sf::Vertex br(sf::Vector2f(200, 200), sf::Vector2f(32, 32));
	// sf::Vertex bl(sf::Vector2f(100, 200), sf::Vector2f(0, 32));
	// // bl.color = sf::Color::Blue;
	// varr.append(tl);
	// varr.append(tr);
	// varr.append(br);
	// varr.append(bl);

	target.draw(this->varray, states);
}