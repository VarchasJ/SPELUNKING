#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdbool.h>

#define MAX_ZOMBIES 10

// Zombie structure
typedef struct {
    int x;
    int y;
    int hp;
    bool active;
    double lastMoveTime;
} Zombie;

// Zombie list for current room
typedef struct {
    Zombie zombies[MAX_ZOMBIES];
    int count;
} ZombieList;

// Initialize zombie list
void InitZombieList(ZombieList* list);

// Spawn zombies in room
void SpawnZombies(ZombieList* list);

// Update all zombies (movement and pathfinding)
void UpdateZombies(ZombieList* list, int playerX, int playerY);

// Check if zombies damage player
int CheckZombieDamage(const ZombieList* list, int playerX, int playerY);

// Damage zombie at position (from player attack)
void DamageZombieAt(ZombieList* list, int x, int y, int damage);

// Draw all zombies
void DrawZombies(const ZombieList* list);

#endif // ZOMBIE_H
