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
    //PathAgent agent(start, 40);

    PlayerIdle playerState;
	//GoToPoint myGoToState;
    Agent agent1(&map, &playerState);
    agent1.SetNode(start);
    agent1.SetSpeed(256);
    agent1.SetColour({ 0, 0, 0, 255 });
    agent1.SetMaxMove(10);
    agent1.SetHealth(10);

    // CREATING ENEMIES
    //--------------------------------------------------------------------------------------
    MeleeAttack attack(&agent1, 1.25f);
    Fleeing flee(&agent1);

    LowHealth hurt(1);
    HighHealth healthy(1);
    attack.AddTransition(&hurt, &flee);
    flee.AddTransition(&healthy, &attack);

    FiniteStateMachine fsm1(&attack);
    FiniteStateMachine fsm2(&attack);

    Agent enemy1(&map, &fsm1);
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
    enemy3.SetMaxMove(6);
    enemy3.SetHealth(3);
    //--------------------------------------------------------------------------------------

    TurnController tc(&agent1);
    tc.AddAgent(&enemy1);
    tc.AddAgent(&enemy2);
    tc.AddAgent(&enemy3);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        //agent1.Update(GetFrameTime()); // Get frame time returns deltaTime
        //agent3.Update(GetFrameTime());
        tc.Update(GetFrameTime());
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            map.Draw();
        	//agent1.Draw();
            //agent3.Draw();
            tc.Draw();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}