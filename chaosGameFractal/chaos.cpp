#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
/// DEFINES
#define WINDOW_HEIGHT 1024
#define WINDOW_WIDTH 764
#define WINDOW_NAME "Fraktal - Chaos Theory"

/// NAMSPACES
using namespace std;
using namespace sf;

/*	
 * 	OPTIONS:
 *  -R <radius> -> the radius should be a float bigger than 0, lower than 1. If not specified the radius is set to 0.5.
 * 	-I <iterations> -> the number of points that should be drawn to screen. If not specified the number will be set to 65536.
 * 	-F <figure> -> generate a specified figure: triangle, XO, snowflake. If not specified you will have to draw the points manually.
 *  -C -> use a conditional generation. Turned off by default. XO and snowflake generation have this option on by default, but using this option will turn it off.
*/

bool			mainVertexPlaced = false; /// CHECK IF THE MAIN VERTEXES HAVE BEEN PLACED, SHOULD BE > 2
bool			firstPointPlaced = false; /// CHECK IF THE FIRST POINT HAS BEEN PLACED
bool			conditionalGeneration = false; /// IF THIS IS TRUE WE WILL USE A SECOND CHECK FOR VERTEXES

unsigned int		mainVertexCount = 0; /// KEEP TRACK OF HOW MANY MAIN VERTEXES WE HAVE
unsigned int		fractalVertexCount = 0; /// KEEP TRACK OF ALL THE VERTEXES SO THE PROGRAM WILL STOP
unsigned int		choice = -1; /// RANDOM CHOICE FOR CHAOS GAME
unsigned int		lastChoice = -1; /// VARIABLE USED FOR THE SECOND CHECK
unsigned int		iterations = 65536; /// HOW MANY ITERATIONS DO WE HAVE

float				radius = 0.5; /// THE DIVISION THAT WE USE FOR THE LINE

VertexArray		vertexArray = VertexArray(Points, 0); /// ARRAY FOR THE POINTS
pair<float, float>	firstPoint; /// THE FIXED RANDOM POINT
pair<float, float>	secondPoint; /// THE DIVISION POINT


/// MAIN FUNCTION
int main(int argc, char **argv)
{
	RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), WINDOW_NAME); /// THE WINDOW
	srand (time(NULL));
	
	/// WE PARSE ARGUMENTS AND USE THEM ACCORDINGLY
	for (size_t index = 1; index < argc; index++)
	{
		cout << argv[index] << '\n';
		
		if (strcmp(argv[index], "-R") == 0)
		{
			if (index + 1 >= argc)
			{
				cout << "Usage: -R <radius>" << '\n';
				return -1;
			}
			
			index++;
			radius = atof(argv[index]);
			cout << "New radius is: " << radius << '\n';
			
			if (radius <= 0 || radius >= 1)
			{
				cout << "Radius should be between 0 and 1" << '\n';
				return -1;
			}
		}
		else if (strcmp(argv[index], "-I") == 0)
		{
			if (index + 1 >= argc)
			{
				cout << "Usage: -I <iterations>" << '\n';
				return -1;
			}
			
			index++;
			iterations = atoi(argv[index]);
			cout << "New iteration number is: " << iterations << '\n';

			if (iterations < 64)
			{
				cout << "Iterations should be bigger than 63" << '\n';
				return -1;
			}
		}
		else if (strcmp(argv[index], "-F") == 0)
		{
			if (index + 1 >= argc)
			{
				cout << "Usage: -F <figure> - triangle / xo / snowflake" << '\n';
				return -1;
			}
			
			index++;
			
			for(size_t aux = 0; argv[index][aux]; aux++)
				argv[index][aux] = tolower(argv[index][aux]);
				
			if (strcmp(argv[index], "triangle") == 0)
			{
				conditionalGeneration = false;
				mainVertexPlaced = true;
				mainVertexCount = 3;
				fractalVertexCount = 3;
				vertexArray.append(Vertex(Vector2f(512, 24), Color::White));
				vertexArray.append(Vertex(Vector2f(24, 740), Color::White));
				vertexArray.append(Vertex(Vector2f(1000, 740), Color::White));
			}
			else if (strcmp(argv[index], "xo") == 0)
			{
				conditionalGeneration = true;
				mainVertexPlaced = true;
				mainVertexCount = 4;
				fractalVertexCount = 4;
				vertexArray.append(Vertex(Vector2f(10, 10), Color::White));
				vertexArray.append(Vertex(Vector2f(10, 758), Color::White));
				vertexArray.append(Vertex(Vector2f(758, 10), Color::White));
				vertexArray.append(Vertex(Vector2f(758, 758), Color::White));
			}
			else if (strcmp(argv[index], "snowflake") == 0)
			{
				conditionalGeneration = true;
				mainVertexPlaced = true;
				mainVertexCount = 5;
				fractalVertexCount = 5;
				vertexArray.append(Vertex(Vector2f(512, 9), Color::White));
				vertexArray.append(Vertex(Vector2f(155, 268), Color::White));
				vertexArray.append(Vertex(Vector2f(292, 687), Color::White));
				vertexArray.append(Vertex(Vector2f(732, 687), Color::White));
				vertexArray.append(Vertex(Vector2f(869, 268), Color::White));
			}
			else
				cout << "Undefined figure... Ignored input" << '\n';
		}
		else if (strcmp(argv[index], "-C") == 0)
		{
			conditionalGeneration = !conditionalGeneration;
			cout << "Generation with extra condition is now ";
			
			if (conditionalGeneration)
				cout << "on";
			else
				cout << "off";
			
			cout << '\n';
		}
		else
		{
			cout << "Error parsing the arguments: " << index << ' ' << argv[index] << '\n';
			return -1;
		}
	}

	while (window.isOpen())
	{
		Event event;

       	while (window.pollEvent(event))
       	{
			/// CLOSE WINDOW WITH X
			if (event.type == Event::Closed)
				window.close();

			/// CLICK EVENT
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				/// IF WE CAN DRAW NEW POINTS, WE ADD THEM TO THE VERTEX ARRAY
				if (!mainVertexPlaced || !firstPointPlaced)
				{
					Vertex point(Vector2f(event.mouseButton.x, event.mouseButton.y), Color::Cyan);
					vertexArray.append(point);
					fractalVertexCount++;
					cout << event.mouseButton.x << ' ' << event.mouseButton.y << ' ' << "point placed" << '\n';
				}
					
				/// COUNT FIXED POINTS
				if (!mainVertexPlaced)
					mainVertexCount++;
				
				/// ADD FIRST POINT OF THE GENERATION
				if (mainVertexPlaced && !firstPointPlaced)
				{
					firstPointPlaced = true;
					firstPoint = pair<float, float>(event.mouseButton.x, event.mouseButton.y);
				}	
					
			}

			if (event.type == Event::KeyPressed)
			{
				/// CLOSE WINDOW WITH ESCAPE KEY
				if (event.key.code == Keyboard::Escape)
					window.close();
				/// IF WE HAVE ALL MAIN POINTS, WE CAN ADD THE FIRST POINT AND START THE GENERATION
				else if (event.key.code == Keyboard::Space && mainVertexCount > 2 && !mainVertexPlaced)
					mainVertexPlaced = true;
			}
		}
		
		/// GENERATE THE POINTS - ALL AT ONCE BECAUSE IT'S FASTER THAN CHECKING THE CONDITION EACH ITERATION
		while (firstPointPlaced && fractalVertexCount < iterations)
		{
			if (conditionalGeneration)
				while (choice == lastChoice)
					choice = rand() % mainVertexCount;
			else
				choice = rand() % mainVertexCount;
			
			/// WE USE THE FORMULA P = P1 + (P2 - P1) * D WHERE P ARE THE X, Y POSITIONS OF THE THREE POINTS	
			pair<float, float> 	secondPoint = pair<float, float>((firstPoint.first + (vertexArray[choice].position.x - firstPoint.first) * radius), (firstPoint.second + (vertexArray[choice].position.y - firstPoint.second) * radius));
			
			/// WE ADD THE NEW GENERATED POINT
			Vertex point(Vector2f(secondPoint.first, secondPoint.second), Color::Magenta);
			vertexArray.append(point);
			fractalVertexCount++;
			
			/// WE CHANGE VARIABLES FOR THEN NEXT ITERATION
			firstPoint = secondPoint;
			lastChoice = choice;
		}
	
		window.draw(vertexArray);
        window.display();
		window.clear();
	}

	return 0;
}
