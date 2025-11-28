#include "treasure.h"
#include "utils.h"
#include <math.h>

void SpawnTreasure(Room* room) {
    // Already handled in GenerateRoom
}

float GetDistanceToTreasure(const Room* room, int playerX, int playerY) {
    if (!room->hasTreasure || room->treasureCollected) {
        return 999.0f; // Very far if no treasure
    }
    
    float dx = (float)(room->treasureX - playerX);
    float dy = (float)(room->treasureY - playerY);
    return sqrtf(dx * dx + dy * dy);
}

const char* GetTrackerText(float distance) {
    if (distance >= 11.0f) return "cold";
    if (distance >= 8.0f) return "chill";
    if (distance >= 5.0f) return "hot";
    if (distance >= 3.0f) return "hotter";
    return "burning";
}

bool TryCollectTreasure(Room* room, int playerX, int playerY) {
    if (!room->hasTreasure || room->treasureCollected) {
        return false;
    }
    
    // Check if player is on a chest tile
    if (room->tiles[playerY][playerX] == TILE_CHEST) {
        room->treasureCollected = true;
        return true;
    }
    
    return false;
}
