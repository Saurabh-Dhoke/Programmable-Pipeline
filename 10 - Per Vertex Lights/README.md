# 💡 Per-Vertex Lighting on a Steady Sphere using OpenGL

# Description : 
      This OpenGL application demonstrates per-vertex Phong lighting on a 
    steady 3D sphere using the Programmable Pipeline (GLSL shaders). Lighting 
    calculations—including ambient, diffuse, and specular components—are
    performed in the vertex shader, and the results are interpolated across
    fragments for smooth shading.

# ✨ Features
      - Phong lighting model (Ambient + Diffuse + Specular) 
      - Lighting calculated per-vertex (Gouraud shading)
      - Configurable material and light properties
      - Keyboard interaction to toggle lighting (L) and fullscreen (F)
      - Uses custom Sphere.h for generating sphere geometry
      - Includes shader compilation and OpenGL setup with error logging
      - Log file generation for debugging (log.txt)

# 🛠️ Technologies Used
      - C / C++
      - OpenGL (GLUT, GLEW)
      - GLSL (Vertex and Fragment Shaders)
      - Windows API for window creation
      - vmath.h for matrix operations

# 🎮 Controls
      Key	Action
      L	   Toggle lighting
      F	   Toggle fullscreen
      ESC	Exit the program
      
# Reference : Super Bibal 7th ediition 

# Technical Details : 
    Rendering API        : OpenGL
    Programming Language : C++ 
    Shading Language     : GLSL
    Operating System     : Windows 10
    CPU                  : Intel(R) Core(TM) i5-6200U 
    GPU                  : AMD Radeon (TM) R5 M330
