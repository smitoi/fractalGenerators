#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string.h>

using namespace std;
using namespace sf;

/// WINDOW VALUES - The size and the name of the window
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_NAME "Fraktal "
/// MULTITHREADING VALUES - There will be HEIGHT_THREAD x WIDTH_THREAD threads since I divide the screen into boxes that I render individually
#define WIDTH_THREAD 16
#define HEIGHT_THREAD 16
/// DEFINES FOR INPUT CONTROL
#define MOVE_FACTOR 0.025



/// A NORMAL WINDOW PROVIDED BY SFML
RenderWindow 	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, Style::Close);

/// RENDERING VARIABLES - I load the image to a texture that gets applied to a sprite, and then I draw the sprite
Image			image;
Texture			texture;
Sprite 			sprite;

/// GUI VARIABLES - I use this to render a small GUI while loading
Font            font;
RectangleShape  loadingOverlay;
Text            loadingText;

/// BOOL VARIABLE - Keeps track of the image changes so it will not get rendered each frame
bool			changed = true;

/// UNSIGNED VARIABLE - Keeps track of the maximum iterations value and the type of fractal we are generating
unsigned int	maxIt = 256;
bool			julia = false;

/// DOUBLE VARIABLES - First variable is used to keep track of the zoom, the other values are used to keep track of the values of the complex plane used to generate the image
double	zoomLevel = 1;
double	minRe = -2.5;
double  maxRe = 1;
double  minIm = -1;
double 	maxIm = 1;
/// These two are used for Julia's Set generation
double  cRe = -0.73;
double  cIm = 0.23;

/// SCALE DOWN FUNCTION - Uses a formula to scale a value from the interval [min, max] to the iterval [a, b]
double	scaleDown(double value, double min, double max, double a, double b)
{
	return	((b - a) * (value - min)) / (max - min) + a;
}

/// ZOOM FUNCTION - Generates new values for the complex plane
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


/// The coloring function is taken from Stack Overflow
unsigned int	coloringFunction(unsigned int iteration, unsigned int phase)
{
	return (255 * pow(cos(sqrt(iteration) + phase), 2));
}


/// GENERATE FUNCTIONS - Computes the image in a square with the upper left point [startRe, startIm] and the lower right point [endRe, endIm]
void    generateMandelbrotFractal(unsigned int startRe, unsigned int endRe, unsigned int startIm, unsigned int endIm)
{
    for (unsigned int x = startRe; x < endRe; x++)
    {
        for (unsigned int y = startIm; y < endIm; y++)
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
                image.setPixel(x, y, Color(coloringFunction(iteration, 0),
                                           coloringFunction(iteration, 120),
                                           coloringFunction(iteration, 240)));
            else
                image.setPixel(x, y, Color(0, 0, 0));
        }
    }
}

void    generateJuliaFractal(unsigned int startRe, unsigned int endRe, unsigned int startIm, unsigned int endIm)
{
    for (unsigned int x = startRe; x < endRe; x++)
    {
        for (unsigned int y = startIm; y < endIm; y++)
        {
            double	re = scaleDown(x, 0, WINDOW_WIDTH, minRe, maxRe);
            double 	im = scaleDown(y, 0, WINDOW_HEIGHT, minIm, maxIm);
            double  R = 2;
            int		iteration = 0;

            for (iteration = 0; iteration < maxIt && re * re + im * im < R * R; iteration++)
            {
                double newRe = re * re - im * im + cRe;
                double newIm = 2 * re * im + cIm;

                re = newRe;
                im = newIm;
            }

            if (iteration != maxIt)
                image.setPixel(x, y, Color(coloringFunction(iteration, 0),
                                           coloringFunction(iteration, 120),
                                           coloringFunction(iteration, 240)));
            else
                image.setPixel(x, y, Color(0, 0, 0));
        }
    }
}

void    generateBurningShip(unsigned int startRe, unsigned int endRe, unsigned int startIm, unsigned int endIm)
{
    for (unsigned int x = startRe; x < endRe; x++)
    {
        for (unsigned int y = startIm; y < endIm; y++)
        {
            double	scaledRe = scaleDown(x, 0, WINDOW_WIDTH, minRe, maxRe);
            double 	scaledIm = scaleDown(y, 0, WINDOW_HEIGHT, minIm, maxIm);
            double 	re = 0;
            double 	im = 0;
            int		iteration = 0;

            for (iteration = 0; iteration < maxIt && re * re + im * im < 2 * 2; iteration++)
            {
                double temp = re * re - im * im + scaledRe;
                im = abs(2 * re * im) + scaledIm;
                re = temp;
            }

            if (iteration != maxIt)
                image.setPixel(x, y, Color(coloringFunction(iteration, 0),
                                           coloringFunction(iteration, 120),
                                           coloringFunction(iteration, 240)));
            else
                image.setPixel(x, y, Color(0, 0, 0));
        }
    }
}

/// We use this function with a reference to the generator we want to use (Mandelbrot or Julia), diving the screen and generating each box in a differend thread
void	generateFractal(void	(&f)(unsigned int, unsigned int, unsigned int, unsigned int) )
{
	vector<thread>  threadVector;
    /// We generate the threads and wait for them to end
    for (unsigned int x = 0; x < WINDOW_WIDTH; x += WINDOW_WIDTH / WIDTH_THREAD)
		for (unsigned int y = 0; y < WINDOW_HEIGHT; y += WINDOW_HEIGHT / HEIGHT_THREAD)
			threadVector.push_back(thread(f, x, x + WINDOW_WIDTH / WIDTH_THREAD, y, y + WINDOW_HEIGHT / HEIGHT_THREAD));
                    
	for (thread & th : threadVector)
		if (th.joinable())
			th.join();
}


/// MAIN FUNCTION
int 	main(int argc, char **argv)
{
	void	(*func)(unsigned int, unsigned int, unsigned int, unsigned int);
	
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " {mandelbrot | julia | burningship} " << '\n';
		return -1;
	}
	
	if (!strcmp(argv[1], "mandelbrot"))
	{
		func = generateMandelbrotFractal;
		window.setTitle("Fraktal - Mandelbrot Set");
	}
	else if (!strcmp(argv[1], "julia"))
	{
		func = generateJuliaFractal;
		window.setTitle("Fraktal - Julia Set");
		julia = true;
	}
	else if (!strcmp(argv[1], "burningship"))
	{
		func = generateBurningShip;
		window.setTitle("Fraktal - Burning Ship");
	}
	else
	{
		cout << "Usage: " << argv[0] << " {mandelbrot | julia | burningship} " << '\n';
		return -1;
	}

    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	font.loadFromFile("arial.ttf");
	loadingText.setFont(font);
	loadingText.setString("Loading...");
	loadingText.setPosition(Vector2f(WINDOW_WIDTH / 2 - loadingText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2));
	loadingOverlay.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	loadingOverlay.setFillColor(Color(0, 0, 0, 128));
	
	while (window.isOpen())
	{
		Event event;

       	while (window.pollEvent(event))
       	{
			/// Close window with X
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				/// Close window with escape key
				if (event.key.code == Keyboard::Escape)
					window.close();


				/// Move the view using the arrows or WASD
				if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W)
				{
					minIm -= (maxIm - minIm) * MOVE_FACTOR;
					maxIm -= (maxIm - minIm) * MOVE_FACTOR;
					changed = true;
				}
				if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S)
				{
					minIm += (maxIm - minIm) * MOVE_FACTOR;
					maxIm += (maxIm - minIm) * MOVE_FACTOR;
					changed = true;
				}
				if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)
				{
					minRe += (maxRe - minRe) * MOVE_FACTOR;
					maxRe += (maxRe - minRe) * MOVE_FACTOR;
					changed = true;
				}
				if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)
				{
					minRe -= (maxRe - minRe) * MOVE_FACTOR;
					maxRe -= (maxRe - minRe) * MOVE_FACTOR;
					changed = true;
				}
				if (julia == true && event.key.code == Keyboard::Z)
				{
					cRe += 0.01;
					changed = true;
				}
				if (julia == true && event.key.code == Keyboard::X)
				{
					cRe -= 0.01;
					changed = true;
				}
				if (julia == true && event.key.code == Keyboard::C)
				{
					cIm += 0.01;
					changed = true;
				}
				if (julia == true && event.key.code == Keyboard::V)
				{
					cIm -= 0.01;
					changed = true;
				}
            }

			if (event.type == Event::MouseButtonPressed)
			{
				/// LEFT CLICK - Zoom In
				if (event.mouseButton.button == Mouse::Left)
				{
					zoom(minRe, maxRe, minIm, maxIm, event.mouseButton.x, event.mouseButton.y, 8);
					zoomLevel *= 8;
					cout << "Zoom in: " << zoomLevel << '\n';
				}
                /// RIGHT CLICK - Zoom Out
				if (event.mouseButton.button == Mouse::Right)
				{
					zoom(minRe, maxRe, minIm, maxIm, event.mouseButton.x, event.mouseButton.y, 1.0 / 2);
					zoomLevel /= 8;
					cout << "Zoom out: " << zoomLevel << '\n';
				}

				changed = true;
			}

            /// Change the number of maximum iterations computed using the mouse wheel
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
		    /// We use a vector of threads to compute the image faster - the clock is used for debug purposes
		    Clock           clock;
		    vector<thread>  threadVector;
		    
			window.draw(loadingOverlay);
			window.draw(loadingText);
			window.display();
			
            cout << "Changed view: " << '\n' << "RE: [" << minRe << ", " << maxRe << "];" << '\n' << "IM: [" << minIm << ", " << maxIm << "];" << '\n';

			generateFractal(*func);
				
            cout << "Time taken by generation: " << clock.getElapsedTime().asMilliseconds() << " microseconds" << endl;
            
            /// We update the image, texture and sprite
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			changed = false;
		}

		/// Draw the sprite (fractal) and clear the screen
		window.clear();
		window.draw(sprite);
        window.display();
	}

	return 0;
}
