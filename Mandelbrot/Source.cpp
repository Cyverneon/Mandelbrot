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

long double zoom_amount = 0.5;

std::vector<sf::Color> mandelbrot_colors()
{
	std::vector<sf::Color> colors;

	sf::Color color1(35, 20, 65);
	sf::Color color2(70, 190, 190);
	sf::Color color3(160, 130, 220);

	for (int i = 0; i <= 100; i++)
	{
		sf::Color start_color;
		sf::Color end_color;
		long double lerp_value = double(i) / 100;
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

	}

	return colors;
}

sf::Color get_color(double lerp_value)
{
	sf::Color color1(35, 20, 65);
	sf::Color color2(70, 190, 190);
	sf::Color color3(160, 130, 220);

	sf::Color start_color;
	sf::Color end_color;

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

	return sf::Color::Color(red, green, blue);
}

sf::Image mandelbrot(std::vector<sf::Color> colors)
{
	sf::Image image;
	image.create(screen_x, screen_y, sf::Color::Black);

	std::vector<std::vector<int>> iteration_counts(screen_x, std::vector<int>(screen_y, 0));

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

			iteration_counts[pixel_x][pixel_y] = iterations;
		}
	}

	std::vector<int> iterations_per_pixel(max_iterations + 1, 0);

	for (int x = 0; x < screen_x; x++)
	{
		for (int y = 0; y < screen_y; y++)
		{
			int i = iteration_counts[x][y];
			iterations_per_pixel[i]++;
		}
	}

	int total = 0;
	for (int i = 0; i < max_iterations; i++)
	{
		total += iterations_per_pixel[i];
	}

	for (int x = 0; x < screen_x; x++)
	{
		for (int y = 0; y < screen_y; y++)
		{
			int iteration;
			iteration = iteration_counts[x][y];
			for (int i = 0; i <= iteration; i++)
			{
				if (iteration_counts[x][y] < max_iterations)
				{
					double value = (double(iterations_per_pixel[i])) / total;
					//std::cout << round(value * 100) << "\n";
					image.setPixel(x, y, colors[round(value * 100)]);
				}
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