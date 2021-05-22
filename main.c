#include "raylib.h"
#include <time.h>
#include <math.h>
const int screenWidth = 1000;
const int screenHeight = 800;
bool gameOver=false;
int SnakeLen=100;
int key_active=0;
int Score=0;
int secs=0;
int mins=0;
int hours=0;
int fps_call=0;

struct Snake{
    int posX,posY,radius;
    Color BodyColor; 
    Texture2D head,body;
    float rotation;
}Snake[1024];

struct Food{
    int posX,posY,radius,alpha;
    Color FoodColor;
}Food;

void InitGame(Texture2D,Texture2D);
void UpdateGame(float,Vector2,Vector2);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Slither-Game");
    Texture2D snakehead= LoadTexture("./assets/snake/head.png");
    Texture2D snakebody= LoadTexture("./assets/snake/body.png");
    InitGame(snakehead,snakebody);
    SetTargetFPS(60);          // Set our game to run at 10 frames-per-`
    //--------------------------------------------------------------------------------------
    
      
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        Vector2 Distance=GetMousePosition();
        Vector2 Center={Snake[0].posX,Snake[0].posY};
        Vector2 MousePos={(Distance.x-Center.x),(Distance.y-Center.y)};
        float magnitude=sqrt(pow(MousePos.x,2)+pow(MousePos.y,2));
        Vector2 unitvector=(Vector2){(MousePos.x/magnitude),(MousePos.y/magnitude)};
        
        float delta=GetFrameTime();
        if(gameOver==false){
            if(fps_call==10){
                secs+=1;
            };
            if(secs==60){
                mins+=1;
                secs=0;
            };
            if(mins==60){
                hours+=1;
                mins=0;
            };      
        };
        UpdateGame(delta,Distance,unitvector);  
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
void InitGame(Texture2D snakehead,Texture2D snakebody){
    gameOver=false;
    Snake[0].head=snakehead;
    Snake[0].radius=20;
    Snake[0].rotation=0.0f;
    Snake[0].posX=200;
    Snake[0].posY=200;
    Snake[0].BodyColor=BLACK;
    for(int i=1;i<SnakeLen;i++){
        Snake[i].body=snakebody;
        Snake[i].radius=20; 
        Snake[i].posX=Snake[i-1].posX-Snake[i-1].radius;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].BodyColor=BLUE;
        if(i%2!=0){ 
        Snake[i].BodyColor=BLACK;
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
        Snake[i].BodyColor=BLUE;
        if(i%2!=0){ 
        Snake[i].BodyColor=BLACK;
        };
    }
    Score++;
}

int blink_secs=0;
float bodyrotation=0.0f;
float scale=1.0;

void UpdateDraw(float delta){
    if(gameOver==false){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        
        ///snake:
        for(int i=1;i<SnakeLen;i++){
            Vector2 position_body={Snake[i].posX,Snake[i].posY};
            DrawTextureEx(Snake[i].body,position_body,bodyrotation,scale,RAYWHITE);
           
        }
    
        Vector2 position_head={Snake[0].posX,Snake[0].posY};
        DrawTextureEx(Snake[0].head,position_head,Snake[0].rotation,scale,RAYWHITE);
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
                gameOver=true;
            };
        };
        */
        DrawText(TextFormat("Time Elapsed: %01i:%i:%i",hours,mins,secs),20,0,20,PURPLE);
        DrawText(TextFormat("Score: %01i", Score),screenWidth-150,0,20,PURPLE);
        
    };
    if(gameOver==true){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        DrawText(TextFormat("Time Elapsed: %01i:%i:%i",hours,mins,secs),screenWidth*0.3,screenHeight*0.4,50,PURPLE);
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
float mylerp(float startpos,float endpos,float step,Vector2 UnitVector,bool specialcase){
    float difference =endpos-startpos;
    float temp=absolute(difference);
    float val=startpos + (difference) * step;
    return val;
}

float dx=0.0f,dy=0.0f;
Vector2 followpoint;
float angle=0.0f;
void UpdateInput(float delta,Vector2 MousePos,Vector2 UnitVector){  
   /*
    dx = MousePos.x -Snake[0].posX ;
    dy = MousePos.y - Snake[0].posY;
    angle=atan2f(dy,dx)*180/3.14;
    Snake[0].rotation=angle;
    
    DrawText(TextFormat("%f",angle),500,200,20,BLACK);
    */
    dx = absolute(MousePos.x -Snake[0].posX) ;
    dy = absolute(MousePos.y - Snake[0].posY);
    if(dx<=100 && dx>=40 && dy<=100 && dy>=40 ){
        followpoint=MousePos;
    }
    else if(dx<40 && dy<40){
        followpoint.x=Snake[0].posX;
        followpoint.y=Snake[0].posY;
    } 
    else{
        followpoint.x=Snake[0].posX+100*UnitVector.x;
        followpoint.y=Snake[0].posY+100*UnitVector.y;
    }
    float reqposX=mylerp(Snake[0].posX,followpoint.x,2*delta,UnitVector,true);
    float reqposY=mylerp(Snake[0].posY,followpoint.y,2*delta,UnitVector,false);
    Snake[0].posX=reqposX;
    Snake[0].posY=reqposY;
    
    for(int i=SnakeLen-1;i>0;i--){
            if(dx<40 && dy<40){
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
    UpdateDraw(delta); 
    fps_call+=1;
    if(fps_call>60){
       fps_call=0;
    };
}