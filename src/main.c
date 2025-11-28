#include "raylib.h"
#include "player.h"
#include "room.h"
#include "zombie.h"
#include "treasure.h"
#include "hud.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

// Global current room and zombie list
Room* currentRoom = NULL;
ZombieList zombieList;
double lastZombieDamageTime = 0.0;
bool batDamageApplied = false;

// Attack indicator
bool showAttackIndicator = false;
double attackIndicatorTime = 0.0;
int attackTilesCache[3][2];
int attackTilesCacheCount = 0;

// Bat attack indicator
bool showBatAttack = false;
double batAttackTime = 0.0;
Vector2 batPositions[4];  // 4 bats flying around

// Textures
Texture2D floorTexture;
Texture2D wallTexture;
Texture2D floorDugTexture;
Texture2D chestTexture;
Texture2D playerTexture;
Texture2D zombieTexture;
Texture2D batsTexture;

// Draw the current room
void DrawRoom(const Room* room) {
    for (int y = 0; y < ROOM_HEIGHT; y++) {
        for (int x = 0; x < ROOM_WIDTH; x++) {
            int screenX = x * TILE_SIZE;
            int screenY = y * TILE_SIZE;
            
            // Scale factor: textures are 16x16, tiles are 32x32
            float scale = (float)TILE_SIZE / 16.0f;
            
            // Draw appropriate texture based on tile type
            switch (room->tiles[y][x]) {
                case TILE_FLOOR:
                    DrawTextureEx(floorTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
                    break;
                case TILE_CORNER:
                    DrawTextureEx(wallTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
                    break;
                case TILE_DUG:
                    DrawTextureEx(floorDugTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
                    break;
                case TILE_CHEST:
                    DrawTextureEx(chestTexture, (Vector2){screenX, screenY}, 0.0f, scale, WHITE);
                    break;
                default:
                    DrawRectangle(screenX, screenY, TILE_SIZE, TILE_SIZE, GRAY);
                    break;
            }
        }
    }
    
    // Draw attack indicator if active
    if (showAttackIndicator) {
        for (int i = 0; i < attackTilesCacheCount; i++) {
            int tileX = attackTilesCache[i][0];
            int tileY = attackTilesCache[i][1];
            
            // Only draw if within bounds
            if (tileX >= 0 && tileX < ROOM_WIDTH && tileY >= 0 && tileY < ROOM_HEIGHT) {
                int screenX = tileX * TILE_SIZE;
                int screenY = tileY * TILE_SIZE;
                
                // Draw semi-transparent red overlay for attack indicator
                DrawRectangle(screenX, screenY, TILE_SIZE, TILE_SIZE, (Color){255, 0, 0, 100});
            }
        }
    }
}

// Handle room transitions
void HandleRoomTransition(Player* player) {
    if (!IsOnEdge(player->x, player->y)) {
        return;
    }
    
    Direction exitDirection = FACING_NORTH;
    int newPlayerX = player->x;
    int newPlayerY = player->y;
    RoomCoord roomCoord = GetCurrentRoomCoord();
    
    // Check which edge and if player is trying to exit
    if (player->y == 0 && player->facing == FACING_NORTH) {
        // Exit north
        roomCoord.y--;
        newPlayerY = ROOM_HEIGHT - 1;
    } else if (player->y == ROOM_HEIGHT - 1 && player->facing == FACING_SOUTH) {
        // Exit south
        roomCoord.y++;
        newPlayerY = 0;
    } else if (player->x == 0 && player->facing == FACING_WEST) {
        // Exit west
        roomCoord.x--;
        newPlayerX = ROOM_WIDTH - 1;
    } else if (player->x == ROOM_WIDTH - 1 && player->facing == FACING_EAST) {
        // Exit east
        roomCoord.x++;
        newPlayerX = 0;
    } else {
        return; // Not trying to exit
    }
    
    // Transition to new room
    SetCurrentRoomCoord(roomCoord.x, roomCoord.y);
    currentRoom = GetRoom(roomCoord.x, roomCoord.y);
    
    // Update player position
    player->x = newPlayerX;
    player->y = newPlayerY;
    
    // Reset bat damage flag and spawn zombies if new room
    if (!currentRoom->visited) {
        currentRoom->visited = true;
        batDamageApplied = false;
        SpawnZombies(&zombieList);
        
        // Apply bat damage if room has bats
        if (currentRoom->hasBats && !batDamageApplied) {
            DamagePlayer(player, 1);
            batDamageApplied = true;
            
            // Show bat attack visual
            showBatAttack = true;
            batAttackTime = GetTime();
            
            // Initialize bat positions at random locations
            for (int i = 0; i < 4; i++) {
                batPositions[i].x = RandomInt(2, ROOM_WIDTH - 3) * TILE_SIZE;
                batPositions[i].y = RandomInt(2, ROOM_HEIGHT - 3) * TILE_SIZE;
            }
        }
    } else {
        // Room already visited, restore zombie state (for now, zombies don't persist)
        InitZombieList(&zombieList);
    }
}

// Get attack cone tiles based on player facing
void GetAttackTiles(const Player* player, int tiles[][2], int* count) {
    *count = 0;
    
    int baseX = player->x;
    int baseY = player->y;
    
    switch (player->facing) {
        case FACING_NORTH:
            // Three tiles in front, spreading upward
            tiles[(*count)][0] = baseX; tiles[(*count)][1] = baseY - 1; (*count)++;
            tiles[(*count)][0] = baseX - 1; tiles[(*count)][1] = baseY - 1; (*count)++;
            tiles[(*count)][0] = baseX + 1; tiles[(*count)][1] = baseY - 1; (*count)++;
            break;
        case FACING_SOUTH:
            tiles[(*count)][0] = baseX; tiles[(*count)][1] = baseY + 1; (*count)++;
            tiles[(*count)][0] = baseX - 1; tiles[(*count)][1] = baseY + 1; (*count)++;
            tiles[(*count)][0] = baseX + 1; tiles[(*count)][1] = baseY + 1; (*count)++;
            break;
        case FACING_WEST:
            tiles[(*count)][0] = baseX - 1; tiles[(*count)][1] = baseY; (*count)++;
            tiles[(*count)][0] = baseX - 1; tiles[(*count)][1] = baseY - 1; (*count)++;
            tiles[(*count)][0] = baseX - 1; tiles[(*count)][1] = baseY + 1; (*count)++;
            break;
        case FACING_EAST:
            tiles[(*count)][0] = baseX + 1; tiles[(*count)][1] = baseY; (*count)++;
            tiles[(*count)][0] = baseX + 1; tiles[(*count)][1] = baseY - 1; (*count)++;
            tiles[(*count)][0] = baseX + 1; tiles[(*count)][1] = baseY + 1; (*count)++;
            break;
    }
}

int main(void) {
    // Initialize window
    const int screenWidth = ROOM_WIDTH * TILE_SIZE;
    const int screenHeight = ROOM_HEIGHT * TILE_SIZE;
    
    InitWindow(screenWidth, screenHeight, "Spelunking - Area 1");
    SetTargetFPS(60);
    
    // Load textures
    floorTexture = LoadTexture("assets/floor.png");
    wallTexture = LoadTexture("assets/wall.png");
    floorDugTexture = LoadTexture("assets/floor_dug.png");
    chestTexture = LoadTexture("assets/chest.png");
    playerTexture = LoadTexture("assets/character.png");
    zombieTexture = LoadTexture("assets/zombie.png");
    batsTexture = LoadTexture("assets/bats.png");
    
    // Initialize game systems
    InitRandom();
    InitRoomSystem();
    
    // Initialize player
    Player player;
    InitPlayer(&player);
    
    // Get starting room
    currentRoom = GetRoom(0, 0);
    currentRoom->visited = true;
    SpawnZombies(&zombieList);
    
    // Apply bat damage on first room if applicable
    if (currentRoom->hasBats) {
        DamagePlayer(&player, 1);
        batDamageApplied = true;
    }
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Input handling
        Direction inputDirection = player.facing;
        bool movePressed = false;
        
        if (IsKeyPressed(KEY_UP)) {
            inputDirection = FACING_NORTH;
            movePressed = true;
        } else if (IsKeyPressed(KEY_DOWN)) {
            inputDirection = FACING_SOUTH;
            movePressed = true;
        } else if (IsKeyPressed(KEY_LEFT)) {
            inputDirection = FACING_WEST;
            movePressed = true;
        } else if (IsKeyPressed(KEY_RIGHT)) {
            inputDirection = FACING_EAST;
            movePressed = true;
        }
        
        if (movePressed) {
            if (inputDirection != player.facing) {
                // Rotate to new direction
                RotatePlayer(&player, inputDirection);
            } else {
                // Move in facing direction
                if (TryMovePlayer(&player)) {
                    // Check for room transition after successful move
                    HandleRoomTransition(&player);
                }
            }
        }
        
        // Digging
        if (IsKeyPressed(KEY_D)) {
            PlayerDig(&player);
        }
        
        // Attacking
        if (IsKeyPressed(KEY_SPACE)) {
            PlayerAttack(&player);
            
            // Get attack cone tiles
            int attackTiles[3][2];
            int attackCount;
            GetAttackTiles(&player, attackTiles, &attackCount);
            
            // Cache attack tiles for indicator
            attackTilesCacheCount = attackCount;
            for (int i = 0; i < attackCount; i++) {
                attackTilesCache[i][0] = attackTiles[i][0];
                attackTilesCache[i][1] = attackTiles[i][1];
            }
            
            // Show attack indicator
            showAttackIndicator = true;
            attackIndicatorTime = GetTime();
            
            // Damage zombies in attack cone
            for (int i = 0; i < attackCount; i++) {
                DamageZombieAt(&zombieList, attackTiles[i][0], attackTiles[i][1], 5);
            }
        }
        
        // Update zombies
        UpdateZombies(&zombieList, player.x, player.y);
        
        // Update attack indicator (hide after 0.15 seconds)
        double currentTime = GetTime();
        if (showAttackIndicator && (currentTime - attackIndicatorTime > 0.15)) {
            showAttackIndicator = false;
        }
        
        // Update bat attack animation (hide after 1 second)
        if (showBatAttack && (currentTime - batAttackTime > 1.0)) {
            showBatAttack = false;
        }
        
        // Animate bat positions (make them move around)
        if (showBatAttack) {
            double elapsed = currentTime - batAttackTime;
            for (int i = 0; i < 4; i++) {
                // Make bats circle around with some randomness
                float angle = (float)(elapsed * 3.0 + i * 1.57);  // Different phase for each bat
                batPositions[i].x += cosf(angle) * 2.0f;
                batPositions[i].y += sinf(angle) * 2.0f;
            }
        }
        
        // Check zombie damage (every 0.75s)
        if (currentTime - lastZombieDamageTime >= 0.75) {
            lastZombieDamageTime = currentTime;
            int damage = CheckZombieDamage(&zombieList, player.x, player.y);
            if (damage > 0) {
                DamagePlayer(&player, damage);
            }
        }
        
        // Check game over
        if (player.hp <= 0) {
            // Game over - for now just reset
            InitPlayer(&player);
            InitRoomSystem();
            SetCurrentRoomCoord(0, 0);
            currentRoom = GetRoom(0, 0);
            currentRoom->visited = true;
            SpawnZombies(&zombieList);
            lastZombieDamageTime = 0.0;
            batDamageApplied = false;
        }
        
        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawRoom(currentRoom);
        DrawZombies(&zombieList);
        DrawPlayer(&player);
        
        // Draw bats if bat attack is active
        if (showBatAttack) {
            float scale = (float)TILE_SIZE / 16.0f;
            for (int i = 0; i < 4; i++) {
                DrawTextureEx(batsTexture, batPositions[i], 0.0f, scale, WHITE);
            }
        }
        
        DrawHUD(&player, currentRoom);
        
        EndDrawing();
    }
    
    // Unload textures
    UnloadTexture(floorTexture);
    UnloadTexture(wallTexture);
    UnloadTexture(floorDugTexture);
    UnloadTexture(chestTexture);
    UnloadTexture(playerTexture);
    UnloadTexture(zombieTexture);
    UnloadTexture(batsTexture);
    
    CloseWindow();
    return 0;
}
