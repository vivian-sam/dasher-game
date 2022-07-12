#include "raylib.h"

//animation variables
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main()
{
    //window dimensions
    const int windowWidth{800};
    const int windowHeight{450};

    //initialize popup window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    //vertical (jump) velocities for character
    int velocity{60};    // intialize velocity to 60 pixels/sec (target FPS is set to 60)
    int jumpVel{-500};   //pixels per second
    int noMove{0};       //when stationary, velocity is 0
    bool isInAir{false}; //boolean to check if rectangle is in the air

    const int gravity{1000}; //acceleration due to gravity in pixels per second per second; (p/s)/s

    //animation data for character "scarfy" variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData
                        { 
                        {0.0, 0.0, scarfy.width/6, scarfy.height}, //rectangle x,y, width, height
                        {windowWidth/2 - scarfyData.rec.width/2, windowHeight - scarfy.height}, //sprite position x and y
                         0,         // int starting sprite frame
                         1.0/12.0,  // float update time for each animation frame
                         0          // float running time
                        };

    //animation daa for nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebData
                    {   
                        {0.0, 0.0, nebula.width/8, nebula.height/8},    //rectangle x,y, width, height
                        {windowWidth, windowHeight - nebData.rec.height}, //sprite position x and y
                        0,          // int starting sprite frame
                        1.0/15.0,   // float update time for each animation frame
                        0           // float running time
                    };

    //velocities for nebula
    int nebVel{-600}; //initialize horizontal velocity in pixels/second

    int fps{60};
    SetTargetFPS(fps);


    //while esc/x button not pressed, then run the game code in while loop
    while(!WindowShouldClose())
    {
        const float dT{ GetFrameTime() }; //gets time since the last frame

        //start drawing
        BeginDrawing();
        ClearBackground(DARKGRAY);

        //check to see if character is on the ground; if so then vertical velocity is 0
        if(scarfyData.pos.y >= windowHeight - scarfy.height)
        {   
            isInAir = false;
            velocity = noMove;
        }

        else
        {
            //apply gravity because character is in the air
            velocity += gravity * dT;     //scaling by delta time, which is the time since the last frame    
            isInAir = true;               
        }
     
        //characer jumps when spacebar is pressed and if character is not already in the air
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
            isInAir = true;     //setting boolean to true so that double/triple jumps not possible
        }


        //update Scarfy's vertical position
        scarfyData.pos.y += velocity * dT; //scaling the position by delta Time

        //update nebula's horizontal postion
        nebData.pos.x += nebVel * dT;

        //update character animation frame if character not already in the air
        if (!isInAir)
        {
            //update the running time by adding the time between frames
            scarfyData.runningTime += dT;
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0.0;
                //updating character animation frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                ++scarfyData.frame;
                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        //update nebula animation frames
        nebData.runningTime += dT;
        if (nebData.runningTime >= nebData.updateTime)
        {
            nebData.runningTime = 0.0;
            nebData.rec.x = nebData.frame* nebData.rec.width;
            ++nebData.frame;
            if(nebData.frame > 7)
            {
                nebData.frame = 0;
            }
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        //Draw nebula
        DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);

    CloseWindow();
    
    return 0;
}