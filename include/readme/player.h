#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

// Player facing direction
typedef enum {
    FACING_NORTH,
    FACING_SOUTH,
    FACING_WEST,
    FACING_EAST
} Direction;

// Player structure
typedef struct {
    int x;
    int y;
    Direction facing;
    int hp;
    int score;
    double lastDigTime;
    double lastAttackTime;
} Player;

// Initialize player
void InitPlayer(Player* player);

// Update player input and movement
void UpdatePlayer(Player* player);

// Try to move player in current facing direction
bool TryMovePlayer(Player* player);

// Rotate player to face direction
void RotatePlayer(Player* player, Direction newDirection);

// Player dig action
void PlayerDig(Player* player);

// Player attack action
void PlayerAttack(Player* player);

// Damage player
void DamagePlayer(Player* player, int damage);

// Draw player
void DrawPlayer(const Player* player);

#endif // PLAYER_H
