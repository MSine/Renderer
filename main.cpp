/*
g++ -o main main.cpp -lsfml-graphics -lsfml-window -lsfml-system; ./main

TODO List:
-Triangulation for better mesh generation

50cm from screen
35x20cm screen
1920x1080 pixel
*/


#include "renderer.hpp"

bool isOpen = false;

void render(sf::RenderWindow* window);

Vec3D camPos(5000, 2500, 9000);
Mesh mesh;


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(SCSZX, SCSZY), "My window", sf::Style::Fullscreen, settings);
    isOpen = true;

    sf::Thread thread(&render, &window);
    thread.launch();

    //render(&window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                isOpen = false;
                thread.wait();
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isOpen = false;
                    thread.wait();
                    window.close();
                }
            }
        }
    }
    return 0;
}

void render(sf::RenderWindow* window) {
	sf::RectangleShape pixel;
	pixel.setSize({ 1., 1. });

    //while (isOpen) {
        window->clear(sf::Color::Black);
        Vec3D intersection;

        for (int y = 0; y < SCSZY; y++) {
            for (int x = 0; x < SCSZX; x++) {
                Vec3D dir(x - SCSZX_2, y - SCSZY_2, -FDP);
                Ray ray(camPos, dir);

			    pixel.setPosition(x, y);

                bool rayActive;

                do {
                    rayActive = false;
                    for (auto trig: mesh.triangles) {
                        if (trig->rayIntersectsTriangle(ray, intersection)) {
                            //TODO  make surface brightness effect the color, arctan(energy*brightness) maybe?
                            if (trig->surface.brightness > 0) {
                                ray.distTrav += ray.rayStart.distance(intersection);
                                pixel.setFillColor(ray.adjustedColor());
                                window->draw(pixel);
                                rayActive = false;
                                break;
                            }
                            else if (ray.reflect(intersection, *trig)) {
                                rayActive = true;
                                continue;
                            }
                            
                        }
                    }
                } while (rayActive);
            }
        }
        window->display();
    //}
}