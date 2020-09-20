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

int SCREEN_WIDE = 640;
int SCREEN_HIGH = 480;
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
const int ZMU = 1;
//
const int boxy=0, zomby=1;
const int dead=0, live=1;

//The areas of the sprite sheet
const int RIGHT=0;
const int LEFT=1;
const int UP=2;
const int DOWN=3;
const int RIGHT_UP=4;
const int LEFT_UP=5;
const int RIGHT_DOWN=6;
const int LEFT_DOWN=7;

// Global variables
vector < SDL_Rect > sample;
vector < vector< SDL_Rect > > cp ( image_number, sample ); // image's Clips

SDL_Surface* images[ image_number ]; // images
SDL_Surface* screen = NULL; // Show Window
SDL_Surface* Score = NULL;
SDL_Surface* END = NULL;
SDL_Surface* BoxHead_health = NULL;

SDL_Rect camera = { 0, 0, SCREEN_WIDE, SCREEN_HIGH };

//The event structure
SDL_Event event;

TTF_Font* font = NULL;
TTF_Font* font_end= NULL;

SDL_Color textcolor = { 255, 255, 0 };
SDL_Color end_color = { 255, 0, 0 };

//The frames per second
const int FRAMES_PER_SECOND = 20;

//The dimenstions of the stick figure
const int BOX_WIDTH = 20;
const int BOX_HEIGHT = 20;


//BoxHead clips [ status ][ frames ]
const int Walk=0; const int Pistol=1; const int MechinGun=2; const int ShotGun=3; const int UZGun=4; const int Launcher=5; const int WhiteZombie=6; const int RedZombie=7;
SDL_Rect clips[ 8 ][ 8 ][ 5 ];


// key states
Uint8 *keystates = SDL_GetKeyState( NULL );
// width height game
int width=0; int height=0;
// variables situation baraye inke 2bare chap nashan
bool BoxHead_Situation=false; 
// game input
char** INPUT;
//total score of game
long int SCORE=1000;
// Game show screen size
int LEVEL_WIDTH=0,LEVEL_HEIGHT=0;
// Bullet
bool bullet=false;
//
int zombie_no=0;



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






int main ( int argc, char* args [] )
{
  int zombie_number = rand () %50 + 1; 
  zombie_no = zombie_number;
  
  Init ();
  loading_files ();
  set_clips ();
  INPUT = input();
  
  //Game_Loading ();
  Create_Map ( INPUT );
  //apply_surface ( 0, 0, screen, screen, &camera );
  camera.w = SCREEN_WIDE;
  camera.h = SCREEN_HIGH;
  
  
  bool quit = false;
  
  //The frame rate regulator
  struct Timer fps;
  Timer ( fps );

  //The stick figure
  struct Object walk;
  BOXHEAD ( walk );
  
  struct Object Z[zombie_number];
  for ( int i=0; i<zombie_number; ++i )
  {
    Zombie ( Z[ i ] );
    First_Zombie_Show ( Z[ i ] );
  }
  
  int ds [ zombie_number ];
  //cerr << "X : " << ex2.offSetX << "  Y : " << ex2.offSetY << endl;
	
  
 
  Show_Boxhead ();
    
  
  while  ( quit != true ) {
    
    while ( SDL_PollEvent ( &event )  ) {
      
      //Handle events for the stick figure
      keystates = SDL_GetKeyState( NULL );
      handle_events ( walk );
      
      if  ( event.type == SDL_QUIT )
	quit = true;
    }
    
    BoxHead_Movment ( walk );
    
    for ( int i=0; i<zombie_number; ++i )
    {
      ds [ i ] = dis ( walk, Z[ i ] );
      
      Zombie_Movement ( Z[ i ], walk );
    
      Contact ( walk,Z[ i ] );
    }
    
    int min = bob_sort ( ds );
    struct Object near;
    
    for(int i=0; i<zombie_number; ++i){
	if ( min == dis ( walk, Z[i] ) ){
	  near.offSetX = Z[i].offSetX; near.offSetY = Z[i].offSetY; near.velocityX = Z[i].velocityX; near.velocityY = Z[i].velocityY;
	  near.frame = Z[i].frame; near.status = Z[i].status; near.health = Z[i].health; near.TZ = Z[i].TZ; near.type = Z[i].type; near.situation = Z[i].situation; }
    }
   
    if ( bullet != false ){
     
      for ( int i=0; i<zombie_number; ++i )
    {
      Shoot ( walk, Z[i] );}
    }
   
    
    
    for ( int i=0; i<zombie_number; ++i ) {
	for ( int j=0; j<zombie_number; ++j ) {
         if ( i!=j )
	   Contact ( Z[ j ],Z[ i ] );
      }
    }
    
    
         
    SHOW_Health ( walk );
    CHECK_END_GAME ( walk, quit );
  
    if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

    //Cap the frame rate
    if( get_ticks( fps ) < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - get_ticks( fps ) );
        }
  }
    
  clean_up ();
  //cerr << "6666" << endl;
}













// getting input
char** input () {

  cin >> width >> height;
  LEVEL_WIDTH = width*40;
  LEVEL_HEIGHT = height*40;
  
  if ( LEVEL_HEIGHT < SCREEN_HIGH )
    SCREEN_HIGH = LEVEL_HEIGHT;
  
  if ( LEVEL_WIDTH < SCREEN_WIDE )
    SCREEN_WIDE = LEVEL_WIDTH;
  
  
  char **inp = new char*[width];
    for (int i=0; i<width; i++)
      inp[i] = new char[height];
	
  for ( int row=0;row<height; ++row ) {
    for ( int col=0; col<width; ++col ) {
      cin >> inp [ row ][ col ];  // in [ satr ] [ sotoun ]
    }
  }
  
  return inp;
}

// image format correction
SDL_Surface* load_image (  string filename ) {
    
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
	//Free the old surface
        SDL_FreeSurface( loadedImage );
    }
    //Return the optimized surface
    return optimizedImage;
}

// apply image on the screen
void apply_surface ( int x, int y, SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip = NULL ) {
  
  SDL_Rect offset;
  
  offset.x = x;
  offset.y = y;
  
  SDL_BlitSurface ( src, clip, des, &offset );
}


void clean_up () {
  
  for (int i=0; i<image_number; i++)
    SDL_FreeSurface ( images [i] );
  
  SDL_FreeSurface ( Score );
  
  TTF_CloseFont ( font );
  TTF_Quit ();
  
  SDL_Quit ();
}
  

// Loading
void Game_Loading () {
  
    apply_surface ( 0, 0, images[ Start ], screen, &cp[ Start ][ 0 ] );
    SDL_Flip ( screen );
    SDL_Delay (3000);
    SDL_FillRect ( screen, &screen->clip_rect, SDL_MapRGB ( screen->format, 0, 0, 0 ) ); //Fill the screen white
  }
  
  
// initialization
bool Init () {
  
  if ( SDL_Init ( SDL_INIT_EVERYTHING ) == -1 )
    return false;
  
  screen = SDL_SetVideoMode ( SCREEN_WIDE, SCREEN_HIGH, SCREEN_BPP, SDL_SWSURFACE ); 
  //SDL_FillRect ( screen, &screen->clip_rect, SDL_MapRGB ( screen->format, 0xFF, 0xFF, 0xFF ) ); //Fill the screen white
  
  if  ( screen == NULL ) 
    return false;
  
  if ( TTF_Init() == -1 ) 
    return false;
  
  SDL_WM_SetCaption ( "BoxHead The Zombie Wars", NULL );
}

  
// loading images
void loading_files () {

  images [ Ground1 ] = load_image ( "ground.bmp" );
  images [ Ground2 ] = load_image ( "image 676.png" );
  images [ Boxhead ] = load_image ( "player.png" );
  images [ Turrentmachine ] = load_image ( "turrentMachine.png" );
  images [ Barricades ] = load_image ( "barricades.png" );
  images [ WZombie ] = load_image ( "zombieWhite.png" );
  images [ RZombie ] = load_image ( "zombieRed.png" );
  images [ Plus ] = load_image ( "plus.png" );
  images [ Barrel ] = load_image ( "barrel.png" );
  images [ Start ] = load_image ( "start.jpg" );

  font = TTF_OpenFont ( "lazy.ttf", 20 );
  font_end = TTF_OpenFont ( "lazy.ttf", 30 );
  
}


// sets image's clips
void set_clips () {
  
  SDL_Rect ex;
  
  // Ground 1 cp
  ex.x = 280 ; ex.y = 81 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][0] 1
  ex.x = 40 ; ex.y = 161 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][1] 2
  ex.x = 120 ; ex.y = 161 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][2] 3
  ex.x = 200 ; ex.y = 161 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][3] 4
  ex.x = 120 ; ex.y = 81 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][4] 5
  ex.x = 120 ; ex.y = 241 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][5] 6
  ex.x = 280 ; ex.y = 161 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][6] 7
  ex.x = 280 ; ex.y = 241 ; ex.w = 40 ; ex.h = 40 ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][7] 8
  /*ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][8]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][9]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][10]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][11]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][12]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground1 ].push_back ( ex ); // Clips [Ground1][13]
  */
  /*
  // Ground 2 cp
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][0]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][1]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][2]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][3]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][4]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][5]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][6]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][7]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][8]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][9]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][10]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][11]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][12]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][13]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][14]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][15]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][16]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Ground2 ].push_back ( ex ); // Clips [Ground2][17]
  */
  
  // Boxhead clips
  
  // .................walk....................//
  //Clip the sprites RIGHT boxhead
    clips [ Walk ][ RIGHT ][ 0 ].x = 554; clips [ Walk ][ RIGHT ][ 0 ].y = 523; clips [ Walk ][ RIGHT ][ 0 ].w = 15; clips [ Walk ][ RIGHT ][ 0 ].h = 45;
    clips [ Walk ][ RIGHT ][ 1 ].x = 571; clips [ Walk ][ RIGHT ][ 1 ].y = 523; clips [ Walk ][ RIGHT ][ 1 ].w = 15; clips [ Walk ][ RIGHT ][ 1 ].h = 45;
    clips [ Walk ][ RIGHT ][ 2 ].x = 590; clips [ Walk ][ RIGHT ][ 2 ].y = 522; clips [ Walk ][ RIGHT ][ 2 ].w = 16; clips [ Walk ][ RIGHT ][ 2 ].h = 48;
    clips [ Walk ][ RIGHT ][ 3 ].x = 608; clips [ Walk ][ RIGHT ][ 3 ].y = 523; clips [ Walk ][ RIGHT ][ 3 ].w = 16; clips [ Walk ][ RIGHT ][ 3 ].h = 46;
    clips [ Walk ][ RIGHT ][ 4 ].x = 626; clips [ Walk ][ RIGHT ][ 4 ].y = 522; clips [ Walk ][ RIGHT ][ 4 ].w = 16; clips [ Walk ][ RIGHT ][ 4 ].h = 46;
    clips [ Walk ][ RIGHT ][ 5 ].x = 644; clips [ Walk ][ RIGHT ][ 5 ].y = 522; clips [ Walk ][ RIGHT ][ 5 ].w = 16; clips [ Walk ][ RIGHT ][ 5 ].h = 46;
    clips [ Walk ][ RIGHT ][ 6 ].x = 514; clips [ Walk ][ RIGHT ][ 6 ].y = 522; clips [ Walk ][ RIGHT ][ 6 ].w = 20; clips [ Walk ][ RIGHT ][ 6 ].h = 46;
    clips [ Walk ][ RIGHT ][ 7 ].x = 536; clips [ Walk ][ RIGHT ][ 7 ].y = 522; clips [ Walk ][ RIGHT ][ 7 ].w = 15; clips [ Walk ][ RIGHT ][ 7 ].h = 45;

    //Clip the sprites LEFT boxhead
    clips [ Walk ][ LEFT ][ 0 ].x = 597; clips [ Walk ][ LEFT ][ 0 ].y = 581; clips [ Walk ][ LEFT ][ 0 ].w = 16; clips [ Walk ][ LEFT ][ 0 ].h = 45;
    clips [ Walk ][ LEFT ][ 1 ].x = 578; clips [ Walk ][ LEFT ][ 1 ].y = 581; clips [ Walk ][ LEFT ][ 1 ].w = 16; clips [ Walk ][ LEFT ][ 1 ].h = 45;
    clips [ Walk ][ LEFT ][ 2 ].x = 560; clips [ Walk ][ LEFT ][ 2 ].y = 581; clips [ Walk ][ LEFT ][ 2 ].w = 16; clips [ Walk ][ LEFT ][ 2 ].h = 45;
    clips [ Walk ][ LEFT ][ 3 ].x = 541; clips [ Walk ][ LEFT ][ 3 ].y = 581; clips [ Walk ][ LEFT ][ 3 ].w = 16; clips [ Walk ][ LEFT ][ 3 ].h = 45;
    clips [ Walk ][ LEFT ][ 4 ].x = 524; clips [ Walk ][ LEFT ][ 4 ].y = 581; clips [ Walk ][ LEFT ][ 4 ].w = 16; clips [ Walk ][ LEFT ][ 4 ].h = 45;
    clips [ Walk ][ LEFT ][ 5 ].x = 507; clips [ Walk ][ LEFT ][ 5 ].y = 580; clips [ Walk ][ LEFT ][ 5 ].w = 16; clips [ Walk ][ LEFT ][ 5 ].h = 45;
    clips [ Walk ][ LEFT ][ 6 ].x = 486; clips [ Walk ][ LEFT ][ 6 ].y = 581; clips [ Walk ][ LEFT ][ 6 ].w = 20; clips [ Walk ][ LEFT ][ 6 ].h = 45;
    clips [ Walk ][ LEFT ][ 7 ].x = 615; clips [ Walk ][ LEFT ][ 7 ].y = 582; clips [ Walk ][ LEFT ][ 7 ].w = 16; clips [ Walk ][ LEFT ][ 7 ].h = 45;
    
    //Clip the sprites UP boxhead
    clips [ Walk ][ UP ][ 0 ].x = 104; clips [ Walk ][ UP ][ 0 ].y = 637; clips [ Walk ][ UP ][ 0 ].w = 25; clips [ Walk ][ UP ][ 0 ].h = 46;
    clips [ Walk ][ UP ][ 1 ].x = 131; clips [ Walk ][ UP ][ 1 ].y = 638; clips [ Walk ][ UP ][ 1 ].w = 25; clips [ Walk ][ UP ][ 1 ].h = 45;
    clips [ Walk ][ UP ][ 2 ].x = 158; clips [ Walk ][ UP ][ 2 ].y = 638; clips [ Walk ][ UP ][ 2 ].w = 25; clips [ Walk ][ UP ][ 2 ].h = 45;
    clips [ Walk ][ UP ][ 3 ].x = 187; clips [ Walk ][ UP ][ 3 ].y = 638; clips [ Walk ][ UP ][ 3 ].w = 25; clips [ Walk ][ UP ][ 3 ].h = 45;
    clips [ Walk ][ UP ][ 4 ].x = 214; clips [ Walk ][ UP ][ 4 ].y = 637; clips [ Walk ][ UP ][ 4 ].w = 25; clips [ Walk ][ UP ][ 4 ].h = 45;
    clips [ Walk ][ UP ][ 5 ].x = 239; clips [ Walk ][ UP ][ 5 ].y = 636; clips [ Walk ][ UP ][ 5 ].w = 25; clips [ Walk ][ UP ][ 5 ].h = 45;
    clips [ Walk ][ UP ][ 6 ].x = 54; clips [ Walk ][ UP ][ 6 ].y = 638; clips [ Walk ][ UP ][ 6 ].w = 25; clips [ Walk ][ UP ][ 6 ].h = 46;
    clips [ Walk ][ UP ][ 7 ].x = 79; clips [ Walk ][ UP ][ 7 ].y = 636; clips [ Walk ][ UP ][ 7 ].w = 25; clips [ Walk ][ UP ][ 7 ].h = 46;
    
    //Clip the sprites DOWN boxhead
    clips [ Walk ][ DOWN ][ 0 ].x = 129; clips [ Walk ][ DOWN ][ 0 ].y = 580; clips [ Walk ][ DOWN ][ 0 ].w = 23; clips [ Walk ][ DOWN ][ 0 ].h = 43;
    clips [ Walk ][ DOWN ][ 1 ].x = 157; clips [ Walk ][ DOWN ][ 1 ].y = 581; clips [ Walk ][ DOWN ][ 1 ].w = 23; clips [ Walk ][ DOWN ][ 1 ].h = 43;
    clips [ Walk ][ DOWN ][ 2 ].x = 182; clips [ Walk ][ DOWN ][ 2 ].y = 581; clips [ Walk ][ DOWN ][ 2 ].w = 25; clips [ Walk ][ DOWN ][ 2 ].h = 43;
    clips [ Walk ][ DOWN ][ 3 ].x = 210; clips [ Walk ][ DOWN ][ 3 ].y = 581; clips [ Walk ][ DOWN ][ 3 ].w = 25; clips [ Walk ][ DOWN ][ 3 ].h = 43;
    clips [ Walk ][ DOWN ][ 4 ].x = 237; clips [ Walk ][ DOWN ][ 4 ].y = 580; clips [ Walk ][ DOWN ][ 4 ].w = 24; clips [ Walk ][ DOWN ][ 4 ].h = 43;
    clips [ Walk ][ DOWN ][ 5 ].x = 263; clips [ Walk ][ DOWN ][ 5 ].y = 579; clips [ Walk ][ DOWN ][ 5 ].w = 24; clips [ Walk ][ DOWN ][ 5 ].h = 43;
    clips [ Walk ][ DOWN ][ 6 ].x = 78; clips [ Walk ][ DOWN ][ 6 ].y = 580; clips [ Walk ][ DOWN ][ 6 ].w = 23; clips [ Walk ][ DOWN ][ 6 ].h = 43;
    clips [ Walk ][ DOWN ][ 7 ].x = 104; clips [ Walk ][ DOWN ][ 7 ].y = 580; clips [ Walk ][ DOWN ][ 7 ].w = 23; clips [ Walk ][ DOWN ][ 7 ].h = 43;
    
    //Clip the sprites RIGHT_UP boxhead
    clips [ Walk ][ RIGHT_UP ][ 0 ].x = 311; clips [ Walk ][ RIGHT_UP ][ 0 ].y = 639; clips [ Walk ][ RIGHT_UP ][ 0 ].w = 21; clips [ Walk ][ RIGHT_UP ][ 0 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 1 ].x = 332; clips [ Walk ][ RIGHT_UP ][ 1 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 1 ].w = 25; clips [ Walk ][ RIGHT_UP ][ 1 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 2 ].x = 360; clips [ Walk ][ RIGHT_UP ][ 2 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 2 ].w = 29; clips [ Walk ][ RIGHT_UP ][ 2 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 3 ].x = 389; clips [ Walk ][ RIGHT_UP ][ 3 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 3 ].w = 25; clips [ Walk ][ RIGHT_UP ][ 3 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 4 ].x = 415; clips [ Walk ][ RIGHT_UP ][ 4 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 4 ].w = 22; clips [ Walk ][ RIGHT_UP ][ 4 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 5 ].x = 438; clips [ Walk ][ RIGHT_UP ][ 5 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 5 ].w = 22; clips [ Walk ][ RIGHT_UP ][ 5 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 6 ].x = 265; clips [ Walk ][ RIGHT_UP ][ 6 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 6 ].w = 22; clips [ Walk ][ RIGHT_UP ][ 6 ].h = 47;
    clips [ Walk ][ RIGHT_UP ][ 7 ].x = 288; clips [ Walk ][ RIGHT_UP ][ 7 ].y = 638; clips [ Walk ][ RIGHT_UP ][ 7 ].w = 22; clips [ Walk ][ RIGHT_UP ][ 7 ].h = 47;

    //Clip the sprites LEFT_UP boxhead
    clips [ Walk ][ LEFT_UP ][ 0 ].x = 691; clips [ Walk ][ LEFT_UP ][ 0 ].y = 581; clips [ Walk ][ LEFT_UP ][ 0 ].w = 24; clips [ Walk ][ LEFT_UP ][ 0 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 1 ].x = 716; clips [ Walk ][ LEFT_UP ][ 1 ].y = 581; clips [ Walk ][ LEFT_UP ][ 1 ].w = 24; clips [ Walk ][ LEFT_UP ][ 1 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 2 ].x = 740; clips [ Walk ][ LEFT_UP ][ 2 ].y = 581; clips [ Walk ][ LEFT_UP ][ 2 ].w = 24; clips [ Walk ][ LEFT_UP ][ 2 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 3 ].x = 765; clips [ Walk ][ LEFT_UP ][ 3 ].y = 581; clips [ Walk ][ LEFT_UP ][ 3 ].w = 24; clips [ Walk ][ LEFT_UP ][ 3 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 4 ].x = 0; clips [ Walk ][ LEFT_UP ][ 4 ].y = 639; clips [ Walk ][ LEFT_UP ][ 4 ].w = 24; clips [ Walk ][ LEFT_UP ][ 4 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 5 ].x = 27; clips [ Walk ][ LEFT_UP ][ 5 ].y = 638; clips [ Walk ][ LEFT_UP ][ 5 ].w = 26; clips [ Walk ][ LEFT_UP ][ 5 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 6 ].x = 633; clips [ Walk ][ LEFT_UP ][ 6 ].y = 581; clips [ Walk ][ LEFT_UP ][ 6 ].w = 29; clips [ Walk ][ LEFT_UP ][ 6 ].h = 46;
    clips [ Walk ][ LEFT_UP ][ 7 ].x = 664; clips [ Walk ][ LEFT_UP ][ 7 ].y = 581; clips [ Walk ][ LEFT_UP ][ 7 ].w = 27; clips [ Walk ][ LEFT_UP ][ 7 ].h = 46;
    
    //Clip the sprites RIGHT_DOWN boxhead
    clips [ Walk ][ RIGHT_DOWN ][ 0 ].x = 722; clips [ Walk ][ RIGHT_DOWN ][ 0 ].y = 523; clips [ Walk ][ RIGHT_DOWN ][ 0 ].w = 21; clips [ Walk ][ RIGHT_DOWN ][ 0 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 1 ].x = 747; clips [ Walk ][ RIGHT_DOWN ][ 1 ].y = 523; clips [ Walk ][ RIGHT_DOWN ][ 1 ].w = 21; clips [ Walk ][ RIGHT_DOWN ][ 1 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 2 ].x = 770; clips [ Walk ][ RIGHT_DOWN ][ 2 ].y = 522; clips [ Walk ][ RIGHT_DOWN ][ 2 ].w = 22; clips [ Walk ][ RIGHT_DOWN ][ 2 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 3 ].x = 1; clips [ Walk ][ RIGHT_DOWN ][ 3 ].y = 581; clips [ Walk ][ RIGHT_DOWN ][ 3 ].w = 21; clips [ Walk ][ RIGHT_DOWN ][ 3 ].h = 47;
    clips [ Walk ][ RIGHT_DOWN ][ 4 ].x = 26; clips [ Walk ][ RIGHT_DOWN ][ 4 ].y = 580; clips [ Walk ][ RIGHT_DOWN ][ 4 ].w = 21; clips [ Walk ][ RIGHT_DOWN ][ 4 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 5 ].x = 51; clips [ Walk ][ RIGHT_DOWN ][ 5 ].y = 581; clips [ Walk ][ RIGHT_DOWN ][ 5 ].w = 25; clips [ Walk ][ RIGHT_DOWN ][ 5 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 6 ].x = 662; clips [ Walk ][ RIGHT_DOWN ][ 6 ].y = 522; clips [ Walk ][ RIGHT_DOWN ][ 6 ].w = 29; clips [ Walk ][ RIGHT_DOWN ][ 6 ].h = 45;
    clips [ Walk ][ RIGHT_DOWN ][ 7 ].x = 693; clips [ Walk ][ RIGHT_DOWN ][ 7 ].y = 523; clips [ Walk ][ RIGHT_DOWN ][ 7 ].w = 26; clips [ Walk ][ RIGHT_DOWN ][ 7 ].h = 45;
    
    //Clip the sprites LEFT_DOWN boxhead
    clips [ Walk ][ LEFT_DOWN ][ 0 ].x = 335; clips [ Walk ][ LEFT_DOWN ][ 0 ].y = 581; clips [ Walk ][ LEFT_DOWN ][ 0 ].w = 21; clips [ Walk ][ LEFT_DOWN ][ 0 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 1 ].x = 357; clips [ Walk ][ LEFT_DOWN ][ 1 ].y = 580; clips [ Walk ][ LEFT_DOWN ][ 1 ].w = 23; clips [ Walk ][ LEFT_DOWN ][ 1 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 2 ].x = 384; clips [ Walk ][ LEFT_DOWN ][ 2 ].y = 582; clips [ Walk ][ LEFT_DOWN ][ 2 ].w = 28; clips [ Walk ][ LEFT_DOWN ][ 2 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 3 ].x = 413; clips [ Walk ][ LEFT_DOWN ][ 3 ].y = 581; clips [ Walk ][ LEFT_DOWN ][ 3 ].w = 25; clips [ Walk ][ LEFT_DOWN ][ 3 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 4 ].x = 440; clips [ Walk ][ LEFT_DOWN ][ 4 ].y = 581; clips [ Walk ][ LEFT_DOWN ][ 4 ].w = 20; clips [ Walk ][ LEFT_DOWN ][ 4 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 5 ].x = 462; clips [ Walk ][ LEFT_DOWN ][ 5 ].y = 581; clips [ Walk ][ LEFT_DOWN ][ 5 ].w = 20; clips [ Walk ][ LEFT_DOWN ][ 5 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 6 ].x = 288; clips [ Walk ][ LEFT_DOWN ][ 6 ].y = 581; clips [ Walk ][ LEFT_DOWN ][ 6 ].w = 22; clips [ Walk ][ LEFT_DOWN ][ 6 ].h = 45;
    clips [ Walk ][ LEFT_DOWN ][ 7 ].x = 311; clips [ Walk ][ LEFT_DOWN ][ 7 ].y = 580; clips [ Walk ][ LEFT_DOWN ][ 7 ].w = 22; clips [ Walk ][ LEFT_DOWN ][ 7 ].h = 45;
  // .................walk....................//
    
    
    
  // .................mini....................//
  //Clip the sprites RIGHT boxhead
    clips [ Pistol ][ RIGHT ][ 0 ].x = 751; clips [ Pistol ][ RIGHT ][ 0 ].y = 346; clips [ Pistol ][ RIGHT ][ 0 ].w = 38; clips [ Pistol ][ RIGHT ][ 0 ].h = 49;
    clips [ Pistol ][ RIGHT ][ 1 ].x = 0; clips [ Pistol ][ RIGHT ][ 1 ].y = 402; clips [ Pistol ][ RIGHT ][ 1 ].w = 35; clips [ Pistol ][ RIGHT ][ 1 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 2 ].x = 33; clips [ Pistol ][ RIGHT ][ 2 ].y = 402; clips [ Pistol ][ RIGHT ][ 2 ].w = 35; clips [ Pistol ][ RIGHT ][ 2 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 3 ].x = 65; clips [ Pistol ][ RIGHT ][ 3 ].y = 402; clips [ Pistol ][ RIGHT ][ 3 ].w = 35; clips [ Pistol ][ RIGHT ][ 3 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 4 ].x = 95; clips [ Pistol ][ RIGHT ][ 4 ].y = 402; clips [ Pistol ][ RIGHT ][ 4 ].w = 35; clips [ Pistol ][ RIGHT ][ 4 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 5 ].x = 130; clips [ Pistol ][ RIGHT ][ 5 ].y = 402; clips [ Pistol ][ RIGHT ][ 5 ].w = 35; clips [ Pistol ][ RIGHT ][ 5 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 6 ].x = 163; clips [ Pistol ][ RIGHT ][ 6 ].y = 402; clips [ Pistol ][ RIGHT ][ 6 ].w = 35; clips [ Pistol ][ RIGHT ][ 6 ].h = 52;
    clips [ Pistol ][ RIGHT ][ 7 ].x = 194; clips [ Pistol ][ RIGHT ][ 7 ].y = 402; clips [ Pistol ][ RIGHT ][ 7 ].w = 35; clips [ Pistol ][ RIGHT ][ 7 ].h = 52;

    //Clip the sprites LEFT boxhead
    clips [ Pistol ][ LEFT ][ 0 ].x = 150; clips [ Pistol ][ LEFT ][ 0 ].y = 463; clips [ Pistol ][ LEFT ][ 0 ].w = 35; clips [ Pistol ][ LEFT ][ 0 ].h = 52;
    clips [ Pistol ][ LEFT ][ 1 ].x = 186; clips [ Pistol ][ LEFT ][ 1 ].y = 463; clips [ Pistol ][ LEFT ][ 1 ].w = 35; clips [ Pistol ][ LEFT ][ 1 ].h = 52;
    clips [ Pistol ][ LEFT ][ 2 ].x = 220; clips [ Pistol ][ LEFT ][ 2 ].y = 463; clips [ Pistol ][ LEFT ][ 2 ].w = 35; clips [ Pistol ][ LEFT ][ 2 ].h = 52;
    clips [ Pistol ][ LEFT ][ 3 ].x = 251; clips [ Pistol ][ LEFT ][ 3 ].y = 463; clips [ Pistol ][ LEFT ][ 3 ].w = 35; clips [ Pistol ][ LEFT ][ 3 ].h = 52;
    clips [ Pistol ][ LEFT ][ 4 ].x = 284; clips [ Pistol ][ LEFT ][ 4 ].y = 463; clips [ Pistol ][ LEFT ][ 4 ].w = 35; clips [ Pistol ][ LEFT ][ 4 ].h = 52;
    clips [ Pistol ][ LEFT ][ 5 ].x = 316; clips [ Pistol ][ LEFT ][ 5 ].y = 463; clips [ Pistol ][ LEFT ][ 5 ].w = 35; clips [ Pistol ][ LEFT ][ 5 ].h = 52;
    clips [ Pistol ][ LEFT ][ 6 ].x = 349; clips [ Pistol ][ LEFT ][ 6 ].y = 463; clips [ Pistol ][ LEFT ][ 6 ].w = 35; clips [ Pistol ][ LEFT ][ 6 ].h = 52;
    clips [ Pistol ][ LEFT ][ 7 ].x = 381; clips [ Pistol ][ LEFT ][ 7 ].y = 463; clips [ Pistol ][ LEFT ][ 7 ].w = 35; clips [ Pistol ][ LEFT ][ 7 ].h = 52;
    
    //Clip the sprites UP boxhead
    clips [ Pistol ][ UP ][ 0 ].x = 613; clips [ Pistol ][ UP ][ 0 ].y = 463; clips [ Pistol ][ UP ][ 0 ].w = 27; clips [ Pistol ][ UP ][ 0 ].h = 54;
    clips [ Pistol ][ UP ][ 1 ].x = 639; clips [ Pistol ][ UP ][ 1 ].y = 463; clips [ Pistol ][ UP ][ 1 ].w = 27; clips [ Pistol ][ UP ][ 1 ].h = 54;
    clips [ Pistol ][ UP ][ 2 ].x = 666; clips [ Pistol ][ UP ][ 2 ].y = 463; clips [ Pistol ][ UP ][ 2 ].w = 27; clips [ Pistol ][ UP ][ 2 ].h = 54;
    clips [ Pistol ][ UP ][ 3 ].x = 692; clips [ Pistol ][ UP ][ 3 ].y = 463; clips [ Pistol ][ UP ][ 3 ].w = 27; clips [ Pistol ][ UP ][ 3 ].h = 54;
    clips [ Pistol ][ UP ][ 4 ].x = 718; clips [ Pistol ][ UP ][ 4 ].y = 463; clips [ Pistol ][ UP ][ 4 ].w = 27; clips [ Pistol ][ UP ][ 4 ].h = 54;
    clips [ Pistol ][ UP ][ 5 ].x = 745; clips [ Pistol ][ UP ][ 5 ].y = 463; clips [ Pistol ][ UP ][ 5 ].w = 27; clips [ Pistol ][ UP ][ 5 ].h = 54;
    clips [ Pistol ][ UP ][ 6 ].x = 771; clips [ Pistol ][ UP ][ 6 ].y = 463; clips [ Pistol ][ UP ][ 6 ].w = 27; clips [ Pistol ][ UP ][ 6 ].h = 54;
    clips [ Pistol ][ UP ][ 7 ].x = 0; clips [ Pistol ][ UP ][ 7 ].y = 520; clips [ Pistol ][ UP ][ 7 ].w = 27; clips [ Pistol ][ UP ][ 7 ].h = 54;
    
    //Clip the sprites DOWN boxhead
    clips [ Pistol ][ DOWN ][ 0 ].x = 426; clips [ Pistol ][ DOWN ][ 0 ].y = 404; clips [ Pistol ][ DOWN ][ 0 ].w = 26; clips [ Pistol ][ DOWN ][ 0 ].h = 51;
    clips [ Pistol ][ DOWN ][ 1 ].x = 453; clips [ Pistol ][ DOWN ][ 1 ].y = 404; clips [ Pistol ][ DOWN ][ 1 ].w = 26; clips [ Pistol ][ DOWN ][ 1 ].h = 51;
    clips [ Pistol ][ DOWN ][ 2 ].x = 480; clips [ Pistol ][ DOWN ][ 2 ].y = 404; clips [ Pistol ][ DOWN ][ 2 ].w = 26; clips [ Pistol ][ DOWN ][ 2 ].h = 51;
    clips [ Pistol ][ DOWN ][ 3 ].x = 506; clips [ Pistol ][ DOWN ][ 3 ].y = 404; clips [ Pistol ][ DOWN ][ 3 ].w = 26; clips [ Pistol ][ DOWN ][ 3 ].h = 51;
    clips [ Pistol ][ DOWN ][ 4 ].x = 532; clips [ Pistol ][ DOWN ][ 4 ].y = 404; clips [ Pistol ][ DOWN ][ 4 ].w = 26; clips [ Pistol ][ DOWN ][ 4 ].h = 51;
    clips [ Pistol ][ DOWN ][ 5 ].x = 558; clips [ Pistol ][ DOWN ][ 5 ].y = 404; clips [ Pistol ][ DOWN ][ 5 ].w = 26; clips [ Pistol ][ DOWN ][ 5 ].h = 51;
    clips [ Pistol ][ DOWN ][ 6 ].x = 584; clips [ Pistol ][ DOWN ][ 6 ].y = 404; clips [ Pistol ][ DOWN ][ 6 ].w = 26; clips [ Pistol ][ DOWN ][ 6 ].h = 51;
    clips [ Pistol ][ DOWN ][ 7 ].x = 610; clips [ Pistol ][ DOWN ][ 7 ].y = 404; clips [ Pistol ][ DOWN ][ 7 ].w = 26; clips [ Pistol ][ DOWN ][ 7 ].h = 51;
    
    //Clip the sprites RIGHT_UP boxhead
    clips [ Pistol ][ RIGHT_UP ][ 0 ].x = 27; clips [ Pistol ][ RIGHT_UP ][ 0 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 0 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 0 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 1 ].x = 66; clips [ Pistol ][ RIGHT_UP ][ 1 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 1 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 1 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 2 ].x = 104; clips [ Pistol ][ RIGHT_UP ][ 2 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 2 ].w =38; clips [ Pistol ][ RIGHT_UP ][ 2 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 3 ].x = 141; clips [ Pistol ][ RIGHT_UP ][ 3 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 3 ].w =38; clips [ Pistol ][ RIGHT_UP ][ 3 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 4 ].x = 178; clips [ Pistol ][ RIGHT_UP ][ 4 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 4 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 4 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 5 ].x = 216; clips [ Pistol ][ RIGHT_UP ][ 5 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 5 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 5 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 6 ].x = 254; clips [ Pistol ][ RIGHT_UP ][ 6 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 6 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 6 ].h = 54;
    clips [ Pistol ][ RIGHT_UP ][ 7 ].x = 292; clips [ Pistol ][ RIGHT_UP ][ 7 ].y = 520; clips [ Pistol ][ RIGHT_UP ][ 7 ].w = 38; clips [ Pistol ][ RIGHT_UP ][ 7 ].h = 54;

    //Clip the sprites LEFT_UP boxhead
    clips [ Pistol ][ LEFT_UP ][ 0 ].x = 413; clips [ Pistol ][ LEFT_UP ][ 0 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 0 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 0 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 1 ].x = 439; clips [ Pistol ][ LEFT_UP ][ 1 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 1 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 1 ].h =54;
    clips [ Pistol ][ LEFT_UP ][ 2 ].x = 463; clips [ Pistol ][ LEFT_UP ][ 2 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 2 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 2 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 3 ].x = 488; clips [ Pistol ][ LEFT_UP ][ 3 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 3 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 3 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 4 ].x = 513; clips [ Pistol ][ LEFT_UP ][ 4 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 4 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 4 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 5 ].x = 539; clips [ Pistol ][ LEFT_UP ][ 5 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 5 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 5 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 6 ].x = 504; clips [ Pistol ][ LEFT_UP ][ 6 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 6 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 6 ].h = 54;
    clips [ Pistol ][ LEFT_UP ][ 7 ].x = 588; clips [ Pistol ][ LEFT_UP ][ 7 ].y = 463; clips [ Pistol ][ LEFT_UP ][ 7 ].w = 26; clips [ Pistol ][ LEFT_UP ][ 7 ].h = 54;
    
    //Clip the sprites RIGHT_DOWN boxhead
    clips [ Pistol ][ RIGHT_DOWN ][ 0 ].x = 193; clips [ Pistol ][ RIGHT_DOWN ][ 0 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 0 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 0 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 1 ].x = 226; clips [ Pistol ][ RIGHT_DOWN ][ 1 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 1 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 1 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 2 ].x = 251; clips [ Pistol ][ RIGHT_DOWN ][ 2 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 2 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 2 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 3 ].x = 276; clips [ Pistol ][ RIGHT_DOWN ][ 3 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 3 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 3 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 4 ].x = 300; clips [ Pistol ][ RIGHT_DOWN ][ 4 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 4 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 4 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 5 ].x = 326; clips [ Pistol ][ RIGHT_DOWN ][ 5 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 5 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 5 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 6 ].x = 353; clips [ Pistol ][ RIGHT_DOWN ][ 6 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 6 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 6 ].h = 51;
    clips [ Pistol ][ RIGHT_DOWN ][ 7 ].x = 377; clips [ Pistol ][ RIGHT_DOWN ][ 7 ].y = 402; clips [ Pistol ][ RIGHT_DOWN ][ 7 ].w = 26; clips [ Pistol ][ RIGHT_DOWN ][ 7 ].h = 51;
    
    //Clip the sprites LEFT_DOWN boxhead
    clips [ Pistol ][ LEFT_DOWN ][ 0 ].x = 636; clips [ Pistol ][ LEFT_DOWN ][ 0 ].y = 404; clips [ Pistol ][ LEFT_DOWN ][ 0 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 0 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 1 ].x = 674; clips [ Pistol ][ LEFT_DOWN ][ 1 ].y = 404; clips [ Pistol ][ LEFT_DOWN ][ 1 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 1 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 2 ].x = 713; clips [ Pistol ][ LEFT_DOWN ][ 2 ].y = 404; clips [ Pistol ][ LEFT_DOWN ][ 2 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 2 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 3 ].x = 751; clips [ Pistol ][ LEFT_DOWN ][ 3 ].y = 404; clips [ Pistol ][ LEFT_DOWN ][ 3 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 3 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 4 ].x = 0; clips [ Pistol ][ LEFT_DOWN ][ 4 ].y = 463; clips [ Pistol ][ LEFT_DOWN ][ 4 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 4 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 5 ].x = 38; clips [ Pistol ][ LEFT_DOWN ][ 5 ].y = 463; clips [ Pistol ][ LEFT_DOWN ][ 5 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 5 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 6 ].x = 76; clips [ Pistol ][ LEFT_DOWN ][ 6 ].y = 463; clips [ Pistol ][ LEFT_DOWN ][ 6 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 6 ].h = 51;
    clips [ Pistol ][ LEFT_DOWN ][ 7 ].x = 113; clips [ Pistol ][ LEFT_DOWN ][ 7 ].y = 463; clips [ Pistol ][ LEFT_DOWN ][ 7 ].w = 38; clips [ Pistol ][ LEFT_DOWN ][ 7 ].h = 51;
  // .................mini....................//
    
    
  // .................MechinGun....................//
  //Clip the sprites RIGHT boxhead
    clips [ MechinGun ][ RIGHT ][ 0 ].x = 83; clips [ MechinGun ][ RIGHT ][ 0 ].y = 175; clips [ MechinGun ][ RIGHT ][ 0 ].w = 43; clips [ MechinGun ][ RIGHT ][ 0 ].h = 48;
    clips [ MechinGun ][ RIGHT ][ 1 ].x = 128; clips [ MechinGun ][ RIGHT ][ 1 ].y = 175; clips [ MechinGun ][ RIGHT ][ 1 ].w = 42; clips [ MechinGun ][ RIGHT ][ 1 ].h = 45;
    clips [ MechinGun ][ RIGHT ][ 2 ].x = 172; clips [ MechinGun ][ RIGHT ][ 2 ].y = 175; clips [ MechinGun ][ RIGHT ][ 2 ].w = 41; clips [ MechinGun ][ RIGHT ][ 2 ].h = 44;
    clips [ MechinGun ][ RIGHT ][ 3 ].x = 215; clips [ MechinGun ][ RIGHT ][ 3 ].y = 175; clips [ MechinGun ][ RIGHT ][ 3 ].w = 42; clips [ MechinGun ][ RIGHT ][ 3 ].h = 46;
    clips [ MechinGun ][ RIGHT ][ 4 ].x = 260; clips [ MechinGun ][ RIGHT ][ 4 ].y = 175; clips [ MechinGun ][ RIGHT ][ 4 ].w = 43; clips [ MechinGun ][ RIGHT ][ 4 ].h = 47;
    clips [ MechinGun ][ RIGHT ][ 5 ].x = 305; clips [ MechinGun ][ RIGHT ][ 5 ].y = 175; clips [ MechinGun ][ RIGHT ][ 5 ].w = 42; clips [ MechinGun ][ RIGHT ][ 5 ].h = 45;
    clips [ MechinGun ][ RIGHT ][ 6 ].x = 349; clips [ MechinGun ][ RIGHT ][ 6 ].y = 175; clips [ MechinGun ][ RIGHT ][ 6 ].w = 41; clips [ MechinGun ][ RIGHT ][ 6 ].h = 45;
    clips [ MechinGun ][ RIGHT ][ 7 ].x = 392; clips [ MechinGun ][ RIGHT ][ 7 ].y = 175; clips [ MechinGun ][ RIGHT ][ 7 ].w = 42; clips [ MechinGun ][ RIGHT ][ 7 ].h = 45;

    //Clip the sprites LEFT boxhead
    clips [ MechinGun ][ LEFT ][ 0 ].x = 494; clips [ MechinGun ][ LEFT ][ 0 ].y = 233; clips [ MechinGun ][ LEFT ][ 0 ].w = 44; clips [ MechinGun ][ LEFT ][ 0 ].h = 47;
    clips [ MechinGun ][ LEFT ][ 1 ].x = 540; clips [ MechinGun ][ LEFT ][ 1 ].y = 233; clips [ MechinGun ][ LEFT ][ 1 ].w = 42; clips [ MechinGun ][ LEFT ][ 1 ].h = 45;
    clips [ MechinGun ][ LEFT ][ 2 ].x = 585; clips [ MechinGun ][ LEFT ][ 2 ].y = 233; clips [ MechinGun ][ LEFT ][ 2 ].w = 41; clips [ MechinGun ][ LEFT ][ 2 ].h = 44;
    clips [ MechinGun ][ LEFT ][ 3 ].x = 628; clips [ MechinGun ][ LEFT ][ 3 ].y = 233; clips [ MechinGun ][ LEFT ][ 3 ].w = 42; clips [ MechinGun ][ LEFT ][ 3 ].h = 45;
    clips [ MechinGun ][ LEFT ][ 4 ].x = 672; clips [ MechinGun ][ LEFT ][ 4 ].y = 233; clips [ MechinGun ][ LEFT ][ 4 ].w = 44; clips [ MechinGun ][ LEFT ][ 4 ].h = 46;
    clips [ MechinGun ][ LEFT ][ 5 ].x = 718; clips [ MechinGun ][ LEFT ][ 5 ].y = 233; clips [ MechinGun ][ LEFT ][ 5 ].w = 42; clips [ MechinGun ][ LEFT ][ 5 ].h = 45;
    clips [ MechinGun ][ LEFT ][ 6 ].x = 2; clips [ MechinGun ][ LEFT ][ 6 ].y = 233; clips [ MechinGun ][ LEFT ][ 6 ].w = 41; clips [ MechinGun ][ LEFT ][ 6 ].h = 45;
    clips [ MechinGun ][ LEFT ][ 7 ].x = 44; clips [ MechinGun ][ LEFT ][ 7 ].y = 233; clips [ MechinGun ][ LEFT ][ 7 ].w = 42; clips [ MechinGun ][ LEFT ][ 7 ].h = 45;
    
    //Clip the sprites UP boxhead
    clips [ MechinGun ][ UP ][ 0 ].x = 393; clips [ MechinGun ][ UP ][ 0 ].y = 291; clips [ MechinGun ][ UP ][ 0 ].w = 23; clips [ MechinGun ][ UP ][ 0 ].h = 49;
    clips [ MechinGun ][ UP ][ 1 ].x = 419; clips [ MechinGun ][ UP ][ 1 ].y = 291; clips [ MechinGun ][ UP ][ 1 ].w = 22; clips [ MechinGun ][ UP ][ 1 ].h = 48;
    clips [ MechinGun ][ UP ][ 2 ].x = 444; clips [ MechinGun ][ UP ][ 2 ].y = 291; clips [ MechinGun ][ UP ][ 2 ].w = 22; clips [ MechinGun ][ UP ][ 2 ].h = 46;
    clips [ MechinGun ][ UP ][ 3 ].x = 468; clips [ MechinGun ][ UP ][ 3 ].y = 291; clips [ MechinGun ][ UP ][ 3 ].w = 24; clips [ MechinGun ][ UP ][ 3 ].h = 47;
    clips [ MechinGun ][ UP ][ 4 ].x = 494; clips [ MechinGun ][ UP ][ 4 ].y = 291; clips [ MechinGun ][ UP ][ 4 ].w = 24; clips [ MechinGun ][ UP ][ 4 ].h = 50;
    clips [ MechinGun ][ UP ][ 5 ].x = 520; clips [ MechinGun ][ UP ][ 5 ].y = 291; clips [ MechinGun ][ UP ][ 5 ].w = 24; clips [ MechinGun ][ UP ][ 5 ].h = 48;
    clips [ MechinGun ][ UP ][ 6 ].x = 547; clips [ MechinGun ][ UP ][ 6 ].y = 291; clips [ MechinGun ][ UP ][ 6 ].w = 22; clips [ MechinGun ][ UP ][ 6 ].h = 47;
    clips [ MechinGun ][ UP ][ 7 ].x = 572; clips [ MechinGun ][ UP ][ 7 ].y = 291; clips [ MechinGun ][ UP ][ 7 ].w = 22; clips [ MechinGun ][ UP ][ 7 ].h = 48;
    
    //Clip the sprites DOWN boxhead
    clips [ MechinGun ][ DOWN ][ 0 ].x = 742; clips [ MechinGun ][ DOWN ][ 0 ].y = 175; clips [ MechinGun ][ DOWN ][ 0 ].w = 23; clips [ MechinGun ][ DOWN ][ 0 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 1 ].x = 767; clips [ MechinGun ][ DOWN ][ 1 ].y = 174; clips [ MechinGun ][ DOWN ][ 1 ].w = 23; clips [ MechinGun ][ DOWN ][ 1 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 2 ].x = 2; clips [ MechinGun ][ DOWN ][ 2 ].y = 233; clips [ MechinGun ][ DOWN ][ 2 ].w = 23; clips [ MechinGun ][ DOWN ][ 2 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 3 ].x = 27; clips [ MechinGun ][ DOWN ][ 3 ].y = 233; clips [ MechinGun ][ DOWN ][ 3 ].w = 23; clips [ MechinGun ][ DOWN ][ 3 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 4 ].x = 53; clips [ MechinGun ][ DOWN ][ 4 ].y = 233; clips [ MechinGun ][ DOWN ][ 4 ].w = 23; clips [ MechinGun ][ DOWN ][ 4 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 5 ].x = 79; clips [ MechinGun ][ DOWN ][ 5 ].y = 233; clips [ MechinGun ][ DOWN ][ 5 ].w = 23; clips [ MechinGun ][ DOWN ][ 5 ].h = 50;
    clips [ MechinGun ][ DOWN ][ 6 ].x = 105; clips [ MechinGun ][ DOWN ][ 6 ].y = 233; clips [ MechinGun ][ DOWN ][ 6 ].w = 23; clips [ MechinGun ][ DOWN ][ 6 ].h = 49;
    clips [ MechinGun ][ DOWN ][ 7 ].x = 130; clips [ MechinGun ][ DOWN ][ 7 ].y = 233; clips [ MechinGun ][ DOWN ][ 7 ].w = 23; clips [ MechinGun ][ DOWN ][ 7 ].h = 49;
    
    //Clip the sprites RIGHT_UP boxhead
    clips [ MechinGun ][ RIGHT_UP ][ 0 ].x = 596; clips [ MechinGun ][ RIGHT_UP ][ 0 ].y = 291; clips [ MechinGun ][ RIGHT_UP ][ 0 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 0 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 1 ].x = 639; clips [ MechinGun ][ RIGHT_UP ][ 1 ].y = 291; clips [ MechinGun ][ RIGHT_UP ][ 1 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 1 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 2 ].x = 681; clips [ MechinGun ][ RIGHT_UP ][ 2 ].y = 291; clips [ MechinGun ][ RIGHT_UP ][ 2 ].w =40; clips [ MechinGun ][ RIGHT_UP ][ 2 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 3 ].x = 723; clips [ MechinGun ][ RIGHT_UP ][ 3 ].y = 291; clips [ MechinGun ][ RIGHT_UP ][ 3 ].w =40; clips [ MechinGun ][ RIGHT_UP ][ 3 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 4 ].x = 2; clips [ MechinGun ][ RIGHT_UP ][ 4 ].y = 349; clips [ MechinGun ][ RIGHT_UP ][ 4 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 4 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 5 ].x = 46; clips [ MechinGun ][ RIGHT_UP ][ 5 ].y = 349; clips [ MechinGun ][ RIGHT_UP ][ 5 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 5 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 6 ].x = 89; clips [ MechinGun ][ RIGHT_UP ][ 6 ].y = 349; clips [ MechinGun ][ RIGHT_UP ][ 6 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 6 ].h = 47;
    clips [ MechinGun ][ RIGHT_UP ][ 7 ].x = 131; clips [ MechinGun ][ RIGHT_UP ][ 7 ].y = 349; clips [ MechinGun ][ RIGHT_UP ][ 7 ].w = 40; clips [ MechinGun ][ RIGHT_UP ][ 7 ].h = 47;

    //Clip the sprites LEFT_UP boxhead
    clips [ MechinGun ][ LEFT_UP ][ 0 ].x = 88; clips [ MechinGun ][ LEFT_UP ][ 0 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 0 ].w = 36; clips [ MechinGun ][ LEFT_UP ][ 0 ].h = 46;
    clips [ MechinGun ][ LEFT_UP ][ 1 ].x = 128; clips [ MechinGun ][ LEFT_UP ][ 1 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 1 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 1 ].h =46;
    clips [ MechinGun ][ LEFT_UP ][ 2 ].x = 165; clips [ MechinGun ][ LEFT_UP ][ 2 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 2 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 2 ].h = 46;
    clips [ MechinGun ][ LEFT_UP ][ 3 ].x = 204; clips [ MechinGun ][ LEFT_UP ][ 3 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 3 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 3 ].h = 46;
    clips [ MechinGun ][ LEFT_UP ][ 4 ].x = 242; clips [ MechinGun ][ LEFT_UP ][ 4 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 4 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 4 ].h = 47;
    clips [ MechinGun ][ LEFT_UP ][ 5 ].x = 280; clips [ MechinGun ][ LEFT_UP ][ 5 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 5 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 5 ].h = 47;
    clips [ MechinGun ][ LEFT_UP ][ 6 ].x = 317; clips [ MechinGun ][ LEFT_UP ][ 6 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 6 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 6 ].h = 47;
    clips [ MechinGun ][ LEFT_UP ][ 7 ].x = 356; clips [ MechinGun ][ LEFT_UP ][ 7 ].y = 291; clips [ MechinGun ][ LEFT_UP ][ 7 ].w = 35; clips [ MechinGun ][ LEFT_UP ][ 7 ].h = 47;
    
    //Clip the sprites RIGHT_DOWN boxhead
    clips [ MechinGun ][ RIGHT_DOWN ][ 0 ].x = 437; clips [ MechinGun ][ RIGHT_DOWN ][ 0 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 0 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 0 ].h = 47;
    clips [ MechinGun ][ RIGHT_DOWN ][ 1 ].x = 476; clips [ MechinGun ][ RIGHT_DOWN ][ 1 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 1 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 1 ].h = 47;
    clips [ MechinGun ][ RIGHT_DOWN ][ 2 ].x = 514; clips [ MechinGun ][ RIGHT_DOWN ][ 2 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 2 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 2 ].h = 46;
    clips [ MechinGun ][ RIGHT_DOWN ][ 3 ].x = 552; clips [ MechinGun ][ RIGHT_DOWN ][ 3 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 3 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 3 ].h = 46;
    clips [ MechinGun ][ RIGHT_DOWN ][ 4 ].x = 589; clips [ MechinGun ][ RIGHT_DOWN ][ 4 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 4 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 4 ].h = 46;
    clips [ MechinGun ][ RIGHT_DOWN ][ 5 ].x = 628; clips [ MechinGun ][ RIGHT_DOWN ][ 5 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 5 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 5 ].h = 46;
    clips [ MechinGun ][ RIGHT_DOWN ][ 6 ].x = 666; clips [ MechinGun ][ RIGHT_DOWN ][ 6 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 6 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 6 ].h = 46;
    clips [ MechinGun ][ RIGHT_DOWN ][ 7 ].x = 704; clips [ MechinGun ][ RIGHT_DOWN ][ 7 ].y = 175; clips [ MechinGun ][ RIGHT_DOWN ][ 7 ].w = 35; clips [ MechinGun ][ RIGHT_DOWN ][ 7 ].h = 46;
    
    //Clip the sprites LEFT_DOWN boxhead
    clips [ MechinGun ][ LEFT_DOWN ][ 0 ].x = 154; clips [ MechinGun ][ LEFT_DOWN ][ 0 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 0 ].w = 40; clips [ MechinGun ][ LEFT_DOWN ][ 0 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 1 ].x = 196; clips [ MechinGun ][ LEFT_DOWN ][ 1 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 1 ].w = 40; clips [ MechinGun ][ LEFT_DOWN ][ 1 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 2 ].x = 238; clips [ MechinGun ][ LEFT_DOWN ][ 2 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 2 ].w = 40; clips [ MechinGun ][ LEFT_DOWN ][ 2 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 3 ].x = 281; clips [ MechinGun ][ LEFT_DOWN ][ 3 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 3 ].w = 40; clips [ MechinGun ][ LEFT_DOWN ][ 3 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 4 ].x = 324; clips [ MechinGun ][ LEFT_DOWN ][ 4 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 4 ].w = 42; clips [ MechinGun ][ LEFT_DOWN ][ 4 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 5 ].x = 368; clips [ MechinGun ][ LEFT_DOWN ][ 5 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 5 ].w = 40; clips [ MechinGun ][ LEFT_DOWN ][ 5 ].h = 47;
    clips [ MechinGun ][ LEFT_DOWN ][ 6 ].x = 411; clips [ MechinGun ][ LEFT_DOWN ][ 6 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 6 ].w = 39; clips [ MechinGun ][ LEFT_DOWN ][ 6 ].h = 46;
    clips [ MechinGun ][ LEFT_DOWN ][ 7 ].x = 453; clips [ MechinGun ][ LEFT_DOWN ][ 7 ].y = 233; clips [ MechinGun ][ LEFT_DOWN ][ 7 ].w = 39; clips [ MechinGun ][ LEFT_DOWN ][ 7 ].h = 46;
  // .................MechinGun....................//
    
  
  // Turrentmachine cp
  ex.x = 0 ; ex.y = 1 ; ex.w = 37 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][0]
  ex.x = 35 ; ex.y = 1 ; ex.w = 37 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][1]
  ex.x = 70 ; ex.y = 1 ; ex.w = 37 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][2]
  ex.x = 104 ; ex.y = 1 ; ex.w = 37 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][3]
  ex.x = 137 ; ex.y = 1 ; ex.w = 34 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][4]
  ex.x = 168 ; ex.y = 1 ; ex.w = 31 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][5]
  ex.x = 196 ; ex.y = 1 ; ex.w = 30 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][6]
  ex.x = 224 ; ex.y = 1 ; ex.w = 31 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][7]
  ex.x = 252 ; ex.y = 1 ; ex.w = 32 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][8]
  ex.x = 281 ; ex.y = 1 ; ex.w = 30 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][9]
  ex.x = 310 ; ex.y = 1 ; ex.w = 28 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][10]
  ex.x = 336 ; ex.y = 1 ; ex.w = 32 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][11]
  ex.x = 364 ; ex.y = 1 ; ex.w = 34 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][12]
  ex.x = 396 ; ex.y = 1 ; ex.w = 36 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][13]
  ex.x = 428 ; ex.y = 1 ; ex.w = 38 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][14]
  ex.x = 462 ; ex.y = 1 ; ex.w = 39 ; ex.h = 38 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][15]
  ex.x = 499 ; ex.y = 1 ; ex.w = 38 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][16]
  ex.x = 534 ; ex.y = 1 ; ex.w = 39 ; ex.h = 36 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][17].
  ex.x = 570 ; ex.y = 1 ; ex.w = 39 ; ex.h = 36 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][18]
  ex.x = 604 ; ex.y = 1 ; ex.w = 37 ; ex.h = 37 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][19]
  ex.x = 638 ; ex.y = 1 ; ex.w = 33 ; ex.h = 39 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][20]
  ex.x = 669 ; ex.y = 1 ; ex.w = 30 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][21]
  ex.x = 696 ; ex.y = 1 ; ex.w = 31 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][22]
  ex.x = 724 ; ex.y = 1 ; ex.w = 31 ; ex.h = 42 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][23]
  ex.x = 751 ; ex.y = 1 ; ex.w = 31 ; ex.h = 42 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][24]
  ex.x = 0 ; ex.y = 44 ; ex.w = 30 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][25]
  ex.x = 27 ; ex.y = 44 ; ex.w = 30 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][26]
  ex.x = 54 ; ex.y = 44 ; ex.w = 32 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][27]
  ex.x = 83 ; ex.y = 44 ; ex.w = 32 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][28]
  ex.x = 114 ; ex.y = 44 ; ex.w = 35 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][29]
  ex.x = 145 ; ex.y = 44 ; ex.w = 39 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][30]
  ex.x = 180 ; ex.y = 44 ; ex.w = 39 ; ex.h = 40 ;  cp [ Turrentmachine ].push_back ( ex ); // Clips [Turrentmachine][31]

    
  // Barricades cp
  ex.x = 2 ; ex.y = 3 ; ex.w = 40 ; ex.h = 43 ;  cp [ Barricades ].push_back ( ex ); // Clips [Barricades][0]
  ex.x = 42 ; ex.y = 5 ; ex.w = 40 ; ex.h = 43 ;  cp [ Barricades ].push_back ( ex ); // Clips [Barricades][1]
  ex.x = 82 ; ex.y = 4 ; ex.w = 40 ; ex.h = 43 ;  cp [ Barricades ].push_back ( ex ); // Clips [Barricades][2]  
  
  
  // WZombie clips
  // .................whitezombie....................//
  //Clip the sprites RIGHT whitezombie
    clips [ WhiteZombie ][ RIGHT ][ 0 ].x = 173; clips [ WhiteZombie ][ RIGHT ][ 0 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 0 ].w = 30; clips [ WhiteZombie ][ RIGHT ][ 0 ].h = 46;
    clips [ WhiteZombie ][ RIGHT ][ 1 ].x = 203; clips [ WhiteZombie ][ RIGHT ][ 1 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 1 ].w = 29; clips [ WhiteZombie ][ RIGHT ][ 1 ].h = 44;
    clips [ WhiteZombie ][ RIGHT ][ 2 ].x = 232; clips [ WhiteZombie ][ RIGHT ][ 2 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 2 ].w = 26; clips [ WhiteZombie ][ RIGHT ][ 2 ].h = 44;
    clips [ WhiteZombie ][ RIGHT ][ 3 ].x = 261; clips [ WhiteZombie ][ RIGHT ][ 3 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 3 ].w = 27; clips [ WhiteZombie ][ RIGHT ][ 3 ].h = 45;
    clips [ WhiteZombie ][ RIGHT ][ 4 ].x = 291; clips [ WhiteZombie ][ RIGHT ][ 4 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 4 ].w = 28; clips [ WhiteZombie ][ RIGHT ][ 4 ].h = 44;
    clips [ WhiteZombie ][ RIGHT ][ 5 ].x = 322; clips [ WhiteZombie ][ RIGHT ][ 5 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 5 ].w = 28; clips [ WhiteZombie ][ RIGHT ][ 5 ].h = 43;
    clips [ WhiteZombie ][ RIGHT ][ 6 ].x = 349; clips [ WhiteZombie ][ RIGHT ][ 6 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 6 ].w = 29; clips [ WhiteZombie ][ RIGHT ][ 6 ].h = 42;
    clips [ WhiteZombie ][ RIGHT ][ 7 ].x = 381; clips [ WhiteZombie ][ RIGHT ][ 7 ].y = 167; clips [ WhiteZombie ][ RIGHT ][ 7 ].w = 26; clips [ WhiteZombie ][ RIGHT ][ 7 ].h = 43;

    //Clip the sprites LEFT whitezombie
    clips [ WhiteZombie ][ LEFT ][ 0 ].x = 442; clips [ WhiteZombie ][ LEFT ][ 0 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 0 ].w = 33; clips [ WhiteZombie ][ LEFT ][ 0 ].h = 49;
    clips [ WhiteZombie ][ LEFT ][ 1 ].x = 473; clips [ WhiteZombie ][ LEFT ][ 1 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 1 ].w = 30; clips [ WhiteZombie ][ LEFT ][ 1 ].h = 49;
    clips [ WhiteZombie ][ LEFT ][ 2 ].x = 503; clips [ WhiteZombie ][ LEFT ][ 2 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 2 ].w = 29; clips [ WhiteZombie ][ LEFT ][ 2 ].h = 46;
    clips [ WhiteZombie ][ LEFT ][ 3 ].x = 531; clips [ WhiteZombie ][ LEFT ][ 3 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 3 ].w = 30; clips [ WhiteZombie ][ LEFT ][ 3 ].h = 46;
    clips [ WhiteZombie ][ LEFT ][ 4 ].x = 560; clips [ WhiteZombie ][ LEFT ][ 4 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 4 ].w = 33; clips [ WhiteZombie ][ LEFT ][ 4 ].h = 46;
    clips [ WhiteZombie ][ LEFT ][ 5 ].x = 592; clips [ WhiteZombie ][ LEFT ][ 5 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 5 ].w = 31; clips [ WhiteZombie ][ LEFT ][ 5 ].h = 48;
    clips [ WhiteZombie ][ LEFT ][ 6 ].x = 622; clips [ WhiteZombie ][ LEFT ][ 6 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 6 ].w = 30; clips [ WhiteZombie ][ LEFT ][ 6 ].h = 47;
    clips [ WhiteZombie ][ LEFT ][ 7 ].x = 651; clips [ WhiteZombie ][ LEFT ][ 7 ].y = 223; clips [ WhiteZombie ][ LEFT ][ 7 ].w = 29; clips [ WhiteZombie ][ LEFT ][ 7 ].h = 48;
    
    //Clip the sprites UP whitezombie
    clips [ WhiteZombie ][ UP ][ 0 ].x = 177; clips [ WhiteZombie ][ UP ][ 0 ].y = 280; clips [ WhiteZombie ][ UP ][ 0 ].w = 31; clips [ WhiteZombie ][ UP ][ 0 ].h = 50;
    clips [ WhiteZombie ][ UP ][ 1 ].x = 206; clips [ WhiteZombie ][ UP ][ 1 ].y = 279; clips [ WhiteZombie ][ UP ][ 1 ].w = 31; clips [ WhiteZombie ][ UP ][ 1 ].h = 50;
    clips [ WhiteZombie ][ UP ][ 2 ].x = 237; clips [ WhiteZombie ][ UP ][ 2 ].y = 280; clips [ WhiteZombie ][ UP ][ 2 ].w = 31; clips [ WhiteZombie ][ UP ][ 2 ].h = 46;
    clips [ WhiteZombie ][ UP ][ 3 ].x = 269; clips [ WhiteZombie ][ UP ][ 3 ].y = 280; clips [ WhiteZombie ][ UP ][ 3 ].w = 31; clips [ WhiteZombie ][ UP ][ 3 ].h = 47;
    clips [ WhiteZombie ][ UP ][ 4 ].x = 300; clips [ WhiteZombie ][ UP ][ 4 ].y = 279; clips [ WhiteZombie ][ UP ][ 4 ].w = 31; clips [ WhiteZombie ][ UP ][ 4 ].h = 48;
    clips [ WhiteZombie ][ UP ][ 5 ].x = 330; clips [ WhiteZombie ][ UP ][ 5 ].y = 280; clips [ WhiteZombie ][ UP ][ 5 ].w = 31; clips [ WhiteZombie ][ UP ][ 5 ].h = 48;
    clips [ WhiteZombie ][ UP ][ 6 ].x = 361; clips [ WhiteZombie ][ UP ][ 6 ].y = 279; clips [ WhiteZombie ][ UP ][ 6 ].w = 31; clips [ WhiteZombie ][ UP ][ 6 ].h = 48;
    clips [ WhiteZombie ][ UP ][ 7 ].x = 392; clips [ WhiteZombie ][ UP ][ 7 ].y = 280; clips [ WhiteZombie ][ UP ][ 7 ].w = 31; clips [ WhiteZombie ][ UP ][ 7 ].h = 48;
    
    //Clip the sprites DOWN whitezombie
    clips [ WhiteZombie ][ DOWN ][ 0 ].x = 691; clips [ WhiteZombie ][ DOWN ][ 0 ].y = 168; clips [ WhiteZombie ][ DOWN ][ 0 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 0 ].h = 38;
    clips [ WhiteZombie ][ DOWN ][ 1 ].x = 721; clips [ WhiteZombie ][ DOWN ][ 1 ].y = 168; clips [ WhiteZombie ][ DOWN ][ 1 ].w = 31; clips [ WhiteZombie ][ DOWN ][ 1 ].h = 25;
    clips [ WhiteZombie ][ DOWN ][ 2 ].x = 751; clips [ WhiteZombie ][ DOWN ][ 2 ].y = 168; clips [ WhiteZombie ][ DOWN ][ 2 ].w = 31; clips [ WhiteZombie ][ DOWN ][ 2 ].h = 38;
    clips [ WhiteZombie ][ DOWN ][ 3 ].x = 0; clips [ WhiteZombie ][ DOWN ][ 3 ].y = 223; clips [ WhiteZombie ][ DOWN ][ 3 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 3 ].h = 43;
    clips [ WhiteZombie ][ DOWN ][ 4 ].x = 32; clips [ WhiteZombie ][ DOWN ][ 4 ].y = 223; clips [ WhiteZombie ][ DOWN ][ 4 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 4 ].h = 43;
    clips [ WhiteZombie ][ DOWN ][ 5 ].x = 63; clips [ WhiteZombie ][ DOWN ][ 5 ].y = 223; clips [ WhiteZombie ][ DOWN ][ 5 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 5 ].h = 43;
    clips [ WhiteZombie ][ DOWN ][ 6 ].x = 93; clips [ WhiteZombie ][ DOWN ][ 6 ].y = 223; clips [ WhiteZombie ][ DOWN ][ 6 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 6 ].h = 43;
    clips [ WhiteZombie ][ DOWN ][ 7 ].x = 125; clips [ WhiteZombie ][ DOWN ][ 7 ].y = 223; clips [ WhiteZombie ][ DOWN ][ 7 ].w = 30; clips [ WhiteZombie ][ DOWN ][ 7 ].h = 43;
    
    //Clip the sprites RIGHT_UP whitezombie
    clips [ WhiteZombie ][ RIGHT_UP ][ 0 ].x = 422; clips [ WhiteZombie ][ RIGHT_UP ][ 0 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 0 ].w = 38; clips [ WhiteZombie ][ RIGHT_UP ][ 0 ].h = 48;
    clips [ WhiteZombie ][ RIGHT_UP ][ 1 ].x = 458; clips [ WhiteZombie ][ RIGHT_UP ][ 1 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 1 ].w = 38; clips [ WhiteZombie ][ RIGHT_UP ][ 1 ].h = 48;
    clips [ WhiteZombie ][ RIGHT_UP ][ 2 ].x = 495; clips [ WhiteZombie ][ RIGHT_UP ][ 2 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 2 ].w = 37; clips [ WhiteZombie ][ RIGHT_UP ][ 2 ].h = 48;
    clips [ WhiteZombie ][ RIGHT_UP ][ 3 ].x = 531; clips [ WhiteZombie ][ RIGHT_UP ][ 3 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 3 ].w = 36; clips [ WhiteZombie ][ RIGHT_UP ][ 3 ].h = 49;
    clips [ WhiteZombie ][ RIGHT_UP ][ 4 ].x = 567; clips [ WhiteZombie ][ RIGHT_UP ][ 4 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 4 ].w = 37; clips [ WhiteZombie ][ RIGHT_UP ][ 4 ].h = 50;
    clips [ WhiteZombie ][ RIGHT_UP ][ 5 ].x = 601; clips [ WhiteZombie ][ RIGHT_UP ][ 5 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 5 ].w = 38; clips [ WhiteZombie ][ RIGHT_UP ][ 5 ].h = 51;
    clips [ WhiteZombie ][ RIGHT_UP ][ 6 ].x = 639; clips [ WhiteZombie ][ RIGHT_UP ][ 6 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 6 ].w = 36; clips [ WhiteZombie ][ RIGHT_UP ][ 6 ].h = 51;
    clips [ WhiteZombie ][ RIGHT_UP ][ 7 ].x = 674; clips [ WhiteZombie ][ RIGHT_UP ][ 7 ].y = 280; clips [ WhiteZombie ][ RIGHT_UP ][ 7 ].w = 36; clips [ WhiteZombie ][ RIGHT_UP ][ 7 ].h = 51;

    //Clip the sprites LEFT_UP whitezombie
    clips [ WhiteZombie ][ LEFT_UP ][ 0 ].x = 679; clips [ WhiteZombie ][ LEFT_UP ][ 0 ].y = 223; clips [ WhiteZombie ][ LEFT_UP ][ 0 ].w = 35; clips [ WhiteZombie ][ LEFT_UP ][ 0 ].h = 51;
    clips [ WhiteZombie ][ LEFT_UP ][ 1 ].x = 714; clips [ WhiteZombie ][ LEFT_UP ][ 1 ].y = 223; clips [ WhiteZombie ][ LEFT_UP ][ 1 ].w = 35; clips [ WhiteZombie ][ LEFT_UP ][ 1 ].h = 51;
    clips [ WhiteZombie ][ LEFT_UP ][ 2 ].x = 748; clips [ WhiteZombie ][ LEFT_UP ][ 2 ].y = 223; clips [ WhiteZombie ][ LEFT_UP ][ 2 ].w = 35; clips [ WhiteZombie ][ LEFT_UP ][ 2 ].h = 48;
    clips [ WhiteZombie ][ LEFT_UP ][ 3 ].x = 0; clips [ WhiteZombie ][ LEFT_UP ][ 3 ].y = 280; clips [ WhiteZombie ][ LEFT_UP ][ 3 ].w = 35; clips [ WhiteZombie ][ LEFT_UP ][ 3 ].h = 48;
    clips [ WhiteZombie ][ LEFT_UP ][ 4 ].x = 35; clips [ WhiteZombie ][ LEFT_UP ][ 4 ].y = 280; clips [ WhiteZombie ][ LEFT_UP ][ 4 ].w = 37; clips [ WhiteZombie ][ LEFT_UP ][ 4 ].h = 48;
    clips [ WhiteZombie ][ LEFT_UP ][ 5 ].x = 72; clips [ WhiteZombie ][ LEFT_UP ][ 5 ].y = 280; clips [ WhiteZombie ][ LEFT_UP ][ 5 ].w = 36; clips [ WhiteZombie ][ LEFT_UP ][ 5 ].h = 48;
    clips [ WhiteZombie ][ LEFT_UP ][ 6 ].x = 107; clips [ WhiteZombie ][ LEFT_UP ][ 6 ].y = 280; clips [ WhiteZombie ][ LEFT_UP ][ 6 ].w = 35; clips [ WhiteZombie ][ LEFT_UP ][ 6 ].h = 48;
    clips [ WhiteZombie ][ LEFT_UP ][ 7 ].x = 142; clips [ WhiteZombie ][ LEFT_UP ][ 7 ].y = 280; clips [ WhiteZombie ][ LEFT_UP ][ 7 ].w = 34; clips [ WhiteZombie ][ LEFT_UP ][ 7 ].h = 48;
    
    //Clip the sprites RIGHT_DOWN whitezombie
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 0 ].x = 411; clips [ WhiteZombie ][ RIGHT_DOWN ][ 0 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 0 ].w = 33; clips [ WhiteZombie ][ RIGHT_DOWN ][ 0 ].h = 40;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 1 ].x = 445; clips [ WhiteZombie ][ RIGHT_DOWN ][ 1 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 1 ].w = 34; clips [ WhiteZombie ][ RIGHT_DOWN ][ 1 ].h = 42;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 2 ].x = 478; clips [ WhiteZombie ][ RIGHT_DOWN ][ 2 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 2 ].w = 36; clips [ WhiteZombie ][ RIGHT_DOWN ][ 2 ].h = 41;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 3 ].x = 509; clips [ WhiteZombie ][ RIGHT_DOWN ][ 3 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 3 ].w = 40; clips [ WhiteZombie ][ RIGHT_DOWN ][ 3 ].h = 40;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 4 ].x = 549; clips [ WhiteZombie ][ RIGHT_DOWN ][ 4 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 4 ].w = 38; clips [ WhiteZombie ][ RIGHT_DOWN ][ 4 ].h = 38;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 5 ].x = 584; clips [ WhiteZombie ][ RIGHT_DOWN ][ 5 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 5 ].w = 38; clips [ WhiteZombie ][ RIGHT_DOWN ][ 5 ].h = 40;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 6 ].x = 621; clips [ WhiteZombie ][ RIGHT_DOWN ][ 6 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 6 ].w = 34; clips [ WhiteZombie ][ RIGHT_DOWN ][ 6 ].h = 39;
    clips [ WhiteZombie ][ RIGHT_DOWN ][ 7 ].x = 656; clips [ WhiteZombie ][ RIGHT_DOWN ][ 7 ].y = 167; clips [ WhiteZombie ][ RIGHT_DOWN ][ 7 ].w = 35; clips [ WhiteZombie ][ RIGHT_DOWN ][ 7 ].h = 40;
    
    //Clip the sprites LEFT_DOWN whitezombie
    clips [ WhiteZombie ][ LEFT_DOWN ][ 0 ].x = 154; clips [ WhiteZombie ][ LEFT_DOWN ][ 0 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 0 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 0 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 1 ].x = 190; clips [ WhiteZombie ][ LEFT_DOWN ][ 1 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 1 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 1 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 2 ].x = 226; clips [ WhiteZombie ][ LEFT_DOWN ][ 2 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 2 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 2 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 3 ].x = 262; clips [ WhiteZombie ][ LEFT_DOWN ][ 3 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 3 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 3 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 4 ].x = 298; clips [ WhiteZombie ][ LEFT_DOWN ][ 4 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 4 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 4 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 5 ].x = 334; clips [ WhiteZombie ][ LEFT_DOWN ][ 5 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 5 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 5 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 6 ].x = 370; clips [ WhiteZombie ][ LEFT_DOWN ][ 6 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 6 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 6 ].h = 45;
    clips [ WhiteZombie ][ LEFT_DOWN ][ 7 ].x = 407; clips [ WhiteZombie ][ LEFT_DOWN ][ 7 ].y = 224; clips [ WhiteZombie ][ LEFT_DOWN ][ 7 ].w = 38; clips [ WhiteZombie ][ LEFT_DOWN ][ 7 ].h = 45;
  // .................whitezombie....................//
  
    
   
  
  // RZombie clips
  
  
  // Plus cp
  /*ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Plus ].push_back ( ex ); // Clips [Plus][0]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Plus ].push_back ( ex ); // Clips [Plus][1]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Plus ].push_back ( ex ); // Clips [Plus][2]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Plus ].push_back ( ex ); // Clips [Plus][3]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Plus ].push_back ( ex ); // Clips [Plus][4]
  */
  
  // Barrel cp
  /*ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Barrel ].push_back ( ex ); // Clips [Barrel][0]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Barrel ].push_back ( ex ); // Clips [Barrel][1]
  ex.x =  ; ex.y =  ; ex.w =  ; ex.h =  ;  cp [ Barrel ].push_back ( ex ); // Clips [Barrel][2]
  
  */
  
  // Start cp
  ex.x = 79 ; ex.y = 319 ; ex.w = 640 ; ex.h = 480 ;  cp [ Start ].push_back ( ex ); // Clips [Plus][1]
  
}


// Creating Map
void Create_Map ( char** in ) {
  
  
   for ( int row=0;row<height; ++row ) {
    for ( int col=0; col<width; ++col ) {
      apply_surface ( (col*40), (row*40), images [ Ground1 ], screen, &cp [ Ground1 ][ 0 ] ); // part 1 of Ground1 image
    }
   }
  
  // showing items
  for ( int row=0;row<height; ++row ) {
    for ( int col=0; col<width; ++col ) {
      
      char temp = in [ row ][ col ];
      char left_temp=0;
      char right_temp=0;
      char up_temp=0;
      char down_temp=0;
      
      if  ( (col-1) >=0  )
	 left_temp = in [ row ][ col-1 ];
      if  ( (col+1) <width  )
	 right_temp = in [ row ][ col+1 ];
      if  ( (row-1) >=0  )
	 up_temp = in [ row-1 ][ col ];
      if  ( (row+1) <height  )
	 down_temp = in [ row+1 ][ col ];
	 

  
      if ( temp == 'A' ) {
	  
	  if ( left_temp == 'G' && right_temp == 'A' || left_temp == 'T' && right_temp == 'A') {
	    //cerr << "#" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 1 ] );} // part 2 of Ground1 image
	  else if ( left_temp == 'A' && right_temp == 'A' ) {
	    //cerr << "##" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 3 of Ground1 image
	  else if ( left_temp == 'A' && right_temp == 'G' || left_temp == 'A' && right_temp == 'T' ) {
	    //cerr << "###" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 3 ] );} // part 4 of Ground1 image
	  else if ( up_temp == 'G' && down_temp == 'A' || up_temp == 'T' && down_temp == 'A' ) {
	    //cerr << "####" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 4 ] );} // part 5 of Ground1 image
	  else if ( up_temp == 'A' && down_temp == 'A' ) {
	    //cerr << "#####" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 4 of Ground1 image
	  else if ( up_temp == 'A' && down_temp == 'G' || up_temp == 'A' && down_temp == 'T' ) {
	   //cerr << "######" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 5 ] );} // part 4 of Ground1 image
	  else{ 
	    //cerr << "@@@@@@" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 2 of Ground1 image
	    
	  apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Barricades ], screen, &cp [ Barricades ][ 2 ] ); // random part of Barricades images
	  
	  left_temp=0; right_temp=0; up_temp=0; down_temp=0;
     //cerr << "####" << endl;
      }
      
      
      else if ( temp == 'T' ) {
	
	if ( left_temp == 'G' && right_temp == 'T' || left_temp == 'A' && right_temp == 'T' ) {
	    //cerr << "#" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 6 ] );} // part 2 of Ground1 image
	  else if ( left_temp == 'T' && right_temp == 'T' ) {
	    //cerr << "##" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 3 of Ground1 image
	  else if ( left_temp == 'T' && right_temp == 'G' || left_temp == 'T' && right_temp == 'A' ) {
	    //cerr << "###" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 7 ] );} // part 4 of Ground1 image
	  else if ( up_temp == 'G' && down_temp == 'T' || up_temp == 'A' && down_temp == 'T' ) {
	    //cerr << "####" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 6 ] );} // part 5 of Ground1 image
	  else if ( up_temp == 'T' && down_temp == 'T' ) {
	    //cerr << "#####" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 4 of Ground1 image
	  else if ( up_temp == 'T' && down_temp == 'G' || up_temp == 'T' && down_temp == 'A' ) {
	   //cerr << "######" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 7 ] );} // part 4 of Ground1 image
	  else{ 
	    //cerr << "@@@@@@" << endl;
	    apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Ground1 ], screen, &cp [ Ground1 ][ 2 ] );} // part 2 of Ground1 image
	    
	apply_surface ( (col*40) - camera.x,  (row*40) - camera.y, images [ Turrentmachine ], screen, &cp [ Turrentmachine ][ 0 ] );
	//cerr << "#####" << endl;
	
      }
    }
  }
  
  // shows Score
  stringstream STRING("");
  STRING << "0x" << SCORE;
  Score = TTF_RenderText_Solid ( font, STRING.str().c_str(), textcolor  );
  apply_surface ( 0, 0, Score, screen , NULL );
  
  SDL_Flip ( screen );
}

// box head functions
void BOXHEAD( struct Object& ex )
{
    int x_boxhead=0,y_boxhead=0;
 
    for ( int row=0;row<height; ++row ) {
      for ( int col=0; col<width; ++col ) {
	if ( INPUT [ row ][ col ] == 'B' ) {
	  x_boxhead = 40*col;
	  y_boxhead = 40*row;
	}
      }
    }
  
    //Initialize movement variables
    ex.offSetX = x_boxhead; ex.offSetY = y_boxhead;
    ex.type=boxy;
    ex.velocityX = 0; ex.velocityY = 0;

    //Initialize animation variables
    ex.frame = 0;
    ex.TZ = Walk;
    ex.situation = live;
    ex.health = 3000;
    ex.status = RIGHT;
}

void handle_events( struct Object& ex )
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
	SCORE++;
	if( keystates[ SDLK_UP ] )
	    ex.velocityY += BOX_WIDTH / 4; 
        //If down is pressed
        if( keystates[ SDLK_DOWN ] )
	    ex.velocityY -= BOX_WIDTH / 4; 
        //If left is pressed
        if( keystates[ SDLK_LEFT ] )
	    ex.velocityX -= BOX_WIDTH / 4; 
        //If right is pressed
        if( keystates[ SDLK_RIGHT ] )
            ex.velocityX += BOX_WIDTH / 4;
	// Shooting
	if ( keystates[ SDLK_SPACE ] ){
	  if ( ex.TZ != Walk )
	    bullet = true;
	}
	//Boxhead types and guns
	if ( keystates[ SDLK_z ] ){
	  ex.TZ++;
	  if ( ex.TZ > 2 )
	    ex.TZ=0;
	}
	
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        ex.velocityX=0;
	ex.velocityY=0;
	
	 if ( keystates[ SDLK_SPACE ] )
	      bullet = false;
    }
}

void move( struct Object& ex ) 
{
    //Move
    ex.offSetX += ex.velocityX;  ex.offSetY -= ex.velocityY;

    //Keep the stick figure in bounds
    if( ( ex.offSetX - camera.x < 0 ) || ( ex.offSetX + 25 > LEVEL_WIDTH ) )
    {
        ex.offSetX -= ex.velocityX;
    }
    
    if( ( ex.offSetY - camera.y < 0 ) || ( ex.offSetY + 40 > LEVEL_HEIGHT ) )
    {
        ex.offSetY += ex.velocityY;
    }
}

void show( struct Object& ex )
{
    //If BoxHead is moving left
    if( ex.velocityX < 0 && ex.velocityY==0 )
    {
        //Set the animation to left
        ex.status = LEFT;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If BoxHead is moving right
    else if( ex.velocityX > 0 && ex.velocityY==0 )
    {
        //Set the animation to right
        ex.status = RIGHT;

        //Move to the next frame in the animation
        ex.frame++;
    }



    // if foo moving down
    else if( ex.velocityY < 0 && ex.velocityX==0 )
    {
        //Set the animation to left
        ex.status = DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If BoxHead is moving up
    else if( ex.velocityY > 0 && ex.velocityX==0 )
    {
        //Set the animation to right
        ex.status = UP;

        //Move to the next frame in the animation
        ex.frame++;
    }

//If BoxHead is moving left down
    else if( ex.velocityX < 0 && ex.velocityY < 0  )
    {
        //Set the animation to left
        ex.status = LEFT_DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If BoxHead is moving right down
    else if( ex.velocityX > 0 && ex.velocityY < 0 )
    {
        //Set the animation to right
        ex.status = RIGHT_DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }



    // if foo moving left up
    else if( ex.velocityX < 0 && ex.velocityY > 0 )
    {
        //Set the animation to left
        ex.status = LEFT_UP;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If BoxHead is moving right up
    else if( ex.velocityX > 0 && ex.velocityY > 0 )
    {
        //Set the animation to right
        ex.status = RIGHT_UP;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If BoxHead standing
    else
    {
        //Restart the animation
        ex.frame = 0;
    }

    //Loop the animation
    if( ex.frame >= 4 )
    {
        ex.frame = 0;
    }

    //Show the stick figure
    //R
    if( ex.status == RIGHT )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ RIGHT ][ ex.frame ] );
    }
    //L
    else if( ex.status == LEFT )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ LEFT ][ ex.frame ] );
    }
    //U
    else if( ex.status == UP )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ UP ][ ex.frame ] );
    }
    //D
    else if( ex.status == DOWN )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ DOWN ][ ex.frame ] );
    }
    //RD
    else if( ex.status == RIGHT_DOWN )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ RIGHT_DOWN ][ ex.frame ] );
    }
    //LD
    else if( ex.status == LEFT_DOWN )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ LEFT_DOWN ][ ex.frame ] );
    }
    //RU
    else if( ex.status == RIGHT_UP )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ RIGHT_UP ][ ex.frame ] );
    }
    //LU
    else if( ex.status == LEFT_UP )
    {
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ Boxhead ], screen, &clips[ ex.TZ ][ LEFT_UP ][ ex.frame ] );
    }
}

// timer functions

void Timer( struct Timer& ex )
{
    //Initialize the variables
    ex.startTicks = 0;
    ex.pausedTicks = 0;
    ex.paused = false;
    ex.started = false;
}

void start ( struct Timer& ex )
{
    //Start the timer
    ex.started = true;

    //Unpause the timer
    ex.paused = false;

    //Get the current clock time
    ex.startTicks = SDL_GetTicks();
}

void stop( struct Timer& ex )
{
    //Stop the timer
    ex.started = false;

    //Unpause the timer
    ex.paused = false;
}

void pause( struct Timer& ex )
{
    //If the timer is running and isn't already paused
    if( ( ex.started == true ) && ( ex.paused == false ) )
    {
        //Pause the timer
        ex.paused = true;

        //Calculate the paused ticks
        ex.pausedTicks = SDL_GetTicks() - ex.startTicks;
    }
}

void unpause( struct Timer& ex )
{
    //If the timer is paused
    if( ex.paused == true )
    {
        //Unpause the timer
        ex.paused = false;

        //Reset the starting ticks
        ex.startTicks = SDL_GetTicks() - ex.pausedTicks;

        //Reset the paused ticks
        ex.pausedTicks = 0;
    }
}

int get_ticks( struct Timer& ex )
{
    //If the timer is running
    if( ex.started == true )
    {
        //If the timer is paused
        if( ex.paused == true )
        {
            //Return the number of ticks when the timer was paused
            return ex.pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - ex.startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool is_started( struct Timer& ex )
{
    return ex.started;
}

bool is_paused( struct Timer& ex )
{
    return ex.paused;
}

void Show_Boxhead ()
{
  for ( int row=0;row<height; ++row ) {
      for ( int col=0; col<width; ++col ) {
	if ( INPUT [ row ][ col ] == 'B' ) {
	  int x_boxhead = 40*col;
	  int y_boxhead = 40*row;
	  apply_surface ( x_boxhead, y_boxhead, images [ Boxhead ], screen, &clips[ Walk ][ RIGHT ][ 0 ]  );
	}
      }
    }
}

				// bh  
void BoxHead_Movment ( struct Object& ex ) 
{
  //SDL_FillRect ( screen, &screen->clip_rect, SDL_MapRGB ( screen->format, 0xFF, 0xFF, 0xFF ) ); //Fill the screen white
  //Move the stick figure
        move( ex ); 
	
  // camera setup
	set_camera( ex );

  //Collision
	collision ( ex );
	
  // Creation map
	Create_Map ( INPUT );
	//apply_surface ( 0, 0, src, screen, &camera );

  //Show the stick figure on the screen
        show( ex );
	
	
  //.....
  
}


void set_camera( struct Object ex )
{
    //Center the camera over the dot
    camera.x = ( ex.offSetX + BOX_WIDTH / 2 ) - SCREEN_WIDE / 2;
    camera.y = ( ex.offSetY + BOX_HEIGHT / 2 ) - SCREEN_HIGH / 2;
    
    //Keep the camera in bounds.
    if( camera.x < 0 )
    {
        camera.x = 0;    
    }
    if( camera.y < 0 )
    {
        camera.y = 0;    
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;    
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;    
    }   
}


void collision ( struct Object& ex ) 
{
      int row = (ex.offSetY/40)+1;
      int col = ex.offSetX/40;
      
      struct Demolation b;
      b.top = ex.offSetY;
      b.bottom = ex.offSetY + clips[ ex.TZ ][ ex.status ][ ex.frame ].h;
      b.left = ex.offSetX;
      b.right = ex.offSetX + clips[ ex.TZ ][ ex.status ][ ex.frame ].w;
      
      char BoxHead = INPUT [ row ][ col ];
      char left_temp=0; struct Demolation L;
      char right_temp=0; struct Demolation R;
      char up_temp=0; struct Demolation U;
      char down_temp=0; struct Demolation D;
      char left_down_temp=0; struct Demolation LD;
      char right_down_temp=0; struct Demolation RD;
      char left_up_temp=0; struct Demolation LU;
      char right_up_temp=0; struct Demolation RU;
      
      if  ( (col-1) >=0  )
      {
	 left_temp = INPUT [ row ][ col-1 ];
	 L.left=(col-1); L.top=row*40; L.right=(col)*40; L.bottom=(row+1)*40;
      }
      if  ( (col+1) <width  )
      {
	 right_temp = INPUT [ row ][ col+1 ];
	 R.left=(col+1)*40; R.top=row*40; R.right=(col+2)*40; R.bottom=(row+1)*40;
      }
      if  ( (row-1) >=0  )
      {
	 up_temp = INPUT [ row ][ col ];
	 U.left=col*40; U.top=(row-1)*40; U.right=(col+1)*40; U.bottom=(row)*40;
      }
      if  ( (row+1) <height  )
      {
	 down_temp = INPUT [ row ][ col ];
	 D.left=col*40; D.top=(row)*40; D.right=(col+1)*40; D.bottom=(row+1)*40;
      }
      if  ( (col-1) >=0 && (row-1) >=0 )
      {
	 left_up_temp = INPUT [ row-1 ][ col-1 ];
	 LU.left=(col-1)*40; LU.top=(row-1*40); LU.right=(col)*40; LU.bottom=(row)*40;
      }
      if  ( (col-1) >=0 && (row+1) <height )
      {
	 left_down_temp = INPUT [ row+1 ][ col-1 ];
	 LD.left=(col-1)*40; LD.top=(row+1)*40; LD.right=(col)*40; LD.bottom=(row+2)*40;
      }
      if  ( (row-1) >=0 && (col+1) <width  )
      {
	 right_up_temp = INPUT [ row-1 ][ col+1 ];
	 RU.left=(col+1)*40; RU.top=(row-1)*40; RU.right=(col+2)*40; RU.bottom=(row)*40;
      } 
      if  ( (row+1) <height && (col+1) <width  )
      {
	 right_down_temp = INPUT [ row+1 ][ col+1 ];
	 RD.left=(col+1)*40; RD.top=(row+1)*40; RD.right=(col+2)*40; RD.bottom=(row+2)*40;
      }
      
      
      //If BoxHead is moving left
    if( ex.status == LEFT )
    {
      if ( left_temp != 0 && left_temp != 'G' )
      { 
	if( ( ( b.bottom <= L.top ) || ( b.top >= L.bottom ) || ( b.right <= L.left ) || ( b.left >= L.right ) ) == false )
	    ex.offSetX -= ex.velocityX;
      }
    }

    //If BoxHead is moving right
    else if( ex.status == RIGHT )
    {
      if ( right_temp != 0 && right_temp != 'G' )
	{ 
	  if( ( ( b.bottom <= R.top ) || ( b.top >= R.bottom ) || ( b.right <= R.left ) || ( b.left >= R.right ) ) == false )
	     ex.offSetX -= ex.velocityX;
	}
    }

    // if BoxHead moving down
    else if( ex.status == DOWN )
    {
      if ( down_temp != 0 && down_temp != 'G' )
	{ 
	  if( ( ( b.bottom <= D.top ) || ( b.top >= D.bottom ) || ( b.right <= D.left ) || ( b.left >= D.right ) ) == false ) 
	      ex.offSetY += ex.velocityY;
	}
    }

    //If BoxHead is moving up
    else if( ex.status == UP )
    {
      if ( up_temp != 0 && up_temp != 'G' ) 
	{ 
	  if( ( ( b.bottom <= U.top ) || ( b.top >= U.bottom ) || ( b.right <= U.left ) || ( b.left >= U.right ) ) == false ) 
	      ex.offSetY += ex.velocityY;
	}
    }

    //If BoxHead is moving left down
    else if( ex.status == LEFT_DOWN  )
    {
      if ( left_down_temp != 0 && left_down_temp != 'G' ) 
	{ 
	  if( ( ( b.bottom <= LD.top ) || ( b.top >= LD.bottom ) || ( b.right <= LD.left ) || ( b.left >= LD.right ) ) == false )
	    ex.offSetX -= ex.velocityX;  ex.offSetY += ex.velocityY;
	}
    }

    //If BoxHead is moving right down
    else if( ex.status == RIGHT_DOWN )
    {
      if ( right_down_temp != 0 && right_down_temp != 'G' ) 
	{ 
	  if( ( ( b.bottom <= RD.top ) || ( b.top >= RD.bottom ) || ( b.right <= RD.left ) || ( b.left >= RD.right ) ) == false )
	    ex.offSetX -= ex.velocityX;  ex.offSetY += ex.velocityY;
	}
    }

    // if BoxHead moving left up
    else if( ex.status == LEFT_UP )
    {
      if ( left_up_temp != 0 && left_up_temp != 'G' ) 
	{ 
	  if( ( ( b.bottom <= LU.top ) || ( b.top >= LU.bottom ) || ( b.right <= LU.left ) || ( b.left >= LU.right ) ) == false )
	    ex.offSetX -= ex.velocityX;  ex.offSetY += ex.velocityY;
	}
    }

    //If BoxHead is moving right up
    else if( ex.status == RIGHT_UP )
    {
      if ( right_up_temp != 0 && right_up_temp != 'G' )
	{ 
	  if( ( ( b.bottom <= RU.top ) || ( b.top >= RU.bottom ) || ( b.right <= RU.left ) || ( b.left >= RU.right ) ) == false )
	    ex.offSetX -= ex.velocityX;  ex.offSetY += ex.velocityY;
	}
    }
    
}

void Zombie ( struct Object& ex ) 
{
    ex.offSetX=rand() % LEVEL_WIDTH; ex.offSetY= rand() % LEVEL_HEIGHT; ex.velocityX=0; ex.velocityY=0;
    //Initialize animation variables
    ex.frame = 0;
    ex.TZ = rand() %2 + WhiteZombie;
    ex.situation=live;
    ex.type=zomby;
    ex.situation = live;
    ex.status = RIGHT;
    if ( ex.TZ == WhiteZombie )
      ex.health = 20;
    if ( ex.TZ == RedZombie )
      ex.health = 40;
 
}
			   // zombie         //box
void move_zombie ( struct Object& a, struct Object& b )
{
 if ( dis ( a, b ) != 0 )
 {
    if ( a.offSetX < b.offSetX && a.offSetY < b.offSetY )
    {a.velocityX = ZMU; a.velocityY = ZMU;}
    
    else if ( a.offSetX > b.offSetX && a.offSetY < b.offSetY )
    {a.velocityX = -ZMU; a.velocityY = ZMU;}
    
    else if ( a.offSetX < b.offSetX && a.offSetY > b.offSetY )
    {a.velocityX = ZMU; a.velocityY = -ZMU;}
    
    else if ( a.offSetX > b.offSetX && a.offSetY > b.offSetY )
    {a.velocityX = -ZMU; a.velocityY = -ZMU;}
    
    if ( ( a.offSetX >= b.offSetX-5 && a.offSetX <= b.offSetX+5 ) && a.offSetY < b.offSetY )
    {a.velocityX = 0; a.velocityY = ZMU;}
    
    if ( a.offSetX > b.offSetX && ( a.offSetY >= b.offSetY-5 && a.offSetY <= b.offSetY+5 ) )
    {a.velocityX = ZMU; a.velocityY = 0;}
    
    if ( ( a.offSetX >= b.offSetX-5 && a.offSetX <= b.offSetX+5 ) && a.offSetY > b.offSetY )
    {a.velocityX = 0; a.velocityY = -ZMU;}
    
    if ( a.offSetX > b.offSetX && ( a.offSetY >= b.offSetY-5 && a.offSetY <= b.offSetY+5 ) )
    {a.velocityX = -ZMU; a.velocityY = 0;}
    
  }
  
  a.offSetX += a.velocityX;  a.offSetY += a.velocityY;
  
  if( ( a.offSetX - camera.x < 0 ) || ( a.offSetX + 25 > LEVEL_WIDTH ) )
    {
        a.offSetX -= a.velocityX;
    }
    
  if( ( a.offSetY - camera.y < 0 ) || ( a.offSetY + 40 > LEVEL_HEIGHT ) )
    {
        a.offSetY -= a.velocityY;
    }
}

void First_Zombie_Show ( struct Object Z ) 
{
   if( Z.TZ == WhiteZombie )
    apply_surface ( Z.offSetX - camera.x, Z.offSetY - camera.y, images [ WZombie ], screen, &clips[ Z.TZ ][ Z.status ][ Z.frame ] );
   else if( Z.TZ == RedZombie )
    apply_surface ( Z.offSetX - camera.x, Z.offSetY - camera.y, images [ RZombie ], screen, &clips[ Z.TZ ][ Z.status ][ Z.frame ] );
}


void Show_Zombie ( struct Object& ex )
{
  
  
//If zombie is moving left
    if( ex.velocityX < 0 && ex.velocityY==0 )
    {
        //Set the animation to left
        ex.status = LEFT;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If zombie is moving right
    else if( ex.velocityX > 0 && ex.velocityY==0 )
    {
        //Set the animation to right
        ex.status = RIGHT;

        //Move to the next frame in the animation
        ex.frame++;
    }



    // if zombie moving down
    else if( ex.velocityY < 0 && ex.velocityX==0 )
    {
        //Set the animation to left
        ex.status = UP;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If zombie is moving up
    else if( ex.velocityY > 0 && ex.velocityX==0 )
    {
        //Set the animation to right
        ex.status = DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }
//If zombie is moving left down
    else if( ex.velocityX < 0 && ex.velocityY < 0  )
    {
        //Set the animation to left
        ex.status = LEFT_UP;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If zombie is moving right down
    else if( ex.velocityX > 0 && ex.velocityY < 0 )
    {
        //Set the animation to right
        ex.status = RIGHT_UP;

        //Move to the next frame in the animation
        ex.frame++;
    }



    // if zombie moving left up
    else if( ex.velocityX < 0 && ex.velocityY > 0 )
    {
        //Set the animation to left
        ex.status = LEFT_DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If zombie is moving right up
    else if( ex.velocityX > 0 && ex.velocityY > 0 )
    {
        //Set the animation to right
        ex.status = RIGHT_DOWN;

        //Move to the next frame in the animation
        ex.frame++;
    }


    //If zombie standing
    else
    {
        //Restart the animation
        ex.frame = 0;
    }

    //Loop the animation
    if( ex.frame >= 4 )
    {
        ex.frame = 0;
    }
    
   
    //Show the stick figure
    //R
    if( ex.status == RIGHT && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ RIGHT ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ RIGHT ][ ex.frame ] );
    }
    //L
    else if( ex.status == LEFT )
    {
      if( ex.TZ == WhiteZombie && ex.situation == live )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ LEFT ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ LEFT ][ ex.frame ] );
    }
    //U
    else if( ex.status == UP && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ UP ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ UP ][ ex.frame ] );
    }
    //D
    else if( ex.status == DOWN && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ DOWN ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ DOWN ][ ex.frame ] );
    }
    //RD
    else if( ex.status == RIGHT_DOWN && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ RIGHT_DOWN ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ RIGHT_DOWN ][ ex.frame ] );
    }
    //LD
    else if( ex.status == LEFT_DOWN && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ LEFT_DOWN ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ LEFT_DOWN ][ ex.frame ] );
    }
    //RU
    else if( ex.status == RIGHT_UP && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ RIGHT_UP ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ RIGHT_UP ][ ex.frame ] );
    }
    //LU
    else if( ex.status == LEFT_UP && ex.situation == live )
    {
      if( ex.TZ == WhiteZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ WZombie ], screen, &clips[ ex.TZ ][ LEFT_UP ][ ex.frame ] );
      else if( ex.TZ == RedZombie  )
        apply_surface( ex.offSetX - camera.x, ex.offSetY - camera.y, images [ RZombie ], screen, &clips[ ex.TZ ][ LEFT_UP ][ ex.frame ] );
    }
}


void Zombie_Movement ( struct Object& ex2, struct Object& ex ) 
{
    move_zombie ( ex2, ex );
    Show_Zombie ( ex2 );
}

void SHOW_Health ( struct Object& ex )
{ 
  stringstream s ("");
  s << "Health : " << ex.health;
  BoxHead_health = TTF_RenderText_Solid ( font, s.str().c_str(), textcolor );
  apply_surface ( 0, 20, BoxHead_health, screen );
}


void Contact ( struct Object& ex, struct Object& ex2 )
{
  if (ex.offSetX >= ex2.offSetX && ex.offSetX <= ex2.offSetX+clips [ex.TZ][ex.status][ex.frame].w  
	    && 
	    ex.offSetY >= ex2.offSetY && ex.offSetY <= ex2.offSetY+clips [ex.TZ][ex.status][ex.frame].h)
	{
	  
	
	    if( ex2.status == LEFT ) {ex2.offSetX += ZMU;}
	      
	    else if( ex2.status == RIGHT ) {ex2.offSetX -= ZMU;}
	    
	    else if( ex2.status == UP ) {ex2.offSetY += ZMU;}
	    
	    else if( ex2.status == DOWN ) {ex2.offSetY -= ZMU;}
	    
	    else if( ex2.status == RIGHT_UP ) {ex2.offSetY += ZMU; ex2.offSetX -= ZMU;}
	    
	    else if( ex2.status == LEFT_UP ) {ex2.offSetY += ZMU; ex2.offSetX += ZMU;}
	    
	    else if( ex2.status == RIGHT_DOWN ) {ex2.offSetY -= ZMU; ex2.offSetX -= ZMU;}
	    
	    else if( ex2.status == LEFT_DOWN ) {ex2.offSetY -= ZMU; ex2.offSetX += ZMU;}
	    
	    if ( ex.type == boxy )
	    {
	      if ( ex2.TZ == WhiteZombie )
		ex.health -= 5;
	      else if ( ex2.TZ == RedZombie ) 
		ex.health -= 10;
	    }
	    
	}
}

void CHECK_END_GAME ( struct Object ex, bool& quit ) 
{
  if ( ex.health <= 0 )
	    {
	      SDL_FillRect ( screen, &screen->clip_rect, SDL_MapRGB ( screen->format, 0, 0, 0 ) ); //Fill the screen black
	      apply_surface ( 0, 0, images [ Start ], screen, &cp[ Start ][ 0 ] );
	      END = TTF_RenderText_Solid ( font_end, "THE GAME IS OVER", end_color );
	      apply_surface ( 210, 220, END, screen, NULL );
	      SDL_Flip( screen );
	      SDL_Delay ( 3000 );
	      quit = true;
	    }
}


void Shoot ( struct Object& b, struct Object& z )
{
  int Ybox=b.offSetY/40,
      Xbox=b.offSetX/40,
      Yzombie=z.offSetY/40,
      Xzombie=z.offSetX/40;
  
  
  if ( Ybox == Yzombie ) {
    if ( Xzombie < Xbox && b.status == RIGHT ) {
     z.health -= 5;
     if ( z.health <= 0 )
       kill ( z );
    }
  }
  
  if ( Ybox == Yzombie ) {
    if ( Xzombie > Xbox && b.status == LEFT ) {
     z.health -= 5;
     if ( z.health <= 0 )
       kill ( z );
    }
  }
  
  if ( Xbox == Xzombie ) {
    if ( Yzombie < Ybox && b.status == UP ) {
     z.health -= 5;
     if ( z.health <= 0 )
       kill ( z );
    }
  }
  
  if ( Xbox == Xzombie ) {
    if ( Yzombie > Ybox && b.status == DOWN ) {
     z.health -= 5;
     if ( z.health <= 0 )
       kill ( z );
    }
  }
    
  bullet = false;
}


void kill ( struct Object& a )
{
 a.situation = dead; 
}


int bob_sort ( int* d ) 
{
  int a;
  
  for( int i=0; i<zombie_no; ++i  ){
    for( int j=0; j<zombie_no; ++j ){
      
      if ( d[ i ] <= d[ j ] ){
	a = d [i];
	d [i] = d [j];
	d [j] = a;
      }
	
    }
  }
  
  
  return d[0];
    
}


long double dis ( struct Object a, struct Object b ) 
{
  long double dx = a.offSetX - b.offSetX;
  long double dy = a.offSetY - b.offSetY;
  
  long double d = sqrt ( pow ( dx, 2 ) + pow ( dy, 2 ) );
  return d;
}