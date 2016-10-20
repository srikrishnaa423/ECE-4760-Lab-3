/* 
 * File:   bb_collision.h
 * Author: Michael
 *
 * Created on October 11, 2016, 1:33 AM
 */

#ifndef BB_COLLISION_H
#define	BB_COLLISION_H


////Define variables used in pt_main
//static fix16 balls[rows][columns];      //2D array to keep track of balls
//static fix16 rij[2] = {0,0};         //rij = ri - rj = "vector from i to j"
//static fix16 vij[2] = {0,0};         //vij = vi - vj = "velocity of i relative to j"
//static fix16 delta_vi[2]  = {0,0};   //chnage in velocity for next frame
//static fix16 mag2_rij = 0;         //squared distance between ball i and j
//static fix16 drag = 0.001;         //drag from table

#define left_end    2
#define right_end   318
#define top_end     2
#define bottom_end  238
#define line_x_coord 80

fix16 temp_rx,temp_ry;
fix16 scalar;

static unsigned char collision_flag[nC2];
static unsigned short i_index = 0;
// NEW
void update_bb_velocity(void)
{
    i_index = rows*i - ((i*i + i)>>1); 
    for(j=i+1; j<rows; j++)
    {
                rij[0] = balls[i][0] - balls[j][0];   //x distance between i and j
                rij[1] = balls[i][1] - balls[j][1];   //y distance between i and j
                
                //Check for collision
                if((abs(rij[0])<int2fix16(4)) && (abs(rij[1])<int2fix16(4)))
                {            
                    mag2_rij = multfix16(rij[0],rij[0]) + multfix16(rij[1],rij[1]);
                    
                    if((mag2_rij < int2fix16(16)) && collision_flag[i_index+j] == 0)
                    { 
                        //Compute vij
                        vij[0] = balls[i][2] - balls[j][2];  //[2] is vx
                        vij[1] = balls[i][3] - balls[j][3];  //[3] is vy
                        
                        //Compute delta_vi
                        scalar = -((multfix16(rij[0],vij[0]) + multfix16(rij[1],vij[1]))>>4);     //constant to multiply rij by
                        
                        delta_vi[0] = multfix16(rij[0],scalar);      //delta v in x component
                        delta_vi[1] = multfix16(rij[1],scalar);      //delta v in y component
                        
                        //Apply delta_vi to balls i and j
                        //Ball i
                        balls[i][2] = balls[i][2] + delta_vi[0];        //vx
                        balls[i][3] = balls[i][3] + delta_vi[1];        //vy
                        //Ball j
                        balls[j][2] = balls[j][2] - delta_vi[0];        
                        balls[j][3] = balls[j][3] - delta_vi[1];        
                        
                        //Set collision flag
                        collision_flag[i_index+j] = 1;
                        
                    }               
                }   //endif   
                else if((collision_flag[i_index+j] == 1) && (mag2_rij > int2fix16(16)))
                {
                    collision_flag[i_index+j] = 0;
                }  //endelseif   
            }   //endfor   
}




void update_b_sidewall_velocity(void)
{
    if(balls[i][0] >= int2fix16(right_end))           //Collision with right wall
    {
        balls[i][2] = -balls[i][2];
        tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
        balls[i][0] = int2fix16(right_end-1);
    }
    
    if(balls[i][0] <= int2fix16(left_end+10))           
    {
        balls[i][5]=int2fix16(0);   //ball does not exist
        num_balls--;
        if(score>0){
            score--;
        }
    
//    Collision with left wall (TESTING PURPOSES ONLY)
//        balls[i][2] = -balls[i][2];
//        tft_fillCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
//        balls[i][0] = int2fix16(left_end+1);
    }

    
    if(  balls[i][1] <= int2fix16(top_end))
    {
        balls[i][3] = -balls[i][3];        //Collision with top of bottom wall
        tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
        balls[i][1] = int2fix16(top_end+1);
    }
    if(   balls[i][1] >= int2fix16(bottom_end)   )
    {
        balls[i][3] = -balls[i][3];        //Collision with top of bottom wall
        tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
        balls[i][1] = int2fix16(bottom_end-1);
    }
    
    
    //COLLISIONS WITH LINE
    if( (balls[i][0] <= int2fix16(line_x_coord+10) ) && (balls[i][0] >= int2fix16(line_x_coord-10)) ) {
        
        if(balls[i][1] >= int2fix16(178) || balls[i][1] <= int2fix16(62) ){
            
            balls[i][2] = -balls[i][2];
            tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
            if(balls[i][0] > int2fix16(line_x_coord))
                balls[i][0] = int2fix16(line_x_coord+11);
            else
                balls[i][0] =  int2fix16(line_x_coord-11);
        }
        
    }
}

void send_sound(void)
{
    if ( game_over == 1)
    {
      DmaChnDisable(DMA_CHANNEL0);
      DmaChnDisable(DMA_CHANNEL1);
      DmaChnEnable(DMA_CHANNEL2); 
    }
    else if (hit_flag == 1) 
    {
       DmaChnDisable(DMA_CHANNEL1);
      DmaChnDisable(DMA_CHANNEL2);
      DmaChnEnable(DMA_CHANNEL0);  
    }
    else if (miss_flag == 1)
    {
        DmaChnDisable(DMA_CHANNEL0);
      DmaChnDisable(DMA_CHANNEL2);
      DmaChnEnable(DMA_CHANNEL1); 
    }
}

void update_game_variables()
{

    if(game_time == end_time)
    {
        game_over = 1;
    }
    //else {
        if (frame_counter < framerate)
        {
            frame_counter++;
        }
        else
        {
            frame_counter = 0;
            game_time++;
            tft_drawLine(80, 0, 80, 60, ILI9340_YELLOW);   //x0, y0, x1, y1, color
            tft_drawLine(80, 180, 80, 240, ILI9340_YELLOW);
        }
    for (i=0; i<rows; i++){
     
        if(balls[i][5] == int2fix16(1) && balls[i][0] <= int2fix16(left_end+1)){
            miss_flag =1;
            break;
    }
    }
    //}
    
    
    //PADDLE HIT SOUND(TODO)
    send_sound();
}

void update_paddle_hit(){
    //Vfix already calculated  
    if( (balls[i][0] <= int2fix16(30)) && (balls[i][1] >= Vfix) && (balls[i][1] <= (Vfix + int2fix16(20))) ){
        score++;
        num_balls--;
        balls[i][5] = int2fix16(0);   //remove ball after hitting paddle
        hit_flag = 1;
    }
     
}



void update_position(){
       //Update position
    for (i=0; i<rows; i++){
        tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_BLACK);   //x0,y0,r,color
        if(balls[i][5] == int2fix16(1))
        {
            if(balls[i][4] >= int2fix16(ageout)){
                balls[i][5]=int2fix16(0);
                num_balls--;
            }
            else{
                balls[i][0] = balls[i][0] + balls[i][2];    //x' = x+dvx
                balls[i][1] = balls[i][1] + balls[i][3];    //y' = y+dvy
                tft_drawCircle(fix2int16(balls[i][0]),fix2int16(balls[i][1]), radius, ILI9340_WHITE);    //TO DO: OPTIMIZE AWAY tft_fillcircle
                balls[i][4] = balls[i][4]+ int2fix16(1);
            }
        }
    }
}

void update_drag(){
    //Update velocity with drag and wall collisions
    balls[i][2] = balls[i][2] - multfix16(balls[i][2],drag);     //account for drag in vx
    balls[i][3] = balls[i][3] - multfix16(balls[i][3],drag);     //account for drag in vy
}


#define vx_max 2.0
#define vy_max 1.0

void inject_balls(){
   // if(frame_counter == 0){
    for(i=0; i<rows; i++){
        if (balls[i][5] == int2fix16(0))      // if the ball does not exist  
        {          
            num_balls++;
            balls[i][0] = int2fix16(290);           // x position
            balls[i][1] = int2fix16(120);          // y position
            
            balls[i][2] = -float2fix16( ((float)rand()/(float)RAND_MAX*vx_max)+2.00);       // x velocity
            balls[i][3] = float2fix16(((2*(float)rand()/(float)RAND_MAX)-1)*vy_max);  // y velocity (can be pos or neg)
            balls[i][4] = int2fix16(0);            // lifetime of the ball
            balls[i][5] = int2fix16(1);            // ball exists
            break;                                 // exit after we find one nonexistent ball
        }
    //}
    }
}


void update_paddle_pos(){
     adc_9 = ReadADC10(0);   // read the result of channel 9 conversion from the idle buffer
        AcquireADC10(); // not needed if ADC_AUTO_SAMPLING_ON below
        
        // convert to fixed voltage
        tft_drawLine(20, fix2int16(Vfix), 20, fix2int16(Vfix) + 20, ILI9340_BLACK);   //x0, y0, x1, y1, color
        Vfix = multfix16(float2fix16(adc_9), ADC_scale);
        tft_drawLine(20, fix2int16(Vfix), 20, fix2int16(Vfix) + 20, ILI9340_YELLOW);   //x0, y0, x1, y1, color
}

#endif	/* BB_COLLISION_H */

