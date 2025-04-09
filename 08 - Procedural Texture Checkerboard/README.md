# Procedural Texture Checkerboard using OpenGL (C++)

# Description : 
      This project demonstrates how to generate a procedural checkerboard 
    texture using pure mathematics in OpenGL without loading any external image files.

      - Two quads are rendered in 3D space.
      - A black and white checkerboard pattern is applied to each quad using a 
        procedurally generated texture created at runtime.
      - No image files are used – the texture is built using mathematical logic 
        with alternating pixel values.
      - The right quad is rotated to show the pattern from a different angle, 
        showcasing how the texture wraps correctly on different orientations.

# Features : 
    - Procedural generation of texture using C++ and OpenGL.
    - Texture generated on CPU and passed to GPU using glTexImage2D.
    - Custom logic to alternate colors in a grid pattern.
    - Clean and minimal setup using modern OpenGL.

# How It Works : 
      A 2D array is filled using a formula like:
      
          int checker = ((i / squareSize) % 2) ^ ((j / squareSize) % 2);
    
       This toggles between 0 and 1 in a grid pattern, allowing black and white
    colors to alternate for the checker effect.

   
# Reference : Super Bibal 7th ediition 

# Technical Details : 
    Rendering API        : OpenGL
    Programming Language : C++ 
    Shading Language     : GLSL
    Operating System     : Windows 10
    CPU                  : Intel(R) Core(TM) i5-6200U 
    GPU                  : AMD Radeon (TM) R5 M330
