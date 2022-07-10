#include "raylib.h"

int main()
{
    //window dimensions
    const int windowWidth{800};
    const int windowHeight{450};

    //initialize popup window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    //velocities for character
    int velocity{60};// intialize velocity to 60 pixels/sec (target FPS is set to 60)
    int jumpVel{-500}; //pixels per second
    int noMove{0};
    bool isInAir{false}; //boolean to check if rectangle is in the air

    const int gravity{1000}; //acceleration due to gravity in pixels per second per second; (p/s)/s

    //below is the code for the character "scarfy"
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;    //defines the specific frame we want, which will be a rectangle drawn on the sprite sheet
    scarfyRec.width = scarfy.width/6;   //there are 6 frames in the sprite sheet; definining the width of just one frame
    scarfyRec.height = scarfy.height;   //defining the height of the rectangle for the sprite
    scarfyRec.x = 0;                    //x position of the rectangle where it starts on sprite sheet
    scarfyRec.y = 0;                    //y position of the rectangle where it starts on the sprite sheet
    Vector2 scarfyPos;      // defines the position of the character;
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2; //positions the sprite image to center of screen
    scarfyPos.y = windowHeight - scarfyRec.height;

    int frame{0}; //initializing animation frame for scarfy, the character
    const float updateTime {1.0/10.0}; //amount of time that passes in between each animation frame; units is in time
    float runningTime{};
    int fps{9};

    SetTargetFPS(fps);


    //while esc/x button not pressed, then run the game code in while loop
    while(!WindowShouldClose())
    {
        const float dT{ GetFrameTime() }; //gets time since the last frame

        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //update running time
        runningTime += dT;
        if (runningTime>= updateTime)
        {
            runningTime = 0.0;
            //updating character animation frame
            scarfyRec.x = frame*scarfyRec.width;
            frame++;
            if (frame > 5)
            {
                frame = 0;
            }

            DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        }

        //check to see if the rectangle is on the ground
        if(scarfyPos.y >= windowHeight - scarfyRec.height)
        {   
            isInAir = false;
            velocity = noMove;
        }

        else
        {
            //apply gravity because rectangle is in the air
            velocity += gravity * dT;     //scaling by delta time    
            isInAir = true;
        }
     
        //check for jump; jumps when spacebar is pressed and if rectangle is not in the air
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
            isInAir = true;
        }


        //update position
        scarfyPos.y += velocity * dT; //scaling the position by delta Time

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
    
    return 0;
}