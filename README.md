# Greyfish Engine
The engine uses OpenGL to render objects in a range of ways. Users can add images, models, and shaders to arrange scenes as they like. 
![image](https://user-images.githubusercontent.com/55700734/196752984-0929851c-d1f2-4da2-9e3a-25c87cfea691.png)

## Source Code
The application works through two core classes, instantiated at the beginning of the application runtime, the Runner class and the Scene class. The Runner regulates the execution of the Scenes methods through the mainloop, and the Scene class contains neccessary variables and objects to represent the objects you see rendered on the screen. The MasterUI class is also another class of note, this class handles the rendering of the UI.
### Requirements
 - glfw
 - glm
 - imgui 1.88
 - yamp-cpp
 - nativefiledialog
###
The imgui dependancies are included within the repository, however, you will need to install glfw, glm, yaml-cpp, and nativefiledialog yourself, and then link these libraries to the Visual Studio solution in order to debug the code. You will also need glad, you can download the include and library files from [here](https://glad.dav1d.de/). Include the glad.c file at the root of the repository, this is essential as this file is machine specific and as such couldn't be included in the shared repository.
## Download
 - [Download Source Code.](https://github.com/lwhite14/GreyfishEngine/archive/main.zip)
 - Or go to the releases tab and download both a build and the source code!
#
Made by Luke White!
