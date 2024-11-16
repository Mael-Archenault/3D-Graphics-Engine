# Context

I wanted to see what was behind a 3D graphics engine. So I made researches, and I found a lot of interesting mathematics behind it. So I challenged myself by trying to build an OBJ file viewer in C. It turned out pretty well

# Credits

The code is not entirely thought by me. I used some help from a series of youtube videos (even though they were in C++). Here they are : 
- part 1 : https://www.youtube.com/watch?v=ih20l3pJoeU&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=22&ab_channel=javidx9
- part 2 : https://www.youtube.com/watch?v=XgMWc6LumG4&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=23&ab_channel=javidx9
- part 3 : https://www.youtube.com/watch?v=HXSuNxpCzdM&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=24&ab_channel=javidx9

# Principle used

All 3D models can be constructed using only triangles. That's how the renderer works. We draw each triangle one after another. 

An OBJ file is constructed as follows :
- gives positions of all vertices of the model, and gives them a label
- gives all triangles of the model by giving 3 labels of vertices.

For each triangle of the model:
- we rotate then translate the triangle so that we can control its position in space (translation is locked in my code)
- for each point of the triangle, we compute the x and y coordinates on the screen by multiplying space coordinates with a specific projections matrix (that takes into account perspective)
- By using dot product, we are able to compute the luminance of the triangle based on the light direction
- By using vector product, we are able to know which triangle of the model to render : triangles that are facing the camera
- We do other operations to get rid of some minor graphic issues
# User manual

I haven't done any interface to use this code. If you are a bit of a coder, you should be able to find what variables to change to try different things. However, I will give you some informations on basic changes.

First, you need to download the git folder and place it in one of your directory (it is important to keep all files together)
## To execute the code

- execute the "main.exe" file to get a quick view of the 3D renderer

If you want to change parameters, you will need a C development environment (I used Visual Studio Code).

Open "main.c", that's where you will do most of the changes.

## Change viewed model
In "main.c" in the function "openObj()", change the path in the first line to the path of the new OBJ file. You already have various files in /3d_models.

## Display lines

In "main.c", change the value of the variable "drawLines" to 1 if you want to see lines between triangles.

## Change color of the model

In "main.c", change the value of the variable "modelColor" to the new RGB value.

## Generate a new "main.exe" with applied changes

To do that, you will need to recompile the project. 
- open a terminal
- go into the project directory using cd commands
- compile the project with this line of code (you need to have gcc compiler installed) :gcc main.c drawing_functions.c -o  main.exe -Isrc/include -Lsrc/lib -lSDL2 -lSDL2main 

The "main.exe" file should now have changed to match the new version of code.

Try everything you want with this code, have fun.
