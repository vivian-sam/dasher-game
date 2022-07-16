#include "raylib.h"
#include <iostream>;

//animation variables
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int height)
{
    return data.pos.y >= height - data.rec.height;
};

AnimData updateAnimData(AnimData data, float deltaTime, Texture2D spriteSheet)
{
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > (spriteSheet.width / data.rec.width))
        {
            data.frame = 0;
        }
    }

    return data;
};

int main()
{
    //window dimensions
    const int windowWidth{800};
    const int windowHeight{450};

    //initialize popup window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    int valueMin {300}; //minimum value between each spawning nebula
    int valueMax {650}; //maximum value between each spawning nebula

    //vertical (jump) velocities for character
    int velocity{60};    // intialize velocity to 60 pixels/sec (target FPS is set to 60)
    int jumpVel{-500};   //pixels per second
    int noMove{0};       //when stationary, velocity is 0
    bool isInAir{false}; //boolean to check if rectangle is in the air

    const int gravity{900}; //acceleration due to gravity in pixels per second per second; (p/s)/s

    //animation data for character "scarfy" variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData
                        { 
                        {0.0, 0.0, scarfy.width/6, scarfy.height}, //rectangle x,y, width, height
                        {windowWidth/2 - scarfyData.rec.width/2, windowHeight - scarfy.height}, //sprite position x and y
                         0,         // int starting sprite frame
                         1.0/15.0,  // float update time for each animation frame
                         0          // float running time
                        };
  
    //animation daa for nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    AnimData nebulae[6]{};

    for (int i = 0; i < 6; i++)
    {
        nebulae[i].rec = {0.0, 0.0, nebula.width/8, nebula.height/8},    //rectangle x,y, width, height
        nebulae[i].pos.y = windowHeight - nebula.height/8;
        nebulae[i].pos.x = windowWidth + i*GetRandomValue(valueMin, valueMax);
        //nebulae[i].pos.x = windowWidth + i*600;
        nebulae[i].frame = 0;   // int starting sprite frame
        nebulae[i].updateTime = 1.0/15.0,   // flo at update time for each animation frame
        nebulae[i].runningTime = 0.0;         // float running time
    }

    //velocities for nebula
    int nebVel{-200}; //initialize horizontal velocity in pixels/second

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
        if(isOnGround(scarfyData, windowHeight))
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
        for( int i = 0; i < 6; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        //update character animation frame if character not already in the air
        if (!isInAir)
        {
            //update the running time by adding the time between frames
           scarfyData = updateAnimData(scarfyData, dT, scarfy);
        }

        //update nebula animation frames
        for( int i = 0; i < 6; i++)
        {
           nebulae[i] = updateAnimData(nebulae[i], dT, nebula);
        }

        //Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Draw nebula
        for(int i = 0; i < 6; i++)
        {
        DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);            
        }

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);

    CloseWindow();
    
    return 0;
}