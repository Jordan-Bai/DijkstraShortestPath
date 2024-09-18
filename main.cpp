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
    raylib::Color textColor = raylib::Color::LightGray();
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
    myPlayer.SetHealth(10);

    // Create enemies
    MeleeAttack attack(&myPlayer, 1.25f);
    RangedAttack shoot(&myPlayer);
    Fleeing flee(&myPlayer);

    HPCondition lowHealth(2, true);
    HPCondition highHealth(2, false);
    attack.AddTransition(&lowHealth, &flee);
    flee.AddTransition(&highHealth, &attack);

    FiniteStateMachine fsm1(&attack);
    FiniteStateMachine fsm2(&attack);

    Agent enemy1(&map, &shoot);
    enemy1.SetNode(map.GetNode(16, 1));
    enemy1.SetSpeed(512);
    enemy1.SetMaxMove(6);
    enemy1.SetHealth(3);

    Agent enemy2(&map, &attack); // Will oly ever attack, never flee
    enemy2.SetNode(map.GetNode(16, 13));
    enemy2.SetSpeed(512);
    enemy2.SetMaxMove(6);
    enemy2.SetHealth(3);
    enemy2.SetColour({ 255, 0, 0, 255 });

    Agent enemy3(&map, &fsm2);
    enemy3.SetNode(map.GetNode(1, 13));
    enemy3.SetSpeed(512);
    enemy3.SetMaxMove(3);
    enemy3.SetHealth(3);

    // TESTING
    //--------------------------------------------------------------------------------------
    //LineOfSight los(&myPlayer);
    //std::cout << los.Evaluate(map.GetNode(1, 7)) << std::endl;
    //std::cout << los.Evaluate(map.GetNode(2, 1)) << std::endl;
    //std::cout << los.Evaluate(map.GetNode(2, 3)) << std::endl;
    //--------------------------------------------------------------------------------------

    // Add agents to turn controller
    TurnController tc(&myPlayer);
    tc.AddAgent(&enemy1);
    //tc.AddAgent(&enemy2);
    //tc.AddAgent(&enemy3);

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
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}