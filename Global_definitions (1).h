#ifndef GLOBAL_VARIABLES_H
#define	GLOBAL_VARIABLES_H


#define	SYS_FREQ 4000000;                 // config.h sets 40 MHz
    // Open the desired DMA channels.
#define dmaChn0     0
#define dmaChn1     1
#define dmaChn2     2



//Fixed point arithmetic

// === the fixed point macros ========================================
typedef signed int fix16 ;
#define multfix16(a,b) ((fix16)(((( signed long long)(a))*(( signed long long)(b)))>>16)) //multiply two fixed 16:16
#define float2fix16(a) ((fix16)((a)*65536.0)) // 2^16
#define fix2float16(a) ((float)(a)/65536.0)
#define fix2int16(a)    ((int)((a)>>16))
#define int2fix16(a)    ((fix16)((a)<<16))
#define divfix16(a,b) ((fix16)((((signed long long)(a)<<16)/(b)))) 
#define sqrtfix16(a) (float2fix16(sqrt(fix2float16(a)))) 
#define absfix16(a) abs(a)
    
#define rows 100  //30      //up to 227 balls
#define nC2  4950 //435               //nC2
#define columns 6       //x, y, vx, vy, HC, exist
#define radius 2        //ball radius (pixels)
//#define HC 3            //hit counter from collision
#define end_time 18    //Game time (sec)
#define framerate 18    //frames/sec
#define ageout 200  //time at which the ball ages out

int actual_frame_rate;
int execution_time;
//Define threads
static struct pt pt_main ;   
//static struct pt pt_dma;
int i;
int j;



char tft_buffer[60];                      // string buffer used to output to TFT




//Ball definitions
static fix16 balls[rows][columns];      //2D array to keep track of balls
static fix16 drag = float2fix16(0.001);         //drag from table
static short num_balls = 0, score = 0, miss_flag = 0, game_time = 0, hit_flag = 0, game_over = 0, frame_counter = 0; 


//BB collision definitions
static fix16 rij[2] = {int2fix16(0),int2fix16(0)};         //rij = ri - rj = "vector from i to j"
static fix16 vij[2] = {int2fix16(0),int2fix16(0)};         //vij = vi - vj = "velocity of i relative to j"
static fix16 delta_vi[2]  = {int2fix16(0),int2fix16(0)};   //chnage in velocity for next frame
static fix16 mag2_rij = int2fix16(0);         //squared distance between ball i and j

    
    //ADC STUFF
    static int adc_9;
    static float V;
    static fix16 Vfix, ADC_scale ;
    
    

static int begin_time = 0;

#endif	/* GLOBAL_VARIABLES_H */

