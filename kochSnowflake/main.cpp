#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

/// DEFINES
#define WINDOW_HEIGHT 768
#define WINDOW_WIDTH 1024
#define WINDOW_NAME "Fraktal - Koch Snowflake"
#define SIN sqrt(3) / 2.f
#define COS 1.f / 2
#define C 320
#define D 128

/// NAMSPACES
using namespace std;
using namespace sf;

/// ARRAYS FOR THE LINES
vector<VertexArray>		vertexArray;

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
	
	for (size_t index = 0; index < vertexArrayCopy.getVertexCount() - 1; index++)
	{
		cout << index << '\n';
		a.x = vertexArrayCopy[index].position.x;
		a.y = vertexArrayCopy[index].position.y;
		
		e.x = vertexArrayCopy[index + 1].position.x;
		e.y = vertexArrayCopy[index + 1].position.y;
		
		b.x = a.x + (e.x - a.x) * (1.f / 3);
		b.y = a.y + (e.y - a.y) * (1.f / 3);
		
		d.x = a.x + (e.x - a.x) * (2.f / 3);
		d.y = a.y + (e.y - a.y) * (2.f / 3);
		
		c.x = COS * (d.x - b.x) - SIN * (d.y - b.y) + b.x;
		c.y = SIN * (d.x - b.x) + COS * (d.y - b.y) + b.y;
		
		vertexArray.append(Vertex(Vector2f(a.x, a.y), Color::Red));
		vertexArray.append(Vertex(Vector2f(b.x, b.y), Color::Blue));
		vertexArray.append(Vertex(Vector2f(c.x, c.y), Color::Yellow));
		vertexArray.append(Vertex(Vector2f(d.x, d.y), Color::Cyan));
		vertexArray.append(Vertex(Vector2f(e.x, e.y), Color::White));
		cout << '(' << a.x << ' ' << a.y << ')' << '\n';
		cout << '(' << b.x << ' ' << b.y << ')' << '\n';
		cout << '(' << c.x << ' ' << c.y << ')' << '\n';
		cout << '(' << d.x << ' ' << d.y << ')' << '\n';
		cout << '(' << e.x << ' ' << e.y << ')' << '\n';
	}
}

/// MAIN FUNCTION
int 	main(int argc, char **argv)
{	
	if (argc == 2)
	{
		if (!strcmp(argv[1], "-T"))
		{
			vertexArray.push_back(VertexArray(LinesStrip, 0));
	
			vertexArray[0].append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2 + D)));	
			vertexArray[0].append(Vertex(Vector2f(WINDOW_WIDTH / 2 + C, WINDOW_HEIGHT / 2 + D)));
			vertexArray[0].append(Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - C * sqrt(2) + D)));
			vertexArray[0].append(Vertex(Vector2f(WINDOW_WIDTH / 2 - C, WINDOW_HEIGHT / 2 + D)));	
		}
		else if (!strcmp(argv[1], "-C"))
		{
			vertexArray.push_back(VertexArray(LinesStrip, 0));
			vertexArray.push_back(VertexArray(LinesStrip, 0));
			vertexArray.push_back(VertexArray(LinesStrip, 0));
			vertexArray.push_back(VertexArray(LinesStrip, 0));

			vertexArray[0].append(Vertex(Vector2f(10, 10)));
			vertexArray[0].append(Vertex(Vector2f(WINDOW_WIDTH - 10, 10)));
			
			vertexArray[1].append(Vertex(Vector2f(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10)));
			vertexArray[1].append(Vertex(Vector2f(10, WINDOW_HEIGHT - 10)));
			
			vertexArray[2].append(Vertex(Vector2f(10, WINDOW_HEIGHT - 10)));
			vertexArray[2].append(Vertex(Vector2f(10, 10)));
			
			vertexArray[3].append(Vertex(Vector2f(WINDOW_WIDTH - 10, 10)));
			vertexArray[3].append(Vertex(Vector2f(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10)));
		}
		else
		{
			cout << "Usage: " << argv[0] << " < -T | -C >" << '\n';
			return -1;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " < -T | -C >" << '\n';
		return -1;
	}
	
	RenderWindow 	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME); /// THE WINDOW
	View			view;
	view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	window.setView(view);
	
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
				if (event.key.code == Keyboard::Space)
				{
					for (size_t index = 0; index < vertexArray.size(); index++)
						generation(vertexArray[index]);
				}
				
				if (event.key.code == Keyboard::Up)
				{
					view.move(0, -50.f);
					window.setView(view);
				}
				
				if (event.key.code == Keyboard::Down)
				{
					view.move(0, 50.f);
					window.setView(view);
				}
				
				if (event.key.code == Keyboard::Right)
				{
					view.move(50.f, 0);
					window.setView(view);
				}
				
				if (event.key.code == Keyboard::Left)
				{
					view.move(-50.f, 0);
					window.setView(view);
				}
			}
			
			if (event.type == Event::MouseButtonPressed)
			{
				Vector2f viewPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
				
				if (event.mouseButton.button == Mouse::Left)
				{
					view.setCenter(viewPos.x, viewPos.y);
					view.zoom(0.75f);
					window.setView(view);
				}
				
				if (event.mouseButton.button == Mouse::Right)
				{
					view.zoom(1 / 0.75f);
					window.setView(view);
				}
			}
		}
		
		for (size_t index = 0; index < vertexArray.size(); index++)
			window.draw(vertexArray[index]);
			
        window.display();
		window.clear();
	}

	return 0;
}
