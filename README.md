# Fraktal

### Description
Fraktal started as a little project of mine to work with C++ and SFML to generate the Mandlebrot set, but I expanded it to include more fractals. While doing this project I learned a lot of about 2D geometry, and also discovered some cool concepts about computer graphics. There are some fractals generators in this repository, each one of them in a separate folder. If you go to any folder, you will find the source, the makefile and a readme that will explain how to use the executable once compiled.


### Compilation

#### Linux:

Go to the folder of the generator you want to compile and use `make`. If you don't have SFML installed you should also use `sudo apt-get install libsfml-dev` to install it.


#### Windows:

Read the [documentation](https://www.sfml-dev.org/tutorials/2.5/) and setup SFML to run the test program. You can then overwrite it with my source and run it.


### Fractals included
* Classic fractals:

    * [Chaos Game](https://en.wikipedia.org/wiki/Chaos_game)
    
    * [Koch Snowflake](https://en.wikipedia.org/wiki/Koch_snowflake)
    
    * ~~Tree fractal~~


* Complex numbers based:

    * ~~Mandlebrot Set~~
    
    * ~~Julia Set~~
    
    * ~~Burning Ship~~
