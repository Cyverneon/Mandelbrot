#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

int screen_y = 500;
int screen_x = 600;

int max_iterations = 100;

long double default_x_range = 3.00;
long double default_x_offset = -2.20;
long double default_y_range = 2.50;
long double default_y_offset = -1.25;

long double x_range = 3.00;
long double x_offset = -2.20;
long double y_range = 2.50;
long double y_offset = -1.25;

long double zoom_amount = 0.9;

std::vector<sf::Color> mandelbrot_colors()
{
	std::vector<sf::Color> colors;

	sf::Color color1(35, 20, 65);
	sf::Color color2(70, 190, 190);
	sf::Color color3(160, 130, 220);

	for (int i = 0; i < max_iterations; i++)
	{
		sf::Color start_color;
		sf::Color end_color;
		long double lerp_value = double(i) / double(max_iterations);
		if (lerp_value < 0.5)
		{
			start_color = color1;
			end_color = color2;
		}
		else
		{
			start_color = color2;
			end_color = color3;
		}

		int red = round(std::lerp(start_color.r, end_color.r, lerp_value));
		int green = round(std::lerp(start_color.g, end_color.g, lerp_value));
		int blue = round(std::lerp(start_color.b, end_color.b, lerp_value));

		colors.push_back(sf::Color::Color(red, green, blue));

		//colors.push_back(sf::Color::Color(round((255 * i) / max_iterations), round((255 * i) / max_iterations), 255));
	}

	return colors;
}

sf::Image mandelbrot(std::vector<sf::Color> colors)
{
	sf::Image image;
	image.create(screen_x, screen_y, sf::Color::Black);

	for (int pixel_x = 0; pixel_x < screen_x; pixel_x++)
	{
		for (int pixel_y = 0; pixel_y < screen_y; pixel_y++)
		{
			// scale x coordinate to lie inside mandelbrot scale (2.47 between -2.00 and 0.47)
			long double x0 = ((pixel_x * x_range) / screen_x) + x_offset;
			// scale y coordinate to lie inside mandelbrot scale (2.24 between -1.12 and 1.12)
			long double y0 = ((pixel_y * y_range) / screen_y) + y_offset;

			long double x = 0.0;
			long double y = 0.0;

			int iterations = 0;
			

			while (((x * x) + (y * y)) < 4 && iterations < max_iterations)
			{
				long double x_temp = x * x - y * y + x0;
				y = 2 * x * y + y0;
				x = x_temp;

				iterations++;
			}

			if (iterations < max_iterations)
			{
				image.setPixel(pixel_x, pixel_y, colors[iterations]);
			}
		}
	}

	return image;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(screen_x, screen_y), "Mandelbrot Shenanigans");

	std::vector<sf::Color> colors = mandelbrot_colors();

	sf::Image image = mandelbrot(colors);
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
			if (event.type == sf::Event::MouseButtonPressed)
			{
				long double x0 = ((sf::Mouse::getPosition(window).x * x_range) / screen_x) + x_offset;
				long double y0 = ((sf::Mouse::getPosition(window).y * y_range) / screen_y) + y_offset;

				x_range *= zoom_amount;
				y_range *= zoom_amount;

				x_offset = x0 - (x_range / 2);
				y_offset = y0 - (y_range / 2);

				image = mandelbrot(colors);
				texture.loadFromImage(image);
				sprite.setTexture(texture);
			}
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