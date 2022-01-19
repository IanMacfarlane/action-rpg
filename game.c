/*******************************************************************************************
*
*   raylib [core] example - Mouse input
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

// TODO need to clean up my code it is pretty messy right now and definitely uses more lines than I think it needs too.

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // screen sizes: 800,450 1600,900 1920,1080
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Game (in progress)");
    
    // sprite stuff
    int direction = 1;
    Texture2D rollerbotStaticIdleRight = LoadTexture("resources/rollerbotStaticIdleRight.png");
    Texture2D rollerbotStaticIdleLeft = LoadTexture("resources/rollerbotStaticIdleLeft.png");
    
    Texture2D rollerbotIdleRight = LoadTexture("resources/rollerbotIdleRight.png");
    Texture2D rollerbotIdleLeft = LoadTexture("resources/rollerbotIdleLeft.png");
    
    Texture2D rollerbotWakeRight = LoadTexture("resources/rollerbotWakeRight.png");
    Texture2D rollerbotWakeLeft = LoadTexture("resources/rollerbotWakeLeft.png");
    
    Texture2D rollerbotMoveRight = LoadTexture("resources/rollerbotMoveRight.png");
    Texture2D rollerbotMoveLeft = LoadTexture("resources/rollerbotMoveLeft.png");
    
    
    Vector2 position;
    // TODO may not need a rec for each direction
    Rectangle rollerbotMoveRightRec = { 0.0f, 0.0f, (float)rollerbotMoveRight.width/8, (float)rollerbotMoveRight.height };
    Rectangle rollerbotMoveLeftRec = { 0.0f, 0.0f, (float)rollerbotMoveLeft.width/8, (float)rollerbotMoveLeft.height };
    Rectangle rollerbotStaticIdleRightRec = { 0.0f, 0.0f, (float)rollerbotStaticIdleRight.width, (float)rollerbotStaticIdleRight.height };
    Rectangle rollerbotWakeRightRec = { 0.0f, 0.0f, (float)rollerbotWakeRight.width/5, (float)rollerbotWakeRight.height };
    
    int currentFrame = 0;
    //if (direction == 0) currentFrame = 4;
    
    int framesCounter = 0;
    int framesSpeed = 6;
    int moveSpeed = 3;

    // movement variables
    Vector2 ballPosition = { screenWidth/2, screenHeight/2 }; 
    Vector2 ballTarget = ballPosition;
    Vector2 pathArray [screenWidth];
    int pathLength = 0;// used to track the end of the the pathArray
    int pathPosition;// track int location on pathArray while moving
    
    int moving = 0;
    int awake = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;
        
        // sprite animation
        if (framesCounter >= (60/framesSpeed)) {
            framesCounter = 0;
            
            if (direction == 1) currentFrame++;// TODO if moving left reverse animation direction currentFrame--
            else if (direction == 0) currentFrame--;
            
            if (moving == 1) {
                if (direction == 1) {
                    if (currentFrame > 7) currentFrame = 0;
                    rollerbotMoveRightRec.x = (float)currentFrame*(float)rollerbotMoveRight.width/8;
                }
                else if (direction == 0) {
                    if (currentFrame < 0) currentFrame = 7;
                    rollerbotMoveLeftRec.x = (float)currentFrame*(float)rollerbotMoveLeft.width/8;
                }
            }
            else if (awake == 1) {
                // wake animation
                if (currentFrame > 4) awake = 2;
                if (direction == 1) {
                    rollerbotWakeRightRec.x = (float)currentFrame*(float)rollerbotWakeRight.width/5;
                }
                else if (direction == 0) {
                    if (currentFrame < 0) awake = 2;
                    rollerbotWakeRightRec.x = (float)currentFrame*(float)rollerbotWakeLeft.width/5;
                }
            }
        }
        
        // shortest path movement algorithm TODO if obstacle calculate path around obstacle
        // if ballPosition is not equal to ballTarget keep moving towards target
        if (!(ballTarget.x == ballPosition.x && ballTarget.y == ballPosition.y)) {
            
            // keep moving down path
            if (pathPosition < pathLength-1 && awake == 2) {
                
                moving = 1;
                
                // TODO character moves faster on 45 than on flat could do some math to make movement look the same speed at any angle?
                if ((pathLength-1) - pathPosition > moveSpeed) {
                    pathPosition+=moveSpeed;
                }
                else {// at end of path
                    pathPosition = pathLength-1;
                    moving = 0;
                }
                ballPosition = pathArray[pathPosition];
            }
            else if (awake == 0) {
                awake = 1;
                if (direction == 1) {
                    currentFrame = 0;
                }
                else if (direction == 0) {
                    rollerbotWakeRightRec.x = (float)4*(float)rollerbotWakeLeft.width/5;
                    currentFrame = 4;
                }
            }
        }
        
        // TODO need to put this in its own movement click function I think
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            ballTarget = GetMousePosition();
            pathPosition = 0;
            
            if (!(ballTarget.x == ballPosition.x && ballTarget.y == ballPosition.y)) {
                // calculate straight line path between ballTarget and ballPosition and move ball down the line
                int width = abs(ballTarget.x - ballPosition.x);
                int height = abs(ballTarget.y - ballPosition.y);
                //int pathLength;
                int smallLength;
                int bigLength;
                int smallLengths;
                int bigLengths;
                
                if (width > height && height != 0) {
                    pathLength = width;
                    smallLength = width / height;
                    bigLength = smallLength + 1;
                    bigLengths = width % height;
                    smallLengths = height - bigLengths;
                }
                else if (height > width && width != 0) {
                    pathLength = height;
                    smallLength = height / width; 
                    bigLength = smallLength + 1;                    
                    bigLengths = height % width;
                    smallLengths = width - bigLengths;

                }
                else if (height == width) {
                    smallLength = 1;
                    pathLength = height;
                    smallLengths = pathLength;
                    bigLength = 0;
                    bigLengths = 0;
                }
                else if (width == 0 || height == 0) { 
                    smallLength = 0;
                    smallLengths = 0;
                    if (width == 0) pathLength = height;
                    else pathLength = width;
                    bigLength = pathLength;
                    bigLengths = 1;
                }
                                                
                // create an array of size = smallLengths+bigLengths fill each index with length and alternate and group accordingly
                int lengthsArray [smallLengths + bigLengths];
                int grouping;// tracks size of grouping for smallGroupingLengths +1 size for bigGroupingLengths
                int bigGroupingLengths;// tracks number of lengths in big groupings
                int smallGroupingLengths = 0;// tracks number of lengths in small groupings
                int smallGroupingTracker;// tracks number of lengths inserted into array
                int bigGroupingTracker;// tracks number of lengths inserted into array
                int moreLengths;// tracks if more smallLengths or bigLengths
                
                // divide larger number of lengths by smaller number of lengths
                if (smallLengths > bigLengths && bigLengths != 0) {
                    // more small lengths
                    grouping = smallLengths / bigLengths;
                    bigGroupingLengths = (smallLengths % bigLengths) * (grouping+1);
                    moreLengths = 0;
                }
                else if (bigLengths > smallLengths && smallLengths != 0) {
                    // more big lengths
                    grouping = bigLengths / smallLengths;
                    bigGroupingLengths = (bigLengths % smallLengths) * (grouping+1);
                    moreLengths = 1;
                }
                else {
                    // small lengths = big lengths
                    grouping = 1;
                    bigGroupingLengths = 0;
                }
                
                for (int j = 0; j < smallLengths+bigLengths; j++) {
                    // if bigLengths == 0 just small length small lengths
                    if (bigLengths == 0) lengthsArray[j] = smallLength;
                    else {
                        if (grouping == 1 && bigGroupingLengths == 0) {
                            // alternate equally between big and small lengths no groupings
                            if (j % 2 == 0) {
                                lengthsArray[j] = bigLength;
                            }
                            else {
                                lengthsArray[j] = smallLength;
                            }
                        }
                        else {
                            if (bigGroupingLengths == 0) {
                                // if whole number alternate that number of larger number of lengths with one of the smaller number of lengths
                                if (j % (grouping + 1) == 0) {
                                    // single less lengths
                                    if (moreLengths == 0) {
                                        lengthsArray[j] = bigLength;
                                    }
                                    else if (moreLengths == 1) {
                                        lengthsArray[j] = smallLength;
                                    }
                                }
                                else {
                                    // group more lengths
                                    if (moreLengths == 0) {
                                        lengthsArray[j] = smallLength;
                                    }
                                    else if (moreLengths == 1) {
                                        lengthsArray[j] = bigLength;
                                    }
                                }
                            }
                            else {// if remainder
                                if (moreLengths == 0) {
                                    if (smallGroupingLengths == 0) {// don't need to reset every loop
                                        smallGroupingLengths = smallLengths - bigGroupingLengths;
                                        smallGroupingTracker = smallGroupingLengths;
                                        bigGroupingTracker = bigGroupingLengths;
                                    }
                                    // alternate grouping smallLength from smallGroupingLengths with bigLength.
                                    // after smallGroupingLengths/2 inserted to array switch to grouping+1 smallLength from bigGroupingLengths
                                    // once all bigGroupingLengths inserted to array switch back to smallGroupingLengths and alternate the other half
                                    if (smallGroupingTracker > smallGroupingLengths/2 || (smallGroupingLengths == grouping && smallGroupingTracker > 0)) {// insert first half of smallGroupings
                                        if (smallGroupingTracker % grouping == 0) {
                                            lengthsArray[j] = bigLength;
                                            j++;
                                            lengthsArray[j] = smallLength;
                                            smallGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = smallLength;
                                            smallGroupingTracker--;
                                        }
                                    }
                                    else if (bigGroupingTracker > 0) {// insert bigGroupings
                                        if (bigGroupingTracker % (grouping + 1) == 0) {
                                            lengthsArray[j] = bigLength;
                                            j++;
                                            lengthsArray[j] = smallLength;
                                            bigGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = smallLength;
                                            bigGroupingTracker--;
                                        }
                                    }
                                    else if (smallGroupingTracker > 0) {// insert second half of smallGroupings
                                        if (smallGroupingTracker % grouping == 0) {
                                            lengthsArray[j] = bigLength;
                                            j++;
                                            lengthsArray[j] = smallLength;
                                            smallGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = smallLength;
                                            smallGroupingTracker--;
                                        }
                                    }
                                }
                                else if (moreLengths == 1) {
                                    // same as prev if but replace smallLengths and smallLength with bigLengths and bigLength                                    
                                    if (smallGroupingLengths == 0) {// don't need to reset every loop
                                        smallGroupingLengths = bigLengths - bigGroupingLengths;
                                        smallGroupingTracker = smallGroupingLengths;
                                        bigGroupingTracker = bigGroupingLengths;
                                    }
                                    // alternate grouping bigLength from smallGroupingLengths with bigLength.
                                    // after smallGroupingLengths/2 inserted to array switch to grouping+1 bigLength from bigGroupingLengths
                                    // once all bigGroupingLengths inserted to array switch back to smallGroupingLengths and alternate the other half
                                    if (smallGroupingTracker > smallGroupingLengths/2 || (smallGroupingLengths == grouping && smallGroupingTracker > 0)) {// insert first half of smallGroupings
                                        if (smallGroupingTracker % grouping == 0) {
                                            lengthsArray[j] = smallLength;
                                            j++;
                                            lengthsArray[j] = bigLength;
                                            smallGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = bigLength;
                                            smallGroupingTracker--;
                                        }
                                    }
                                    else if (bigGroupingTracker > 0) {// insert bigGroupings
                                        if (bigGroupingTracker % (grouping + 1) == 0) {
                                            lengthsArray[j] = smallLength;
                                            j++;
                                            lengthsArray[j] = bigLength;
                                            bigGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = bigLength;
                                            bigGroupingTracker--;
                                        }
                                    }
                                    else if (smallGroupingTracker > 0) {// insert second half of smallGroupings
                                        if (smallGroupingTracker % grouping == 0) {
                                            lengthsArray[j] = smallLength;
                                            j++;
                                            lengthsArray[j] = bigLength;
                                            smallGroupingTracker--;
                                        }
                                        else {
                                            lengthsArray[j] = bigLength;
                                            smallGroupingTracker--;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                // fill array of size path Length with points on path alternating between small lengths and big length
                Vector2 pen = ballPosition;
                int lengths = smallLengths + bigLengths;
                int currentLength = 0;
                
                for (int i = 0; i < pathLength; i++) {
                    
                    pathArray[i] = pen;
                    //DrawPixel(pathArray[i].x, pathArray[i].y, BLACK);
                    lengthsArray[currentLength]--;
                    
                    // fill array with points on path
                    if (ballPosition.x - ballTarget.x > 0 && ballPosition.y - ballTarget.y > 0) {// move up and left
                        direction = 0;
                        if (pathLength == height) {// moving in y direction
                            pen.y--;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.x--;
                            }
                        }
                        else if (pathLength == width) {//moving in x direction
                            pen.x--;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.y--;
                            }
                        }
                    }
                    else if (ballPosition.x - ballTarget.x < 0 && ballPosition.y - ballTarget.y > 0) {// move up and right
                        direction = 1;
                        if (pathLength == height) {// moving in y direction
                            pen.y--;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.x++;
                            }
                        }
                        else if (pathLength == width) {//moving in x direction
                            pen.x++;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.y--;
                            }
                        }
                    }
                    else if (ballPosition.x - ballTarget.x < 0 && ballPosition.y - ballTarget.y < 0) {//move down and right
                        direction = 1;
                        if (pathLength == height) {// moving in y direction
                            pen.y++;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.x++;
                            }
                        }
                        else if (pathLength == width) {//moving in x direction
                            pen.x++;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.y++;
                            }
                        }
                    }
                    else if (ballPosition.x - ballTarget.x > 0 && ballPosition.y - ballTarget.y < 0) {//move down and left
                        direction = 0;
                        if (pathLength == height) {// moving in y direction
                            pen.y++;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.x--;
                            }
                        }
                        else if (pathLength == width) {//moving in x direction
                            pen.x--;
                            if (lengthsArray[currentLength] == 0) {
                                currentLength++;
                                pen.y++;
                            }
                        }
                    }
                    else {
                        // move straight up down left or right
                        if (pathLength == height && ballTarget.y < ballPosition.y) {// down 
                            pen.y--;
                        }
                        else if (pathLength == height && ballTarget.y > ballPosition.y) {// up
                            pen.y++;
                        }
                        else if (pathLength == width && ballTarget.x < ballPosition.x) {// left
                            direction = 0;
                            pen.x--;
                        }
                        else if (pathLength == width && ballTarget.x > ballPosition.x) {// right
                            direction = 1;
                            pen.x++;
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKGRAY);
            
            if (moving == 1) {
                if (direction == 1) {// move right animation
                    position.x = ballPosition.x - 22*2;
                    position.y = ballPosition.y - 25*2;
                    DrawTextureRec(rollerbotMoveRight, rollerbotMoveRightRec, position, WHITE);
                }
                else if (direction == 0) {// move left animation
                    position.x = ballPosition.x - 189;
                    position.y = ballPosition.y - 25*2;
                    DrawTextureRec(rollerbotMoveLeft, rollerbotMoveLeftRec, position, WHITE);
                }
            }
            else if (moving == 0) {
                // TODO on stop and start moving run wake animation before moving
                if (awake == 0) {
                // idle animation
                    if (direction == 1) {
                        position.x = ballPosition.x - 22*2;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotStaticIdleRight, rollerbotStaticIdleRightRec, position, WHITE);
                    }
                    else if (direction == 0) {
                        position.x = ballPosition.x - 189;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotStaticIdleLeft, rollerbotStaticIdleRightRec, position, WHITE);
                    }
                }
                else if (awake == 1) {
                    if (direction == 1) {
                        position.x = ballPosition.x - 22*2;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotWakeRight, rollerbotWakeRightRec, position, WHITE);
                    }
                    else if (direction == 0) {
                        position.x = ballPosition.x - 189;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotWakeLeft, rollerbotWakeRightRec, position, WHITE);
                    }
                }
                else if (awake == 2) {
                    if (direction == 1) {
                        position.x = ballPosition.x - 22*2;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotIdleRight, rollerbotStaticIdleRightRec, position, WHITE);
                    }
                    else if (direction == 0) {
                        position.x = ballPosition.x - 189;
                        position.y = ballPosition.y - 25*2;
                        DrawTextureRec(rollerbotIdleLeft, rollerbotStaticIdleRightRec, position, WHITE);
                    }
                }
            }
            
            // TODO placeholder enemies that on right click charge and shoot animation
            // TODO if awake == 1 and havent move == 0 for amount of time reverse wake animation to static idle
            // TODO could have a hold right click update movement target at some rate
            // TODO on change direction reset currentFrame
            // TODO on stop moving could make transition from move animation complete before switching to idle?
            
            DrawText("right click to move", 10, 10, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(rollerbotMoveRight);
    UnloadTexture(rollerbotMoveLeft);
    UnloadTexture(rollerbotStaticIdleRight);
    UnloadTexture(rollerbotWakeRight);
    UnloadTexture(rollerbotWakeLeft);
    UnloadTexture(rollerbotIdleLeft);
    UnloadTexture(rollerbotIdleRight);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}