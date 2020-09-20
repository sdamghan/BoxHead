#ifndef __BOXHEADGAME__
#define __BOXHEADGAME__


#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
//#include "SDL/SDL_mixer.h"
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

extern int SCREEN_WIDE;
extern int SCREEN_HIGH;
#define SCREEN_BPP 32

#define image_number 10
#define Ground1 0
#define Ground2 1
#define Boxhead 2
#define Turrentmachine 3
#define Barricades 4
#define WZombie 5
#define RZombie 6
#define Plus 7
#define Barrel 8
#define Start 9

// Zombie movement unit
extern const int ZMU;
//
extern const int boxy, zomby;
extern const int dead, live;

//The areas of the sprite sheet
extern const int RIGHT;
extern const int LEFT;
extern const int UP;
extern const int DOWN;
extern const int RIGHT_UP;
extern const int LEFT_UP;
extern const int RIGHT_DOWN;
extern const int LEFT_DOWN;

// Global variables
extern vector < SDL_Rect > sample;
extern vector < vector< SDL_Rect > > cp; // image's Clips

extern SDL_Surface* images[ image_number ]; // images
extern SDL_Surface* screen; // Show Window
 extern SDL_Surface* Score;
extern SDL_Surface* END;
extern SDL_Surface* BoxHead_health ;

extern SDL_Rect camera;

//The event structure
extern SDL_Event event;

extern TTF_Font* font ;
extern TTF_Font* font_end;

extern SDL_Color textcolor ;
extern SDL_Color end_color ;

//The frames per second
extern const int FRAMES_PER_SECOND;

//The dimenstions of the stick figure
extern const int BOX_WIDTH;
extern const int BOX_HEIGHT;


//BoxHead clips [ status ][ frames ]
extern const int Walk; extern const int Pistol;extern const int MechinGun; extern const int ShotGun;
extern const int UZGun;extern  const int Launcher;extern const int WhiteZombie;extern  const int RedZombie;
extern SDL_Rect clips[ 8 ][ 8 ][ 5 ];


// key states
extern Uint8 *keystates ;
// width height game
extern int width;extern int height;
// variables situation baraye inke 2bare chap nashan
extern bool BoxHead_Situation; 
// game input
extern char** INPUT;
//total score of game
extern long int SCORE;
// Game show screen size
extern int LEVEL_WIDTH,LEVEL_HEIGHT;
// Bullet
extern bool bullet;
//
extern int zombie_no;



//image loading function
SDL_Surface* load_image ( string filename );
//apply surface on screen
void apply_surface ( int x, int y, SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip );
//initialiazian function
bool Init ();
//loading files
void loading_files ();
//set up clips
void set_clips ();
//input function
char** input();
//map creation
void Create_Map ( char** in );
//cleaning function
void clean_up();
//game loading function
void Game_Loading ();
//boxhead moving function
void BoxHead_Movment ( struct Object& ex );
//
void BOXHEAD( struct Object& ex );
//
void handle_events( struct Object& ex );
//
void move( struct Object& ex );
//
void show( struct Object& ex );
//
void Timer( struct Timer& ex );
//
void start ( struct Timer& Sex );
//
void stop( struct Timer& ex );
//
void pause( struct Timer& ex );
//
void unpause( struct Timer& ex );
//
int get_ticks( struct Timer& ex );
//
bool is_started( struct Timer& ex );
//
bool is_paused( struct Timer& ex );
//
void Show_Boxhead ();
//
void collision ( struct Object& ex );
//
void set_camera( struct Object ex );
//
void Show_Zombie ( struct Object& ex );
//
void Zombie ( struct Object& ex );
//
void move_zombie ( struct Object& a, struct Object& b );
//
void Zombie_Movement ( struct Object& ex2, struct Object& ex );
//
void Contact ( struct Object& ex, struct Object& ex2 );
//
void First_Zombie_Show ( struct Object Z );
//
void SHOW_Health ( struct Object& ex );
//
void Shoot ( struct Object& b, struct Object& z );
//
void kill ( struct Object& z );
//
int bob_sort ( int* d );
//
void CHECK_END_GAME ( struct Object ex, bool& quit );
//
long double dis ( struct Object a, struct Object b );


// Demolation
 struct Demolation 
{
  int top;
  int bottom;
  int left;
  int right;
};
//The stick figure
 struct Object
{ 
    //The offset
    int offSetX;
    //The offset
    int offSetY;
    //Its rate of movement
    int velocityX;
    //Its rate of movement
    int velocityY;
    //Its current frame
    int frame;
    //Its animation status
    int status;
    // Its Health
    int health;
    //Zombies type
    int TZ;
    //
    int type;
    //
    int situation;
    
};

//The timer
 struct Timer
{
    //The clock time when the timer started
    int startTicks;
    //The ticks stored when the timer was paused
    int pausedTicks;
    //The timer status
    bool paused;
    bool started;
};


#endif
