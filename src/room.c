#include "room.h"
#include "utils.h"
#include <string.h>

// Room storage
static Room rooms[MAX_ROOMS];
static int roomCount = 0;
static RoomCoord currentRoomCoord = {0, 0};

void InitRoomSystem(void) {
    roomCount = 0;
    currentRoomCoord.x = 0;
    currentRoomCoord.y = 0;
    memset(rooms, 0, sizeof(rooms));
}

// Hash function for room coordinates
static int GetRoomIndex(int roomX, int roomY) {
    for (int i = 0; i < roomCount; i++) {
        // We'll store coordinates in a simple array and search
        // For simplicity, we'll use a linear search
    }
    return -1;
}

Room* GetRoom(int roomX, int roomY) {
    // Simple linear search for existing room
    for (int i = 0; i < roomCount; i++) {
        // We need to track room coordinates separately
        // For now, just use array index as room progression
    }
    
    // Create new room if not found
    if (roomCount < MAX_ROOMS) {
        Room* newRoom = &rooms[roomCount];
        roomCount++;
        GenerateRoom(newRoom);
        return newRoom;
    }
    
    return &rooms[0]; // Fallback
}

void GenerateRoom(Room* room) {
    memset(room, 0, sizeof(Room));
    
    // Initialize all tiles as floor
    for (int y = 0; y < ROOM_HEIGHT; y++) {
        for (int x = 0; x < ROOM_WIDTH; x++) {
            room->tiles[y][x] = TILE_FLOOR;
        }
    }
    
    // Set 2x2 corners as non-walkable
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            room->tiles[y][x] = TILE_CORNER; // Top-left
            room->tiles[y][ROOM_WIDTH - 1 - x] = TILE_CORNER; // Top-right
            room->tiles[ROOM_HEIGHT - 1 - y][x] = TILE_CORNER; // Bottom-left
            room->tiles[ROOM_HEIGHT - 1 - y][ROOM_WIDTH - 1 - x] = TILE_CORNER; // Bottom-right
        }
    }
    
    // 75% chance to spawn treasure
    if (RandomInt(1, 100) <= 75) {
        room->hasTreasure = true;
        // Spawn treasure at least 2 tiles from borders: x ∈ [2,17], y ∈ [2,12]
        room->treasureX = RandomInt(2, 17);
        room->treasureY = RandomInt(2, 12);
        room->treasureCollected = false;
    }
    
    // 25% chance for zombies
    room->hasZombies = (RandomInt(1, 100) <= 25);
    
    // 25% chance for bats
    room->hasBats = (RandomInt(1, 100) <= 25);
    
    room->visited = false;
}

bool IsTileWalkable(Room* room, int x, int y) {
    if (x < 0 || x >= ROOM_WIDTH || y < 0 || y >= ROOM_HEIGHT) {
        return false;
    }
    
    TileType tile = room->tiles[y][x];
    return tile != TILE_CORNER;
}

bool IsOnEdge(int x, int y) {
    return (x == 0 || x == ROOM_WIDTH - 1 || y == 0 || y == ROOM_HEIGHT - 1);
}

RoomCoord GetCurrentRoomCoord(void) {
    return currentRoomCoord;
}

void SetCurrentRoomCoord(int x, int y) {
    currentRoomCoord.x = x;
    currentRoomCoord.y = y;
}
