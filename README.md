# pacman

A Pac-Man clone using Weaver Engine, play this game at:
https://thiagoharry.github.io/pacman/

## Game Controls

* ARROW KEYS: Control pacman and the cursor at title screen

* ENTER: Select option in the title screen.

## Compiling the game

This is a game inspired by classical Pac-Man written in C using the
[Weaver Game Engine](https://thiagoharry.github.io/weaver/). The game
can be compiled in a Linux environment with a C compiler and GNU Make.

### Compiling the game as a Linux executable

First install the dependencies. In Ubuntu, Linux Mint and related
distros, the following command should work:

```
sudo apt-get install build-essential libx11-dev mesa-common-dev libxrandr-dev libglew-dev libopenal1 libopenal-dev vim-common libpng16-dev libmpg123-dev
```
In other distros, research how to install the equivalent packages.

Check file `conf/conf.h` and change the line which starts with
`#define W_TARGET` to:

```C
#define W_TARGET W_ELF
```

Then run the command `make` and `make install` as root. And finally you can run the game with
`pacman` at the command line.

### Compiling the game as a web page

First install the dependencies. In Ubuntu, Linux Mint and related
distros, the following command should work:

```
sudo apt-get install emscripten vim-common
```

But you must ensure that Emscripten is in version 1.34.0 or
greater. Otherwise, uninstall it and install from another source. In
other distros, research how to install the equivalent packages.

Check file `conf/conf.h` and change the line which starts with
`#define W_TARGET` to:

```C
#define W_TARGET W_WEB
```

Then run the command `make`. It should produce a directory called
`docs` where you can open the file `index.html` in a web browser to play
the game.

## Credits

###### Created and programmed by

* [thiagoharry](https://github.com/thiagoharry/)

###### Images and illustrations created by

* [thiagoharry](https://github.com/thiagoharry/)
* [Daniel Cook](http://www.lostgarden.com/2007/05/dancs-miraculously-flexible-game.html)
* [David Vignoni](http://www.icon-king.com/)
* [Sangjun Oh](https://pixabay.com/en/users/JJuni-27151/)
* [Felikin100](https://pixabay.com/en/users/Felikin100-3374205/)
* [Clker-Free-Vector-Images](https://pixabay.com/en/users/Clker-Free-Vector-Images-3736/)

###### Sound effects created by

* [Joseph Levesque "josepharaoh99."](https://freesound.org/people/josepharaoh99/)
* [Vladislav Krotov](https://opengameart.org/users/vinrax)
* [ggctuk](https://freesound.org/people/ggctuk/)
* [Pac-Guy](https://opengameart.org/content/pac-guy)

###### Music created by

* [Kevin Bouchard](https://www.youtube.com/channel/UCcGuSbMWfKhHBl0YDmBsmQA)

