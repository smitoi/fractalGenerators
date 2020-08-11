# Fraktal - Chaos Game Fractal

### Usage
`make`. If you don't have SFML installed you should also use `sudo apt-get install libsfml-dev` to install it. When you open it, you should draw points with left click (three or more points are required). Press space when you are done, and then draw the first point. After the first point is drawn, the generation will start.

Extra flags are:

`-R <radius>` - each point is a given fraction between two other points (the previous and one randomly selected) - without this flag it will be set to 0.5

`-I <iterations>` - the number of points that should be drawn on screen - without this flag it will be set to 65536

`-F < triangle | xo | snowflake >` - if you don't want to draw the figure yourself you should use this. Triangle will draw a triangle, xo will draw a square and snowflake will draw a pentagon (the names are describing the generated image).

`-C` - this is an extra option, if you don't want the same point to be selected twice.
