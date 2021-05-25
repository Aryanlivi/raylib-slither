#include "raylib.h"
#include <time.h>
#include <math.h>

//Global Variables:
const int screenWidth = 1000,screenHeight = 800;
bool isgameOver=false;
int snakeLen=50,gameScore=0,sec=0,min=0,hour=0;
//----------------------------------

//Structures:
struct Snake{
    int posX,posY,radius,scleraRadius,irisRadius;
    Color tint; 
    Texture2D headTex,bodyTex;
    float rotation,scale,eyeAngle1,eyeAngle2;
    Vector2 headVel,bodyVel,lefteyePos,righteyePos;
}Snake[1024];

struct Food{
    int posX,posY,radius,alpha;
    Color foodColor;
}Food;
//-----------------------


//pre-defining Functions:
void InitGame(Texture2D,Texture2D,float,float);
void UpdateGame(float,Vector2,Vector2);
//----------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Slither-Game");
    
    // Asset Loading -------------------------
    Texture2D snakeHead= LoadTexture("./assets/snake/snakeHead.png");
    Texture2D snakeBody= LoadTexture("./assets/snake/snakeBody.png");
    Texture2D background= LoadTexture("./assets/background2.png");
    
    //Initializing Game -------------------------
    InitGame(snakeHead,snakeBody,200,200);
    int FPS=60;
    SetTargetFPS(FPS);          // Set our game to run at 10 frames-per-`
    //--------------------------------------------------------------------------------------
    
    //Changes to 1 every second in 60 FPS
    int framesCounter = 0;     
    
    //Variables ----
    float magnitude=0.0f;
    float delta=0.0f;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
    // Provides Direction of Mouse cursor i.e Point to be followed by player
        Vector2 cursor=GetMousePosition();
        Vector2 screenCenter={Snake[0].posX,Snake[0].posY};
        Vector2 distanceCenter={(cursor.x-screenCenter.x),(cursor.y-screenCenter.y)};
        magnitude=sqrt(pow(distanceCenter.x,2)+pow(distanceCenter.y,2));
        Vector2 unitVec=(Vector2){(distanceCenter.x/magnitude),(distanceCenter.y/magnitude)};
    //----------------------------------------
    
    //Returns Delta time for last frame----
        delta=GetFrameTime();
        framesCounter++;
    //------------------------------- 
    
    //Background:
        DrawTexture(background,0, 0, RAYWHITE);     
    //---------------------------
    
    //While game is running:
        if(isgameOver==false){
    //-----------------------
    
    //Calculate Time Elapsed:
            if (((framesCounter/FPS)%2) == 1)
            {
            sec+=1;
            if(sec==60){
                min+=1;
                sec=0;
            }
            if(min==60){
                hour+=1;
                min=0;
            }
            framesCounter = 0;
            }   
    //---------------------------------
        
    //Updates Game every frame
            UpdateGame(delta,cursor,unitVec);  
        };
    //-------------------------
    }
    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
void InitGame(Texture2D snakeHead,Texture2D snakeBody,float posX,float posY){
    //Init game:
    isgameOver=false;
    //---------------
    
    //Init Snake Head
    Snake[0].headTex=snakeHead;
    Snake[0].radius=20;
    Snake[0].rotation=0.0f;
    Snake[0].scale=2.0f;
    Snake[0].posX=posX;
    Snake[0].posY=posY;
    Snake[0].headVel=(Vector2){2,2};
    Snake[0].tint=RAYWHITE;
    //------------------------
    
    //Init Snake Eyes:
    Snake[0].lefteyePos=(Vector2){Snake[0].posX,Snake[0].posY};
    Snake[0].righteyePos=(Vector2) { GetScreenWidth()/2.0f + 100.0f, GetScreenHeight()/2.0f };
    Snake[0].scleraRadius=8;
    Snake[0].irisRadius=5;
    //---------------------------
    
    //Init Snake Body:
    for(int i=1;i<snakeLen;i++){
        Snake[i].bodyTex=snakeBody;
        Snake[i].radius=20; 
        Snake[i].scale=2.0f;
        Snake[i].posX=Snake[i-1].posX-Snake[i-1].radius;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].bodyVel=(Vector2){15,15};
        Snake[i].tint=RAYWHITE;
        //Every alternate index:
        if(i%2!=0){ 
        Snake[i].tint=RAYWHITE;
        };
    }
    //-------------------------------
    
    //Init Food:
    Food.posX= GetRandomValue(30,screenWidth-30);
    Food.posY= GetRandomValue(30,screenHeight-30);
    Food.radius=5;
    Food.alpha=255;
    Food.foodColor=(Color){255,0,0,Food.alpha};
    //-------------
}

void Grow(){
    for(int i=snakeLen-1;i>0;i--){
        Snake[i].posX=Snake[i-1].posX;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].radius=20;
        Snake[i].tint=BLUE;
        if(i%2!=0){ 
        Snake[i].tint=BLACK;
        };
    }
    gameScore++;
}

void UpdateSnakeEye(Vector2 headPos){
    Vector2 irisLeftPosition = GetMousePosition();
    Vector2 irisRightPosition = GetMousePosition();
    //Changing Snake Left Eye:
    Snake[0].lefteyePos=(Vector2){headPos.x+10,headPos.y+10};
    
    float leftdx = irisLeftPosition.x - Snake[0].lefteyePos.x;
    float leftdy = irisLeftPosition.y - Snake[0].lefteyePos.y;

    Snake[0].eyeAngle1 = atan2f(leftdy, leftdx);

    float leftdxx = ( Snake[0].scleraRadius -  Snake[0].irisRadius)*cosf(Snake[0].eyeAngle1);
    float leftdyy = ( Snake[0].scleraRadius - Snake[0].irisRadius)*sinf(Snake[0].eyeAngle1);

    irisLeftPosition.x = Snake[0].lefteyePos.x + leftdxx;
    irisLeftPosition.y = Snake[0].lefteyePos.y + leftdyy;
    
    DrawCircleV(Snake[0].lefteyePos, Snake[0].scleraRadius, RAYWHITE);
    DrawCircleV(irisLeftPosition,Snake[0].irisRadius, BLACK);
    
    //Changing Snake Right Eye:
    Snake[0].righteyePos=(Vector2){headPos.x+30,headPos.y+10};
    float rightdx = irisRightPosition.x - Snake[0].righteyePos.x;
    float rightdy = irisRightPosition.y - Snake[0].righteyePos.y;

    Snake[0].eyeAngle2 = atan2f(rightdy, rightdx);

    float rightdxx = (Snake[0].scleraRadius -  Snake[0].irisRadius)*cosf(Snake[0].eyeAngle2);
    float rightdyy = (Snake[0].scleraRadius -  Snake[0].irisRadius)*sinf(Snake[0].eyeAngle2);

    irisRightPosition.x = Snake[0].righteyePos.x + rightdxx;
    irisRightPosition.y = Snake[0].righteyePos.y + rightdyy;
    
    DrawCircleV(Snake[0].righteyePos, Snake[0].scleraRadius, RAYWHITE);
    DrawCircleV(irisRightPosition, Snake[0].irisRadius, BLACK);
}
void UpdateSnake(void){
    //Draws Snake bodyTex:
        for(int i=1;i<snakeLen;i++){
            Vector2 Bodypos={Snake[i].posX,Snake[i].posY};
            DrawTextureEx(Snake[i].bodyTex,Bodypos,Snake[i].rotation,Snake[i].scale,Snake[i].tint);
        }
    
    //Draws Snake headTex:
        Vector2 headPos={Snake[0].posX,Snake[0].posY};
        DrawTextureEx(Snake[0].headTex,headPos,Snake[0].rotation,Snake[0].scale,Snake[0].tint);
    
    //Snake Eyes:
        UpdateSnakeEye(headPos);
        
}
void UpdateFood(void){
    DrawCircle(Food.posX,Food.posY,Food.radius,Food.foodColor);
}
void UpdateGameInfo(void){
    DrawText(TextFormat("Time Elapsed: %d:%d:%d",hour,min,sec),20,0,30,PURPLE);
    DrawText(TextFormat("Score: %01i", gameScore),screenWidth-150,0,30,PURPLE);
}
void UpdateDraw(void){
    if(isgameOver==false){
        BeginDrawing();
        ClearBackground(BLACK);
        UpdateSnake();
        UpdateFood();
        UpdateGameInfo();
    };
    if(isgameOver==true){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        DrawText(TextFormat("Time Elapsed: %0d:%d:%d",hour,min,sec),screenWidth*0.3,screenHeight*0.4,50,PURPLE);
        DrawText(TextFormat("gameScore: %01i", gameScore),screenWidth*0.3,screenHeight*0.6,50,ORANGE);
        EndDrawing();
        TakeScreenshot("capture.png");//bmp tga    
    };
    EndDrawing();
}

float _Absolute(float n){
    if(n<0){
        n=-1*(n);
    }
    return n;
}
float _Lerp(float start,float end,float step,Vector2 unitVec){
    float difference = (end-start);
    float val = start + (difference) * step;
    return val;
}

void UpdateInput(float delta,Vector2 mousePos,Vector2 unitVec){  

    Vector2 target;
    //Checks the distance of mouse from snake head:
    float checkx = _Absolute(mousePos.x -Snake[0].posX) ;
    float checky = _Absolute(mousePos.y - Snake[0].posY);
    
    if(checkx<=100 && checkx>=40 && checky<=100 && checky>=40 ){
        target=mousePos;
    }
    else if(checkx<40 && checky<40){
        //stops snakehead if mouse stops:
        target.x=Snake[0].posX;
        target.y=Snake[0].posY;
    } 
    else{
        //Move with limited speed towards mouse direction:
        target.x=Snake[0].posX+100*unitVec.x;
        target.y=Snake[0].posY+100*unitVec.y;
    }
    
    //Head follows Mouse:
    float reqposX=_Lerp(Snake[0].posX,target.x,Snake[0].headVel.x*delta,unitVec);
    float reqposY=_Lerp(Snake[0].posY,target.y,Snake[0].headVel.y*delta,unitVec);
    Snake[0].posX=reqposX;
    Snake[0].posY=reqposY;
    
    //Body follows Head:
    for(int i=snakeLen-1;i>0;i--){
        float val=_Lerp(Snake[i].posX,Snake[i-1].posX,(Snake[i].bodyVel.x)*delta,unitVec);
        float val2=_Lerp(Snake[i].posY,Snake[i-1].posY,(Snake[i].bodyVel.y)*delta,unitVec);
        
        if(checkx<40 && checky<40){
            Snake[i].posX=Snake[i].posX;
            Snake[i].posY=Snake[i].posY;
        }
        else{
        Snake[i].posX=val;
        Snake[i].posY=val2;
        }
    }
}

void UpdateGame(float delta,Vector2 mousePos,Vector2 unitVec){
    UpdateInput(delta,mousePos,unitVec);
    UpdateDraw(); 
}