#include <SFML/Graphics.hpp>
#include <iostream>

int screen_y = 896;
int screen_x = 988;

sf::Image mandelbrot()
{
	sf::Image image;
	image.create(screen_x, screen_y, sf::Color::Black);

	for (int pixel_x = 0; pixel_x < screen_x; pixel_x++)
	{
		for (int pixel_y = 0; pixel_y < screen_y; pixel_y++)
		{
			// scale x coordinate to lie inside mandelbrot scale (2.47 between -2.00 and 0.47)
			double x0 = ((pixel_x * 2.47) / screen_x) - 2.00;
			// scale y coordinate to lie inside mandelbrot scale (2.24 between -1.12 and 1.12)
			double y0 = ((pixel_y * 2.24) / screen_y) - 1.12;

			double x = 0.0;
			double y = 0.0;

			int iterations = 0;
			int max_iterations = 1000;

			while (((x * x) + (y * y)) < 4 && iterations < max_iterations)
			{
				double x_temp = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = x_temp;

				iterations++;
			}

			if (iterations < max_iterations)
			{
				image.setPixel(pixel_x, pixel_y, sf::Color::White);
			}
		}
	}

	return image;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(screen_x, screen_y), "Mandelbrot Shenanigans");

	sf::Image image = mandelbrot();
	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		return 1;
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		window.draw(sprite);

		window.display();
	}
	return 0;
}