#include <iostream>

#include <tmxparser/Tmx.h>

#include "level.hpp"

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

	for (int i = 0; i < this->tmxMap.GetNumTileLayers(); i++) {
		const Tmx::TileLayer* layer = this->tmxMap.GetTileLayer(i);

		std::cout << "Found tile layer " << layer->GetName() << std::endl;
		for (int y = 0; y < layer->GetHeight(); y++) {
			for (int x = 0; x < layer->GetWidth(); x++) {
				const int bla = layer->GetTileTilesetIndex(x, y);
				if (bla != -1) {
					const int tileid  = layer->GetTileId(x, y);
					const int tilegid = layer->GetTileGid(x, y);
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = this->tmxMap.GetTileset(tile.tilesetId);

					if (tileset != 0) {
						// following the `tileid = y * width + x` basics, we
						// actually rewrite the formula to get the y and x.
						const int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
						int tiley = tileid / tilesPerRow;
						int tilex = tileid - (tiley * tilesPerRow);

						sf::IntRect r = this->bleh(tileset, tileid); 
						std::clog
							<< "Tileid " << tileid
							<< " with " << r.left << ", " << r.top << " - " << r.width << ", " << r.height
							<< std::endl; 
						// std::cout
						// 	<< "Tileid " << tileid
						// 	<< " corresponds with (" << tilex << ", " << tiley << ")"
						// 	<< std::endl;

					}
				}
			}
		}
	}
}

sf::IntRect Map::bleh(const Tmx::Tileset* const tileset, const int tileid) {
	const int tileWidth = tileset->GetTileWidth();
	const int tilesPerRow = tileset->GetImage()->GetWidth() / tileWidth;
	int tiley = tileid / tilesPerRow;
	int tilex = tileid - (tiley * tilesPerRow);
	return sf::IntRect(tilex * tileWidth, tiley * tileWidth, tileWidth, tileWidth);
}