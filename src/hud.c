#include "hud.h"
#include "treasure.h"
#include "raylib.h"
#include <stdio.h>

void DrawHUD(const Player* player, const Room* room) {
    // Draw HP (top-left)
    char hpText[32];
    snprintf(hpText, sizeof(hpText), "HP: %d", player->hp);
    DrawText(hpText, 10, 10, 10, RED);
    
    // Draw hearts
    for (int i = 0; i < player->hp && i < 10; i++) {
        DrawRectangle(10 + i * 12, 25, 10, 10, RED);
    }
    
    // Draw score (top-right)
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", player->score);
    int scoreWidth = MeasureText(scoreText, 10);
    DrawText(scoreText, ROOM_WIDTH * TILE_SIZE - scoreWidth - 10, 10, 10, YELLOW);
    
    // Draw tracker (bottom-right)
    float distance = GetDistanceToTreasure(room, player->x, player->y);
    const char* trackerText = GetTrackerText(distance);
    
    if (room->hasTreasure && !room->treasureCollected) {
        int trackerWidth = MeasureText(trackerText, 10);
        int trackerX = ROOM_WIDTH * TILE_SIZE - trackerWidth - 10;
        int trackerY = ROOM_HEIGHT * TILE_SIZE - 20;
        
        // Draw text multiple times with slight offsets to make it bold
        Color darkColor = (Color){40, 20, 10, 255};  // Very dark brown
        DrawText(trackerText, trackerX, trackerY, 10, darkColor);
        DrawText(trackerText, trackerX + 1, trackerY, 10, darkColor);
        DrawText(trackerText, trackerX, trackerY + 1, 10, darkColor);
        DrawText(trackerText, trackerX + 1, trackerY + 1, 10, darkColor);
    }
}
