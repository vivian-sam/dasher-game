#include "raylib.h"

int main()
{
    //window dimensions
    const int windowWidth{800};
    const int windowHeight{450};

    //initialize popup window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    //rectangle dimensions
    const int rectWidth{50};
    const int rectHeight{80};

    //position and velocities of rectangle
    int posY{windowHeight - rectHeight}; //intializes so that rectangle starts on ground
    int velocity{0};// intialize velocity to 10 pixels/frame
    int jumpVel{-15};
    int noMove{0};
    bool isInAir{false}; //boolean to check if rectangle is in the air

    const int gravity{1}; //acceleration due to gravity in pixels per frame per frame; (p/f)/f

    int fps{60};

    SetTargetFPS(fps);

    //while esc/x button not pressed, then run the game code in while loop
    while(!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);


        //check to see if the rectangle is on the ground
        if(posY >= windowHeight - rectHeight)
        {   
            isInAir = false;
            velocity = noMove;
        }

        else
        {
            //apply gravity because rectangle is in the air
            velocity += gravity;        
            isInAir = true;
        }
     
        //check for jump; jumps when spacebar is pressed and if rectangle is not in the air
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
            isInAir = true;
        }


        //update position
        posY += velocity;


        DrawRectangle(windowWidth/2, posY, rectWidth, rectHeight, BLUE);

        //stop drawing
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}