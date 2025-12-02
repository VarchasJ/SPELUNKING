#ifndef TREASURE_H
#define TREASURE_H

#include "room.h"
#include <stdbool.h>

// Spawn treasure in room (75% chance)
void SpawnTreasure(Room* room);

// Get distance to treasure
float GetDistanceToTreasure(const Room* room, int playerX, int playerY);

// Get tracker text based on distance
const char* GetTrackerText(float distance);

// Check if player can collect treasure
bool TryCollectTreasure(Room* room, int playerX, int playerY);

#endif // TREASURE_H
