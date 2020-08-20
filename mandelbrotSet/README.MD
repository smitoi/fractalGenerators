# Fraktal - Mandelbrot Set

<img src="https://media.giphy.com/media/cPO0gkPEJtEceKmsw6/giphy.gif">

### Usage
`make`. If you don't have SFML installed you should also use `sudo apt-get install libsfml-dev` to install it. To launch it simply use `./mandelbrot` or click the executable. To move around use WASD or the arrows, to zoom in use left click, to zoom out use right click, and to change the number of iterations (the complexity of the fractal) move the mouse wheel forward (to increase number of maximum iterations) or backwards (to decrease number of maximum iterations)

### Performance:
With one thread it used to generate the fractal in ~5s. Now it works on 256 threads, and generates the image in ~0.2s. Note that these resuls were observed with the default number of iterations, and no zoom.

### Resources:
* [SFML Learn](https://www.sfml-dev.org/learn.php)
* [Wikipedia - Mandelbrot Set](https://en.wikipedia.org/wiki/Mandelbrot_set)
* [Stackoverflow for color](https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia)
