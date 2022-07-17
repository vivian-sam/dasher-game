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

//boolean to check if sprite is on the ground
bool isOnGround(AnimData data, int height)
{
    return data.pos.y >= height - data.rec.height;
};


//function to update animation frames of sprite that is passed by value
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


//function to draw back/mid/foreground and update so that it's continuous
float drawBackground(Texture2D sheet, float sheetX, float scale)
{
    Vector2 sheet1Pos{sheetX, 0.0};
    DrawTextureEx(sheet, sheet1Pos, 0.0, scale, WHITE);
    Vector2 sheet2Pos{sheetX + sheet.width*2, 0.0};
    DrawTextureEx(sheet, sheet2Pos, 0.0, scale, WHITE); 

    if (sheetX <= -sheet.width*2) 
    {
        sheetX = 0.0;
    }
    return sheetX;
}

//-------------------------------------------------------------------------------------
// START OF MAIN CODE
//-------------------------------------------------------------------------------------


int main()
{

//-------------------------------------------------------------------------------------
// WINDOW PARAMETERS
//-------------------------------------------------------------------------------------
    //window dimensions
    const int windowWidth{500};
    const int windowHeight{350};

    //initialize popup window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    //fps for running the game
    int fps{60};
    SetTargetFPS(fps);

//-------------------------------------------------------------------------------------
// CHARACTER VARIABLES
//-------------------------------------------------------------------------------------
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
  
//-------------------------------------------------------------------------------------
// NEBULA VARIABLES
//-------------------------------------------------------------------------------------
    //animation data for nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    
    //creating an array to store multiple nebulae
    int totalNebulae{6};
    AnimData nebulae[totalNebulae]{};

    //initializing the values in each element of the nebulae array
    for (int i = 0; i < totalNebulae ; i++)
    {
        nebulae[i].rec = {0.0, 0.0, nebula.width/8, nebula.height/8},    //rectangle x,y, width, height
        nebulae[i].pos.y = windowHeight - nebula.height/8;
        nebulae[i].pos.x = windowWidth + i*300;
        //nebulae[i].pos.x = windowWidth + i*600;
        nebulae[i].frame = 0;   // int starting sprite frame
        nebulae[i].updateTime = 1.0/15.0,   // flo at update time for each animation frame
        nebulae[i].runningTime = 0.0;         // float running time
    }
    //velocities for nebula
    int nebVel{-200}; //initialize horizontal velocity in pixels/second

//-------------------------------------------------------------------------------------
// GAME WIN OR GAME OVER CONDITIONS
//-------------------------------------------------------------------------------------

    float finishLine{ nebulae[totalNebulae - 1].pos.x + 20};
    
    //collision of scarfy with nebula
    bool collision{false};

//-------------------------------------------------------------------------------------
// BACKGROUND, MIDGROUND AND FOREGROUND
//-------------------------------------------------------------------------------------
    //setting up background, midground and foreground
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

//-------------------------------------------------------------------------------------
// RUNNING THE GAME 
//-------------------------------------------------------------------------------------

    //while esc/x button not pressed, then run the game code in while loop
    while(!WindowShouldClose())
    {
        const float dT{ GetFrameTime() }; //gets time since the last frame

        //start drawing
        BeginDrawing();
        ClearBackground(DARKGRAY);

        //set the x position of background, midground and foreground so all three scroll left
        bgX -= 20 * dT;
        mgX -= 40 * dT;
        fgX -= 60 * dT;

        //draw the background, midground, foreground and update all three so it loops
        bgX = drawBackground(background, bgX, 2.0);
        mgX = drawBackground(midground, mgX, 1.7);
        fgX = drawBackground(foreground, fgX, 2.0);

        //check to see if character is on the ground; if so then vertical velocity is 0
        if(isOnGround(scarfyData, windowHeight))
        {   
            isInAir = false;
            velocity = noMove;

            //update the running time by adding the time between frames
           scarfyData = updateAnimData(scarfyData, dT, scarfy);
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

        //update finishline position
        finishLine += nebVel * dT;

        //update nebula animation frames
        for( int i = 0; i < 6; i++)
        {
           nebulae[i] = updateAnimData(nebulae[i], dT, nebula);
        }

        for(AnimData nebula : nebulae)
        {
            float pad{50}; //padding around rectangle due to how sprite sheet drawn 

            Rectangle nebRec
            {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad,
            };

            Rectangle scarfyRec
            {
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

//-------------------------------------------------------------------------------------
// LOSING CONDITION
//-------------------------------------------------------------------------------------

        if (collision)
        {
            //Game over
            DrawText("GAME OVER!", 200, 150, 15, YELLOW);
            DrawText("PLAY AGAIN? Y/N", 200, 200, 15, YELLOW);

            //If user pressed Y, reset the game variables
            if(IsKeyPressed(KEY_Y))
            {
                collision = false;
                
                ClearBackground(DARKGRAY);
                // reset nebulae variables
                for (int i = 0; i < totalNebulae ; i++)
                {
                    nebulae[i].rec = {0.0, 0.0, nebula.width/8, nebula.height/8},    //rectangle x,y, width, height
                    nebulae[i].pos.y = windowHeight - nebula.height/8;
                    nebulae[i].pos.x = windowWidth + i*300;
                    //nebulae[i].pos.x = windowWidth + i*600;
                    nebulae[i].frame = 0;   // int starting sprite frame
                    nebulae[i].updateTime = 1.0/15.0,   // flo at update time for each animation frame
                    nebulae[i].runningTime = 0.0;         // float running time
                }

                //reset finish line
                finishLine = { nebulae[totalNebulae - 1].pos.x + 20};
                
                //reset Scarfy's position
                scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
            }

            if(IsKeyPressed(KEY_N))
            {
                EndDrawing();

                UnloadTexture(scarfy);
                UnloadTexture(nebula);
                UnloadTexture(background);
                UnloadTexture(midground);
                UnloadTexture(foreground);
 
                CloseWindow();
    
                return 0;
            }

        }

        else
        {
            //Draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

            //Draw nebula
            for(int i = 0; i < 6; i++)
            {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);            
            }

//-------------------------------------------------------------------------------------
// WIN CONDITION
//-------------------------------------------------------------------------------------

            if(scarfyData.pos.x >= finishLine && collision == false)
            {
                DrawText("YOU WIN!", 200, 150, 20, GREEN);
                DrawText("PLAY AGAIN? Y/N", 200, 200, 15, GREEN);

                //If user pressed Y, reset the game variables
                if(IsKeyPressed(KEY_Y))
                {
                    collision = false;
                    ClearBackground(DARKGRAY);

                    // reset nebulae variables
                    for (int i = 0; i < totalNebulae ; i++)
                    {
                        nebulae[i].rec = {0.0, 0.0, nebula.width/8, nebula.height/8},    //rectangle x,y, width, height
                        nebulae[i].pos.y = windowHeight - nebula.height/8;
                        nebulae[i].pos.x = windowWidth + i*300;
                        //nebulae[i].pos.x = windowWidth + i*600;
                        nebulae[i].frame = 0;   // int starting sprite frame
                        nebulae[i].updateTime = 1.0/15.0,   // float update time for each animation frame
                        nebulae[i].runningTime = 0.0;         // float running time
                    }
                    
                    //reset finish line
                    finishLine = { nebulae[totalNebulae - 1].pos.x + 20};

                    //reset Scarfy's position
                    scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
                }

                if(IsKeyPressed(KEY_N))
                {
                    EndDrawing();

                    UnloadTexture(scarfy);
                    UnloadTexture(nebula);
                    UnloadTexture(background);
                    UnloadTexture(midground);
                    UnloadTexture(foreground);
    
                    CloseWindow();
        
                    return 0;
                }

            }
        }

        //stop drawing
        EndDrawing();
    }

//-------------------------------------------------------------------------------------
// CLOSING THE FILE
//-------------------------------------------------------------------------------------
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
 
    CloseWindow();
    
    return 0;
}