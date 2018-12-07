#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "anim.hpp"
#include "level.hpp"
#include "player.hpp"
#include "text.hpp"
#include "util.hpp"

int main() {
	// std::mt19937 rng;
	// rng.seed(std::random_device()());
	// std::uniform_real_distribution<> dist(-1, 1);
	// float f = dist(rng);

	// return 0;

	std::shared_ptr<Map> lolmap = std::make_shared<Map>();
	lolmap->load("map01.tmx");

	std::shared_ptr<Player> player = std::make_shared<Player>();
	player->setMap(lolmap);

	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");

	Text t2(font);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->setMap(lolmap);
	camera->setPlayer(player);

	ParticleGenerator pgen;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Blox", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	ParallaxView bgView(player, camera);
	Background bg;

	FpsCounter fps;

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape: window.close(); break;
				default: break;
				}
			}

			camera->handleEvent(event);
			player->handleEvent(event);
		}

		std::stringstream ss;
		ss
			<< "Player at ("
				<< static_cast<int>(player->getBounds().left) << ", "
				<< static_cast<int>(player->getBounds().top) << ")"
				<< std::endl
			<< "FPS: " << fps.getFps() << std::endl
			<< "Camera at (" << camera->getCenter().x << ", " << camera->getCenter().y << ")" << std::endl
			<< "Camera size is " << camera->getSize().x << " x " << camera->getSize().y << std::endl
			<< "Bg view size is " << bgView.getSize().x << " x " << bgView.getSize().y << std::endl;
		t2.setText(0, 0, ss.str());

		// Note: the order of updating camera, player and background view
		// is important so you don't get a "jittery" effect when drawing the
		// backgrounds first.
		camera->update();
		player->update(elapsed);
		bgView.update();

		pgen.update(elapsed);
		fps.update(elapsed);

		window.clear();
		window.setView(bgView);
		window.draw(bg);
		window.setView(*camera);
		lolmap->drawBackgrounds(window);
		window.draw(*player);
		lolmap->drawForegrounds(window);
		window.draw(pgen);
		window.setView(window.getDefaultView());
		window.draw(t2);
		window.display();
	}

	return 0;
}