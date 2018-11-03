#include <iomanip>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "text.hpp"

int main() {
	Tmx::Map map;
	map.ParseFile("map01.tmx");
	if (map.HasError()) {
		std::clog << "Error code: " << map.GetErrorCode() << std::endl;
		std::clog << "Error text: " << map.GetErrorText() << std::endl;
	}
	std::cout << "Tileset " << map.GetTileset(0)->GetImage()->GetSource() << std::endl;

	//https://en.sfml-dev.org/forums/index.php?topic=12052.msg83668#msg83668
	//https://gamedev.stackexchange.com/questions/21238/tiled-tmx-c-library-parser

	for (int i = 0; i < map.GetNumTileLayers(); i++) {
		const Tmx::TileLayer* layer = map.GetTileLayer(i);

		std::cout << "Found tile layer " << layer->GetName() << std::endl;
		for (int y = 0; y < layer->GetHeight(); y++) {
			for (int x = 0; x < layer->GetWidth(); x++) {
				const int bla = layer->GetTileTilesetIndex(x, y);
				if (bla != -1) {
					const int tileid  = layer->GetTileId(x, y);
					const int tilegid = layer->GetTileGid(x, y);
					const Tmx::MapTile& tile = layer->GetTile(x, y);
					const Tmx::Tileset* tileset = map.GetTileset(tile.tilesetId);

					if (tileset != 0) {
						// following the `tileid = y * width + x` basics, we
						// actually rewrite the formula to get the y and x.
						const int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
						int tiley = tileid / tilesPerRow;
						int tilex = tileid - (tiley * tilesPerRow);

						std::cout
							<< "Tileid " << tileid
							<< " corresponds with (" << tilex << ", " << tiley << ")"
							<< std::endl;

					}
				}
			}
		}
	}


	for (int i = 0; i < map.GetNumLayers(); i++) {
		const Tmx::Layer* layer = map.GetLayer(i);
		std::cout << "Layer " << i << " has name " << layer->GetName() << std::endl;
	}

	return 0;

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");

	Text t1(font);
	t1.setText(0, 0, "The quick brown fox jumps over the lazy doggo.");
	Text t2(font);
	t2.setText(0, 10, "Yo dude!");

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		(void)elapsed;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		std::stringstream ss;
		ss << "The elapsed time is " << std::setfill(' ') << std::setw(5) << elapsed.asMicroseconds() << " microseconds.";
		t2.setText(0, 10, ss.str());

		window.clear();
		window.draw(shape);
		window.draw(t1);
		window.draw(t2);
		window.display();
	}

	return 0;
}