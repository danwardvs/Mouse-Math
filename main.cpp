#include<allegro.h>
#include<alpng.h>
#include<cmath>

BITMAP* buffer;
BITMAP* cursor;
BITMAP* pointer;

float distance_to_mouse;
float inverse_tangent_argument;
float angle_radians;
float angle_degrees;
float angle_allegro;
float tan_1;
float tan_2;

int point_x=400;
int point_y=300;

unsigned int positive_mouse_x;
unsigned int positive_mouse_y;

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}
int distance_to_object(int x_1, int y_1,int x_2,int y_2){
    return sqrt((pow(x_1-x_2,2))+(pow(y_1-y_2,2)));

}
float find_angle(int x_1, int y_1, int x_2, int y_2){
        float tan_1;
        float tan_2;
        if(x_1-x_2!=0 && y_1-y_2!=0){
            tan_1=y_1-y_2;
            tan_2=x_1-x_2;
        }

    return atan2(tan_1,tan_2);
}

void update(){

    rectfill(buffer,0,0,800,600,makecol(255,255,250));

    float mouse_x_float=mouse_x;
    float mouse_y_float=mouse_y;

    draw_sprite(buffer,cursor,mouse_x,mouse_y);
    putpixel(buffer,point_x,point_y,makecol(0,0,0));

    distance_to_mouse=distance_to_object(mouse_x,mouse_y,point_x,point_y);
    angle_radians=find_angle(point_x,point_y,mouse_x,mouse_y);

    angle_degrees=(angle_radians*57.2957795);
    angle_allegro=(angle_degrees/1.41176470588);

    textprintf_ex(buffer,font,5,5,makecol(0,0,0),-1,"Mouse X:%i",mouse_x);
    textprintf_ex(buffer,font,5,15,makecol(0,0,0),-1,"Mouse Y:%i",mouse_y);
    textprintf_ex(buffer,font,5,25,makecol(0,0,0),-1,"Distance to mouse:%4.2f",distance_to_mouse);
    textprintf_ex(buffer,font,5,35,makecol(0,0,0),-1,"Angle,radians,%4.2f",angle_radians);
    textprintf_ex(buffer,font,5,45,makecol(0,0,0),-1,"Angle,degrees:%4.2f",angle_degrees);
    textprintf_ex(buffer,font,5,55,makecol(0,0,0),-1,"Angle,allegro:%4.2f",angle_allegro);

    if(key[KEY_LEFT])point_x--;
    if(key[KEY_RIGHT])point_x++;
    if(key[KEY_UP])point_y--;
    if(key[KEY_DOWN])point_y++;

    rotate_sprite(buffer,pointer,point_x-30,point_y-30,itofix(angle_allegro));
    draw_sprite(screen,buffer,0,0);


}








void setup(){
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
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Angle Finder");
  setup();


    while(!key[KEY_ESC]){
        update();
  	}

	return 0;
}
END_OF_MAIN()
