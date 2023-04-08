# Requirements
Project written in C++17 with glfw, requires you to have 
- glfw3
- glm

# TODO Lists
Lists of things that need to be done to bring this game up to basic performance and gameplay standards

## Gameplay
- [ ] Dynamic chunk generation and deletion
- [ ] More elaborate procedural generation of chunks (see Perlin noise)
- [ ] Removal and addition of blocks to the world
- [ ] Other entities to interact with (hostile/friendly)
- [ ] New shaders
- [ ] Better 3D player navigation
- [ ] Addition of new block textures
- [ ] Simple player UI

## Performance
- [ ] Freeing up VRAM by using more compact data types
- [ ] Different data structures for world composition

## Refactoring
- [ ] Move some generic functions to util.cpp
- [ ] Addition of prefix 'm' to all class variables (debateable?)