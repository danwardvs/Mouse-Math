#include<allegro.h>
#include<alpng.h>
#include<cmath>

BITMAP* buffer;
BITMAP* cursor;
BITMAP* pointer;

float distance_to_mouse;
float angle_radians;
float angle_degrees;
float angle_allegro;

int point_x=400;
int point_y=300;

float angle_x;
float angle_y;

bool create_bullet;
int bullet_delay;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

bool close_button_pressed;

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)


struct bullet{
    float x;
    float y;
    float vector_x;
    float vector_y;
    bool on_screen=false;
}bullets[100];

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}
//Finds distance between point 1 and point 2
int distance_to_object(int x_1, int y_1,int x_2,int y_2){
    return sqrt((pow(x_1-x_2,2))+(pow(y_1-y_2,2)));

}

//Finds angle of point 2 relative to point 1
float find_angle(int x_1, int y_1, int x_2, int y_2){
    float tan_1;
    float tan_2;

    tan_1=y_1-y_2;
    tan_2=x_1-x_2;
    return atan2(tan_1,tan_2);




}


void update(){

    //Calls the function, solves the math
    distance_to_mouse=distance_to_object(mouse_x,mouse_y,point_x,point_y);
    angle_radians=find_angle(point_x,point_y,mouse_x,mouse_y);

    //Converts radians to usable(by allegro) values
    angle_degrees=(angle_radians*57.2957795);
    angle_allegro=(angle_degrees/1.41176470588);

    angle_x=sin(angle_degrees);
    angle_y=cos(angle_degrees);

    //Moves the point
    if(key[KEY_LEFT] || key[KEY_A])point_x--;
    if(key[KEY_RIGHT] || key[KEY_D])point_x++;
    if(key[KEY_UP] || key[KEY_W])point_y--;
    if(key[KEY_DOWN] || key[KEY_S])point_y++;

    bullet_delay++;
    if(key[KEY_SPACE] && bullet_delay>3 || mouse_b & 1 && bullet_delay>3 ){
        create_bullet=true;
        bullet_delay=0;
    }

    for(int i=0; i<100; i++){
        if(bullets[i].on_screen){
           bullets[i].x+=bullets[i].vector_x;
           bullets[i].y+=bullets[i].vector_y;


           if(bullets[i].x>800 || bullets[i].y>600 || bullets[i].x<0 || bullets[i].y<0)bullets[i].on_screen=false;
        }else if(create_bullet==true){
            bullets[i].on_screen=true;
            create_bullet=false;
            bullets[i].x=point_x;
            bullets[i].y=point_y;
            bullets[i].vector_x=-5*cos(angle_radians);
            bullets[i].vector_y=-5*sin(angle_radians);
        }
    }

}

void draw(){
   //Draws the screen
    rectfill(buffer,0,0,800,600,makecol(255,255,250));
    textprintf_ex(buffer,font,5,5,makecol(0,0,0),-1,"Mouse X:%i",mouse_x);
    textprintf_ex(buffer,font,5,15,makecol(0,0,0),-1,"Mouse Y:%i",mouse_y);
    textprintf_ex(buffer,font,5,25,makecol(0,0,0),-1,"Distance to mouse:%4.2f",distance_to_mouse);
    textprintf_ex(buffer,font,5,35,makecol(0,0,0),-1,"Radians:%4.2f,Degrees%4.2f,Allegro%4.2f",angle_radians,angle_degrees,angle_allegro);
    textprintf_ex(buffer,font,5,45,makecol(0,0,0),-1,"Vector X:%4.2f,Vector Y:%4.2f",angle_x,angle_y);
    rotate_sprite(buffer,pointer,point_x-30,point_y-30,itofix(angle_allegro));
    putpixel(buffer,point_x,point_y,makecol(0,0,0));
    for(int i=0; i<100; i++){
        if(bullets[i].on_screen){
            putpixel(buffer,bullets[i].x,bullets[i].y,makecol(255,0,0));
            putpixel(buffer,bullets[i].x,bullets[i].y-1,makecol(255,0,0));
            putpixel(buffer,bullets[i].x+1,bullets[i].y,makecol(255,0,0));
            putpixel(buffer,bullets[i].x+1,bullets[i].y-1,makecol(255,0,0));
        }
    }
    draw_sprite(buffer,cursor,mouse_x,mouse_y);
    draw_sprite(screen,buffer,0,0);



}








void setup(){

    // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);


    buffer=create_bitmap(800,600);

    if(!(cursor = load_bitmap("cursor.png",NULL))){
        abort_on_error( "Cannot find cursor.png.\n Please check your files and try again.");
    }
    if(!(pointer = load_bitmap("pointer.png",NULL))){
        abort_on_error( "Cannot find pointer.png.\n Please check your files and try again.");
    }
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);

  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,600, 0, 0);



  set_window_title("Mouse Math");
  setup();


    while(!key[KEY_ESC] && !close_button_pressed){
       while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }
	return 0;
}
END_OF_MAIN()
