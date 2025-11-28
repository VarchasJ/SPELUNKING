#include "player.h"
#include "room.h"
#include "treasure.h"
#include "raylib.h"
#include <math.h>

// External reference to current room
extern Room* currentRoom;

void InitPlayer(Player* player) {
    player->x = 10;
    player->y = 7;
    player->facing = FACING_SOUTH;
    player->hp = 5;
    player->score = 0;
    player->lastDigTime = 0.0;
    player->lastAttackTime = 0.0;
}

void RotatePlayer(Player* player, Direction newDirection) {
    player->facing = newDirection;
}

bool TryMovePlayer(Player* player) {
    int newX = player->x;
    int newY = player->y;
    
    switch (player->facing) {
        case FACING_NORTH: newY--; break;
        case FACING_SOUTH: newY++; break;
        case FACING_WEST: newX--; break;
        case FACING_EAST: newX++; break;
    }
    
    if (IsTileWalkable(currentRoom, newX, newY)) {
        player->x = newX;
        player->y = newY;
        
        // Try to collect treasure if on chest
        if (TryCollectTreasure(currentRoom, player->x, player->y)) {
            player->score += 100;
            // Refill HP to max
            player->hp = 5;
            // Change the chest tile to black (dug) after collection
            currentRoom->tiles[player->y][player->x] = TILE_DUG;
        }
        
        return true;
    }
    
    return false;
}

void PlayerDig(Player* player) {
    double currentTime = GetTime();
    
    // Check cooldown (0.5s)
    if (currentTime - player->lastDigTime < 0.5) {
        return;
    }
    
    player->lastDigTime = currentTime;
    
    // Get tile in front of player
    int digX = player->x;
    int digY = player->y;
    
    switch (player->facing) {
        case FACING_NORTH: digY--; break;
        case FACING_SOUTH: digY++; break;
        case FACING_WEST: digX--; break;
        case FACING_EAST: digX++; break;
    }
    
    // Check bounds
    if (digX < 0 || digX >= ROOM_WIDTH || digY < 0 || digY >= ROOM_HEIGHT) {
        return;
    }
    
    // Check if this is the treasure location
    if (currentRoom->hasTreasure && !currentRoom->treasureCollected &&
        digX == currentRoom->treasureX && digY == currentRoom->treasureY) {
        // Correct dig - reveal chest
        currentRoom->tiles[digY][digX] = TILE_CHEST;
    } else if (currentRoom->tiles[digY][digX] == TILE_FLOOR) {
        // Incorrect dig - mark as dug
        currentRoom->tiles[digY][digX] = TILE_DUG;
    }
}

void PlayerAttack(Player* player) {
    double currentTime = GetTime();
    
    // Check cooldown (0.12s)
    if (currentTime - player->lastAttackTime < 0.12) {
        return;
    }
    
    player->lastAttackTime = currentTime;
    
    // Attack is handled in main.c with zombie damage
}

void DamagePlayer(Player* player, int damage) {
    player->hp -= damage;
    if (player->hp < 0) player->hp = 0;
}

void DrawPlayer(const Player* player) {
    // External reference to player texture (defined in main.c)
    extern Texture2D playerTexture;
    
    int screenX = player->x * TILE_SIZE;
    int screenY = player->y * TILE_SIZE;
    
    // Scale factor: texture is 16x16, tile is 32x32
    float scale = (float)TILE_SIZE / 16.0f;
    
    // Draw player texture scaled to fit tile
    DrawTextureEx(playerTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
}
