# Blocker

Sampo Siltanen 2017

Blocker sets out to become Minecraft-like game that is built with my own game engine based on OpenGL 3.3. 

The main purpose of this project is for me to learn to use OpenGL. The secondary objective is to produce a project with professional standards. To achieve these standards, I intend to pay close attention to code quality, efficiency, architecture, project management, branching, testing, documenting, DRY principle, and Design by Contract principle. Because C++ does not support DbC, I implemented my own contract enforcment logic.

In order to keep documentation up-to-date, it is automatized with doxygen. In the spirit of DRY, contract and function documentation is kept up-to-date by using CodeCommenter script ( https://github.com/nanhasa/CodeCommenter ) that creates documentation based on the actual code and contracts in it.

Contents:
- Assets: 	Holds the original formats of assets used in this project   
- Docs: 	Holds doxygen template and codeCommenter script described above   
- Game: 	Holds self-contained version of the game   
- Libs: 	Holds libraries used in building the game   
- Source: 	Holds source files and includes used in this game   
- Temp: 	Holds all the intermediate files used in building the game   
- Test: 	Holds everything that concerns testing the game   