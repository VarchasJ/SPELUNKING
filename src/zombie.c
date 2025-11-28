#include "zombie.h"
#include "room.h"
#include "raylib.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

// External reference to current room
extern Room* currentRoom;

void InitZombieList(ZombieList* list) {
    memset(list, 0, sizeof(ZombieList));
}

void SpawnZombies(ZombieList* list) {
    InitZombieList(list);
    
    if (!currentRoom->hasZombies) {
        return;
    }
    
    // Spawn 2-4 zombies
    int numZombies = RandomInt(2, 4);
    list->count = numZombies;
    
    for (int i = 0; i < numZombies; i++) {
        list->zombies[i].active = true;
        list->zombies[i].hp = 15;  // Increased from 5 to 15
        list->zombies[i].lastMoveTime = 0.0;
        
        // Spawn at random walkable position (not on corners)
        do {
            list->zombies[i].x = RandomInt(2, ROOM_WIDTH - 3);
            list->zombies[i].y = RandomInt(2, ROOM_HEIGHT - 3);
        } while (!IsTileWalkable(currentRoom, list->zombies[i].x, list->zombies[i].y));
    }
}

// Simple BFS pathfinding
static void MoveZombieTowardsPlayer(Zombie* zombie, int playerX, int playerY) {
    if (!zombie->active) return;
    
    // Simple greedy approach - move closer on Manhattan distance
    int dx = playerX - zombie->x;
    int dy = playerY - zombie->y;
    
    int newX = zombie->x;
    int newY = zombie->y;
    
    // Try to move in direction that reduces distance most
    if (abs(dx) > abs(dy)) {
        newX += (dx > 0) ? 1 : -1;
    } else if (dy != 0) {
        newY += (dy > 0) ? 1 : -1;
    }
    
    // Check if new position is walkable
    if (IsTileWalkable(currentRoom, newX, newY)) {
        zombie->x = newX;
        zombie->y = newY;
    } else if (abs(dx) > abs(dy)) {
        // Try moving in Y direction instead
        newX = zombie->x;
        newY = zombie->y + ((dy > 0) ? 1 : -1);
        if (IsTileWalkable(currentRoom, newX, newY)) {
            zombie->x = newX;
            zombie->y = newY;
        }
    } else {
        // Try moving in X direction instead
        newX = zombie->x + ((dx > 0) ? 1 : -1);
        newY = zombie->y;
        if (IsTileWalkable(currentRoom, newX, newY)) {
            zombie->x = newX;
            zombie->y = newY;
        }
    }
}

void UpdateZombies(ZombieList* list, int playerX, int playerY) {
    double currentTime = GetTime();
    
    for (int i = 0; i < list->count; i++) {
        if (!list->zombies[i].active) continue;
        
        // Move every 0.75s
        if (currentTime - list->zombies[i].lastMoveTime >= 0.75) {
            list->zombies[i].lastMoveTime = currentTime;
            MoveZombieTowardsPlayer(&list->zombies[i], playerX, playerY);
        }
    }
}

int CheckZombieDamage(const ZombieList* list, int playerX, int playerY) {
    int damage = 0;
    
    for (int i = 0; i < list->count; i++) {
        if (!list->zombies[i].active) continue;
        
        // Check if zombie is on same tile as player
        if (list->zombies[i].x == playerX && list->zombies[i].y == playerY) {
            damage += 1;
        }
    }
    
    return damage;
}

void DamageZombieAt(ZombieList* list, int x, int y, int damage) {
    for (int i = 0; i < list->count; i++) {
        if (!list->zombies[i].active) continue;
        
        if (list->zombies[i].x == x && list->zombies[i].y == y) {
            list->zombies[i].hp -= damage;
            if (list->zombies[i].hp <= 0) {
                list->zombies[i].active = false;
            }
        }
    }
}

void DrawZombies(const ZombieList* list) {
    // External reference to zombie texture (defined in main.c)
    extern Texture2D zombieTexture;
    
    // Scale factor: texture is 16x16, tile is 32x32
    float scale = (float)TILE_SIZE / 16.0f;
    
    for (int i = 0; i < list->count; i++) {
        if (!list->zombies[i].active) continue;
        
        int screenX = list->zombies[i].x * TILE_SIZE;
        int screenY = list->zombies[i].y * TILE_SIZE;
        
        // Draw zombie texture scaled to fit tile
        DrawTextureEx(zombieTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
        
        // Draw HP bar above zombie
        int barWidth = TILE_SIZE;
        int barHeight = 4;
        int barX = screenX;
        int barY = screenY - 6;
        
        // Background (red)
        DrawRectangle(barX, barY, barWidth, barHeight, RED);
        
        // Health bar (green) - proportional to current HP
        int maxHP = 15;
        float healthPercent = (float)list->zombies[i].hp / (float)maxHP;
        int healthWidth = (int)(barWidth * healthPercent);
        DrawRectangle(barX, barY, healthWidth, barHeight, GREEN);
        
        // Border
        DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
    }
}
