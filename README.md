# Fractal Generators

### Description
This started as a little project of mine to work with C++ and SFML to generate the Mandlebrot set, but I expanded it to include more fractals and options.
### Fractals included:

* [Mandlebrot Set](https://en.wikipedia.org/wiki/Mandelbrot_set)
* [Julia Set](https://en.wikipedia.org/wiki/Burning_Ship_fractal)
* [Burning Ship](https://en.wikipedia.org/wiki/Burning_Ship_fractal)
    
### Compilation

#### Linux:

Go to the folder of the generator and use `make`. If you don't have SFML installed you should also use `sudo apt-get install libsfml-dev` to install it. 


#### Windows:

Read the [documentation](https://www.sfml-dev.org/tutorials/2.5/) and setup SFML to run the test program. You can then overwrite it with the source in this repo and run it. 


<img src="https://media.giphy.com/media/cPO0gkPEJtEceKmsw6/giphy.gif">

### Usage

To launch it simply use `./complex {mandelbrot | julia | burningship}`. To move around use WASD or the arrows, to zoom in use left click, to zoom out use right click, and to change the number of iterations (the complexity of the fractal) move the mouse wheel forward (to increase number of maximum iterations) or backwards (to decrease number of maximum iterations).

Julia Set also has an option so you can modify the number used in the generation, the keys are Z and X to increase and decrease the real part of the number, and C and V to increase and decrease the imaginary part of the number.




### Resources:
* [SFML Learn](https://www.sfml-dev.org/learn.php)
* [Wikipedia - Mandelbrot Set](https://en.wikipedia.org/wiki/Mandelbrot_set)
* [Wikipedia - Julia Set](https://en.wikipedia.org/wiki/Julia_set)
* [Wikipedia - Burning Ship](https://en.wikipedia.org/wiki/Burning_Ship_fractal)
* [StackOverflow](https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia) for color
