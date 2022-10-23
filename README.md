# openNES Snake
## Rebuilding classic games for the NES as Open Source  
![IMG1](snake-screenhost-1.png) ![IMG2](snake-screenhost-2.png) ![IMG3](snake-screenhost-3.png)

## Project Description
This project is the first entry in what I hope will be a long line of projects aimed at recreating classic video games   
for the Nintendo Entertainment System on an open source basis.  
The purpose of these projects is to get more enthusiastic people interested in programming for the NES by providing code examples
and providing a starting point for developing new games from scratch.

This particular project aims to recreate the classic game Snake, also known as Nibbles. As it is the first of its kind in this series of projects,
it is reduced to the core mechanics of the original game, in order to avoid information overload for future beginners
but also because of the lack of knowledge of the main author at that time :stuck_out_tongue_winking_eye:.

The project was programmed entirely in C, using Shiru's NESLib and the cc65 cross compiler (for more references see the Toolkit section).

## License
This project is licensed under the zlib license. See also the attached README file.

## Toolkit
* [cc65 cross compiler v.2.5](https://github.com/cc65/cc65) - A freeware C compiler for 6502 based systems

* [Shiru's NESLib ](http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm) - An programming library on top of cc65 for more comfortable NES development. See also the more up to date fork [here](https://github.com/clbr/neslib).

* [Tile Molester v.0.16](http://www.romhacking.net/utilities/109/) - A tileset editor, for designing tiles/sprites

* [NES Screen Tool v.2.04](https://shiru.untergrund.net/software.shtml) - A tool that allows to create and edit NES nametables, attributes, palettes, patterns (tiles), and metasprites

* [FamiTracker v.0.4.2](http://famitracker.com) - Famous tracker for producing music for the NES/Famicom-systems

* [Doxygen v.1.8.12](http://www.stack.nl/~dimitri/doxygen/) - Code documentation tool for C/C++ projects

* [FCEUX v.2.2.3](http://www.fceux.com/web/home.html) - Famous NES emulator in the homebrew scene with many debugging utilities.

* [NesDev Forum](http://forums.nesdev.com/) - You will always find friendly, skilled and helpful people in this community, who are willing to help with problems you are facing during your project.

* [Nerdy Nights Tutorials](http://nintendoage.com/pub/faq/NA/index.html?load=nerdy_nights_out.html) - Even if it is based on introducing you to assembly, this is the classic lecture for understanding the NES architecture for many years.


## How To Build Locally
To build a `.NES` rom from the project's sources, execute the build script at `build/build.sh` from the `build` directory or the script `build/build.bat` if you are working on Windows. Note that these scripts depend on the cc65 cross compiler, so you need ensure that this is available on your system.

## Development Container
This project provides [VS Code Development Container](https://code.visualstudio.com/docs/remote/containers) configuration at `.devcontainer/`. Via this configuration, you automatically have a complete development environment for developing NES games.

## Notes for Contributors
Feel free to fork this project and extend it. If you find bugs, please submit them as issues so we can discuss together how you can fix them. 
All your contributions need to include explaining Doxygen comments in order to share your knowledge with future contributors and maintaining the project's code documentation.
