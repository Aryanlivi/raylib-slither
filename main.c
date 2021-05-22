#include "raylib.h"
#include <time.h>
#include <math.h>
const int screenWidth = 1000,screenHeight = 800;
bool GameOver=false;
int SnakeLen=100;
int key_active=0,Score=0,sec=0,min=0,hour=0;
struct Snake{
    int posX,posY,radius,scleraRadius,irisRadius;
    Color tint; 
    Texture2D head,body;
    float rotation,scale,eyeAngle;
    Vector2 leftEyePos,rightEyePos;
}Snake[1024];

struct Food{
    int posX,posY,radius,alpha;
    Color FoodColor;
}Food;

void InitGame(Texture2D,Texture2D,float,float);
void UpdateGame(float,Vector2,Vector2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Slither-Game");
    
    // Asset Loading -------------------------
    Texture2D snakehead= LoadTexture("./assets/snake/body.png");
    Texture2D snakebody= LoadTexture("./assets/snake/body.png");
    
    //Initializing Game -------------------------
    InitGame(snakehead,snakebody,200,200);
    int fps=60;
    SetTargetFPS(fps);          // Set our game to run at 10 frames-per-`
    //--------------------------------------------------------------------------------------
    
    //Changes to 1 every second in 60 fps
    int framesCounter = 0;     
    
    //Variables ----
    float magnitude=0.0f;
    float delta=0.0f;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        Vector2 Cursor=GetMousePosition();
        Vector2 Center={Snake[0].posX,Snake[0].posY};
        Vector2 CursorfromCenter={(Cursor.x-Center.x),(Cursor.y-Center.y)};
        magnitude=sqrt(pow(CursorfromCenter.x,2)+pow(CursorfromCenter.y,2));
        // Provides Direction of Mouse Cursor i.e Point to be followed by player
        Vector2 unitvector=(Vector2){(CursorfromCenter.x/magnitude),(CursorfromCenter.y/magnitude)};
        
        //Returns Delta time for last frame----
        delta=GetFrameTime();
        framesCounter++;
        
        if(GameOver==false){
            if (((framesCounter/fps)%2) == 1)
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
            
            //contains update functions

            UpdateGame(delta,Cursor,unitvector);  
        };
       
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
void InitGame(Texture2D snakehead,Texture2D snakebody,float initialposx,float initialposy){
    GameOver=false;
    Snake[0].head=snakehead;
    Snake[0].radius=20;
    Snake[0].rotation=0.0f;
    Snake[0].scale=2.0f;
    Snake[0].posX=initialposx;
    Snake[0].posY=initialposy;
    Snake[0].tint=RAYWHITE;
    
    Snake[0].leftEyePos=(Vector2){Snake[0].posX,Snake[0].posY};
    Snake[0].rightEyePos=(Vector2) { GetScreenWidth()/2.0f + 100.0f, GetScreenHeight()/2.0f };
    Snake[0].scleraRadius=10;
    Snake[0].irisRadius=5;
    
    for(int i=1;i<SnakeLen;i++){
        Snake[i].body=snakebody;
        Snake[i].radius=20; 
        Snake[i].scale=2.0f;
        Snake[i].posX=Snake[i-1].posX-Snake[i-1].radius;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].tint=RAYWHITE;
        if(i%2!=0){ 
        Snake[i].tint=RAYWHITE;
        };
    }
    Food.posX= GetRandomValue(30,screenWidth-30);
    Food.posY= GetRandomValue(30,screenHeight-30);
    Food.radius=5;
    Food.alpha=255;
    Food.FoodColor=(Color){255,0,0,Food.alpha};
}
void Grow(){
    for(int i=SnakeLen-1;i>0;i--){
        Snake[i].posX=Snake[i-1].posX;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].radius=20;
        Snake[i].tint=BLUE;
        if(i%2!=0){ 
        Snake[i].tint=BLACK;
        };
    }
    Score++;
}

int blink_secs=0;
float dx = 0.0f, dy = 0.0f, dxx = 0.0f, dyy = 0.0f;
void UpdateDraw(float delta,Vector2 MousePos){
    if(GameOver==false){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ///snake:
        //Draws Snake body
        for(int i=1;i<SnakeLen;i++){
            Vector2 Bodypos={Snake[i].posX,Snake[i].posY};
            DrawTextureEx(Snake[i].body,Bodypos,Snake[i].rotation,Snake[i].scale,Snake[i].tint);
        }
        Vector2 Headpos={Snake[0].posX,Snake[0].posY};
        DrawTextureEx(Snake[0].head,Headpos,Snake[0].rotation,Snake[0].scale,Snake[0].tint);
        
        Snake[0].leftEyePos=(Vector2){Headpos.x+10,Headpos.y+20};
        dx = MousePos.x - Snake[0].leftEyePos.x;
        dy = MousePos.y - Snake[0].leftEyePos.y;

        Snake[0].eyeAngle = atan2f(dy, dx);

        dxx = ( Snake[0].scleraRadius -  Snake[0].irisRadius)*cosf(Snake[0].eyeAngle);
        dyy = ( Snake[0].scleraRadius - Snake[0].irisRadius)*sinf(Snake[0].eyeAngle);

        MousePos.x = Snake[0].leftEyePos.x + dxx;
        MousePos.y = Snake[0].leftEyePos.y + dyy;
        
        DrawCircleV(Snake[0].leftEyePos, Snake[0].scleraRadius, RAYWHITE);
        //DrawCircleV(MousePos, Snake[0].irisRadius, BROWN);
        DrawCircleV(MousePos,Snake[0].irisRadius, BLACK);
        
        if(Snake[0].posX>=screenWidth && key_active==0){
            Snake[0].posX=0;
        };
        if(Snake[0].posX<=0 && key_active==1){
            Snake[0].posX=screenWidth;
        };
        if(Snake[0].posY<=0 && key_active==2){
            Snake[0].posY=screenHeight;
        };
        if(Snake[0].posY>=screenHeight && key_active==3){
            Snake[0].posY=0;
        };
        
        //////food:
        DrawCircle(Food.posX,Food.posY,Food.radius,Food.FoodColor);
        if(Food.alpha==255 && blink_secs==20){
            Food.alpha=0;
            Food.FoodColor=(Color){255,0,0,Food.alpha};
            blink_secs=0;
        }
        else{
            Food.alpha=255;
            Food.FoodColor=(Color){255,0,0,Food.alpha};
            blink_secs+=1;
        }
        
        /*
        ///collision:
        bool Food_ate=CheckCollisionCircleRec((Vector2){Food.posX,Food.posY},Food.radius,Snake[0].rect);
        if(Food_ate){
            Food.posX= GetRandomValue(30,screenWidth-30);
            Food.posY= GetRandomValue(30,screenHeight-30);
            SnakeLen+=1;
            Grow();
        };
        */
        /*
        for(int j=0;j<SnakeLen;j++){
            bool is_dead=CheckCollisionRecs(Snake[0].rect,Snake[j].rect);   
            if(is_dead && j!=0){
                GameOver=true;
            };
        };
        */
        DrawText(TextFormat("Time Elapsed: %d:%d:%d",hour,min,sec),20,0,20,BLACK);
        DrawText(TextFormat("Score: %01i", Score),screenWidth-150,0,20,PURPLE);
        
    };
    if(GameOver==true){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        DrawText(TextFormat("Time Elapsed: %0d:%d:%d",hour,min,sec),screenWidth*0.3,screenHeight*0.4,50,PURPLE);
        DrawText(TextFormat("Score: %01i", Score),screenWidth*0.3,screenHeight*0.6,50,ORANGE);
        EndDrawing();
        TakeScreenshot("capture.png");//bmp tga    
    };
    EndDrawing();
}
float absolute(float number){
    if(number<0){
        number=-1*(number);
    }
    return number;
}
float mylerp(float startpos,float endpos,float step,Vector2 UnitVector){
    float difference =endpos-startpos;
    float temp=absolute(difference);
    float val=startpos + (difference) * step;
    return val;
}




float checkx=0.0f,checky=0.0f;
Vector2 followpoint;

void UpdateInput(float delta,Vector2 MousePos,Vector2 UnitVector){  
    checkx = absolute(MousePos.x -Snake[0].posX) ;
    checky = absolute(MousePos.y - Snake[0].posY);
    if(checkx<=100 && checkx>=40 && checky<=100 && checky>=40 ){
        followpoint=MousePos;
    }
    else if(checkx<40 && checky<40){
        followpoint.x=Snake[0].posX;
        followpoint.y=Snake[0].posY;
    } 
    else{
        followpoint.x=Snake[0].posX+100*UnitVector.x;
        followpoint.y=Snake[0].posY+100*UnitVector.y;
    }
    float reqposX=mylerp(Snake[0].posX,followpoint.x,2*delta,UnitVector);
    float reqposY=mylerp(Snake[0].posY,followpoint.y,2*delta,UnitVector);
    Snake[0].posX=reqposX;
    Snake[0].posY=reqposY;
    
    for(int i=SnakeLen-1;i>0;i--){
        float val=mylerp(Snake[i].posX,Snake[i-1].posX,15*delta,UnitVector);
        float val2=mylerp(Snake[i].posY,Snake[i-1].posY,15*delta,UnitVector);
            if(checkx<40 && checky<40){
                Snake[i].posX=Snake[i].posX;
                Snake[i].posY=Snake[i].posY;
            }
            else{
            Snake[i].posX=Snake[i-1].posX;
            Snake[i].posY=Snake[i-1].posY;
            }
        }
}

void UpdateGame(float delta,Vector2 MousePos,Vector2 UnitVector){
    UpdateInput(delta,MousePos,UnitVector);
    UpdateDraw(delta,MousePos); 
}