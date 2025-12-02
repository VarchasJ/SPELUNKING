#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>

#define ROOM_WIDTH 20
#define ROOM_HEIGHT 15
#define TILE_SIZE 32
#define MAX_ROOMS 100

// Tile types
typedef enum {
    TILE_FLOOR,
    TILE_CORNER,
    TILE_DUG,
    TILE_CHEST
} TileType;

// Room structure
typedef struct {
    TileType tiles[ROOM_HEIGHT][ROOM_WIDTH];
    bool hasTreasure;
    int treasureX;
    int treasureY;
    bool treasureCollected;
    bool hasZombies;
    bool hasBats;
    bool visited;
    int x;
    int y;
} Room;

// Room coordinate
typedef struct {
    int x;
    int y;
} RoomCoord;

// Initialize room system
void InitRoomSystem(void);

// Get or create room at coordinate
Room* GetRoom(int roomX, int roomY);

// Generate a new room
void GenerateRoom(Room* room);

// Check if tile is walkable
bool IsTileWalkable(Room* room, int x, int y);

// Check if position is on room edge (for transitions)
bool IsOnEdge(int x, int y);

// Get current room coordinates
RoomCoord GetCurrentRoomCoord(void);

// Set current room coordinates
void SetCurrentRoomCoord(int x, int y);

#endif // ROOM_H
