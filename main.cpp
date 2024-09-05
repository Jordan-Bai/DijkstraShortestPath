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

	GoToPoint myGoToState;
    Agent agent1(&map, &myGoToState);
    agent1.SetNode(start);
    agent1.SetSpeed(64);
    agent1.SetColour({ 0, 0, 0, 255 });

    //Wander myWanderState;
    /*Follow myFollowState(&agent1);
    Agent agent2(&map, &myFollowState);
    agent2.SetNode(map.GetNode(10, 1));
    agent2.SetSpeed(32);
    agent2.SetColour({ 0, 0, 255, 255 });*/

    // USING FSM
    //--------------------------------------------------------------------------------------
    DistanceCondition* closerThan5 = new DistanceCondition(&agent1, 5.0f * map.GetTileSize(), true);
    DistanceCondition* furtherThan7 = new DistanceCondition(&agent1, 7.0f * map.GetTileSize(), false);

    Wander myWander2;
    Follow myFollow2(&agent1);

    myWander2.AddTransition(closerThan5, &myFollow2);
    myFollow2.AddTransition(furtherThan7, &myWander2);

    FiniteStateMachine fsm(&myWander2);
    Agent agent3(&map, &fsm);
    agent3.SetNode(map.GetNode(16, 1));
    agent3.SetSpeed(32);
    //--------------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        agent1.Update(GetFrameTime()); // Get frame time returns deltaTime
        agent3.Update(GetFrameTime());
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            map.Draw();
        	agent1.Draw();
            agent3.Draw();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}