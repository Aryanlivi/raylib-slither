
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
