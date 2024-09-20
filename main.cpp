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
#include "FiniteStateMachine.h"
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
    Player playerState;
    Agent myPlayer(&map, &playerState);
    myPlayer.SetNode(start);
    myPlayer.SetSpeed(256);
    myPlayer.SetColour({ 0, 0, 0, 255 });
    myPlayer.SetMaxMove(15);
    myPlayer.SetHealth(1);

    // Create transtions
    //-----------------------------------------------------------------------------------------------------
    CloserThan inRange(&myPlayer, 1.25f, false);
    CloserThan notInRange(&myPlayer, 1.25f, true);
    LowHealth lowHP(2, false);
    HasLineOfSight hasLOS(&myPlayer, false);
    HasLineOfSight noLOS(&myPlayer, true);
    //-----------------------------------------------------------------------------------------------------

    // Create melee enemies
    //-----------------------------------------------------------------------------------------------------
    MeleeChase mChase1(&myPlayer);
    MeleeAttack mAttack1(&myPlayer);

    mChase1.AddTransition(&inRange, &mAttack1);
    mAttack1.AddTransition(&notInRange, &mChase1);

    FiniteStateMachine fsm1(&mChase1);

    Agent enemy1(&map, &fsm1); // Will only ever attack, never flee
    enemy1.SetNode(map.GetNode(1, 13));
    enemy1.SetSpeed(512);
    enemy1.SetMaxMove(6);
    enemy1.SetHealth(3);

    // Have to create new versions of these states since they'll have different transitions
    MeleeChase mChase2(&myPlayer);
    MeleeAttack mAttack2(&myPlayer);
    Fleeing flee(&myPlayer);

    mChase2.AddTransition(&inRange, &mAttack2);
    mChase2.AddTransition(&lowHP, &flee);
    mAttack2.AddTransition(&notInRange, &mChase2);
    mAttack2.AddTransition(&lowHP, &flee);

    FiniteStateMachine fsm2(&mChase2);

    Agent enemy2(&map, &fsm2); // Will flee if health is low
    enemy2.SetNode(map.GetNode(16, 1));
    enemy2.SetSpeed(512);
    enemy2.SetMaxMove(6);
    enemy2.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------

    // Create ranged enemy
    //-----------------------------------------------------------------------------------------------------
    RangedChase rChase1(&myPlayer);
    RangedAttack rAttack1(&myPlayer);

    rChase1.AddTransition(&hasLOS, &rAttack1);
    rAttack1.AddTransition(&noLOS, &rChase1);

    FiniteStateMachine fsm3(&rChase1);

    Agent enemy3(&map, &fsm3);
    enemy3.SetNode(map.GetNode(16, 13));
    enemy3.SetSpeed(512);
    enemy3.SetMaxMove(6);
    enemy3.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------

    // Create melee & ranged enemy
    //-----------------------------------------------------------------------------------------------------
    MeleeChase mChase3(&myPlayer);
    MeleeAttack mAttack3(&myPlayer);
    RangedChase rChase2(&myPlayer);
    RangedAttack rAttack2(&myPlayer);

    CombinedCon lowHPHasLOS(&lowHP, &hasLOS, false);

    // Melee transitions
    mChase3.AddTransition(&inRange, &mAttack3);
    mAttack3.AddTransition(&notInRange, &mChase3);
    // Melee-Ranged transitions (if low hp, switch to ranged)
    mChase3.AddTransition(&lowHP, &rChase2);
    mAttack3.AddTransition(&lowHP, &rChase2);
    mChase3.AddTransition(&lowHPHasLOS, &rAttack2);
    mAttack3.AddTransition(&lowHPHasLOS, &rAttack2);
    // Ranged transitions
    rChase2.AddTransition(&hasLOS, &rAttack2);
    rAttack2.AddTransition(&noLOS, &rChase2);

    FiniteStateMachine fsm4(&mChase3);

    Agent enemy4(&map, &fsm4);
    enemy4.SetNode(map.GetNode(8, 10));
    enemy4.SetSpeed(512);
    enemy4.SetMaxMove(6);
    enemy4.SetHealth(3);
    //-----------------------------------------------------------------------------------------------------


    // Add agents to turn controller
    TurnController tc(&myPlayer);
    tc.AddAgent(&enemy1);
    tc.AddAgent(&enemy2);
    tc.AddAgent(&enemy3);
    tc.AddAgent(&enemy4);

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