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

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");

    NodeMap map;
    /*std::vector<std::string> asciiMap;
    asciiMap.push_back("11111111111111");
    asciiMap.push_back("10000000000001");
    asciiMap.push_back("10101110111001");
    asciiMap.push_back("10101011101101");
    asciiMap.push_back("10101000000001");
    asciiMap.push_back("10101111111101");
    asciiMap.push_back("10100000010001");
    asciiMap.push_back("10111111111101");
    asciiMap.push_back("10000000000001");
    asciiMap.push_back("11111111111111");
    map.Initialise(asciiMap, 40);*/

    map.Initialise("map3.txt", 30);

    //std::vector<Node*> nodePath = PathSearch(map.GetNode(1, 1), map.GetNode(6, 7));
    PathAgent agent(map.GetNode(1, 1), 30); 

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonPressed(0))
        {
            Vector2 mousePos = GetMousePosition();
            Node* target = map.GetNearestNode(mousePos.x, mousePos.y);
            if (target != nullptr)
            {
                //nodePath = PathSearch(target, map.GetNode(6, 7));
                agent.GoToNode(target);
            }
        }
        if (IsKeyPressed(KEY_W))
        {
            agent.SpeedUp();
        }
        if (IsKeyPressed(KEY_S))
        {
            agent.SlowDown();
        }

        agent.Update(GetFrameTime()); // Get frame time returns deltaTime
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            map.Draw();
        	agent.Draw();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}