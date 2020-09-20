#include "../headers/header.h"
int SCREEN_WIDE = 640;
int SCREEN_HIGH = 480;


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



int main ( int argc, char* args [] )
{
  int zombie_number = rand () %50 + 1; 
  zombie_no = zombie_number;
  
  Init ();
  loading_files ();
  set_clips ();
  INPUT = input();
  
  Game_Loading ();
  Create_Map ( INPUT );
  
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
  
}

