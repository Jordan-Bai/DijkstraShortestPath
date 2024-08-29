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

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");

    //TEST
    //-------------------------------------------
    Node test(1, 1);
    std::vector<Edge> edges;
    edges.push_back(Edge(&test, 1));
    //-------------------------------------------

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

    map.Initialise("map2.txt", 30);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);
            map.Draw();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}