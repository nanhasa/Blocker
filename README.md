# Blocker

Sampo Siltanen 2017

Blocker sets out to become Minecraft-like game that is built with my own game engine based on OpenGL 3.3. 

The main purpose of this project is for me to learn to use OpenGL. The secondary objective is to produce a project with professional standards. To achieve these standards, I intend to pay close attention to code quality, efficiency, architecture, project management, branching, testing, documenting, DRY principle, and Design by Contract principle. Because C++ does not support DbC, I implemented my own contract enforcement logic.

In order to keep documentation up-to-date, it is automatized with doxygen. In the spirit of DRY, contract and function documentation is kept up-to-date by using CodeCommenter script ( https://github.com/nanhasa/CodeCommenter ) that creates documentation based on the actual code and contracts in it.

Contents:
- Assets: 	Holds the original formats of assets used in this project   
- Docs: 	Holds doxygen template and codeCommenter script described above   
- Game: 	Holds self-contained version of the game   
- Libs: 	Holds libraries used in building the game   
- Scripts:	Holds scripts used to build and update project   
- Source: 	Holds source files and includes used in this game   
- Temp: 	Holds all the intermediate files used in building the game   
- Test: 	Holds everything that concerns testing the game   
   
   
   
How to build:   
install python, make sure it is in path (https://www.python.org/downloads/)    
install doxygen, make sure it is in path (https://sourceforge.net/projects/doxygen/)    
    
Build dependencies     
- release build multi-threaded dll, debug multi-threaded debug dll     
- only win32 target    
- Place .lib files to Libs/x86/[Debug|Release]/    
    
Following above instruction build    
- glew 2.1.0 (http://glew.sourceforge.net/)      
- glfw 3.2.1 (http://www.glfw.org/)    
- glm 0.9.8.4 (https://glm.g-truc.net/0.9.8/index.html)    
- googletest (https://github.com/google/googletest)     
     
Add main project dependency include files to Source/3rdParty/x/     
- glew (GL)    
- glfw (GLFW)    
- glm (glm)     
- rapidjson (rapidjson)    
Add test project dependency include files to Test/Source/3rdParty/x/   
- googletest (gtest)  
     
If Blocker is being run through Visual Studio:
- In Visual Studio, change the working directory for Blocker project
	Settings > Project Settings > Debugging > Working Directory to $(TargetDir)