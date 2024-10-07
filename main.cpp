/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   You can find all examples for the C++ wrapper on its GitHub repository at
*   https://github.com/RobLoach/raylib-cpp/tree/master/examples
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*   Copyright (c) 2019-2021 Rob Loach (@RobLoach)
*   Copyright (c) 2024 Academy of Interactive Entertainment (@AcademyOfInteractiveEntertainment)
*
********************************************************************************************/

#include "raylib-cpp.hpp"
#include "Pathfinder.h"
#include "PathAgent.h"
#include "Agent.h"
#include "States.h"
#include "Conditions.h"
#include "Actions.h"
#include "TurnController.h"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = {0, 128, 255, 255};
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");

    NodeMap map;
    map.Initialise("map3.txt", glm::vec2(screenWidth, screenHeight));

    Node* start = map.GetNode(1, 1);

    // Create player
    Selector B_Player;
    Agent myPlayer(&map, &B_Player);
    myPlayer.SetNode(start);
    myPlayer.SetSpeed(256);
    myPlayer.SetColour({ 0, 0, 0, 255 });
    myPlayer.SetMaxMove(15);
    myPlayer.SetHealth(20);

    // Create Actions & Conditions
    //-----------------------------------------------------------------------------------------------------
    CloserThan inRange(&myPlayer, 1.25f, false);
    LowHealth lowHP(2, false);
    HasLineOfSight hasLOS(&myPlayer, false);
    IsMoving isMoving(false);
    CanMove canMove(false);
    A_KeyPressed xPressed(false);
    A_MousePressed leftClick(false);

    MeleeChase mChase(&myPlayer);
    Attack eAttack(&myPlayer);
    Flee eFlee(&myPlayer);
    PlayerMove pMove;
    PlayerAttack pAttack;
    FinishTurn endTurn;
    //-----------------------------------------------------------------------------------------------------

    // Create behaviours
    //-----------------------------------------------------------------------------------------------------
    // Player
    Sequence B_PlayerAttack;
    B_PlayerAttack.AddChild(&xPressed);
    B_PlayerAttack.AddChild(&pAttack);
    B_PlayerAttack.AddChild(&endTurn);

    Sequence B_PlayerMove;
    B_PlayerMove.AddChild(&leftClick);
    B_PlayerMove.AddChild(&canMove);
    B_PlayerMove.AddChild(&pMove);

    B_Player.AddChild(&isMoving);
    B_Player.AddChild(&B_PlayerAttack);
    B_Player.AddChild(&B_PlayerMove);

    // Melee Enemies
    Sequence B_MeleeAttack;
    B_MeleeAttack.AddChild(&inRange);
    B_MeleeAttack.AddChild(&eAttack);
    B_MeleeAttack.AddChild(&endTurn);

    Sequence B_StartChasing;
    B_StartChasing.AddChild(&canMove);
    B_StartChasing.AddChild(&mChase);

    Selector B_MeleeMove;
    B_MeleeMove.AddChild(&B_StartChasing);
    B_MeleeMove.AddChild(&endTurn);

    // Fleeing
    Sequence B_StartFleeing;
    B_StartFleeing.AddChild(&canMove);
    B_StartFleeing.AddChild(&eFlee);

    Selector B_FleeMove;
    B_FleeMove.AddChild(&B_StartFleeing);
    B_FleeMove.AddChild(&endTurn);

    Sequence B_Flee;
    B_Flee.AddChild(&lowHP);
    B_Flee.AddChild(&B_FleeMove);
    //-----------------------------------------------------------------------------------------------------

    // Create transtions
    //-----------------------------------------------------------------------------------------------------
    //CloserThan inRange(&myPlayer, 1.25f, false);
    //CloserThan notInRange(&myPlayer, 1.25f, true);
    //LowHealth lowHP(2, false);
    //HasLineOfSight hasLOS(&myPlayer, false);
    //HasLineOfSight noLOS(&myPlayer, true);
    //-----------------------------------------------------------------------------------------------------

    // Create melee enemies
    //-----------------------------------------------------------------------------------------------------
    Selector MeleeEnemy1;
    MeleeEnemy1.AddChild(&isMoving);
    MeleeEnemy1.AddChild(&B_MeleeAttack);
    MeleeEnemy1.AddChild(&B_MeleeMove);

    Agent enemy1(&map, &MeleeEnemy1); // Will only ever attack, never flee
    enemy1.SetNode(map.GetNode(1, 13));
    enemy1.SetSpeed(512);
    enemy1.SetMaxMove(6);
    enemy1.SetHealth(3);

    Selector MeleeEnemy2;
    MeleeEnemy2.AddChild(&isMoving);
    MeleeEnemy2.AddChild(&B_Flee);
    MeleeEnemy2.AddChild(&B_MeleeAttack);
    MeleeEnemy2.AddChild(&B_MeleeMove);

    Agent enemy2(&map, &MeleeEnemy2); // Will flee if health is low
    enemy2.SetNode(map.GetNode(16, 1));
    enemy2.SetSpeed(512);
    enemy2.SetMaxMove(6);
    enemy2.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------

    // Create ranged enemy
    //-----------------------------------------------------------------------------------------------------
    //Agent enemy3(&map, &fsm3);
    //enemy3.SetNode(map.GetNode(16, 13));
    //enemy3.SetSpeed(512);
    //enemy3.SetMaxMove(6);
    //enemy3.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------

    // Create melee & ranged enemy
    //-----------------------------------------------------------------------------------------------------
    //Agent enemy4(&map, &fsm4);
    //enemy4.SetNode(map.GetNode(8, 10));
    //enemy4.SetSpeed(512);
    //enemy4.SetMaxMove(6);
    //enemy4.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------


    // Add agents to turn controller
    TurnController tc(&myPlayer);
    //tc.AddAgent(&enemy1);
    tc.AddAgent(&enemy2);
    //tc.AddAgent(&enemy3);
    //tc.AddAgent(&enemy4);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        tc.Update(GetFrameTime());
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            map.Draw();
            tc.Draw();

            if (tc.BattleOver())
            {
                int mapx = map.GetWidth() * map.GetTileSize();
                int mapy = map.GetHeight() * map.GetTileSize();
                int fontSize = mapx / 10;

                if (myPlayer.IsDead()) // Means the player lost
                {
                    int x = (mapx / 2) - (fontSize * 3);
                    int y = (mapy / 2) - (fontSize * 0.5);
                    textColor.DrawText("GAME OVER", x, y, fontSize);
                }
                else
                {
                    int x = (mapx / 2) - (fontSize * 2.25);
                    int y = (mapy / 2) - (fontSize * 0.5);
                    textColor.DrawText("YOU WON", x, y, fontSize);
                }
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}