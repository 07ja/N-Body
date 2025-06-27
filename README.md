# N-Body Simulation
#### A real time N-body simulation with interactive first person camera controls, built in C using SDL3 for input and window management, OpenGL for rendering, and OpenCL for parallel computation

## How to Run
- Have the following dependencies installed: SDL3, glad (OpenGL Loader), OpenCL
  - Place glad.c in the src/ directory
  - Copy the corresponding .h files into the include/ directory
  - Update the Makefile to include correct paths for SDL3 and OpenCL headers and libraries
- Use 'make run' to build and run the simulation
- Use 'make clean' to remove the simulation from folder

## Controls
- [R] - Restart simulation
- [T] - Toggle simulation on/off
- [W], [A], [S], [D] - Move around in the world
- [Mouse] - Look around the world

## Demo
<div align="center">
  <a href="misc/Nbody1.gif"><img src="misc/Nbody1.gif" width="700"/></a><br>
  <a href="misc/Nbody2.gif"><img src="misc/Nbody2.gif" width="700"/></a><br>
  <a href="misc/Nbody3.gif"><img src="misc/Nbody3.gif" width="700"/></a>
</div>
