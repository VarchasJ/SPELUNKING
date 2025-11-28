# SPELUNKING

![SPELUNKING Logo](LOGO>jpg)

## Spelunking - Area 1

A top-down, tile-based spelunking game built with C and Raylib.

## Game Overview

Play as Peach exploring procedurally generated rooms, searching for treasure while avoiding zombies and bats. Use your digging ability and treasure tracker to find hidden chests!

## Controls

- **Arrow Keys**: Move and rotate (press opposite direction to rotate, same direction to move)
- **D**: Dig tile in front of you
- **SPACE**: Attack (3-tile cone in facing direction)
- **ESC**: Quit game

## Game Mechanics

- **Rooms**: 20×15 tiles, each 16×16 pixels
- **Treasure**: 75% chance per room, use tracker ("cold" to "burning") to find it
- **Digging**: Dig the correct tile to reveal a red chest, walk on it to collect
- **Zombies**: Spawn in 25% of rooms, 5 HP each, move every 0.75s, deal 1 HP damage per tick
- **Bats**: Spawn in 25% of rooms, deal instant 1 HP damage on room entry
- **HP**: Start with 5 HP, game resets on death
- **Room Transitions**: Walk to edge and press movement key again to enter adjacent room

## Building

### Prerequisites

- GCC compiler
- Raylib library installed
- Linux system (tested on Ubuntu/Debian)

### Install Raylib

```bash
sudo apt-get update
sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

### Compile and Run

```bash
cd spelunking_area1
make
./build/spelunking_area1
```

Or use the shortcut:

```bash
make run
```

### Clean Build

```bash
make clean
```

## Project Structure

```
spelunking_area1/
├── src/
│   ├── main.c          # Main game loop and integration
│   ├── player.c/h      # Player movement, digging, attacking
│   ├── room.c/h        # Room generation and transitions
│   ├── zombie.c/h      # Zombie AI and pathfinding
│   ├── treasure.c/h    # Treasure spawning and tracking
│   ├── hud.c/h         # HUD rendering
│   └── utils.c/h       # Utility functions
├── build/              # Compiled binaries
├── assets/             # Placeholder for future sprites
├── Makefile
└── README.md
```

## Gameplay Tips

- Use the treasure tracker to find hidden chests efficiently
- Avoid zombies by planning your path carefully
- Watch your HP - bats can catch you off guard when entering new rooms
- Dig strategically to reveal treasure without wasting moves

## Future Enhancements

- Replace placeholder rectangles with actual sprites
- Add more enemy types
- Implement multiple areas
- Add sound effects and music
- Save/load game state
- Boss battles

## License

This project is open source and available under the MIT License.

## Author

Varchas J
