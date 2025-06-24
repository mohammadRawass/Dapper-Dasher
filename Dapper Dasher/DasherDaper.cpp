#include <iostream>
#include <string>
#include "raylib.h"
// AnimData struct
struct AnimData
{
    Vector2 pos;
    Rectangle rec;
    int frame;
    float updateTime, runningTime;
};
// Functions Headers
bool isOnGround(AnimData data, int windowHeight);
AnimData updateAnimData(AnimData data, float DeltaTime, int maxFrame);
int main()
{
    // Windows width and height
    int windowsDimensions[2] = {800, 800};
    InitWindow(windowsDimensions[0], windowsDimensions[1], "Dapper Dasher");
    // Velocity (pixels/seconds)
    int velocity{0};
    int nebVelocity{-400};
    // Gravity
    const int gravity{2'200};
    bool isInAir{};
    // Jumping
    const int jumpVel{-900};
    // Loading the textures
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // Scarfy's Rectangle
    AnimData scarfyData;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    // Scarfy's Position
    scarfyData.pos.x = windowsDimensions[0] / 2.1 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowsDimensions[1] - scarfyData.rec.height + 1;
    // Scarfy's Frames
    scarfyData.frame = 0;
    // Scarfy update time
    scarfyData.updateTime = 1.0 / 12.0;
    // Scarfy running time
    scarfyData.runningTime = 0.0;
    // Number of nubelus
    const int numOfNub{12};
    // Nebula Textures
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    // Nebulas Array
    AnimData nebulae[numOfNub] = {};
    for (int i = 0; i < numOfNub; i++)
    {
        nebulae[i].rec.x = 0;
        nebulae[i].rec.y = 0;
        nebulae[i].rec.height = static_cast<float>(nebula.height / 8);
        nebulae[i].rec.width = static_cast<float>(nebula.width / 8);
        nebulae[i].pos.y = static_cast<float>(windowsDimensions[1] - nebula.height / 8);
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0;
        nebulae[i].pos.x = windowsDimensions[0] + i * 450;
        if (i >= 8)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, RED);
            nebulae[i].rec.x = 0;
            nebulae[i].rec.y = 0;
            nebulae[i].rec.height = static_cast<float>(nebula.height / 8);
            nebulae[i].rec.width = static_cast<float>(nebula.width / 8);
            nebulae[i].pos.y = static_cast<float>(windowsDimensions[1] - nebula.height / 8);
            nebulae[i].frame = 0;
            nebulae[i].runningTime = 0;
            nebulae[i].pos.x = windowsDimensions[0] + i * 1000;
        }
    }
    // Finishing line
    float finishLine{nebulae[numOfNub - 1].pos.x};
    // BackGrounds Textures
    Texture2D backGround = LoadTexture("textures/far-buildings.png");
    Texture2D midGround = LoadTexture("textures/back-buildings.png");
    Texture2D foreGround = LoadTexture("textures/foreground.png");
    // BackGrounds Positions
    float bgX{}, mgX{}, fgX{};
    // For Collision
    bool collision{};
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        //  Delta Time: Time since last frame
        //  Multiply by Delta when: Updating position with velocity, or velocity with acceleration
        const float DeltaTime{GetFrameTime()};
        // BackGrounds Animation Movement
        bgX -= 20 * DeltaTime;
        mgX -= 40 * DeltaTime;
        fgX -= 80 * DeltaTime;
        // When the animations end, restart
        if (bgX <= -backGround.width * 4.7)
            bgX = 0.0;
        if (mgX <= -midGround.width * 4.7)
            mgX = 0.0;
        if (fgX <= -foreGround.width * 4.7)
            fgX = 0.0;
        // BackGrounds Positions
        Vector2 bgPos{bgX, 0.0};
        DrawTextureEx(backGround, bgPos, 0.0, 4.7, WHITE);
        Vector2 bgPos2{static_cast<float>(bgX + backGround.width * 4.7), 0.0};
        DrawTextureEx(backGround, bgPos2, 0.0, 4.7, WHITE);
        Vector2 mgPos{mgX, 0.0};
        DrawTextureEx(midGround, mgPos, 0.0, 4.7, WHITE);
        Vector2 mgPos2{static_cast<float>(mgX + midGround.width * 4.7), 0.0};
        DrawTextureEx(midGround, mgPos2, 0.0, 4.7, WHITE);
        Vector2 fgPos{fgX, 0.0};
        DrawTextureEx(foreGround, fgPos, 0.0, 4.7, WHITE);
        Vector2 fgPos2{static_cast<float>(fgX + foreGround.width * 4.7), 0.0};
        DrawTextureEx(foreGround, fgPos2, 0.0, 4.7, WHITE);
        // Checking if scarfy on ground or jumping
        if (isOnGround(scarfyData, windowsDimensions[1]))
        {
            // Scarfy is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // Scarfy is in the air
            // Apply gravity
            velocity += gravity * DeltaTime;
            isInAir = true;
        }
        // If we pressed space, scarfy will jump
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        //  Updating scarfy's position
        scarfyData.pos.y += velocity * DeltaTime;
        if (!isInAir)
        {
            // Updating scarfy animations
            scarfyData = updateAnimData(scarfyData, DeltaTime, 5);
        }
        for (int i = 0; i < numOfNub; i++)
        {
            // Updating nebulas positions
            nebulae[i].pos.x += nebVelocity * DeltaTime;
            if (i >= 8)
            {
                nebVelocity = -400;
                nebulae[i].pos.x += nebVelocity * DeltaTime;
            }
            // Updating nebulas animations
            nebulae[i] = updateAnimData(nebulae[i], DeltaTime, 7);
        }
        // Making two Rec for collisions
        for (AnimData nebu : nebulae)
        {
            float pad{55};
            Rectangle nebRec{
                nebu.pos.x + pad,
                nebu.pos.y + pad,
                nebu.rec.width - 2 * pad,
                nebu.rec.height - 2 * pad};
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height};
            if (CheckCollisionRecs(nebRec, scarfyRec))
                collision = true;
        }
        if (collision)
        {
            // When losing
            DrawText("GAME OVER!", 220, 400, 60, RED);
        }
        else if (scarfyData.pos.x >= finishLine - 5275)
        {
            // When wining
            DrawText("YOU WIN!", 250, 385, 60, GREEN);
        }
        else // When running
        {
            for (int i = 0; i < numOfNub; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                if (i >= 8)
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, RED);
            }
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        finishLine += nebVelocity * DeltaTime;
        EndDrawing();
    }
    // Unloading textures
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(backGround);
    UnloadTexture(midGround);
    UnloadTexture(foreGround);
    CloseWindow();
    std::cout << "Thanks for using the app" << std::endl;
}
// Function for if scarfy on ground
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}
// Function for animations updates
AnimData updateAnimData(AnimData data, float DeltaTime, int maxFrame)
{
    data.runningTime += DeltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
            data.frame = 0;
    }
    return data;
}