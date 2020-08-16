#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

/// DEFINES
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Fraktal - Mandelbrot Set"




/// USES A FORMULA TO SCALE THE VALUE FROM THE INTERVAL [min, max] TO THE INTERVAL [a, b]
double	scaleDown(double value, double min, double max, double a, double b)
{
	return	((b - a) * (value - min)) / (max - min) + a;
}



/// ZOOM FUNCTION
void 	zoom (double& minRe, double& maxRe, double& minIm, double& maxIm, double x, double y, double zoomLevel)
{
	double newCenterRe = scaleDown(x, 0, WINDOW_WIDTH, minRe, maxRe);
	double newCenterIm = scaleDown(y, 0, WINDOW_HEIGHT, minIm, maxIm);
	double diffRe = (maxRe - minRe) / zoomLevel;
	double diffIm = (maxIm - minIm) / zoomLevel;

	minRe = newCenterRe - diffRe;
	maxRe = newCenterRe + diffRe;
					
	minIm = newCenterIm - diffIm;
	maxIm = newCenterIm + diffIm;
}


/// MAIN FUNCTION
int 	main(int argc, char **argv)
{
	if (argc != 1)
		return -1;
	
	/// WINDOW
	RenderWindow 	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);

	Image			image;
	image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	Texture			texture;
	Sprite 			sprite;
	
	bool			changed = true;
	
	double			zoomLevel = 1;
	double	minRe = -2.5;
	double  maxRe = 1;
	double  minIm = -1;
	double 	maxIm = 1;

	unsigned int	maxIt = 1024;
	
	vector<Color> colorMap = {
		Color(66, 30, 15),
		Color(25, 7, 26),
		Color(9, 1, 47),
		Color(4, 4, 73),
		Color(0, 7, 100),
		Color(12, 44, 138),
		Color(24, 82, 177),
		Color(57, 125, 209),
		Color(134, 181, 229),
		Color(211, 236, 248),
		Color(241, 233, 191),
		Color(248, 201, 95),
		Color(255, 170, 0),
		Color(204, 128, 0),
		Color(153, 87, 0),
		Color(106, 52, 3)
	};
	
	while (window.isOpen())
	{
		Event event;
		
       	while (window.pollEvent(event))
       	{
			/// CLOSE WINDOW WITH X
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				/// CLOSE WINDOW WITH ESCAPE KEY
				if (event.key.code == Keyboard::Escape)
					window.close();
			
				
				/// MOVE THE VIEW
				if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
				{
					minIm -= (maxIm - minIm) * 0.1;
					maxIm -= (maxIm - minIm) * 0.1;
					changed = true;
				}
				
				if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
				{
					minIm += (maxIm - minIm) * 0.1;
					maxIm += (maxIm - minIm) * 0.1;
					changed = true;
				}
				
				if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)
				{
					minRe += (maxRe - minRe) * 0.1;
					maxRe += (maxRe - minRe) * 0.1;
					changed = true;
				}
				
				if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)
				{
					minRe -= (maxRe - minRe) * 0.1;
					maxRe -= (maxRe - minRe) * 0.1;
					changed = true;
				}
				
			}
			
			if (event.type == Event::MouseButtonPressed)
			{		
				/// ZOOM IN WITH LEFT CLICK, ZOOM OUT WITH RIGHT CLICK
				if (event.mouseButton.button == Mouse::Left)
				{
					zoom(minRe, maxRe, minIm, maxIm, event.mouseButton.x, event.mouseButton.y, 8);		
					zoomLevel *= 8;
					cout << "Zoom in: " << zoomLevel << '\n';
				}
					
				if (event.mouseButton.button == Mouse::Right)
				{
					zoom(minRe, maxRe, minIm, maxIm, event.mouseButton.x, event.mouseButton.y, 1.0 / 8);		
					zoomLevel /= 8;
					cout << "Zoom out: " << zoomLevel << '\n';
				}
				changed = true;
			}
			
			if (event.type == Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta < 0 && maxIt != 1) 
					maxIt /= 2;
				else 
					maxIt *= 2;
				
				cout << "Iterations: " << maxIt << '\n';
				changed = true;
			}
		}
		
		if (changed)
		{
			for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
				for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
				{
					double	scaledRe = scaleDown(x, 0, WINDOW_WIDTH, minRe, maxRe);
					double 	scaledIm = scaleDown(y, 0, WINDOW_HEIGHT, minIm, maxIm);
					double 	re = 0;
					double 	im = 0;
					int		iteration = 0;
							
					for (iteration = 0; iteration < maxIt && re * re + im * im < 2 * 2; iteration++)
					{
						double temp = re * re - im * im + scaledRe;
						im = 2 * re * im + scaledIm;
						re = temp;
					}
					
					if (iteration != maxIt)
						image.setPixel(x, y, colorMap[iteration % 16]);
					else
						image.setPixel(x, y, Color(0, 0, 0));
				}
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			changed = false;
		}
		
			
		/// DRAW THE FRACTAL, CLEAR THE SCREEN
		window.draw(sprite);
        window.display();
		window.clear();
	}

	return 0;
}
