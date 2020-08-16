#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

/// DEFINES
#define WINDOW_HEIGHT 768
#define WINDOW_WIDTH 1024
#define WINDOW_NAME "Fraktal - Koch Snowflake"
/// MATH FUNCTIONS FOR 60 DEGREES
#define SIN sqrt(3) / 2.f
#define COS 1.f / 2
/// THERE ARE SOME CONSTANTS THAT I USED FOR POSITIONING THE FIGURE ON THE SCREEN
#define C 320
#define D 128

/// NAMSPACES
using namespace std;
using namespace sf;

/// ARRAYS FOR THE LINES
VertexArray		vertexArray;

typedef struct {
	float x;
	float y;
} point;


/// GENERATION FUNCTION
void	generation(VertexArray&		vertexArray)
{
	VertexArray		vertexArrayCopy;
	point	a, b, c, d, e;
	
	vertexArrayCopy = vertexArray;
	vertexArray = VertexArray(LinesStrip, 0);
	
	/// WE DIVIDE THE LINE INTO 3 SEGMENTS, A - B - D - E AND WE ADD A NEW POINT C THAT FORMS AN EQUILATERAL TRIANGLE WITH B AND D
	for (size_t index = 0; index < vertexArrayCopy.getVertexCount() - 1; index++)
	{
		a.x = vertexArrayCopy[index].position.x;
		a.y = vertexArrayCopy[index].position.y;
		
		e.x = vertexArrayCopy[index + 1].position.x;
		e.y = vertexArrayCopy[index + 1].position.y;
		
		b.x = a.x + (e.x - a.x) * (1.f / 3);
		b.y = a.y + (e.y - a.y) * (1.f / 3);
		
		d.x = a.x + (e.x - a.x) * (2.f / 3);
		d.y = a.y + (e.y - a.y) * (2.f / 3);
		
		/// FORMULA TO MOVE D BY 60 DEGREES RELATIVE TO B
		c.x = COS * (d.x - b.x) - SIN * (d.y - b.y) + b.x;
		c.y = SIN * (d.x - b.x) + COS * (d.y - b.y) + b.y;
		
		vertexArray.append(Vertex(Vector2f(a.x, a.y), Color::Red));
		vertexArray.append(Vertex(Vector2f(b.x, b.y), Color::Blue));
		vertexArray.append(Vertex(Vector2f(c.x, c.y), Color::Yellow));
		vertexArray.append(Vertex(Vector2f(d.x, d.y), Color::Cyan));
		vertexArray.append(Vertex(Vector2f(e.x, e.y), Color::White));
	}
}

/// MAIN FUNCTION
int 	main(int argc, char **argv)
{	
	if (argc == 2)
	{
		if (!strcmp(argv[1], "-T"))
		{
			/// WE GENERATE AN EQUILATERAL TRIANGLE USING THE HEIGHT FORMULA L * sqrt(3) / 2
			vertexArray = VertexArray(LinesStrip, 0);
	
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2 + D + D / 2 - D / 16)));	
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 + C, WINDOW_HEIGHT / 2  + D + D / 2 - D / 16)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - C * sqrt(3) + D + D / 2 - D / 16)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2  + D + D / 2 - D / 16)));	
		}
		else if (!strcmp(argv[1], "-C"))
		{
			/// WE GENERATE A SQUARE
			vertexArray = VertexArray(LinesStrip, 0);
			
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10)));
			vertexArray.append(Vertex(Vector2f(10, WINDOW_HEIGHT - 10)));
			vertexArray.append(Vertex(Vector2f(10, 10)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH - 10, 10)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10)));

		}
		else if (!strcmp(argv[1], "-A"))
		{
			/// WE GENERATE THE SAME TRIANGLE AS ABOVE, BUT WE DO IT IN INVERSE ORDER TO GENERATE THE ANTISNOWFLAKE
			vertexArray = VertexArray(LinesStrip, 0);
			
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2 + D + D)));	
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - C * sqrt(3) + D + D)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 + C, WINDOW_HEIGHT / 2  + D + D)));
			vertexArray.append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2  + D + D)));	
		}
		else
		{
			cout << "Usage: " << argv[0] << " < -T | -C | -A >" << '\n';
			return -1;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " < -T | -C | -A >" << '\n';
		return -1;
	}
	
	/// WINDOW AND VIEW
	RenderWindow 	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME); /// THE WINDOW
	View			view;
	view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	window.setView(view);
	
	/// KEEPING TRACK OF ZOOM LEVEL
	unsigned int	zoomLevel = 1;
	float			moveViewLevel = 50.f;
	
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
					
				/// GENERATE A NEW LEVEL OF THE FRACTAL
				if (event.key.code == Keyboard::Space)
					generation(vertexArray);
				
				/// MOVE THE VIEW
				if (event.key.code == Keyboard::Up)
				{
					view.move(0, -moveViewLevel / zoomLevel);
					window.setView(view);
					cout << view.getCenter().x << ' ' << view.getCenter().y << '\n';
					cout << view.getSize().x << ' ' << view.getSize().y << '\n';
				}
				
				if (event.key.code == Keyboard::Down)
				{
					view.move(0, moveViewLevel / zoomLevel);
					window.setView(view);
				}
				
				if (event.key.code == Keyboard::Right)
				{
					view.move(moveViewLevel / zoomLevel, 0);
					window.setView(view);
				}
				
				if (event.key.code == Keyboard::Left)
				{
					view.move(-moveViewLevel / zoomLevel, 0);
					window.setView(view);
				}
			}
			
			if (event.type == Event::MouseButtonPressed)
			{
				Vector2f viewPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
				
				/// ZOOM IN AND ZOOM OUT
				if (event.mouseButton.button == Mouse::Left)
				{
					view.setCenter(viewPos.x, viewPos.y);
					view.zoom(0.75f);
					window.setView(view);
					zoomLevel++;
				}
				
				if (event.mouseButton.button == Mouse::Right)
				{
					view.zoom(1 / 0.75f);
					window.setView(view);
					zoomLevel--;
				}
			}
		}
		
		/// DRAW THE FRACTAL, CLEAR THE SCREEN
		window.draw(vertexArray);
        window.display();
		window.clear();
	}

	return 0;
}
