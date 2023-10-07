/*****************************************************************

	   ---------------------- KOF91 -----------------------

	    ------------ 2D Fighting Game Engine -------------

		   by Nicolas Sevez , aka Moah (moah@kof91.com)

				     http://www.kof91.com

			Patches by Rick Leite (ric@asbyte.com.br)
	
				    http://kof.asbyte.com.br
   	  ------------------------------------------------------

  === Required software ===
   
   - Allegro WIP 3.9.31 (http://www.talula.demon.co.uk/allegro/)
   - Microsoft Visual C++ 
   - Recent set of DirectX and other Windows SDK headers.
   - Xaudio SDK 3.0.7 (http://www.xaudio.com/)

  Set up your compiler so that your INCLUDE and LIB environment 
  variables point to the \include and \lib directories of the
  Allegro, DirectX and Xaudio SDKs .

  This source code shall be compiled with Microsoft Visual C++ 
  and DirectX . However , due to the ever growing platforms 
  supported by the Allegro Library , it should be easily portable 
  to other systems/compilers including : DJGPP, Watcom, Borland, 
  Linux, Unix, BeOS ...

  PLAN :
  ====

  Lines (approx)

     0 -  730	Declarations
   730 - 1050   Initialisations and Intro
  1050 - 1290   Main Menu
  1290 - 1895   Character selection
  1895 - 2070	Check and update story events (endings)
  2070 - 2250	Background selection
  2250 - 3170	Retrieve characters sprites and info
  3170 - 3240	Retrieve background sprite and MIDI
  3240 - 3390	Various sprites loading and initialisations
  3390 - 3620	Round intro							_____________
  3620 - 4090	Draw sprites									 |
  4090 - 5890	Update unbreakable animations					 |
				(all but walk,static and crouch)				 |
  5890 - 6380	Check player 1 inputs + update walk		     MAIN LOOP
  6380 - 7370	AI or check player 2 inputs + update walk		 |
  7370 - 7970	Check successfull attacks						 |
  7970 - 8020	Check and update lifebars		    _____________|	
  8020 - 8560	KO/end of round sequence
  8560 - 8720	Intro function
  8720 - 8780   Blit functions
  8780 - 8940	Sound loading and playing functions
  8940 - 9530	Sprites drawing functions
  9530 -11040	Sprites loading functions
 11040 -11230	Smove detection functions
 11230 -11280	Directory scanning function
 11280 -		MP3 functions


  ESSENTIAL VARIABLES
  ===================

BITMAPS		
			zoomscreen			where the background and sprites 
								are drawn .
			virtscreen			receives streched zoomscreen
								and then lifebars .
			screen				the actual screen .
					

INTEGERS		
			spr1x , spr1y		top-left sprites coordinates on
			spr2x , spr2y		zoomscreen .

			cx , cy				bkgd coordinates displayed at
								top-left of virtscreen with
								0 < cx < bkgd width - 320
								0 < cy < bkgd height - 200

			flag , flag2		sprites animation variables with
								0 < flag < (move_nbf * move_spd)

			actual , actual2	ID of the character's current move

*****************************************************************

Have fun !

Moah (moah@online.fr) , 10/03/2000

*****************************************************************/


#include <string.h>
#include <stdio.h>

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include <dirent.h>

//#include "xaudio.h"
//#include "decoder.h"

//#include "file_input.h"
//#include "audio_output.h"




/** MACROS **/

#define limd            (zoomwidth - (4*p2.width/5))
#define limg            (2 - (p1.width/5))

#define ZMRATIO			(int)(zoomwidth/1.6)


#define JOY1_WP		(joy[0].button[0].b)
#define JOY1_SP		(joy[0].button[1].b)
#define JOY1_WK		(joy[0].button[2].b)
#define JOY1_SK		(joy[0].button[3].b)

#define JOY1_UP		(joy[0].stick[0].axis[1].d1)
#define JOY1_DN		(joy[0].stick[0].axis[1].d2)
#define JOY1_LT		(joy[0].stick[0].axis[0].d1)
#define JOY1_RT		(joy[0].stick[0].axis[0].d2)

#define P1_UP		( key[p1_up] || JOY1_UP )
#define P1_DN		( key[p1_down] || JOY1_DN )
#define P1_LT		( key[p1_left] || JOY1_LT )
#define P1_RT		( key[p1_right] || JOY1_RT )

#define P1_WP		( key[p1_wpunch] || JOY1_WP )
#define P1_SP		( key[p1_spunch] || JOY1_SP )
#define P1_WK		( key[p1_wkick] || JOY1_WK )
#define P1_SK		( key[p1_skick] || JOY1_SK )

// PATCHES defines

#define DIR_BAR "/"
#define DIR_SPRITES "data" DIR_BAR "sprites" DIR_BAR
#define DIR_WAV "data" DIR_BAR "wav" DIR_BAR
#define DIR_MIDI "data" DIR_BAR "midi" DIR_BAR
#define DIR_BKGDS "data" DIR_BAR "bkgds" DIR_BAR

#define fill_sprite(a) rectfill(a, 0, 0,a->w,a->h, makecol(255,0,255));

#define TCLOCK 1 
#define PERFECT 1 
#define TIMEOVER 1
#define SCREENSHOT 0
#define SHADOWS 0
#define CLONE 1

#define GAME_DEBUG 0

////////////////////

struct {
    struct {
        int port;
        int volume;
        char *name;
    } output;
} Options;

//XA_DecoderInfo *decoder;
//int  mp3status;

int MP3_Init (void) ;
void MP3_Exit (void) ;

int MP3_Open (char *mp3file) ;
int MP3_Play () ;
int MP3_Close () ;


// global variables

DATAFILE *creditflic ;

DATAFILE *fonts_dat ;

FONT *small_font ;

MIDI *music ;


SAMPLE *rd1 ;
SAMPLE *rd2 ;
SAMPLE *rd3 ;

SAMPLE *fight ;

SAMPLE *hit_alwa ;
SAMPLE *hit_good ;

SAMPLE *wp_alwa ;
SAMPLE *wk_alwa ;
SAMPLE *sp_alwa ;
SAMPLE *sk_alwa ;

SAMPLE *hit1 ;
SAMPLE *hit2 ;

SAMPLE *block_snd ;

SAMPLE *hadoken1 ;
SAMPLE *hadoken2 ;
SAMPLE *copter1 ;
SAMPLE *copter2 ;
SAMPLE *dragon1 ;
SAMPLE *dragon2 ;

SAMPLE *hadokenx1 ;
SAMPLE *hadokenx2 ;
SAMPLE *copterx1 ;
SAMPLE *copterx2 ;
SAMPLE *dragonx1 ;
SAMPLE *dragonx2 ;

SAMPLE *done ;
SAMPLE *bing ;
SAMPLE *buzz ;

SAMPLE *ko_snd ;
SAMPLE *argh_snd ;
SAMPLE *female_snd ;


#if PERFECT == 1
SAMPLE *prfct_snd ;
#endif

#if TIMEOVER == 1
SAMPLE *to_snd ;
#endif


BITMAP *virtscreen ;             /* virtual screen                   */

BITMAP *zoomscreen ;

BITMAP *scanliner ;              /* blit screen for scanlines        */


PALLETE Pal ;                    
PALLETE TPal ;

BITMAP *Title ;

BITMAP *Selecteur1 ;
BITMAP *Selecteur2 ;

BITMAP *LBbanner ;

BITMAP *Bar ; 
BITMAP *Bar2 ;

BITMAP *Power ;

BITMAP *PBar ;

BITMAP *WImpact[11] ;
BITMAP *SImpact[11] ;
BITMAP *BlockImpact[11] ;

BITMAP *Carton_R1 ;
BITMAP *Carton_R2 ;
BITMAP *Carton_R3 ;

BITMAP *Carton_FT ;

BITMAP *Carton_WN ;

BITMAP *Carton_KO ;

BITMAP *novic0 ;
BITMAP *novic1 ;

BITMAP *avic0 ;
BITMAP *avic1 ;

BITMAP *Face1 ;
BITMAP *Face2 ;

BITMAP *Icon1 ;
BITMAP *Icon2 ;

BITMAP *Lock ;


BITMAP *Bkgd ;

BITMAP *Frgd ;

BITMAP *Blood ;

BITMAP *B_Temp ;

#if TIMEOVER == 1
BITMAP *Carton_TO ;
#endif

#if TCLOCK == 1 
BITMAP *B_clock[10];
#endif

#if PERFECT == 1
BITMAP *B_prfct;
#endif

/******************************************/

struct PLAYER_BMP_STRUCT
{
	BITMAP *flStatic[21] ;
	RLE_SPRITE *Static[21] ;

	BITMAP *flWalk[16] ;
	RLE_SPRITE *Walk[26] ;

	BITMAP *flSingle[4] ;
	RLE_SPRITE *Single[4] ;

	BITMAP *flPunch[21] ;
	RLE_SPRITE *Punch[21] ;

	BITMAP *flWPunch[11] ;
	RLE_SPRITE *WPunch[11] ;

	BITMAP *flKick[21] ;
	RLE_SPRITE *Kick[21] ;

	BITMAP *flWKick[11] ;
	RLE_SPRITE *WKick[11] ;

	BITMAP *flJump[6] ;
	RLE_SPRITE *Jump[6] ;

	BITMAP *flKO[6] ;
	RLE_SPRITE *KO[6] ;

	BITMAP *flHurt[4] ;
	RLE_SPRITE *Hurt[4] ;

	BITMAP *flGHurt[4] ;
	RLE_SPRITE *GHurt[4] ;

	BITMAP *flAKick[11] ;
	RLE_SPRITE *AKick[11] ;

	BITMAP *flAPunch[11] ;
	RLE_SPRITE *APunch[11] ;

	BITMAP *flGKick[11] ;
	RLE_SPRITE *GKick[11] ;

	BITMAP *flGPunch[11] ;
	RLE_SPRITE *GPunch[11] ;

	BITMAP *flFireB[21] ;
	RLE_SPRITE *FireB[21] ;

	BITMAP *flFBall[11] ;
	RLE_SPRITE *FBall[11] ;

	BITMAP *flRush[21] ;
	RLE_SPRITE *Rush[21] ;

	BITMAP *flSMove[21] ;
	RLE_SPRITE *SMove[21] ;

	BITMAP *flFireBX[41] ;
	RLE_SPRITE *FireBX[41] ;

	BITMAP *flFBallX[11] ;
	RLE_SPRITE *FBallX[11] ;

	BITMAP *flRushX[31] ;
	RLE_SPRITE *RushX[31] ;

	BITMAP *flSMoveX[31] ;
	RLE_SPRITE *SMoveX[31] ;
};


struct MOVE
{
	int w ;
	char nbf, spd ;
	int defx , defy ;
};

struct AMOVE
{
	int  w ;
	char nbf, spd ;
	int defx , defy ;

	char hit, dmg ;
	int offx , offy ;
};

struct SMOVE
{
	int  w ;
	char nbf, spd ;
	int defx , defy ;

	char hit, dmg ;
	int offx , offy ;

	char flg, rot, pix, end, succ, spec, sflg ;
	char *exec ;
	char seq[40] ;
	char nb ;

};

struct PLAYER_DATA_STRUCT
{
	char flag ;
	int  height, width ;

	int limoffx , limoffy ;
	int limdefx , limdefy ;

	int wimpact_ct ;
	int simpact_ct ;
	int bimpact_ct ;
	
	struct MOVE statik ;
	struct MOVE walk ;
	struct MOVE crouch ;
	struct MOVE hurt ;
	struct MOVE ghurt ;
	struct MOVE jump ;
	struct MOVE intro ;
	struct MOVE outwin ;
	struct MOVE ko ;

	struct AMOVE wpunch ;
	struct AMOVE spunch ;
	struct AMOVE apunch ;
	struct AMOVE gpunch ;
	struct AMOVE wkick ;
	struct AMOVE skick ;
	struct AMOVE gkick ;
	struct AMOVE akick ;

	struct SMOVE fireb ;
	struct SMOVE fball ;
	struct SMOVE rush ;
	struct SMOVE smove ;

	struct SMOVE firebx ;
	struct SMOVE fballx ;
	struct SMOVE rushx ;
	struct SMOVE smovex ;

};

struct PLAYER_BMP_STRUCT Player1 ;
struct PLAYER_BMP_STRUCT Player2 ;

struct PLAYER_DATA_STRUCT p1 ;
struct PLAYER_DATA_STRUCT p2 ;


void GStatic (int gueu,int x , int y) ;
void GSingle (int gueu,int x , int y) ;
void GWalk (int gueu,int x , int y) ;
void GPunch (int gueu,int x , int y) ;
void GWPunch (int gueu,int x , int y) ;
void GKick (int gueu,int x , int y , int z) ;
void GWKick (int gueu,int x , int y) ;
void GAKick (int gueu,int x , int y) ;
void GAPunch (int gueu,int x , int y) ;
void GJump (int gueu,int x , int y) ;
void GGKick (int gueu,int x , int y) ;
void GGPunch (int gueu,int x , int y) ;
void GFireB (int gueu,int x , int y) ;
void GRush (int gueu,int x , int y) ;
void GSMove (int gueu,int x , int y) ;
void GFireBX (int gueu,int x , int y) ;
void GRushX (int gueu,int x , int y) ;
void GSMoveX (int gueu,int x , int y) ;
void GHurt (int gueu,int x , int y) ;
void GGHurt (int gueu,int x , int y) ;
void GKO (int gueu,int x , int y) ;
void GDead (int gueu,int x , int y) ;

void GStatic2 (int gueu,int x , int y) ;
void GSingle2 (int gueu,int x , int y) ;
void GWalk2 (int gueu,int x , int y) ;
void GPunch2 (int gueu,int x , int y) ;
void GWPunch2 (int gueu,int x , int y) ;
void GKick2 (int gueu,int x , int y , int z) ;
void GWKick2 (int gueu,int x , int y ) ;
void GAKick2 (int gueu,int x , int y) ;
void GAPunch2 (int gueu,int x , int y) ;
void GJump2 (int gueu,int x , int y) ;
void GGKick2 (int gueu,int x , int y) ;
void GGPunch2 (int gueu,int x , int y) ;
void GFireB2 (int gueu,int x , int y) ;
void GRush2 (int gueu,int x , int y) ;
void GSMove2 (int gueu,int x , int y) ;
void GFireBX2 (int gueu,int x , int y) ;
void GRushX2 (int gueu,int x , int y) ;
void GSMoveX2 (int gueu,int x , int y) ;
void GHurt2 (int gueu,int x , int y) ;
void GGHurt2 (int gueu,int x , int y) ;
void GKO2 (int gueu,int x , int y, int z) ;
void GDead2 (int gueu,int x , int y) ;

void GFBall (int gueu,int x , int y) ;
void GFBall2 (int gueu,int x , int y) ;

void GFBallX (int gueu,int x , int y) ;
void GFBallX2 (int gueu,int x , int y) ;

/******************************************/

int scan_files(char *name, int attrib, char what );

void blit_KOF91 (void) ;

void blit_Bkgd (void) ;

char animated ;
char foreground ;

char bkgd_counter ;
char bkgd_delay ;

int bkgdlim ;
int zoomsup ;
int zoomwidth = 320 ;

unsigned char j ;

char gmode , diff ;


// story variables 

char story ;

int sel1,sel2 ;

char debug ;


// string and key variables

char charname [1000][30] ;
char bkgdname [200][30] ;
char flcname [100][30] ;

int nbchar ;
int nbbkgd ;
int nbflc ;

char defeated[1000];
char present[1000];
char secretchar[1000];

int num ;
char tilenum[4] ;

char charn1[40] ;
char charn2[40] ;

char bkgdnum[4] ;

char file[200] ;
char passeur[100] ;
char sectn[50] ;

char temp_strg[2] ;

char last_char ;

char get_kstatus ( char strg[] ) ;

char check_kstatus1 ( char ctrl_key ) ;
char check_kstatus2 ( char ctrl_key ) ;

char last_status1 ;
char last_status2 ;

int fball_h1 , fball_h2 ;
int fballx_h1 , fballx_h2 ;

char autho_ht1 , autho_ht2 ;

char wimpact_nbf ;
char simpact_nbf ;
char bimpact_nbf ;

// control keys variables

   int p1_up 	 , p2_up ;
   int p1_down	 , p2_down ;
   int p1_left	 , p2_left ;
   int p1_right  , p2_right ;
   int p1_wpunch , p2_wpunch ;
   int p1_spunch , p2_spunch ;
   int p1_wkick  , p2_wkick ;
   int p1_skick  , p2_skick ;

// AI variables

int offense , defense ;

char easy_def , easy_off ;
char medium_def , medium_off ;
char hard_def , hard_off ;

int ai_walkf , ai_walkb ;
int ai_static ;


#if TCLOCK == 1 
   int t_clock, t_clock_ini, t_short, t_decrement; 
   int clock_frame ;
   char s_cron[] = "99" ;
   int c_dez, c_uni;		    
#endif

int clone = 0 ;


// Sprites loading functions

void load_sprites1 (void) ;
void load_sprites2 (void) ;

void load_kick1 (void) ;
void load_kick2 (void) ;
void load_win1 (void) ;
void load_win2 (void) ;
void load_lose1 (void) ;
void load_lose2 (void) ;

void destroy_bitmaps (void) ;


// misc

int mp3 ;

int intro_delay ;

int pan = 128;
int pitch = 1000;

int snd_vol , midi_vol ;

int start_x , start_y ;

char gfmode ;

int screen_height ;
int screen_width ;

char autorise ;

void Intro(void) ;


/* Sound Fonctions */

void loadsounds(void) ;
void freesounds(void) ;

/* * * * * * * * * * */

void Buzz (void) ;
void Bing (void) ;
void Paf (char gana) ;
void Hurt (char gana) ;
void Block (void) ;
void Done (void) ;
void Round (char which) ;
void Fight (void) ;

// END OF GLOBAL DEFINITIONS

int main(int,char **)
{

   char c ;
   int  cx, cy;                         // Current X and Y position in the bkgd + misc
   int  x, y;                           // Used in for loops  + misc
   int  fx,fy;
   int  ok = 1;                         // Loop until ok==0                
   int  s = 2;                          // Scroll speed (2 pixels per frame)

   int misc ;

   //long t1, t2, 
   long n;								// For calculating FPS             

   int  flag  = 0 ;                     // sprite cycling flags            
   int  flag2 = 0 ;

   char hflag = 0, hflag2 = 0;

   int checkint = 0 ;
   
   char vic_flag ;

   char frameskip = 0;
   char skp_flag = 0;

   int  sens1 = 0 ;                     // jump flags            
   int  sens2 = 0 ;
   int  g , h , i , nb ;			    // Miscellaneous variables      

   char com_block = 0;
   char com_gblock = 0 ;

   char com_walkf = 0 ;
   char com_walkb = 0 ;

   int  spr1x = 60 , spr1y = 77 ;       // Sprites coordinates		   
   int  spr2x = 210, spr2y = 77 ;

   char oldsel1 , oldsel2 ;
   char last_sel1, last_sel2 ;

   char p2_sel ;

   int angle ;

   char bgd ;

   int sel_player1 , sel_player2 ;

   int alt_color , tcolor , bcolor ;

   int sel_bkgd ;

   int text_color ;

   int deb ;
   char round ;
   char vic1 , vic2 ;

   int actual  = 0 ;			// actual sprite status 
   int actual2 = 0 ;

   char ak , ak2 = 0 ;          // flag air-kick 
   char apch , apch2 = 0 ;      // flag air-punch 

   char limak , limak2 = 1 ;

   signed char limp1 , limp2  ;

   char nbpar = 0 ;				

   char vsynch = 0 ;

   int life1 , life2 ;

   int spower1 , spower2 ;

   int delayer = 2 ;

// character size info

   int sol1 , sol2 ;
   int tol1 , tol2 ;  
   int jump_how1 , jump_how2 ;

   int hitfram1 , hitfram2 ;

   char female1 , female2 ;

// character animation info

   int rush_stop1 , rush_stop2 ;
   int smove_stop1 , smove_stop2 ;

   int rushx_stop1 , rushx_stop2 ;
   int smovex_stop1 , smovex_stop2 ;


   char damag1 , damag2 ;

   char fball1 , fball2 ;

   int fb1x , fb2x ;
   int fb1y , fb2y ;

   int fball_cx1 , fball_cx2 ;
   int fball_cy1 , fball_cy2 ;

   char fball_flg1 , fball_flg2 ;

   char fballx1 , fballx2 ;

   int fbx1x , fbx2x ;
   int fbx1y , fbx2y ;

   int fballx_cx1 , fballx_cx2 ;
   int fballx_cy1 , fballx_cy2 ;

   char fballx_flg1 , fballx_flg2 ;


   char accel_flag , gfx_flag ;
   char snd_flag , midi_flag ;

   char wind_flag ;

   char tnt2 = 0 ;

   char *open1 ;
   char *open2 ;
   char *open3 ;
   char *open4 ;
   char *open5 ;
   char *title ;


// end of definitions

   story = 0 ;

//   srand(time(NULL)) ;

   allegro_init() ;

   set_window_title ("initialising engine ...");

   set_config_file ( "cfgdata.stp" ) ;

   accel_flag = get_config_int(0,"accel",1);

   debug = get_config_int(0,"dbug",0);

   mp3 = get_config_int(0,"mp3",1);

   gfx_flag = get_config_int(0,"gfmode",0);

   wind_flag = get_config_int(0,"win",0);

   switch (gfx_flag) 
   { 

   case 2 : screen_height = 240 ; 
	        screen_width = 320 ;
			start_y = 20 ; break ;

   case 0 : screen_height = 200 ;
			screen_width = 320 ;
			start_y = 0 ; break ; 

   case 1 : screen_height = 400 ;
			screen_width = 320 ;
			start_y = 0 ; break ; 

   case 4 :
   case 3 : screen_height = 400 ; 
	        screen_width = 640 ;
			start_y = 0 ; break ;
   
   }

   gfmode = gfx_flag ;

   snd_flag = get_config_int(0,"snd_driver",0);
   midi_flag = get_config_int(0,"midi_driver",0);

   midi_vol = (3*get_config_int(0,"midi_vol",70));
   snd_vol = (3*get_config_int(0,"snd_vol",40));

   override_config_file("game.ini");

   title = get_config_string ( 0 , "title" , " " ) ;

   set_window_title (title);

   open1 = get_config_string ( 0 , "open1" , " " ) ;
   open2 = get_config_string ( 0 , "open2" , " " ) ;
   open3 = get_config_string ( 0 , "open3" , " " ) ;
   open4 = get_config_string ( 0 , "open4" , " " ) ;
   open5 = get_config_string ( 0 , "open5" , " " ) ;
   
#if TCLOCK == 1
/** TIMER SETINGS **/
   t_clock_ini = atoi( get_config_string("timer","t_clock",0 ) ) ; 
   t_decrement = atoi( get_config_string("timer","t_decrement",0 ) ) ;
   t_short = atoi( get_config_string("timer","t_short",0) );
#endif

/** REGULAR INITIALISATIONS **/

   install_timer();

   install_keyboard();

   fonts_dat = load_datafile ("fonts.dat") ;

   if ( get_config_int(0,"joy1",0) )
   {
	   // JOYSTICK INSTALL
	   if (install_joystick(JOY_TYPE_AUTODETECT) != 0) {
		  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		  allegro_message("Error initialising joystick\n%s\n", allegro_error);
		  return 1;
	   }

	   // make sure that we have a joystick 
//	   if (joy_type == JOY_TYPE_NONE) {
//		  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
//		  allegro_message("Error: joystick not found\n");
//		  return 1;
//	   }

   }
 
   allegro_message( 

	"%s\n\n%s\n%s\n%s\n%s\n%s\n\n"

	"KOF91 engine v.1.2 by Moah (moah@kof91.com)\n"
	"http://www.kof91.com\n\n"

	"Patches by Rick Leite (ric@asbyte.com.br)\n"
	"http://kof.asbyte.com.br\n\n"

	"KOF91 is FREEWARE and CANNOT BE SOLD"

	,title,open1,open2,open3,open4,open5 );


/* GFX mode selection */
  
   set_color_depth (16) ;

if (wind_flag)
{
   if (set_gfx_mode( GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
   {
		set_color_depth (15) ;
		if (set_gfx_mode(GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
		{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error);
		return 1;
		}
   }
}

else
if (accel_flag)
{
   if (set_gfx_mode( GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
   {
		set_color_depth (15) ;
		if (set_gfx_mode(GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
		{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error);
		return 1;
		}
   }
}

else
{
   if (set_gfx_mode( GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
   {
		set_color_depth (15) ;
		if (set_gfx_mode(GFX_AUTODETECT, screen_width, screen_height, 0, 0)!= 0)
		{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting graphics mode\n%s\nTry a different video mode !\n", allegro_error);
		return 1;
		}
   }
}

   start_x = 0 ; cx = 0 ; n = 0 ;
 
  /*** end of GFX mode setting ***/


   text_mode (-1) ;

   small_font = font ;

   virtscreen = create_bitmap(325, 205);

   if ((gfmode == 1)||(gfmode>2)) 
	   scanliner = create_bitmap(screen_width, 400);

   else scanliner = create_bitmap(10, 10);

// Get INFO from GAME.INI FILE

   loadsounds();
   set_volume (snd_vol , midi_vol);

   nbchar = scan_files( DIR_SPRITES "*" , FA_DIREC, 0 ) ;

   nbbkgd = scan_files( DIR_BKGDS "*.pcx" , 0, 1 ) ;

   nbflc = scan_files( DIR_BKGDS "*.flc" , 0, 2 ) ;

   for ( cx = 0 ; cx < nbflc ; cx++ )
   {
	   strcpy ( bkgdname[nbbkgd+cx], flcname[cx] );
   }

   nbbkgd = nbbkgd + nbflc ;

   if (debug)
   allegro_message ("%d characters\n%d backgrounds\nfound .",nbchar, nbbkgd);

#if TCLOCK == 1
   clock_frame = 0;
   t_clock = t_clock_ini;
   sprintf(s_cron,"%d",t_clock);
   c_dez =  s_cron[0]-48;
   c_uni =  s_cron[1]-48;
#endif

   sel_player1 = makecol(255,0,0);
   sel_player2 = makecol(0,255,0);

   alt_color = makecol(120,0,0);
   tcolor = makecol(255,0,255);
   bcolor = makecol(0,0,0);
   sel_bkgd = makecol(0,255,0);

   text_color = -1 ;

   intro_delay = get_config_int ( 0 , "intro_delay" , 125 ) ;

   wimpact_nbf = get_config_int ( 0 , "wimpact_nbf" , 1 ) ;
   simpact_nbf = get_config_int ( 0 , "simpact_nbf" , 1 ) ;
   bimpact_nbf = get_config_int ( 0 , "bimpact_nbf" , 1 ) ;

   if((Lock = load_pcx ( DIR_SPRITES "lock.pcx" , 0 ))==NULL)
   {
		allegro_message("lock.pcx missing");
		exit(0);
   }

   for ( num=1 ; num<(wimpact_nbf+1) ; ++num )
   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, DIR_SPRITES "wimpact" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    if((WImpact[num] = load_pcx ( file , 0 ))==NULL)
	{
		allegro_message("pas moyen de loader les gfx");
		exit(0);
	}
   }

   for ( num=1 ; num<(simpact_nbf+1) ; ++num )
   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, DIR_SPRITES "simpact" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    SImpact[num] = load_pcx ( file , 0 ) ;
   }

   for ( num=1 ; num<(bimpact_nbf+1) ; ++num )
   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, DIR_SPRITES "bimpact" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    BlockImpact[num] = load_pcx ( file , 0 ) ;
   }


// Get Control Keys

   p1_up = get_config_int ( "controls" , "p1_up" , 6 ) ;
   p1_down = get_config_int ( "controls" , "p1_down" , 22 ) ;
   p1_left = get_config_int ( "controls" , "p1_left" , 3 ) ;
   p1_right = get_config_int ( "controls" , "p1_right" , 2 ) ;
   p1_wpunch = get_config_int ( "controls" , "p1_wpunch" , 17 ) ;
   p1_spunch = get_config_int ( "controls" , "p1_spunch" , 1 ) ;
   p1_wkick = get_config_int ( "controls" , "p1_wkick" , 26 ) ;
   p1_skick = get_config_int ( "controls" , "p1_skick" , 23 ) ;

   p2_up = get_config_int ( "controls" , "p2_up" , 84 ) ;
   p2_down = get_config_int ( "controls" , "p2_down" , 85 ) ;
   p2_left = get_config_int ( "controls" , "p2_left" , 82 ) ;
   p2_right = get_config_int ( "controls" , "p2_right" , 83 ) ;
   p2_wpunch = get_config_int ( "controls" , "p2_wpunch" , 15 ) ;
   p2_spunch = get_config_int ( "controls" , "p2_spunch" , 16 ) ;
   p2_wkick = get_config_int ( "controls" , "p2_wkick" , 11 ) ;
   p2_skick = get_config_int ( "controls" , "p2_skick" , 12 ) ;

   //get AI parameters

   easy_def = get_config_int ( 0, "easy_def" , 60 ) ;
   easy_off = get_config_int ( 0, "easy_off" , 30 ) ;

   medium_def = get_config_int ( 0, "medium_def" , 70 ) ;
   medium_off = get_config_int ( 0, "medium_off" , 50 ) ;

   hard_def = get_config_int ( 0, "hard_def" , 60 ) ;
   hard_off = get_config_int ( 0, "hard_off" , 80 ) ;

   if (debug)
   allegro_message("load font");


   if( !(font = fonts_dat[0].dat) )
   {
			allegro_message("failed loading font");
			allegro_exit();
			exit(0);
   }

   // SCAN CHARS DIRECTORY

   for ( cx = 0 ; cx < nbchar ; cx++ )
   {
	   strcpy ( passeur, DIR_SPRITES );
	   strcat ( passeur, charname[cx] );
	   strcat ( passeur, DIR_BAR "static1.pcx" );

	   if ( load_pcx (passeur ,Pal ) != NULL )
	   {
		   present[cx] = 1 ;
	   }

	   else present[cx] = 0 ;

	   strcpy ( passeur, DIR_SPRITES );
	   strcat ( passeur, charname[cx] );
	   strcat ( passeur, DIR_BAR "char.ini" );

	   override_config_file( passeur );

	   secretchar[cx] = get_config_int ( charname[cx], "locked" , 0 );

   }

/* Intro *********************************/

   clear_keybuf() ;

   Intro();

   if (install_sound(snd_flag?DIGI_NONE:DIGI_AUTODETECT,
					 midi_flag?MIDI_NONE:MIDI_AUTODETECT, 0) != 0)
	{
	     allegro_message("Error initialising sound system\n");
		 return 1;
	}



   if ( (music = load_midi (DIR_MIDI "select.mid" ))==NULL)
	  {
			allegro_message("failed loading midi");
			allegro_exit();
			exit(0);
	  }

   play_midi ( music , 1 ) ;

 /**** MAIN MENU ****/

Demoniac:


   clear_keybuf() ;

   for ( cx = 0 ; cx < nbchar ; cx++ )
   {
	   defeated[cx] = 0 ;
   }


   story = 0 ;

   cx = 1 ;

   cy = 27 ;

   Selecteur1 = create_bitmap(400, 28);
   Selecteur2 = create_bitmap(400, 28);

   Bar2 = create_bitmap(127,7) ;

   Carton_KO = load_pcx( DIR_SPRITES "mode.pcx", Pal);

   Carton_WN = load_pcx( DIR_SPRITES "pick.pcx", Pal);

#if TIMEOVER == 1
   Carton_TO = load_pcx( DIR_SPRITES "timeup.pcx", Pal);
#endif

   gmode = 2 ;
   p2_sel = 0 ;

   clear_keybuf() ;

   while (cx)
   {
	   draw_sprite( virtscreen , Title, 0 , 0 ) ;

	   masked_blit ( Carton_KO , virtscreen , 0 , 0
						 , (320-Carton_KO->w)/2
						 , (3*200/4) - (Carton_KO->h/2)
						 , Carton_KO->w ,Carton_KO->h) ;

	   masked_blit ( Carton_WN , virtscreen , 0 , 0
						 , (320-Carton_KO->w)/2 + 12
						 , (3*200/4) - (Carton_KO->h/2) + cy
						 , Carton_WN->w ,Carton_WN->h) ;

	   blit_KOF91() ;

	   if ( key[KEY_F1] && key[KEY_F5] && key[KEY_F9] )
	   {
		   for ( x = 0 ; x < nbchar ; x++ )
		   secretchar[x] = 0 ;

		   rectfill(virtscreen, 10, 64,310,90, sel_player1);
		   textout_centre ( virtscreen , font ,"SECRET CHARACTERS UNLOCKED",
					  160 , 70 , bcolor  ) ;

		   blit_KOF91() ;
		   Done();
		   rest(700);
	   }

	   rest(150) ;

	   poll_joystick() ;

		if ( (key[p2_spunch]) || (key[p2_skick])
			  || (key[p2_wpunch]) || (key[p2_wkick])
			  || ( key[KEY_ENTER])
			  || P1_WK || P1_SK || P1_WP || P1_SP )

		{ Done(); cx = 0 ; }

		if ( (key[p2_up]) || P1_UP )
		{
			if (gmode<2) gmode++ ;

			else gmode = 0 ;

			Bing();

		}

		if ( (key[p2_down]) || P1_DN  )
		{
			if (gmode>0) gmode-- ;

			else gmode = 2 ;

			Bing();

		}

		switch (gmode)
		{

		case 0 : p2_sel = 1 ; cy = 57 ; break ;

		case 1 : p2_sel = 0 ; cy = 42 ; break ;

		case 2 : p2_sel = 0 ; cy = 27 ; break ;

		}

		if ( key[KEY_ESC] )
        {

                  /* Stop trapping the keys */
                  remove_keyboard() ;

                  destroy_midi (music) ;

                  destroy_bitmaps() ;

                  /* Deallocate sounds */
                  freesounds();

				  /* Return to text mode */
                  allegro_exit() ;

                  /* Display some stats if available */
/*                  if ( nbpar > 0 )

                  allegro_message( "----> %ld FPS\n\n", n / (t2 - t1 ) );
*/
				  exit(0) ;

        }

   }

   if (gmode)
   {
	   cx = 1 ;
	   cy = 42 ;

	   diff = 1 ;

	   Carton_KO = load_pcx( DIR_SPRITES "diff.pcx", Pal);

	   clear_keybuf() ;

	   while (cx)
	   {
		   draw_sprite( virtscreen , Title, 0 , 0 ) ;

		   masked_blit ( Carton_KO , virtscreen , 0 , 0
							 , (320-Carton_KO->w)/2
							 , (3*200/4) - (Carton_KO->h/2)
							 , Carton_KO->w ,Carton_KO->h) ;

		   masked_blit ( Carton_WN , virtscreen , 0 , 0
							 , (320-Carton_KO->w)/2 + 12
							 , (3*200/4) - (Carton_KO->h/2) + cy
							 , Carton_WN->w ,Carton_WN->h) ;

		   blit_KOF91() ;

		   rest(150) ;

		   poll_joystick() ;

			if ( (key[p2_spunch]) || (key[p2_skick])
				  || (key[p2_wpunch]) || (key[p2_wkick])
				  || ( key[KEY_ENTER])
				  || P1_WK || P1_SK || P1_WP || P1_SP )

			{ Done(); cx = 0 ; }

			if ( (key[p2_up]) || P1_UP )
			{
				if (diff>0) diff-- ;

				else diff = 2 ;

				Bing();

			}

			if ( (key[p2_down]) || P1_DN  )
			{
				if (diff<2) diff++ ;

				else diff = 0 ;

				Bing();

			}

			switch (diff)
			{

			case 0 : cy = 27 ; break ;

			case 1 : cy = 42 ; break ;

			case 2 : cy = 57 ; break ;

			}

	   }


   }

if (debug)
allegro_message("step 1 : clear screen");

   clear (screen) ;

// Selection Menu

if (debug)
allegro_message("step 2 : destroy midi");

   if (music!=NULL) destroy_midi ( music ) ;

StartMenu:

if (debug)
allegro_message("step 3 : clear selecteurs");

   rectfill(Selecteur1, 0, 0,Selecteur1->w,Selecteur1->h, makecol(255,0,255));
   rectfill(Selecteur2, 0, 0,Selecteur2->w,Selecteur2->h, makecol(255,0,255));

   //clear_to_color( Selecteur1 , makecol(255,0,255) ) ;
   //clear_to_color( Selecteur2 , makecol(255,0,255) ) ;

   if (gmode) p2_sel = 0 ;

if (debug)
allegro_message("step 4 : load midi");

   if ( (music = load_midi (DIR_MIDI "select.mid" ))==NULL)
   {
			allegro_message("failed loading midi");
			allegro_exit();
			exit(0);
   }

   else
   play_midi ( music , 1 ) ;

if (debug)
allegro_message("step 5 : initialisations");

 /**** initialisations ****/

//   set_palette ( Pal ) ;

   sel1 = 1 ; cx = 1 ;
   sel2 = 1 ; cy = 1 ;

   oldsel1 =2 ;
   oldsel2 =2 ;

   flag  = 0 ;
   flag2 = 0 ;
   sens1 = 0 ;
   sens2 = 0 ;

   frameskip = 0 ;

   round = 0 ;
   vic1 = vic2 = 0 ;

   actual  = 0 ;
   actual2 = 0 ;

   last_char = 0 ;

   ak = ak2 = 0 ;
   apch = apch2 = 0 ;

   limak = limak2 = 1 ;

   limp1 = 1 ;
   limp2 = 1 ;

if (debug)
allegro_message("step 7 : clear virtscreen");

   clear (virtscreen) ;

if (debug)
allegro_message("step 8 : build up selecteurs");

	for (cx=-4 ; cx<7 ; cx++)
	{
		fx = cx ;

		while ( fx<0 ) fx = fx + nbchar ;

		while ( fx>(nbchar-1) ) fx = fx - nbchar ;

		strcpy ( file, DIR_SPRITES );
		strcat ( file, charname[fx] );
		strcat ( file, DIR_BAR "selsmall.pcx" );

		if ( (Icon2 = load_pcx ( file , Pal) ) == NULL )
		Icon2 = load_pcx ( DIR_SPRITES "selmiss.pcx", Pal);

		if (Icon2!=NULL)
		{
			if(secretchar[fx])
			{
				for (x = 0 ; x < Icon2->w ; x++)
				for (y = 0 ; y < Icon2->h ; y++)
				{
					if ( getpixel(Icon2, x, y) != tcolor )
						putpixel(Icon2, x, y, bcolor);
				}
			}

			blit ( Icon2, Selecteur2,  0 , 0, ((cx+4)*36)+4, 1, 26, 26) ;
			blit ( Icon2, Selecteur1,  0 , 0, ((cx+4)*36)+4, 1, 26, 26) ;

			if (secretchar[fx])
			{
				masked_blit ( Lock, Selecteur2,  0 , 0, ((cx+4)*36)+4, 1, 26, 26) ;
			    masked_blit ( Lock, Selecteur1,  0 , 0, ((cx+4)*36)+4, 1, 26, 26) ;
			}

		}
		else
		{
			allegro_message("failed loading selmiss.pcx");
			allegro_exit();
			exit(0);
		}

	}

if (debug)
allegro_message("step 9 : load pattern");

   Carton_KO = load_pcx( DIR_SPRITES "pattern.pcx", Pal);

   drawing_mode(DRAW_MODE_COPY_PATTERN, Carton_KO, 0, 0);

if (debug)
allegro_message("step 10 : Blit");

   blit_KOF91() ;

   clear_keybuf() ;

   bgd = 0 ;
   sol1 = 0 ;

   cx = cy = fx = fy = 1 ;

if (debug)
allegro_message("step 11 : retrace_count = 0");

   retrace_count = 0 ;

if (debug)
allegro_message("step 12 : !story mode");

if (!story)
{

while ( cy || cx )

{

	if ( bgd < Carton_KO->w-1 ) bgd++ ;
	else bgd = 0 ;

	if ( sol1 > 0) sol1-- ;
	else sol1 = Carton_KO->h-1 ;

	drawing_mode(DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1 );

	rectfill(virtscreen, 0, 0,320,200, 12);

	drawing_mode(DRAW_MODE_SOLID , 0, 0, 0);

// update character images if necessary
if ( (oldsel1 != sel1) || (oldsel2 != sel2) )
{


/*** load the face2 ***/

if ( p2_sel &&(oldsel2 != sel2) )
{
	strcpy ( passeur, DIR_SPRITES );
	strcat ( passeur, charname[sel2] );
	strcat ( passeur, DIR_BAR "static1.pcx" );

	if ( load_pcx( passeur , Pal ) == NULL )

	{
		Face2 = load_pcx ( DIR_SPRITES "none.pcx", Pal);
		draw_sprite( virtscreen , Face2, 220 , 45 ) ;

		Icon2 = load_pcx ( DIR_SPRITES "selmiss.pcx", Pal);
		//draw_sprite( virtscreen , Icon2, 145 , 174 ) ;

        textout_centre ( virtscreen , font ,
        open2="No character found" ,160 , 120 , sel_player2 ) ;

		strcpy ( charn2, open2 );
	}

	else

	{
		strcpy ( file, DIR_SPRITES );
		strcat ( file, charname[sel2] );
		strcat ( file, DIR_BAR "selbig.pcx" );

		strcpy ( passeur, DIR_SPRITES );
	    strcat ( passeur, charname[sel2] );
	    strcat ( passeur, DIR_BAR "char.ini" );

		override_config_file( passeur );

        textout_centre ( virtscreen , font ,
			secretchar[sel2] ? (open2="LOCKED") :
        ( open2=get_config_string ( charname[sel2] , "name" , "No name found" )) ,
        160 , 120 , sel_player2 ) ;

		strcpy ( charn2, open2 );

        if ( Face2 = load_pcx ( file , Pal) )
		{
			if(secretchar[sel2])
			{
				for (x = 0 ; x < Face2->w ; x++)
				for (y = 0 ; y < Face2->h ; y++)
				{
					if ( getpixel(Face2, x, y) != tcolor )
						putpixel(Face2, x, y, bcolor);
				}
			}

			draw_sprite_h_flip ( virtscreen, Face2, 220, 45 ) ;
		}

        else
        {
			Face2 = load_pcx ( DIR_SPRITES "miss.pcx", Pal);

			draw_sprite( virtscreen , Face2, 220 , 45 ) ;
		}

		masked_blit ( Selecteur2, virtscreen , 39, 0, 0, 172, 320, 27);
		rect ( virtscreen , 144 , 172 , 171 , 199 , sel_player2 ) ;

	}

}

/*** load the face1 ***/

if (oldsel1 != sel1)
{
	strcpy ( passeur, DIR_SPRITES );
	strcat ( passeur, charname[sel1] );
	strcat ( passeur, DIR_BAR "static1.pcx" );

	if ( load_pcx (passeur ,Pal ) == NULL )
	{
         Face1 = load_pcx ( DIR_SPRITES "none.pcx", Pal);

         draw_sprite( virtscreen , Face1, 0 , 45 ) ;

         textout_centre ( virtscreen , font ,
         open1="No character found" ,160 , 70 , sel_player1 ) ;

		 Icon1 = load_pcx ( DIR_SPRITES "selmiss.pcx", Pal);

		 draw_sprite( virtscreen , Icon1, 145 , 1 ) ;

		 strcpy ( charn1, open1 );
	}

	else

	{
		strcpy ( file, DIR_SPRITES );
		strcat ( file, charname[sel1] );
		strcat ( file, DIR_BAR "selbig.pcx" );

		strcpy ( passeur, DIR_SPRITES );
	    strcat ( passeur, charname[sel1] );
	    strcat ( passeur, DIR_BAR "char.ini" );

		override_config_file( passeur );

        textout_centre ( virtscreen , font ,
			secretchar[sel1] ? (open1="LOCKED") :
        ( open1=get_config_string ( charname[sel1] , "name" , "No name found" )) ,
		160 , 70 , sel_player1 ) ;

		strcpy ( charn1, open1 );

		if ( Face1 = load_pcx ( file , Pal) )
		{
			if(secretchar[sel1])
			{
				for (x = 0 ; x < Face1->w ; x++)
				for (y = 0 ; y < Face1->h ; y++)
				{
					if ( getpixel(Face1, x, y) != tcolor )
						putpixel(Face1, x, y, bcolor);
				}
			}

			draw_sprite( virtscreen , Face1, 0 , 45 ) ;
		}

        else

		{
            Face1 = load_pcx ( "sprites\\miss.pcx", Pal);

            draw_sprite( virtscreen , Face1, 0 , 45 ) ;
		}

		masked_blit ( Selecteur1, virtscreen , 39, 0, 0, 0, 320, 27);
		rect ( virtscreen , 144 , 0 , 171 , 27 , sel_player1 ) ;
	}

}

}


if ( (oldsel1 == sel1) || (oldsel2 == sel2) )
{

	if (oldsel1 == sel1)
	{
		if (Face1 != NULL)
		draw_sprite( virtscreen , Face1, 0 , 45 ) ;

		textout_centre ( virtscreen , font ,(charn1)?charn1:"problem" ,
        160 , 70 , sel_player1 ) ;

		masked_blit ( Selecteur1, virtscreen , 39, 0, 0, 0, 320, 27);

		rect ( virtscreen , 144 , 0 , 171 , 27 , sel_player1 ) ;
	}

	if (oldsel2 == sel2)
	{
		if ( p2_sel && (Face2 != NULL) )
		{
			textout_centre ( virtscreen , font ,(charn2)?charn2:"problem",
				160 , 120 , sel_player2 ) ;

			draw_sprite_h_flip( virtscreen , Face2, 220 , 45 ) ;

			masked_blit ( Selecteur2, virtscreen , 39, 0, 0, 172, 320, 27);

			rect ( virtscreen , 144 , 172 , 171 , 199 , sel_player2 ) ;

		}
	}

}

	retrace_count-- ;

	blit_KOF91();

	while (!retrace_count) ;;

	rest(20);

	if ( key[KEY_F1] && key[KEY_F5] && key[KEY_F9] )
	{
		   for ( x = 0 ; x < nbchar ; x++ )
		   secretchar[x] = 0 ;

		   rectfill(virtscreen, 10, 64,310,90, sel_player1);
		   textout_centre ( virtscreen , font ,"SECRET CHARACTERS UNLOCKED",
					  160 , 70 , bcolor  ) ;

		   blit_KOF91() ;
		   Done();
		   rest(700);
	}


if ( key[KEY_ESC] )
{
	goto Demoniac ;
}


if (fy<1) fy++ ;

else fy = 0 ;

if (fy<1)
{

	if (cy && p2_sel)
	{
           oldsel2 = sel2 ;

           poll_joystick() ;

	       if ( ( !gmode && ((key[p2_spunch]) || (key[p2_skick]) || (key[p2_wpunch]) || (key[p2_wkick]) ) )
			   || ( gmode && ( P1_SP || P1_WP || P1_SK || P1_WK ) ) )
	       {
	            strcpy ( passeur, DIR_SPRITES );
                strcat ( passeur, charname[sel2] );
                strcat ( passeur, DIR_BAR "static1.pcx" );

                if ( ( load_pcx( passeur , Pal ) == NULL )
					|| secretchar[sel2] ) Buzz() ;

                else { Done() ; cy = 0 ;}
           }

	       if ((key[p2_left] && !gmode)||(gmode && P1_LT))
	       {
			Bing();
			sel2-- ;
            if ( sel2 == -1 ) sel2 = nbchar-1 ;

	 		fx = sel2-5 ;
			while ( fx<0 ) fx = fx + nbchar ;

			strcpy ( file, DIR_SPRITES );
			strcat ( file, charname[fx] );
			strcat ( file, DIR_BAR "selsmall.pcx" );

			if ( (Icon2 = load_pcx ( file , Pal) ) == NULL )
			Icon2 = load_pcx ( "sprites\\selmiss.pcx", Pal);
			else
			if(secretchar[fx])
			{
				for (x = 0 ; x < Icon2->w ; x++)
				for (y = 0 ; y < Icon2->h ; y++)
				{
					if ( getpixel(Icon2, x, y) != tcolor )
						putpixel(Icon2, x, y, bcolor);
				}
			}

			blit ( Selecteur2 , Selecteur2 , 0 , 0, 36, 0, 360, 27 ) ;

			blit ( Icon2, Selecteur2,  0 , 0, 4, 1, 26, 26) ;

			if (secretchar[fx])
			masked_blit ( Lock, Selecteur2,  0 , 0, 4, 1, 26, 26) ;

	       }

	       if ((key[p2_right] && !gmode)||(gmode && P1_RT))
		   {
			Bing();
			sel2++ ;
            if ( sel2 == nbchar ) sel2 = 0 ;

			fx = sel2+5 ;
			while ( fx>(nbchar-1) ) fx = fx - nbchar ;

			strcpy ( file, DIR_SPRITES );
			strcat ( file, charname[fx] );
			strcat ( file, DIR_BAR "selsmall.pcx" );

			if ( (Icon2 = load_pcx ( file , Pal) ) == NULL )
			Icon2 = load_pcx ( "sprites\\selmiss.pcx", Pal);
			else
			if(secretchar[fx])
			{
				for (x = 0 ; x < Icon2->w ; x++)
				for (y = 0 ; y < Icon2->h ; y++)
				{
					if ( getpixel(Icon2, x, y) != tcolor )
						putpixel(Icon2, x, y, bcolor);
				}
			}

			blit ( Selecteur2 , Selecteur2 , 36 , 0, 0, 0, 360, 27 ) ;

			blit ( Icon2, Selecteur2,  0 , 0, 364, 1, 26, 26) ;

			if (secretchar[fx])
			masked_blit ( Lock, Selecteur2,  0 , 0, 364, 1, 26, 26) ;

	       }
	}


	if (cx)
	{
           oldsel1 = sel1 ;

		   poll_joystick() ;

		   if ( P1_WK || P1_SK || P1_WP || P1_SP )

               {
	                strcpy ( passeur, DIR_SPRITES );
                    strcat ( passeur, charname[sel1] );
                    strcat ( passeur, DIR_BAR "static1.pcx" );

                    if ( ( load_pcx( passeur , Pal ) == NULL )
						|| secretchar[sel1] ) Buzz() ;

                    else
					{
						Done() ;
						if (gmode==2)
						{
							retrace_count = 0 ;
							story=1 ;
							last_sel1 = sel1 ;
							rest(100);
							poll_joystick();
							clear_keybuf();
							cy=0 ;
						}

						p2_sel = 1 ; oldsel2 =2 ;
						rest(80);
						poll_joystick();
						clear_keybuf();
						cx = 0 ;
					}
               }

	       if ( P1_LT )
	       {
				Bing();
				sel1-- ;
				if ( sel1 == -1 ) sel1 = nbchar-1 ;

				fx = sel1-5 ;
				while ( fx<0 ) fx = fx + nbchar ;

				strcpy ( file, DIR_SPRITES );
				strcat ( file, charname[fx] );
				strcat ( file, DIR_BAR "selsmall.pcx" );

				if ( (Icon1 = load_pcx ( file , Pal) ) == NULL )
				Icon1 = load_pcx ( "sprites\\selmiss.pcx", Pal);
				else
				if(secretchar[fx])
				{
					for (x = 0 ; x < Icon1->w ; x++)
					for (y = 0 ; y < Icon1->h ; y++)
					{
						if ( getpixel(Icon1, x, y) != tcolor )
							putpixel(Icon1, x, y, bcolor);
					}
				}


				blit ( Selecteur1 , Selecteur1 , 0 , 0, 36, 0, 360, 27 ) ;

				blit ( Icon1, Selecteur1,  0 , 0, 4, 1, 26, 26) ;

				if (secretchar[fx])
				masked_blit ( Lock, Selecteur1,  0 , 0, 4, 1, 26, 26) ;
	       }

	       if ( P1_RT )
	       {
				Bing();
				sel1++ ;
				if ( sel1 == nbchar ) sel1 = 0 ;

				fx = sel1+5 ;
				while ( fx>(nbchar-1) ) fx = fx - nbchar ;

				strcpy ( file, DIR_SPRITES );
				strcat ( file, charname[fx] );
				strcat ( file, DIR_BAR "selsmall.pcx" );

				if ( (Icon1 = load_pcx ( file , Pal) ) == NULL )
				Icon1 = load_pcx ( "sprites\\selmiss.pcx", Pal);
				else
				if(secretchar[fx])
				{
					for (x = 0 ; x < Icon1->w ; x++)
					for (y = 0 ; y < Icon1->h ; y++)
					{
						if ( getpixel(Icon1, x, y) != tcolor )
							putpixel(Icon1, x, y, bcolor);
					}
				}

				blit ( Selecteur1 , Selecteur1 , 36 , 0, 0, 0, 360, 27 ) ;

				blit ( Icon1, Selecteur1,  0 , 0, 364, 1, 26, 26) ;

				if (secretchar[fx])
				masked_blit ( Lock, Selecteur1,  0 , 0, 364, 1, 26, 26) ;
	       }


	}

}

}

}


  clear (virtscreen) ;
  clear (scanliner) ;

clear_keybuf();

retrace_count = 0 ;

if (story)
{
	sel1 = last_sel1 ;

	cy = cx = 1 ;

	for ( x = 0 ; x < nbchar ; x++ )
	{
		if (!defeated[x] && !secretchar[x] && present[x]) cy = 0 ;
	}

	if (cy) // end of game
	{
		for ( x = 0 ; x < nbchar ; x++ )
		secretchar[x] = 0 ;

		if((Selecteur1 = load_pcx ( "sprites\\end.pcx" , 0 ))==NULL)
		{
			allegro_message("end.pcx missing");
			exit(0);
		}

		cx=1 ;
		while (cx)
		{
			if ( bgd < Carton_KO->w-1 ) bgd++ ;
			else bgd = 0 ;

			if ( sol1 > 0) sol1-- ;
			else sol1 = Carton_KO->h-1 ;

			drawing_mode(DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1 );

			rectfill(virtscreen, 0, 0,320,200, 12);

			drawing_mode(DRAW_MODE_SOLID , 0, 0, 0);

			retrace_count-- ;

			masked_blit ( Selecteur1, virtscreen , 0, 0, 0, 0, 320, 200);

			if (cx==2)
			{
				textout_centre ( virtscreen , font ,"GAME" ,
				240 , 70 , sel_player1 ) ;
				textout_centre ( virtscreen , font ,"OVER" ,
				240 , 90 , sel_player1 ) ;
			}


			blit_KOF91();

			while (!retrace_count) ;;

			rest(20);


			if ( (key[p2_spunch]) || (key[p2_skick])
			|| (key[p2_wpunch]) || (key[p2_wkick])
			|| ( key[KEY_ENTER])
			|| P1_WK || P1_SK || P1_WP || P1_SP )

			{
				if (cx==1)
				{
					strcpy ( passeur, DIR_SPRITES );
					strcat ( passeur, charname[sel1] );
					strcat ( passeur, DIR_BAR "endimg.pcx" );

					if((Selecteur2 = load_pcx ( passeur , 0 ))!=NULL)
					{
						Selecteur1 = Selecteur2 ;
						cx = 2 ;
						clear_keybuf();
					}

					else cx = 0 ;
				}

				else cx = 0 ;

			}


		}

		goto Demoniac ;
	}

	cy = 1 ;

	while (cy)
	{
		sel2 = rand()%nbchar ;

		if (!defeated[sel2] && !secretchar[sel2] & present[sel2]) cy = 0 ;
	}

	strcpy ( file, DIR_SPRITES );
	strcat ( file, charname[sel2] );
	strcat ( file, DIR_BAR "selbig.pcx" );

	strcpy ( sectn, charname[sel2] );

	strcpy ( passeur, DIR_SPRITES );
	strcat ( passeur, sectn );
	strcat ( passeur, DIR_BAR "char.ini" );

	override_config_file( passeur );

	Face2 = load_pcx( file , Pal ) ;

	strcpy ( passeur, DIR_SPRITES );
	strcat ( passeur, charname[sel1] );
	strcat ( passeur, DIR_BAR "selbig.pcx" );

    Face1 = load_pcx ( passeur , Pal);

	bgd = 0 ;
    sol1 = 0 ;

	cx = 1 ;

	while (cx)
	{
		if ( key[KEY_ESC] )
		{
			goto Demoniac ;
		}

		if ( bgd < Carton_KO->w-1 ) bgd++ ;
		else bgd = 0 ;

		if ( sol1 > 0) sol1-- ;
		else sol1 = Carton_KO->h-1 ;

		drawing_mode(DRAW_MODE_COPY_PATTERN, Carton_KO, bgd, sol1 );

		rectfill(virtscreen, 0, 0,320,200, 12);

		drawing_mode(DRAW_MODE_SOLID , 0, 0, 0);

		retrace_count-- ;

			if (Face2 != NULL) draw_sprite_h_flip( virtscreen , Face2, 220 , 45 ) ;

			if (Face1 != NULL) draw_sprite( virtscreen , Face1, 0 , 45 ) ;

			textout_centre ( virtscreen , font ,"NEXT OPPONENT" ,
			160 , 70 , sel_player1 ) ;

			textout_centre ( virtscreen , font ,
			open2=get_config_string ( sectn , "name" , "No name found" ) ,
			160 , 120 , sel_player2 ) ;

			blit_KOF91();

			while (!retrace_count) ;

			rest(20);


		if ( (key[p2_spunch]) || (key[p2_skick])
		|| (key[p2_wpunch]) || (key[p2_wkick])
		|| ( key[KEY_ENTER])
		|| P1_WK || P1_SK || P1_WP || P1_SP )

		{ Done(); cx = 0 ; }


	}


}

   clear( virtscreen );

   blit_KOF91();

if (debug)
allegro_message("check strings :\n  char1 : %s\n  char2 : %s",
				charname[sel1],charname[sel2]);

   rest(15);
   poll_joystick();
   clear_keybuf();

   cx = 1 ;
   bgd = 1 ;

   Carton_R1 = create_bitmap(260, 160);

   strcpy ( passeur, "bkgds\\" );
   strcat ( passeur, bkgdname[bgd] );

	if ( (Carton_R2 = load_pcx (passeur ,Pal )) == NULL )
	{
		if ( (open_fli ( passeur ) == FLI_OK) )
		{
			Carton_R2 = create_bitmap(320, 200);
			reset_fli_variables() ;
			next_fli_frame(1) ;
			set_palette( fli_palette );
			blit ( fli_bitmap , Carton_R2 , (fli_bitmap->w/2)-160, fli_bitmap->h-200, 0, 0, 320, 200 ) ;
			close_fli();
		}
		else
			{
				allegro_message("can't load flc movie");
				exit(0);
			}
	}

	stretch_blit ( Carton_R2 , Carton_R1 , (Carton_R2->w/2)-160, Carton_R2->h-200, 320,200,
										0, 0,260, 160) ;

   //draw_sprite( virtscreen , Carton_R1 , 30 , 10 );


   fx =1 ;

   retrace_count = 0 ;

   poll_joystick();

   while ( cx )
   {
		if ( sol2 < Carton_KO->w-1 ) sol2++ ;
		else sol2 = 0 ;

		if ( sol1 > 0) sol1-- ;
		else sol1 = Carton_KO->h-1 ;

		drawing_mode(DRAW_MODE_COPY_PATTERN, Carton_KO,
			sol2, sol1 );

		rectfill(virtscreen, 0, 0,320,200, 12);

		drawing_mode(DRAW_MODE_SOLID , 0, 0, 0);

		draw_sprite( virtscreen , Carton_R1 , 30 , 10 );

		textout_centre ( virtscreen , font ,"< select background >" ,
        160 , 180 , sel_player1 ) ;

		blit_KOF91();

		poll_joystick();

		while (!retrace_count) ;

		rest(20);


	//rest(10);

	poll_joystick() ;


if (fx<2) fx++ ;

else fx = 0 ;

if (fx<1)
{

	if ( (key[p2_spunch]) || (key[p2_skick])
		  || (key[p2_wpunch]) || (key[p2_wkick])
          || ( key[KEY_ENTER])
		  || P1_WK || P1_SK || P1_WP || P1_SP )

	{ Done(); cx = 0 ; }

	if( P1_LT || (key[p2_left]) )

	{
		bgd-- ;

		while ( bgd<0 ) bgd = bgd + nbbkgd ;

		strcpy ( passeur, "bkgds\\" );
	    strcat ( passeur, bkgdname[bgd] );

		if ( (Carton_R2 = load_pcx (passeur ,Pal )) == NULL )
		{
			if ( (open_fli ( passeur ) == FLI_OK) )
			{
				Carton_R2 = create_bitmap(320, 200);
				reset_fli_variables() ;
				next_fli_frame(1) ;
				set_palette( fli_palette );
				blit ( fli_bitmap , Carton_R2 , (fli_bitmap->w/2)-160, fli_bitmap->h-200, 0, 0, 320, 200 ) ;
				close_fli();
			}
			else
			{
				allegro_message("can't load flc");
				exit(0);
			}
		}

		stretch_blit ( Carton_R2 , Carton_R1 , (Carton_R2->w/2)-160, Carton_R2->h-200, 320,200,
										0, 0,260, 160) ;

		Bing();

	}

	if( P1_RT || (key[p2_right]) )

	{
        bgd++ ;

		while ( bgd>(nbbkgd-1) ) bgd = bgd - nbbkgd ;

		strcpy ( passeur, "bkgds\\" );
	    strcat ( passeur, bkgdname[bgd] );

		if ( (Carton_R2 = load_pcx (passeur ,Pal )) == NULL )
		{
			if ( (open_fli ( passeur ) == FLI_OK) )
			{
				Carton_R2 = create_bitmap(320, 200);
				reset_fli_variables() ;
				next_fli_frame(1) ;
				set_palette( fli_palette );
				blit ( fli_bitmap , Carton_R2 ,(fli_bitmap->w/2)-160, fli_bitmap->h-200,
					0, 0, 320, 200 ) ;
				close_fli();
			}
			else
			{
				allegro_message("pas moyen de loader le flc");
				exit(0);
			}
		}

		stretch_blit ( Carton_R2 , Carton_R1 , (Carton_R2->w/2)-160, Carton_R2->h-200,
			320,200, 0, 0,260, 160) ;

		Bing();
    }


}


}


   destroy_midi (music) ;

if (debug)
allegro_message("check bkgd path :\n %s", passeur );

//   fade_out( 2 );

   clear (screen) ;
   clear (virtscreen) ;

   textout_centre ( virtscreen , font ,
   "Loading..." ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);

   blit_KOF91();

/******* GET CHARACTER 2 INFO *********************/

   last_sel2 = sel2 ;

   strcpy ( sectn, charname[sel2] );

   strcpy ( passeur, DIR_SPRITES );
   strcat ( passeur, sectn );
   strcat ( passeur, DIR_BAR "char.ini" );

if (debug)
allegro_message("check char1 path :\n %s", passeur );

   override_config_file( passeur );


   p2.width = get_config_int ( sectn , "static_w" , 100 ) ;
   p2.spunch.w = get_config_int ( sectn , "spunch_w" , 150 ) ;
   p2.wpunch.w = get_config_int ( sectn , "wpunch_w" , 150 ) ;
   p2.skick.w = get_config_int ( sectn , "skick_w" , 150 ) ;
   p2.wkick.w = get_config_int ( sectn , "wkick_w" , 150 ) ;
   p2.apunch.w = get_config_int ( sectn , "apunch_w" , 150 ) ;
   p2.gpunch.w = get_config_int ( sectn , "gpunch_w" , 150 ) ;
   p2.akick.w = get_config_int ( sectn , "akick_w" , 150 ) ;
   p2.gkick.w = get_config_int ( sectn , "gkick_w" , 150 ) ;
   p2.intro.w = get_config_int ( sectn , "intro_w" , 150 ) ;
   p2.ko.w = get_config_int ( sectn , "ko_w" , 150 ) ;
   p2.outwin.w = get_config_int ( sectn , "outwin_w" , 150 ) ;

   p2.height = get_config_int ( sectn , "height" , 130 ) ;

   p2.statik.nbf = get_config_int ( sectn , "static_nbf" , 4 ) ;
   p2.walk.nbf = get_config_int ( sectn , "walk_nbf" , 5 ) ;
   p2.spunch.nbf = get_config_int ( sectn , "spunch_nbf" , 3 ) ;
   p2.wpunch.nbf = get_config_int ( sectn , "wpunch_nbf" , 3 ) ;
   p2.skick.nbf = get_config_int ( sectn , "skick_nbf" , 4 ) ;
   p2.wkick.nbf = get_config_int ( sectn , "wkick_nbf" , 4 ) ;
   p2.apunch.nbf = get_config_int ( sectn , "apunch_nbf" , 1 ) ;
   p2.gpunch.nbf = get_config_int ( sectn , "gpunch_nbf" , 4 ) ;
   p2.akick.nbf = get_config_int ( sectn , "akick_nbf" , 1 ) ;
   p2.gkick.nbf = get_config_int ( sectn , "gkick_nbf" , 4 ) ;

   p2.hurt.nbf = get_config_int ( sectn , "hurt_nbf" , 1 ) ;
   p2.ghurt.nbf = get_config_int ( sectn , "ghurt_nbf" , 1 ) ;

   p2.intro.nbf = get_config_int ( sectn , "intro_nbf" , 1 ) ;
   p2.outwin.nbf = get_config_int ( sectn , "outwin_nbf" , 1 ) ;

   p2.statik.spd = get_config_int ( sectn , "static_spd" , 8 ) ;
   p2.walk.spd = get_config_int ( sectn , "walk_spd" , 8 ) ;
   p2.spunch.spd = get_config_int ( sectn , "spunch_spd" , 8 ) ;
   p2.wpunch.spd = get_config_int ( sectn , "wpunch_spd" , 8 ) ;
   p2.skick.spd = get_config_int ( sectn , "skick_spd" , 8 ) ;
   p2.wkick.spd = get_config_int ( sectn , "wkick_spd" , 8 ) ;
   p2.apunch.spd = get_config_int ( sectn , "apunch_spd" , 8 ) ;
   p2.gpunch.spd = get_config_int ( sectn , "gpunch_spd" , 8 ) ;
   p2.akick.spd = get_config_int ( sectn , "akick_spd" , 8 ) ;
   p2.gkick.spd = get_config_int ( sectn , "gkick_spd" , 8 ) ;
   p2.outwin.spd = get_config_int ( sectn , "outwin_spd" , 10 ) ;

   p2.spunch.hit = get_config_int ( sectn , "spunch_hit" , 2 ) ;
   p2.wpunch.hit = get_config_int ( sectn , "wpunch_hit" , 2 ) ;
   p2.skick.hit = get_config_int ( sectn , "skick_hit" , 2 ) ;
   p2.wkick.hit = get_config_int ( sectn , "wkick_hit" , 2 ) ;
   p2.apunch.hit = get_config_int ( sectn , "apunch_hit" , 2 ) ;
   p2.gpunch.hit = get_config_int ( sectn , "gpunch_hit" , 2 ) ;
   p2.akick.hit = get_config_int ( sectn , "akick_hit" , 2 ) ;
   p2.gkick.hit = get_config_int ( sectn , "gkick_hit" , 2 ) ;

   p2.spunch.dmg = get_config_int ( sectn , "spunch_dmg" , 2 ) ;
   p2.wpunch.dmg = get_config_int ( sectn , "wpunch_dmg" , 1 ) ;
   p2.skick.dmg = get_config_int ( sectn , "skick_dmg" , 2 ) ;
   p2.wkick.dmg = get_config_int ( sectn , "wkick_dmg" , 1 ) ;
   p2.apunch.dmg = get_config_int ( sectn , "apunch_dmg" , 1 ) ;
   p2.gpunch.dmg = get_config_int ( sectn , "gpunch_dmg" , 1 ) ;
   p2.akick.dmg = get_config_int ( sectn , "akick_dmg" , 2 ) ;
   p2.gkick.dmg = get_config_int ( sectn , "gkick_dmg" , 2 ) ;

   p2.spunch.offx = get_config_int ( sectn , "spunch_offx" , p2.spunch.w ) ;
   p2.wpunch.offx = get_config_int ( sectn , "wpunch_offx" , p2.wpunch.w ) ;
   p2.skick.offx = get_config_int ( sectn , "skick_offx" , p2.skick.w ) ;
   p2.wkick.offx = get_config_int ( sectn , "wkick_offx" , p2.wkick.w ) ;
   p2.apunch.offx = get_config_int ( sectn , "apunch_offx" , p2.apunch.w ) ;
   p2.gpunch.offx = get_config_int ( sectn , "gpunch_offx" , p2.gpunch.w ) ;
   p2.akick.offx = get_config_int ( sectn , "akick_offx" , p2.akick.w ) ;
   p2.gkick.offx = get_config_int ( sectn , "gkick_offx" , p2.gkick.w ) ;

   p2.spunch.offy = get_config_int ( sectn , "spunch_offy" , p2.height-50 ) ;
   p2.wpunch.offy = get_config_int ( sectn , "wpunch_offy" , p2.height-50 ) ;
   p2.skick.offy = get_config_int ( sectn , "skick_offy" , p2.height-50 ) ;
   p2.wkick.offy = get_config_int ( sectn , "wkick_offy" , p2.height-50 ) ;
   p2.apunch.offy = get_config_int ( sectn , "apunch_offy" , p2.height-50 ) ;
   p2.gpunch.offy = get_config_int ( sectn , "gpunch_offy" , p2.height-50 ) ;
   p2.akick.offy = get_config_int ( sectn , "akick_offy" , p2.height-50 ) ;
   p2.gkick.offy = get_config_int ( sectn , "gkick_offy" , p2.height-50 ) ;


   p2.statik.defx = get_config_int ( sectn , "static_defx" , p2.width ) ;
   p2.walk.defx = get_config_int ( sectn , "walk_defx" , p2.width ) ;
   p2.jump.defx = get_config_int ( sectn , "jump_defx" , p2.width ) ;
   p2.crouch.defx = get_config_int ( sectn , "crouch_defx" , p2.width ) ;
   p2.spunch.defx = get_config_int ( sectn , "spunch_defx" , p2.width ) ;
   p2.wpunch.defx = get_config_int ( sectn , "wpunch_defx" , p2.width ) ;
   p2.skick.defx = get_config_int ( sectn , "skick_defx" , p2.width ) ;
   p2.wkick.defx = get_config_int ( sectn , "wkick_defx" , p2.width ) ;
   p2.apunch.defx = get_config_int ( sectn , "apunch_defx" , p2.width ) ;
   p2.gpunch.defx = get_config_int ( sectn , "gpunch_defx" , p2.width ) ;
   p2.akick.defx = get_config_int ( sectn , "akick_defx" , p2.width ) ;
   p2.gkick.defx = get_config_int ( sectn , "gkick_defx" , p2.width ) ;

   p2.statik.defy = get_config_int ( sectn , "static_defy" , p2.height-50 ) ;
   p2.walk.defy = get_config_int ( sectn , "walk_defy" , p2.height-50 ) ;
   p2.jump.defy = get_config_int ( sectn , "jump_defy" , p2.height-50 ) ;
   p2.crouch.defy = get_config_int ( sectn , "crouch_defy" , p2.height-50 ) ;
   p2.spunch.defy = get_config_int ( sectn , "spunch_defy" , p2.height-50 ) ;
   p2.wpunch.defy = get_config_int ( sectn , "wpunch_defy" , p2.height-50 ) ;
   p2.skick.defy = get_config_int ( sectn , "skick_defy" , p2.height-50 ) ;
   p2.wkick.defy = get_config_int ( sectn , "wkick_defy" , p2.height-50 ) ;
   p2.apunch.defy = get_config_int ( sectn , "apunch_defy" , p2.height-50 ) ;
   p2.gpunch.defy = get_config_int ( sectn , "gpunch_defy" , p2.height-50 ) ;
   p2.akick.defy = get_config_int ( sectn , "akick_defy" , p2.height-50 ) ;
   p2.gkick.defy = get_config_int ( sectn , "gkick_defy" , p2.height-50 ) ;


   female2 = get_config_int ( sectn , "female" , 0 ) ;

   if (gmode)
   {
		switch (diff)
		{

		case 0 : offense = get_config_int ( sectn , "easy_off" , easy_off) ;
				 defense = get_config_int ( sectn , "easy_def" , easy_def) ;
				 break ;

		case 1 : offense = get_config_int ( sectn , "medium_off" , medium_off) ;
				 defense = get_config_int ( sectn , "medium_def" , medium_def) ;
				 break ;

		case 2 : offense = get_config_int ( sectn , "hard_off" , hard_off) ;
				 defense = get_config_int ( sectn , "hard_def" , hard_def) ;
				 break ;

		default: offense = get_config_int ( sectn , "medium_off" , medium_off) ;
				 defense = get_config_int ( sectn , "medium_def" , medium_def) ;
				 break ;

		}

   }

   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.fireb.flg = get_config_int ( sectn , "fireb" , 0 ) ;

   if ( p2.fireb.flg )
   {
	   p2.fball.w = get_config_int ( sectn , "fball_w" , 1 ) ;
	   p2.fball.nbf = get_config_int ( sectn , "fball_nbf" , 1 ) ;
	   p2.fball.spd = get_config_int ( sectn , "fball_spd" , 1 ) ;
	   p2.fball.pix = get_config_int ( sectn , "fball_pix" , 1 ) ;
	   p2.fball.offx = get_config_int ( sectn , "fball_offx" , p2.fball.w ) ;
	   p2.fball.offy = get_config_int ( sectn , "fball_offy" , 10 ) ;


	   fball_h2 = get_config_int ( sectn , "fball_h" , 1 ) ;
	   fball_cx2 = get_config_int ( sectn , "fball_cx" , 1 ) ;
	   fball_cy2 = get_config_int ( sectn , "fball_cy" , 1 ) ;

	   p2.fireb.w = get_config_int ( sectn , "fireb_w" , 1 ) ;
	   p2.fireb.nbf = get_config_int ( sectn , "fireb_nbf" , 1 ) ;
	   p2.fireb.spd = get_config_int ( sectn , "fireb_spd" , 1 ) ;
	   p2.fireb.hit = get_config_int ( sectn , "fireb_hit" , 1 ) ;
	   p2.fireb.exec = get_config_string ( sectn , "fireb_exec" , "" ) ;
	   p2.fireb.dmg = get_config_int ( sectn , "fireb_dmg" , 3 ) ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.fireb.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.fireb.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.fireb.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }


   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.rush.flg = get_config_int ( sectn , "rush" , 0 ) ;

   if ( p2.rush.flg )
   {
	   p2.rush.w = get_config_int ( sectn , "rush_w" , 1 ) ;
	   p2.rush.nbf = get_config_int ( sectn , "rush_nbf" , 1 ) ;
	   p2.rush.spd = get_config_int ( sectn , "rush_spd" , 1 ) ;
	   p2.rush.hit = get_config_int ( sectn , "rush_hit" , 1 ) ;
	   p2.rush.rot = get_config_int ( sectn , "rush_rot" , 1 ) ;
	   p2.rush.pix = get_config_int ( sectn , "rush_pix" , 1 ) ;
	   p2.rush.exec = get_config_string ( sectn , "rush_exec" , "" ) ;
	   p2.rush.dmg = get_config_int ( sectn , "rush_dmg" , 3 ) ;
	   p2.rush.offx = get_config_int ( sectn , "rush_offx" , p2.rush.w ) ;
	   p2.rush.offy = get_config_int ( sectn , "rush_offy" , 10 ) ;


	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.rush.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.rush.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.rush.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }

   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.smove.flg = get_config_int ( sectn , "smove" , 0 ) ;

   if ( p2.smove.flg )
   {
	   p2.smove.w = get_config_int ( sectn , "smove_w" , 1 ) ;
	   p2.smove.nbf = get_config_int ( sectn , "smove_nbf" , 1 ) ;
	   p2.smove.spd = get_config_int ( sectn , "smove_spd" , 1 ) ;
	   p2.smove.hit = get_config_int ( sectn , "smove_hit" , 1 ) ;
	   p2.smove.rot = get_config_int ( sectn , "smove_rot" , 1 ) ;
	   p2.smove.exec = get_config_string ( sectn , "smove_exec" , "" ) ;
	   p2.smove.dmg = get_config_int ( sectn , "smove_dmg" , 3 ) ;
	   p2.smove.offx = get_config_int ( sectn , "smove_offx" , p2.smove.w ) ;
	   p2.smove.offy = get_config_int ( sectn , "smove_offy" , 10 ) ;


	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.smove.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.smove.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.smove.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }

   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.firebx.flg = get_config_int ( sectn , "firebx" , 0 ) ;

   if ( p2.firebx.flg )
   {
	   p2.fballx.w = get_config_int ( sectn , "fballx_w" , 1 ) ;
	   p2.fballx.nbf = get_config_int ( sectn , "fballx_nbf" , 1 ) ;
	   p2.fballx.spd = get_config_int ( sectn , "fballx_spd" , 1 ) ;
	   p2.fballx.pix = get_config_int ( sectn , "fballx_pix" , 1 ) ;
	   p2.fballx.offx = get_config_int ( sectn , "fballx_offx" , p2.fballx.w ) ;
	   p2.fballx.offy = get_config_int ( sectn , "fballx_offy" , 10 ) ;


	   fballx_h2 = get_config_int ( sectn , "fballx_h" , 1 ) ;
	   fballx_cx2 = get_config_int ( sectn , "fballx_cx" , 1 ) ;
	   fballx_cy2 = get_config_int ( sectn , "fballx_cy" , 1 ) ;

	   p2.firebx.w = get_config_int ( sectn , "firebx_w" , 1 ) ;
	   p2.firebx.nbf = get_config_int ( sectn , "firebx_nbf" , 1 ) ;
	   p2.firebx.spd = get_config_int ( sectn , "firebx_spd" , 1 ) ;
	   p2.firebx.hit = get_config_int ( sectn , "firebx_hit" , 1 ) ;
	   p2.firebx.exec = get_config_string ( sectn , "firebx_exec" , "" ) ;
	   p2.firebx.spec = get_config_int ( sectn , "firebx_spec" , 0 ) ;
	   p2.firebx.dmg = get_config_int ( sectn , "firebx_dmg"
										, p2.firebx.spec ? 5 : 3 ) ;

	   if(!p2.firebx.spec ) p2.firebx.sflg = 1 ;

	   else p2.firebx.sflg = 0 ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.firebx.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.firebx.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.firebx.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }


   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.rushx.flg = get_config_int ( sectn , "rushx" , 0 ) ;

   if ( p2.rushx.flg )
   {
	   p2.rushx.w = get_config_int ( sectn , "rushx_w" , 1 ) ;
	   p2.rushx.nbf = get_config_int ( sectn , "rushx_nbf" , 1 ) ;
	   p2.rushx.spd = get_config_int ( sectn , "rushx_spd" , 1 ) ;
	   p2.rushx.hit = get_config_int ( sectn , "rushx_hit" , 1 ) ;
	   p2.rushx.rot = get_config_int ( sectn , "rushx_rot" , 1 ) ;
	   p2.rushx.pix = get_config_int ( sectn , "rushx_pix" , 1 ) ;
	   p2.rushx.exec = get_config_string ( sectn , "rushx_exec" , "" ) ;
	   p2.rushx.spec = get_config_int ( sectn , "rushx_spec" , 0 ) ;
	   p2.rushx.dmg = get_config_int ( sectn , "rushx_dmg" ,p2.rushx.spec ? 5 : 3 ) ;
	   p2.rushx.offx = get_config_int ( sectn , "rushx_offx" , p2.rushx.w ) ;
	   p2.rushx.offy = get_config_int ( sectn , "rushx_offy" , 10 ) ;

	   if(!p2.rushx.spec ) p2.rushx.sflg = 1 ;

	   else p2.rushx.sflg = 0 ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.rushx.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.rushx.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.rushx.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }

   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p2.smovex.flg = get_config_int ( sectn , "smovex" , 0 ) ;

   if ( p2.smovex.flg )
   {
	   p2.smovex.w = get_config_int ( sectn , "smovex_w" , 1 ) ;
	   p2.smovex.nbf = get_config_int ( sectn , "smovex_nbf" , 1 ) ;
	   p2.smovex.spd = get_config_int ( sectn , "smovex_spd" , 1 ) ;
	   p2.smovex.hit = get_config_int ( sectn , "smovex_hit" , 1 ) ;
	   p2.smovex.rot = get_config_int ( sectn , "smovex_rot" , 1 ) ;
	   p2.smovex.exec = get_config_string ( sectn , "smovex_exec" , "" ) ;
	   p2.smovex.spec = get_config_int ( sectn , "smovex_spec" , 0 ) ;
	   p2.smovex.dmg = get_config_int ( sectn , "smovex_dmg" , p2.smovex.spec ? 5 : 3 ) ;
	   p2.smovex.offx = get_config_int ( sectn , "smovex_offx" , p2.smovex.w ) ;
	   p2.smovex.offy = get_config_int ( sectn , "smovex_offy" , 10 ) ;

	   if(!p2.smovex.spec ) p2.smovex.sflg = 1 ;

	   else p2.smovex.sflg = 0 ;


	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p2.smovex.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p2.smovex.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p2.smovex.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }


   tol2 = p2.width - p2.statik.defx ;



/******* LOAD SPRITES2 ****************************/


   textout_centre ( virtscreen , font ,
   "Loading..."  ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);
   rectfill(virtscreen, 40, 150,80,180, sel_player1);
   blit_KOF91();

   rectfill(virtscreen, 0, 0,virtscreen->w,virtscreen->h, makecol(255,0,255));
   //clear_to_color( virtscreen , makecol(255,0,255) ) ;

   if (sel2==sel1) clone = 1;

	else clone = 0;

if (debug)
allegro_message("loading sprites 2" );

  load_sprites2 () ;

  rectfill(virtscreen, 0, 0,virtscreen->w,virtscreen->h, makecol(0,0,0));
  //clear ( virtscreen );

if (debug)
allegro_message("sprites 2 loaded" );

   textout_centre ( virtscreen , font ,
   "Loading..." ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);
   rectfill(virtscreen, 40, 150,150,180, sel_player1);
   blit_KOF91();


/******* GET CHARACTER 1 INFO *********************/

   last_sel1 = sel1 ;

   strcpy ( sectn, charname[sel1] );

   strcpy ( passeur, DIR_SPRITES );
   strcat ( passeur, sectn );
   strcat ( passeur, DIR_BAR "char.ini" );

if (debug)
allegro_message("check char2 path :\n %s", passeur );

   override_config_file( passeur );


   p1.width = get_config_int ( sectn , "static_w" , 100 ) ;
   p1.spunch.w = get_config_int ( sectn , "spunch_w" , 150 ) ;
   p1.wpunch.w = get_config_int ( sectn , "wpunch_w" , 150 ) ;
   p1.skick.w = get_config_int ( sectn , "skick_w" , 150 ) ;
   p1.wkick.w = get_config_int ( sectn , "wkick_w" , 150 ) ;
   p1.apunch.w = get_config_int ( sectn , "apunch_w" , 150 ) ;
   p1.gpunch.w = get_config_int ( sectn , "gpunch_w" , 150 ) ;
   p1.akick.w = get_config_int ( sectn , "akick_w" , 150 ) ;
   p1.gkick.w = get_config_int ( sectn , "gkick_w" , 150 ) ;
   p1.intro.w = get_config_int ( sectn , "intro_w" , 150 ) ;
   p1.ko.w = get_config_int ( sectn , "ko_w" , 150 ) ;
   p1.outwin.w = get_config_int ( sectn , "outwin_w" , 150 ) ;

   p1.height = get_config_int ( sectn , "height" , 130 ) ;


   p1.statik.nbf = get_config_int ( sectn , "static_nbf" , 4 ) ;
   p1.walk.nbf = get_config_int ( sectn , "walk_nbf" , 5 ) ;
   p1.spunch.nbf = get_config_int ( sectn , "spunch_nbf" , 3 ) ;
   p1.wpunch.nbf = get_config_int ( sectn , "wpunch_nbf" , 3 ) ;
   p1.skick.nbf = get_config_int ( sectn , "skick_nbf" , 4 ) ;
   p1.wkick.nbf = get_config_int ( sectn , "wkick_nbf" , 4 ) ;
   p1.apunch.nbf = get_config_int ( sectn , "apunch_nbf" , 1 ) ;
   p1.gpunch.nbf = get_config_int ( sectn , "gpunch_nbf" , 4 ) ;
   p1.akick.nbf = get_config_int ( sectn , "akick_nbf" , 1 ) ;
   p1.gkick.nbf = get_config_int ( sectn , "gkick_nbf" , 4 ) ;

   p1.hurt.nbf = get_config_int ( sectn , "hurt_nbf" , 1 ) ;
   p1.ghurt.nbf = get_config_int ( sectn , "ghurt_nbf" , 1 ) ;

   p1.intro.nbf = get_config_int ( sectn , "intro_nbf" , 1 ) ;
   p1.outwin.nbf = get_config_int ( sectn , "outwin_nbf" , 1 ) ;
//   outlose_nbf1 = get_config_int ( sectn , "outlose_nbf" , 1 ) ;

   p1.statik.spd = get_config_int ( sectn , "static_spd" , 8 ) ;
   p1.walk.spd = get_config_int ( sectn , "walk_spd" , 8 ) ;
   p1.spunch.spd = get_config_int ( sectn , "spunch_spd" , 8 ) ;
   p1.wpunch.spd = get_config_int ( sectn , "wpunch_spd" , 8 ) ;
   p1.skick.spd = get_config_int ( sectn , "skick_spd" , 8 ) ;
   p1.wkick.spd = get_config_int ( sectn , "wkick_spd" , 8 ) ;
   p1.apunch.spd = get_config_int ( sectn , "apunch_spd" , 8 ) ;
   p1.gpunch.spd = get_config_int ( sectn , "gpunch_spd" , 8 ) ;
   p1.akick.spd = get_config_int ( sectn , "akick_spd" , 8 ) ;
   p1.gkick.spd = get_config_int ( sectn , "gkick_spd" , 8 ) ;
   p1.outwin.spd = get_config_int ( sectn , "outwin_spd" , 10 ) ;

   p1.spunch.hit = get_config_int ( sectn , "spunch_hit" , 2 ) ;
   p1.wpunch.hit = get_config_int ( sectn , "wpunch_hit" , 2 ) ;
   p1.skick.hit = get_config_int ( sectn , "skick_hit" , 2 ) ;
   p1.wkick.hit = get_config_int ( sectn , "wkick_hit" , 2 ) ;
   p1.apunch.hit = get_config_int ( sectn , "apunch_hit" , 1 ) ;
   p1.gpunch.hit = get_config_int ( sectn , "gpunch_hit" , 2 ) ;
   p1.akick.hit = get_config_int ( sectn , "akick_hit" , 1 ) ;
   p1.gkick.hit = get_config_int ( sectn , "gkick_hit" , 2 ) ;

   p1.spunch.dmg = get_config_int ( sectn , "spunch_dmg" , 2 ) ;
   p1.wpunch.dmg = get_config_int ( sectn , "wpunch_dmg" , 1 ) ;
   p1.skick.dmg = get_config_int ( sectn , "skick_dmg" , 2 ) ;
   p1.wkick.dmg = get_config_int ( sectn , "wkick_dmg" , 1 ) ;
   p1.apunch.dmg = get_config_int ( sectn , "apunch_dmg" , 1 ) ;
   p1.gpunch.dmg = get_config_int ( sectn , "gpunch_dmg" , 1 ) ;
   p1.akick.dmg = get_config_int ( sectn , "akick_dmg" , 2 ) ;
   p1.gkick.dmg = get_config_int ( sectn , "gkick_dmg" , 2 ) ;

   p1.spunch.offx = get_config_int ( sectn , "spunch_offx" , p1.spunch.w ) ;
   p1.wpunch.offx = get_config_int ( sectn , "wpunch_offx" , p1.wpunch.w ) ;
   p1.skick.offx = get_config_int ( sectn , "skick_offx" , p1.skick.w ) ;
   p1.wkick.offx = get_config_int ( sectn , "wkick_offx" , p1.wkick.w ) ;
   p1.apunch.offx = get_config_int ( sectn , "apunch_offx" , p1.apunch.w ) ;
   p1.gpunch.offx = get_config_int ( sectn , "gpunch_offx" , p1.gpunch.w ) ;
   p1.akick.offx = get_config_int ( sectn , "akick_offx" , p1.akick.w ) ;
   p1.gkick.offx = get_config_int ( sectn , "gkick_offx" , p1.gkick.w ) ;

   p1.spunch.offy = get_config_int ( sectn , "spunch_offy" , p1.height-50 ) ;
   p1.wpunch.offy = get_config_int ( sectn , "wpunch_offy" , p1.height-50 ) ;
   p1.skick.offy = get_config_int ( sectn , "skick_offy" , p1.height-50 ) ;
   p1.wkick.offy = get_config_int ( sectn , "wkick_offy" , p1.height-50 ) ;
   p1.apunch.offy = get_config_int ( sectn , "apunch_offy" , p1.height-50 ) ;
   p1.gpunch.offy = get_config_int ( sectn , "gpunch_offy" , p1.height-50 ) ;
   p1.akick.offy = get_config_int ( sectn , "akick_offy" , p1.height-50 ) ;
   p1.gkick.offy = get_config_int ( sectn , "gkick_offy" , p1.height-50 ) ;


   p1.statik.defx = get_config_int ( sectn , "static_defx" , p1.width ) ;
   p1.walk.defx = get_config_int ( sectn , "walk_defx" , p1.width ) ;
   p1.jump.defx = get_config_int ( sectn , "jump_defx" , p1.width ) ;
   p1.crouch.defx = get_config_int ( sectn , "crouch_defx" , p1.width ) ;
   p1.spunch.defx = get_config_int ( sectn , "spunch_defx" , p1.width ) ;
   p1.wpunch.defx = get_config_int ( sectn , "wpunch_defx" , p1.width ) ;
   p1.skick.defx = get_config_int ( sectn , "skick_defx" , p1.width ) ;
   p1.wkick.defx = get_config_int ( sectn , "wkick_defx" , p1.width ) ;
   p1.apunch.defx = get_config_int ( sectn , "apunch_defx" , p1.width ) ;
   p1.gpunch.defx = get_config_int ( sectn , "gpunch_defx" , p1.width ) ;
   p1.akick.defx = get_config_int ( sectn , "akick_defx" , p1.width ) ;
   p1.gkick.defx = get_config_int ( sectn , "gkick_defx" , p1.width ) ;

   p1.statik.defy = get_config_int ( sectn , "static_defy" , p1.height-50 ) ;
   p1.walk.defy = get_config_int ( sectn , "walk_defy" , p1.height-50 ) ;
   p1.jump.defy = get_config_int ( sectn , "jump_defy" , p1.height-50 ) ;
   p1.crouch.defy = get_config_int ( sectn , "crouch_defy" , p1.height-50 ) ;
   p1.spunch.defy = get_config_int ( sectn , "spunch_defy" , p1.height-50 ) ;
   p1.wpunch.defy = get_config_int ( sectn , "wpunch_defy" , p1.height-50 ) ;
   p1.skick.defy = get_config_int ( sectn , "skick_defy" , p1.height-50 ) ;
   p1.wkick.defy = get_config_int ( sectn , "wkick_defy" , p1.height-50 ) ;
   p1.apunch.defy = get_config_int ( sectn , "apunch_defy" , p1.height-50 ) ;
   p1.gpunch.defy = get_config_int ( sectn , "gpunch_defy" , p1.height-50 ) ;
   p1.akick.defy = get_config_int ( sectn , "akick_defy" , p1.height-50 ) ;
   p1.gkick.defy = get_config_int ( sectn , "gkick_defy" , p1.height-50 ) ;


   female1 = get_config_int ( sectn , "female" , 0 ) ;


   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.fireb.flg = get_config_int ( sectn , "fireb" , 0 ) ;

   if ( p1.fireb.flg )
   {
	   fball_h1 = get_config_int ( sectn , "fball_h" , 1 ) ;
	   p1.fball.w = get_config_int ( sectn , "fball_w" , 1 ) ;
	   p1.fball.nbf = get_config_int ( sectn , "fball_nbf" , 1 ) ;
	   p1.fball.spd = get_config_int ( sectn , "fball_spd" , 1 ) ;
	   p1.fball.pix = get_config_int ( sectn , "fball_pix" , 1 ) ;
	   fball_cx1 = get_config_int ( sectn , "fball_cx" , 1 ) ;
	   fball_cy1 = get_config_int ( sectn , "fball_cy" , 1 ) ;
	   p1.fball.offx = get_config_int ( sectn , "fball_offx" , p1.fball.w ) ;
	   p1.fball.offy = get_config_int ( sectn , "fball_offy" , 10 ) ;

	   p1.fireb.w = get_config_int ( sectn , "fireb_w" , 1 ) ;
	   p1.fireb.nbf = get_config_int ( sectn , "fireb_nbf" , 1 ) ;
	   p1.fireb.spd = get_config_int ( sectn , "fireb_spd" , 1 ) ;
	   p1.fireb.hit = get_config_int ( sectn , "fireb_hit" , 1 ) ;
	   p1.fireb.exec = get_config_string ( sectn , "fireb_exec" , "" ) ;
	   p1.fireb.dmg = get_config_int ( sectn , "fireb_dmg" , 3 ) ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.fireb.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.fireb.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.fireb.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }



   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.rush.flg = get_config_int ( sectn , "rush" , 0 ) ;

   if ( p1.rush.flg )
   {
	   p1.rush.w = get_config_int ( sectn , "rush_w" , 1 ) ;
	   p1.rush.nbf = get_config_int ( sectn , "rush_nbf" , 1 ) ;
	   p1.rush.spd = get_config_int ( sectn , "rush_spd" , 1 ) ;
	   p1.rush.hit = get_config_int ( sectn , "rush_hit" , 1 ) ;
	   p1.rush.rot = get_config_int ( sectn , "rush_rot" , 1 ) ;
	   p1.rush.pix = get_config_int ( sectn , "rush_pix" , 1 ) ;
	   p1.rush.exec = get_config_string ( sectn , "rush_exec" , "" ) ;
	   p1.rush.dmg = get_config_int ( sectn , "rush_dmg" , 3 ) ;
	   p1.rush.offx = get_config_int ( sectn , "rush_offx" , p1.rush.w ) ;
	   p1.rush.offy = get_config_int ( sectn , "rush_offy" , 10 ) ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.rush.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.rush.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.rush.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }



   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.smove.flg = get_config_int ( sectn , "smove" , 0 ) ;

   if ( p1.smove.flg )
   {
	   p1.smove.w = get_config_int ( sectn , "smove_w" , 1 ) ;
	   p1.smove.nbf = get_config_int ( sectn , "smove_nbf" , 1 ) ;
	   p1.smove.spd = get_config_int ( sectn , "smove_spd" , 1 ) ;
	   p1.smove.hit = get_config_int ( sectn , "smove_hit" , 1 ) ;
	   p1.smove.rot = get_config_int ( sectn , "smove_rot" , 1 ) ;
	   p1.smove.exec = get_config_string ( sectn , "smove_exec" , "" ) ;
	   p1.smove.dmg = get_config_int ( sectn , "smove_dmg" , 3 ) ;
	   p1.smove.offx = get_config_int ( sectn , "smove_offx" , p1.smove.w ) ;
	   p1.smove.offy = get_config_int ( sectn , "smove_offy" , 10 ) ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.smove.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.smove.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.smove.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }

// SPECIALS

   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.firebx.flg = get_config_int ( sectn , "firebx" , 0 ) ;

   if ( p1.firebx.flg )
   {
	   fballx_h1 = get_config_int ( sectn , "fballx_h" , 1 ) ;
	   p1.fballx.w = get_config_int ( sectn , "fballx_w" , 1 ) ;
	   p1.fballx.nbf = get_config_int ( sectn , "fballx_nbf" , 1 ) ;
	   p1.fballx.spd = get_config_int ( sectn , "fballx_spd" , 1 ) ;
	   p1.fballx.pix = get_config_int ( sectn , "fballx_pix" , 1 ) ;
	   fballx_cx1 = get_config_int ( sectn , "fballx_cx" , 1 ) ;
	   fballx_cy1 = get_config_int ( sectn , "fballx_cy" , 1 ) ;
	   p1.fballx.offx = get_config_int ( sectn , "fballx_offx" , p1.fballx.w ) ;
	   p1.fballx.offy = get_config_int ( sectn , "fballx_offy" , 10 ) ;

	   p1.firebx.w = get_config_int ( sectn , "firebx_w" , 1 ) ;
	   p1.firebx.nbf = get_config_int ( sectn , "firebx_nbf" , 1 ) ;
	   p1.firebx.spd = get_config_int ( sectn , "firebx_spd" , 1 ) ;
	   p1.firebx.hit = get_config_int ( sectn , "firebx_hit" , 1 ) ;
	   p1.firebx.exec = get_config_string ( sectn , "firebx_exec" , "" ) ;
	   p1.firebx.spec = get_config_int ( sectn , "firebx_spec" , 0 ) ;
	   p1.firebx.dmg = get_config_int ( sectn , "firebx_dmg" , p1.firebx.spec ? 5 : 3 ) ;


	   if(!p1.firebx.spec ) p1.firebx.sflg = 1 ;

	   else p1.firebx.sflg = 0 ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.firebx.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.firebx.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.firebx.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }



   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.rushx.flg = get_config_int ( sectn , "rushx" , 0 ) ;

   if ( p1.rushx.flg )
   {
	   p1.rushx.w = get_config_int ( sectn , "rushx_w" , 1 ) ;
	   p1.rushx.nbf = get_config_int ( sectn , "rushx_nbf" , 1 ) ;
	   p1.rushx.spd = get_config_int ( sectn , "rushx_spd" , 1 ) ;
	   p1.rushx.hit = get_config_int ( sectn , "rushx_hit" , 1 ) ;
	   p1.rushx.rot = get_config_int ( sectn , "rushx_rot" , 1 ) ;
	   p1.rushx.pix = get_config_int ( sectn , "rushx_pix" , 1 ) ;
	   p1.rushx.exec = get_config_string ( sectn , "rushx_exec" , "" ) ;
	   p1.rushx.spec = get_config_int ( sectn , "rushx_spec" , 0 ) ;
	   p1.rushx.dmg = get_config_int ( sectn , "rushx_dmg" , p1.rushx.spec ? 5 : 3 ) ;
	   p1.rushx.offx = get_config_int ( sectn , "rushx_offx" , p1.rushx.w ) ;
	   p1.rushx.offy = get_config_int ( sectn , "rushx_offy" , 10 ) ;

	   if(!p1.rushx.spec ) p1.rushx.sflg = 1 ;

	   else p1.rushx.sflg = 0 ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.rushx.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.rushx.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.rushx.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }



   for ( i = 0 ; i<2 ; i++) temp_strg[i] = 0 ;

   p1.smovex.flg = get_config_int ( sectn , "smovex" , 0 ) ;

   if ( p1.smovex.flg )
   {
	   p1.smovex.w = get_config_int ( sectn , "smovex_w" , 1 ) ;
	   p1.smovex.nbf = get_config_int ( sectn , "smovex_nbf" , 1 ) ;
	   p1.smovex.spd = get_config_int ( sectn , "smovex_spd" , 1 ) ;
	   p1.smovex.hit = get_config_int ( sectn , "smovex_hit" , 1 ) ;
	   p1.smovex.rot = get_config_int ( sectn , "smovex_rot" , 1 ) ;
	   p1.smovex.exec = get_config_string ( sectn , "smovex_exec" , "" ) ;
	   p1.smovex.spec = get_config_int ( sectn , "smovex_spec" , 0 ) ;
	   p1.smovex.dmg = get_config_int ( sectn , "smovex_dmg" , p1.smovex.spec ? 5 : 3 ) ;
	   p1.smovex.offx = get_config_int ( sectn , "smovex_offx" , p1.smovex.w ) ;
	   p1.smovex.offy = get_config_int ( sectn , "smovex_offy" , 10 ) ;

	   if(!p1.smovex.spec ) p1.smovex.sflg = 1 ;

	   else p1.smovex.sflg = 0 ;

	   cx=1 ; nb = 0 ; i=0 ; y = 0;

	   while ( cx )
	   {
			c = p1.smovex.exec[i] ;

			if ( c != ' ' )
			{
				if ( c == '*' ) { p1.smovex.end = nb ; cx = 0 ; }

				temp_strg[y] = c  ;
				y++;
			}
			else
			{
				p1.smovex.seq[nb] = get_kstatus ( temp_strg ) ;
				nb++ ;
				y = 0 ;
			}

			i++ ;
	   }

   }


   tol1 = ( p1.statik.defx ) ;



/*********** LOAD SPRITES1 *********************/

   textout_centre ( virtscreen , font ,
   "Loading..."  ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);
   rectfill(virtscreen, 40, 150,190,180, sel_player1);
   blit_KOF91();

   load_sprites1 () ;

   textout_centre ( virtscreen , font ,
   "Loading..."  ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);
   rectfill(virtscreen, 40, 150,250,180, sel_player1);
   blit_KOF91();

   override_config_file( "bkgds\\bkgd.ini" );


/*********** ALL SPRITES ARE NOW IN MEMORY ********/


   /* LOADING THE BACKGROUND NOW */

foreground = animated = 0 ;

   strcpy ( file , "bkgds\\" );
   strcat ( file , bkgdname[bgd] );

if (debug)
allegro_message("check bkgd path (again) :\n %s", file );

          if ((Bkgd = load_pcx( file , 0 ))==NULL)
		  {
			  if ( (open_fli ( file ) != FLI_OK) )
			  {
				  allegro_message("failed loading Bkgd");
				  allegro_exit();
				  exit(0);
			  }
			  else
			  {
				  reset_fli_variables() ;
				  next_fli_frame(1) ;
				  set_palette( fli_palette );
				  Bkgd = fli_bitmap ;
				  animated = 1 ;
				  foreground = 0 ;
			  }
		  }

		  else
		  {
			  animated = 0 ;

			  strcpy ( file , "bkgds\\frgds\\" );
			  strcat ( file , bkgdname[bgd] );

			  if ((Frgd = load_pcx( file , 0 ))==NULL)
			  foreground = 0 ;

			  else foreground = 1 ;

		  }



	  bkgdlim = Bkgd->w - 320 ;

      zoomsup = Bkgd->h - 200 ;

      bkgd_delay = 15 ;

	zoomscreen = create_bitmap( Bkgd->w, Bkgd->h) ;


/* BACKGROUND LOADED */

	  y = ustrlen( bkgdname[bgd] );

      strcpy ( file , "midi\\" );

      ustrncat( file , bkgdname[bgd], y-4 );

      strcat ( file , ".mid" );

      if ( (music = load_midi ( file ))==NULL)
	  {
			allegro_message("failed loading midi");
			allegro_exit();	
			exit(0);
	  }


/* MIDI FILE LOADED */

   override_config_file( "game.ini" );

   LBbanner = load_pcx( "sprites\\lifebars.pcx", 0);

   Bar = load_pcx( "sprites\\bar.pcx", 0);

   draw_sprite_h_flip (Bar2, Bar, 0, 0) ; 

   novic0 = load_pcx ( "sprites\\novic0.pcx", 0 );
   novic1 = load_pcx ( "sprites\\novic1.pcx", 0 );

   avic0 = load_pcx ( "sprites\\vic0.pcx", 0 );
   avic1 = load_pcx ( "sprites\\vic1.pcx", 0 );

   Blood = load_pcx ( "sprites\\lastpal.pcx", Pal );


   Carton_R1 = load_pcx("sprites\\round1.pcx", 0); 

   Carton_R2 = load_pcx("sprites\\round2.pcx", 0); 

   Carton_R3 = load_pcx("sprites\\round3.pcx", 0); 

   Carton_FT = load_pcx("sprites\\fight.pcx", 0); 
   
   Carton_WN = load_pcx("sprites\\winner.pcx", 0); 

   Carton_KO = load_pcx("sprites\\ko.pcx", 0); 

   Power = load_pcx("sprites\\power.pcx", 0); 

   PBar = load_pcx("sprites\\pbar.pcx", 0); 

#if TCLOCK == 1   
   B_clock[0] = load_pcx("sprites\\0.pcx",0);
   B_clock[1] = load_pcx("sprites\\1.pcx",0);
   B_clock[2] = load_pcx("sprites\\2.pcx",0);
   B_clock[3] = load_pcx("sprites\\3.pcx",0);
   B_clock[4] = load_pcx("sprites\\4.pcx",0);
   B_clock[5] = load_pcx("sprites\\5.pcx",0);
   B_clock[6] = load_pcx("sprites\\6.pcx",0);
   B_clock[7] = load_pcx("sprites\\7.pcx",0);
   B_clock[8] = load_pcx("sprites\\8.pcx",0);
   B_clock[9] = load_pcx("sprites\\9.pcx",0);
#endif   

#if PERFECT == 1
   B_prfct = load_pcx("sprites\\perfect.pcx",0);
#endif

   textout_centre ( virtscreen , font ,
   "Loading..."  ,160 , 120 , sel_player1 ) ;
   rect(virtscreen, 40, 150,280,180, sel_player1);
   rectfill(virtscreen, 40, 150,280,180, sel_player1);
   blit_KOF91();

   /* some more character info that depends from zoomsup */

   sol2 = (zoomsup + 185) - p2.height ;

   jump_how2 = (zoomsup + 125) - p2.height ;

   sol1 = (zoomsup + 185) - p1.height ;

   jump_how1 = (zoomsup + 125) - p1.height ;

   /******/

   cx = (int)(bkgdlim/2) ;

   cy = zoomsup ;

   zoomwidth = 320 ;

   spr1x = ((cx+75) - (p1.width/2)) , spr1y = sol1 ;
   spr2x = ((cx+250) - (p2.width/2)), spr2y = sol2 ;

   fx = fy = 1 ;

   flag = flag2 = 1 ;

   fball1 = fball2 = 0 ;
   fballx1 = fballx2 = 0 ;

   spower1 = spower2 = 0 ;

   p1.fireb.succ = p2.fireb.succ = 0 ;
   p1.rush.succ = p2.rush.succ = 0 ;
   rush_stop1 = rush_stop2 = 0 ;
   smove_stop1 = smove_stop2 = 0 ;

   p1.firebx.succ = p2.firebx.succ = 0 ;
   p1.rushx.succ = p2.rushx.succ = 0 ;
   rushx_stop1 = rushx_stop2 = 0 ;
   smovex_stop1 = smovex_stop2 = 0 ;

   p1.wimpact_ct = p2.wimpact_ct = 0 ;
   p1.simpact_ct = p2.simpact_ct = 0 ;
   p1.bimpact_ct = p2.bimpact_ct = 0 ;

   damag1 = damag2 = 1 ;

   autho_ht1 = autho_ht2 = 0 ;

   life1 = life2 = 501 ;

#if TCLOCK == 1 
   clock_frame = 0;
   t_clock = t_clock_ini;
   sprintf(s_cron,"%d",t_clock);
   c_dez =  s_cron[0]-48;
   c_uni =  s_cron[1]-48;
#endif

   deb = 1 ;

   round = 1 ;
   vic1 = vic2 = 0 ;

   vsynch = 1 ;

   ok = 1 ;

   vic_flag = 1 ;

//   set_palette (black_palette) ;


set_palette( fli_palette );

//   freesounds () ;

//   loadsounds () ;


   clear(screen);




   /* Display the background (not necessary) */

   if (animated)
   {
     reset_fli_variables() ;

     next_fli_frame(1) ;

     blit ( fli_bitmap , virtscreen , cx , zoomsup , 0, 0, 320, 200 ) ;
   }

   else

   blit ( Bkgd, virtscreen , cx , zoomsup , 0, 0, 320, 200 ) ;


   blit_KOF91();


   play_midi (music , 1) ;

   set_palette( fli_palette );


/********* THE REAL GAME STARTS HERE ***********************/


   /* Loop until we're asked to exit */
   while( ok )
   {
      /* Round beginning Loop */

      while ( deb )
      {
        if ( (round == 1) && (!checkint) )

        {
			retrace_count = 0 ;

                if (p1.intro.nbf<p2.intro.nbf)

                for ( flag = 1 ; flag < (p2.intro.nbf*9) ; ++flag )

                {
                        //vsync() ;

						if (animated)bkgd_counter++;

						if (retrace_count>2) retrace_count = 0 ;

						retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;

						blit_KOF91();

						blit_Bkgd() ;

						while (!retrace_count) ;

					}

                        flag2++ ;

                        if ( flag2 > ((p1.statik.spd*p1.statik.nbf)-1) ) flag2 = 0 ;

                        if ( flag<(p1.intro.nbf*9) )

                        GKick ( (int)(flag/9) , spr1x , spr1y , 2) ;

                        else  GStatic( (int)(flag2/p1.statik.spd), spr1x , spr1y ) ;

                        GKick2 ( (int)(flag/9) , spr2x-(p2.intro.w-p2.width) , spr2y , 2) ;
                }

                else

                for ( flag = 1 ; flag < (p1.intro.nbf*9) ; ++flag )

                {
                        //vsync() ;

						if (retrace_count>2) retrace_count = 0 ;

						if (animated)bkgd_counter++;

						retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
							
						blit_KOF91();

						blit_Bkgd() ;

						while (!retrace_count) ;
						
					}


                        flag2++ ;

                        if ( flag2 > ((p2.statik.spd*p2.statik.nbf)-1) ) flag2 = 0 ;

                        if ( flag<(p2.intro.nbf*9) )

                        GKick2 ( (int)(flag/9) , spr2x-(p2.intro.w-p2.width) , spr2y , 2) ;

                        else  GStatic2( (int)(flag2/p2.statik.spd), spr2x , spr2y ) ;

                        GKick ( (int)(flag/9) , spr1x , spr1y , 2) ;

                }

                checkint = 1 ;

        }

	deb++;

	if ( deb == 2 ) { load_kick1 () ;load_kick2 () ; retrace_count = 0 ;}

	if ( deb == 50 ) Round (round) ;
     
	if ( deb == 240 ) Fight() ;


	/* Update the real screen */

        //vsync() ;

	if (animated) bkgd_counter++;

	retrace_count-- ;

	if (retrace_count>0) ;

	else
	{
		blit_KOF91();

        blit_Bkgd() ;

        GStatic2( (int)(flag2/p2.statik.spd), spr2x , spr2y ) ;

		GStatic( (int)(flag/p1.statik.spd), spr1x , spr1y ) ;

		if (foreground)
		masked_blit ( Frgd, zoomscreen , 
		0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

        blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;

		while (!retrace_count) ;
	}

		flag++;

        flag2++;

	    if (flag > ((p1.statik.spd*p1.statik.nbf)-1) ) flag = 0 ;

        if (flag2 > ((p2.statik.spd*p2.statik.nbf)-1) ) flag2 = 0 ;



	if ( (deb > 50) && (deb < 150) )
	{
		if (deb <76) angle = (deb - 50) * (Carton_R1->w/25) ;
		else
		if (deb >124) angle = (25-(deb - 125)) * (Carton_R1->w/25) ;
		else angle = Carton_R1->w ;

		switch ( round )

		{
			case 1 : stretch_sprite (  virtscreen ,Carton_R1 
						 , (320-angle)/2 
						 , (200-Carton_R1->h)/2 
						 , angle , Carton_R1->h);
					break;

			case 2 : stretch_sprite (  virtscreen ,Carton_R2 
						 , (320-angle)/2 
						 , (200-Carton_R2->h)/2 
						 , angle , Carton_R2->h);
					break;

			case 3 : stretch_sprite (  virtscreen ,Carton_R3
						 , (320-angle)/2 
						 , (200-Carton_R3->h)/2 
						 , angle , Carton_R3->h);
					break;

			default : stretch_sprite (  virtscreen ,Carton_R1 
						 , (320-angle)/2 
						 , (200-Carton_R1->h)/2 
						 , angle , Carton_R1->h);
					break;
		}
	}


	else

	if ( (deb > 240) && (deb < 320) )
	{
		masked_blit ( Carton_FT , virtscreen , 0 , 0
						 , (320-Carton_FT->w)/2 
						 , (200-Carton_FT->h)/2 
						 , Carton_FT->w ,Carton_FT->h) ;
	}

	else

	if ( deb > 320 )

	{
	        if (!nbpar)
	        {
		        /* Frame count starts at 0 */
		        n = 0;

		        //t1 = time( 0 );

				clear_keybuf();

				poll_joystick();
            }

            deb = 0 ;

			retrace_count = 0 ;
    }

  }

      

  if (frameskip<skp_flag) frameskip++;
	  
  else frameskip = 0 ; 

  if (animated) bkgd_counter++;

  if (retrace_count>4) retrace_count = 0 ;

  retrace_count-- ;

// the big drawing loop

if (retrace_count > 0) ;

else
if (!frameskip)
{

	  /* Increment the frame count */
      if (!nbpar) n++;

	  vic_flag++;
	  if (vic_flag>40) vic_flag = 1 ;

      blit_KOF91();

	  blit_Bkgd() ;

      /* Invert Sprite priority when hurt */

      switch ( actual2 )

      {
      case  8: GSingle2 ( 0, spr2x , spr2y );
		 break;
      case 18: GSingle2 ( 2, spr2x , spr2y );
		 break;

      case  9: GHurt2 ( (hflag2/(25/p2.hurt.nbf)) , spr2x , spr2y );
               break;
      case 19: GGHurt2 ( (hflag2/(25/p2.ghurt.nbf)) , spr2x , spr2y );
               break;
      case 20: GKO2 ( (hflag2/7), (spr2x- (p2.ko.w - p2.width)) , spr2y ,(p2.ko.w - p2.width) );
               break;
      }

      // Draw the sprites 

      if (ak) 
	  {
		  GAKick ( (ak/p1.akick.spd), spr1x , spr1y ) ;
		  p1.limdefx = p1.akick.defx ;
		  p1.limdefy = p1.akick.defy ;
	  }

      else
	  if (apch) 
	  {
		  GAPunch ( (apch/p1.apunch.spd), spr1x , spr1y ) ;
		  p1.limdefx = p1.apunch.defx ;
		  p1.limdefy = p1.apunch.defy ;
	  }

      else

      switch ( actual )

      {
		case  0: GStatic( (flag/p1.statik.spd), spr1x , spr1y ) ;
				 p1.limdefx = p1.statik.defx ;
				 p1.limdefy = p1.statik.defy ;
				 break;

		case 111 :
		case  1: GWalk( (flag/p1.walk.spd), spr1x , spr1y ) ;
				 p1.limdefx = p1.walk.defx ;
				 p1.limdefy = p1.walk.defy ;
			 break;

		case  2: GSingle( 1, spr1x , spr1y );
				 p1.limdefx = p1.crouch.defx ;
				 p1.limdefy = p1.crouch.defy ;
			 break;

		case  3:
		case  4:
		case  5: if (spr1y < (sol1-50) ) GJump( 3, spr1x , spr1y );

					 else

					 if (sens1)
					 {
					   if (spr1y < (sol1-20) )
					   GJump( 4, spr1x , spr1y );
					   else
					   GJump( 5, spr1x , spr1y );
					 }

					 else

					 {
					   if (spr1y < (sol1-20) )
					   GJump( 2, spr1x , spr1y );
					   else
					   GJump( 1, spr1x , spr1y );
					 }

				 p1.limdefx = p1.jump.defx ;
				 p1.limdefy = p1.jump.defy ;

			 break;

		case 30: GFireB( (flag/p1.fireb.spd), spr1x , spr1y );
			 break;
		case 40: GRush( (flag/p1.rush.spd), spr1x , spr1y );
			 break;
		case 50: GSMove( (flag/p1.smove.spd), spr1x , spr1y );
			 break;

		case 31: GFireBX( (flag/p1.firebx.spd), spr1x , spr1y );
			 break;
		case 41: GRushX( (flag/p1.rushx.spd), spr1x , spr1y );
			 break;
		case 51: GSMoveX( (flag/p1.smovex.spd), spr1x , spr1y );
			 break;

		case  6: GPunch( (flag/p1.spunch.spd), spr1x , spr1y );
				 p1.limdefx = p1.spunch.defx ;
				 p1.limdefy = p1.spunch.defy ;
			 break;
		case 61: GWPunch( (flag/p1.wpunch.spd), spr1x , spr1y );
				 p1.limdefx = p1.wpunch.defx ;
				 p1.limdefy = p1.wpunch.defy ;
			 break;

		case  7: GKick( (flag/p1.skick.spd), spr1x , spr1y , 1 );
				 p1.limdefx = p1.skick.defx ;
				 p1.limdefy = p1.skick.defy ;
			 break;
		case 71: GWKick( (flag/p1.wkick.spd), spr1x , spr1y );
				 p1.limdefx = p1.wkick.defx ;
				 p1.limdefy = p1.wkick.defy ;
			 break;

		case 10: GGKick( (flag/p1.gkick.spd), spr1x , spr1y );
				 p1.limdefx = p1.gkick.defx ;
				 p1.limdefy = p1.gkick.defy ;
			 break;
		case 13: GGPunch( (flag/p1.gpunch.spd), spr1x , spr1y );
				 p1.limdefx = p1.gpunch.defx ;
				 p1.limdefy = p1.gpunch.defy ;
			 break;

		case  8: GSingle ( 0, spr1x , spr1y );
			 break;
		case 18: GSingle ( 2, spr1x , spr1y );
			 break;
		case  9: GHurt ((hflag/(25/p1.hurt.nbf)) , spr1x , spr1y );
					 break;
		case 19: GGHurt ( (hflag/(25/p1.ghurt.nbf)) , spr1x , spr1y );
					 break;
		case 20: GKO ( (hflag/7), spr1x , spr1y );
				   break;
      }



      if (ak2) 
	  {
		  GAKick2( (ak2/p2.akick.spd), (spr2x - (p2.akick.w - p2.width) ) , spr2y ) ;
		  p2.limdefx = p2.akick.defx ;
		  p2.limdefy = p2.akick.defy ;
	  }
      else

	  if (apch2) 
	  {
		  GAPunch2( (apch2/p2.apunch.spd), (spr2x - (p2.apunch.w - p2.width) ) , spr2y ) ;
		  p2.limdefx = p2.apunch.defx ;
		  p2.limdefy = p2.apunch.defy ;
	  }

	  else
      switch ( actual2 )
      {
		case  0: GStatic2 ( (flag2/p2.statik.spd), spr2x , spr2y );
				 p2.limdefx = p2.statik.defx ;
				 p2.limdefy = p2.statik.defy ;
			break;

		case 111 :
		case  1: GWalk2( (flag2/p2.walk.spd), spr2x , spr2y );
				 p2.limdefx = p2.walk.defx ;
				 p2.limdefy = p2.walk.defy ;
			 break;

		case  2: GSingle2( 1, spr2x , spr2y );
				 p2.limdefx = p2.crouch.defx ;
				 p2.limdefy = p2.crouch.defy ;
			 break;

		case  3:
		case  4:
		case  5: if (spr2y < (sol2-50) ) GJump2( 3, spr2x , spr2y );

					 else

					 if (sens2)
					 {
					   if (spr2y < (sol2-20) )
					   GJump2( 4, spr2x , spr2y );
					   else
					   GJump2( 5, spr2x , spr2y );
					 }

					 else

					 {
					   if (spr2y < (sol2-20) )
					   GJump2( 2, spr2x , spr2y );
					   else
					   GJump2( 1, spr2x , spr2y );
					 }

				 p2.limdefx = p2.crouch.defx ;
				 p2.limdefy = p2.crouch.defy ;

			 break;

		case 30: GFireB2( (flag2/p2.fireb.spd), spr2x - (p2.fireb.w - p2.width), spr2y );
			 break;
		case 40: GRush2( (flag2/p2.rush.spd), spr2x - (p2.rush.w - p2.width), spr2y );
			 break;
		case 50: GSMove2( (flag2/p2.smove.spd), spr2x - (p2.smove.w - p2.width), spr2y );
			 break;

		case 31: GFireBX2( (flag2/p2.firebx.spd), spr2x - (p2.firebx.w - p2.width), spr2y );
			 break;
		case 41: GRushX2( (flag2/p2.rushx.spd), spr2x - (p2.rushx.w - p2.width), spr2y );
			 break;
		case 51: GSMoveX2( (flag2/p2.smovex.spd), spr2x - (p2.smovex.w - p2.width), spr2y );
			 break;

		case  6: GPunch2( (flag2/p2.spunch.spd), spr2x - (p2.spunch.w - p2.width), spr2y );
				 p2.limdefx = p2.spunch.defx ;
				 p2.limdefy = p2.spunch.defy ;
			 break;
		case 61: GWPunch2( (flag2/p2.wpunch.spd), spr2x - (p2.wpunch.w - p2.width), spr2y );
				 p2.limdefx = p2.wpunch.defx ;
				 p2.limdefy = p2.wpunch.defy ;
			 break;

		case  7: GKick2( (flag2/p2.skick.spd), spr2x - (p2.skick.w - p2.width), spr2y , 1 );
				 p2.limdefx = p2.skick.defx ;
				 p2.limdefy = p2.skick.defy ;
			 break;
		case 71: GWKick2( (flag2/p2.wkick.spd), spr2x - (p2.wkick.w - p2.width), spr2y );
				 p2.limdefx = p2.wkick.defx ;
				 p2.limdefy = p2.wkick.defy ;
			 break;

		case 10: GGKick2( (flag2/p2.gkick.spd), spr2x - (p2.gkick.w - p2.width), spr2y );
				 p2.limdefx = p2.gkick.defx ;
				 p2.limdefy = p2.gkick.defy ;
			 break;
		case 13: GGPunch2( (flag2/p2.gpunch.spd), spr2x - (p2.gpunch.w - p2.width), spr2y );
				 p2.limdefx = p2.gpunch.defx ;
				 p2.limdefy = p2.gpunch.defy ;
			 break;

    }

	if (fball1) GFBall ( (fball_flg1/p1.fball.spd) , fb1x , fb1y ) ;

	if (fball2) GFBall2 ( (fball_flg2/p2.fball.spd) , fb2x , fb2y ) ;

	if (fballx1) GFBallX ( (fballx_flg1/p1.fballx.spd) , fbx1x , fbx1y ) ;

	if (fballx2) GFBallX2 ( (fballx_flg2/p2.fballx.spd) , fbx2x , fbx2y ) ;

	// impacts

	if (p1.wimpact_ct)
	{
		p1.wimpact_ct-- ;

		misc = (11 - p1.wimpact_ct) / ( 10 / wimpact_nbf ) ;

		if (misc > wimpact_nbf) { misc = wimpact_nbf ; p1.wimpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, WImpact[misc],
				  p1.limoffx - (WImpact[misc]->w / 2),
				  p1.limoffy - (WImpact[misc]->h / 2));
	}

	if (p2.wimpact_ct)
	{
		p2.wimpact_ct-- ;

		misc = (11 - p2.wimpact_ct) / ( 10 / wimpact_nbf ) ;

		if (misc > wimpact_nbf) { misc = wimpact_nbf ; p2.wimpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		B_Temp = create_bitmap(WImpact[misc]->w, WImpact[misc]->h);
		rectfill(B_Temp, 0, 0, WImpact[misc]->w, WImpact[misc]->h,
			 makecol(255, 0, 255));
		draw_sprite_h_flip(B_Temp, WImpact[misc], 0, 0);
		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, B_Temp,
				  p2.limoffx - (WImpact[misc]->w / 2),
				  p2.limoffy - (WImpact[misc]->h / 2));

	}


	if (p1.simpact_ct)
	{
		p1.simpact_ct-- ;

		misc = (11 - p1.simpact_ct) / ( 10 / simpact_nbf ) ;

		if (misc > simpact_nbf) { misc = simpact_nbf ; p1.simpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, SImpact[misc],
				  p1.limoffx - (SImpact[misc]->w / 2),
				  p1.limoffy - (SImpact[misc]->h / 2));
	}

	if (p2.simpact_ct)
	{
		p2.simpact_ct-- ;

		misc = (11 - p2.simpact_ct) / ( 10 / simpact_nbf ) ;

		if (misc > simpact_nbf) { misc = simpact_nbf ; p2.simpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		B_Temp = create_bitmap(SImpact[misc]->w,SImpact[misc]->h);
		rectfill(B_Temp, 0, 0, SImpact[misc]->w, SImpact[misc]->h,
			 makecol(255, 0, 255));
		draw_sprite_h_flip(B_Temp, SImpact[misc], 0, 0);
		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, B_Temp,
				  p2.limoffx - (SImpact[misc]->w / 2),
				  p2.limoffy - (SImpact[misc]->h / 2));
	}


	if (p1.bimpact_ct)
	{
		p1.bimpact_ct-- ;

		misc = (11 - p1.bimpact_ct) / ( 10 / bimpact_nbf ) ;

		if (misc > bimpact_nbf) { misc = bimpact_nbf ; p1.bimpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, BlockImpact[misc],
				  p1.limoffx - (BlockImpact[misc]->w / 2),
				  p1.limoffy - (BlockImpact[misc]->h / 2));
	}

	if (p2.bimpact_ct)
	{
		p2.bimpact_ct-- ;

		misc = (11 - p2.bimpact_ct) / ( 10 / bimpact_nbf ) ;

		if (misc > bimpact_nbf) { misc = bimpact_nbf ; p2.bimpact_ct = 0 ; }

		if (misc == 0) misc = 1 ;

		B_Temp = create_bitmap(BlockImpact[misc]->w,BlockImpact[misc]->h);
		rectfill(B_Temp, 0, 0, BlockImpact[misc]->w,
			 BlockImpact[misc]->h, makecol(255, 0, 255));
		draw_sprite_h_flip(B_Temp, BlockImpact[misc], 0, 0);
		set_trans_blender(255, 255, 255, 170);
		draw_trans_sprite(zoomscreen, B_Temp,
				  p2.limoffx - (BlockImpact[misc]->w / 2),
				  p2.limoffy - (BlockImpact[misc]->h / 2));
	}



	if (foreground)
	masked_blit ( Frgd, zoomscreen ,0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;


      cy = (200+zoomsup) - ZMRATIO ;

      if (cy<1) cy =1 ;

      if ( (cy<zoomsup) && (cy>0) )

      stretch_blit ( zoomscreen , virtscreen ,

      cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

      else

      {
          blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
      }


	/* If a key is hit, check it out */

      if( keypressed )
      {
          if ( key[KEY_ESC] ) 
		  {
			  x=1 ;
			  y=1 ;

			  while (x)
			  {
				  blit_KOF91() ;
				  
			      rectfill(virtscreen, 60, 50,260,110, 0);

				  textout_centre ( virtscreen , font ,"CONTINUE",
					  160 , 65 , (y)? sel_player1 : alt_color ) ;

				  textout_centre ( virtscreen , font ,"QUIT TO MAIN MENU",
					  160 , 80 , (y)?  alt_color : sel_player1 ) ;

				  if ( (key[p2_spunch]) || (key[p2_skick]) 
					|| (key[p2_wpunch]) || (key[p2_wkick])
					|| ( key[KEY_ENTER]) 
					|| P1_WK || P1_SK || P1_WP || P1_SP )
				  { 
					  if (y) x = 0 ; 

					  else { ok = 0 ; x = 0 ; }

				  }

				  if ( key[p2_up] || key[p2_down]
					  || key[p2_left] || key[p2_right]
					  || P1_UP || P1_DN || P1_RT || P1_LT )
				  {
					  Bing() ;

					  if (y) y=0 ;

					  else y = 1 ;
				  }

		
				  rest(90);
			  }

			  clear_keybuf();

		  }			  

          else

          if ( key[KEY_F1] )

          {
                if (skp_flag==1) skp_flag = 0 ;

                else skp_flag = 1 ;
          }


	}


	while (!retrace_count);

// end of the big drawing loop
     
}


/********** PRIORITARY ANIMATIONS UPDATING ************/


/***** PLAYER1 **********/

if ( actual == 3 )    // SIMPLE JUMP

{
	if (spr1y < jump_how1) sens1 = 1 ;

	if ( sens1 )
	{
		if ( spr1y < (jump_how1+4) ) spr1y++ ;
		else
		if ( spr1y < (jump_how1+17) ) spr1y += 2 ;
		else 
		spr1y += 3 ;

		if (spr1y > sol1) { actual = 0 ; spr1y = sol1 ; sens1 = 0 ;limak = 1;}
	}

	else

	{
		if ( spr1y < (jump_how1+4) ) spr1y-- ;
		else
		if ( spr1y < (jump_how1+17) ) spr1y -= 2 ;
		else 
		spr1y -= 3 ;

	}

}

else
if ( actual == 4 )    // JUMP BACKWARD
{
	if (spr1y < jump_how1) sens1 = 1 ;

	if ( sens1 )
	{
		if ( spr1y < (jump_how1+4) ) spr1y++ ;
		else
		if ( spr1y < (jump_how1+30) ) spr1y += 2 ;
		else 
		spr1y += 3 ;

		if ( spr1x > (limg+cx) ) spr1x -= 2 ;

		else

                if ( spr2x < (limd+cx) )

                for( i = 0; i < s; i++ )

                { if( cx > 1 ) { cx--; spr1x--;}  }

                else

                for( i = 0; i < s; i++ )
	        {
                        if( cx > 1 )

                        if (zoomwidth< ((200+zoomsup)*1.6) )
                        {
                         zoomwidth++;
                         cx--;
                         spr1x--;
                        }
                }
	     
		if (spr1y > sol1) { actual = 0 ; spr1y = sol1 ; sens1 = 0 ;limak = 1;}
	}

	else

	{
		if ( spr1y < (jump_how1+4) ) spr1y-- ;
		else
		if ( spr1y < (jump_how1+30) ) spr1y -= 2 ;
		else 
		spr1y -= 3 ;

		if ( spr1x > (limg+cx) ) spr1x -= 2 ;

		else

		if ( spr2x < (limd+cx) )

                for( i = 0; i < s; i++ )

                { if( cx > 1){ cx--; spr1x--;} }

                else

                for( i = 0; i < s; i++ )
	        {
                        if( cx > 1 )

                        if (zoomwidth< ((200+zoomsup)*1.6) )
                        {
                         zoomwidth++;
                         cx--;
                         spr1x--;
                        }
                }

	    
	}

}

else
if ( actual == 5 )    // JUMP FORWARD
{
	if (spr1y < jump_how1) sens1 = 1 ;

	if ( sens1 )
	{
		if ( spr1y < (jump_how1+4) ) spr1y++ ;
		else
		if ( spr1y < (jump_how1+30) ) spr1y += 2 ;
		else 
		spr1y += 3 ;

        if ( zoomwidth > 320 ) { zoomwidth -- ; cx++ ; }

		if ( (spr1x+tol1) < (spr2x+tol2) ) spr1x+= 2;

		else

		if ( spr2x < (limd+cx) ) { spr1x++ ; spr2x++ ;}

		else

                if( cx < (bkgdlim+320-zoomwidth) ) {cx++;spr1x++ ; spr2x++ ;}


	    
		if (spr1y > sol1) { actual = 0 ; spr1y = sol1 ; sens1 = 0 ;limak = 1;}
	}

	else

	{
		if ( spr1y < (jump_how1+4) ) spr1y-- ;
		else
		if ( spr1y < (jump_how1+30) ) spr1y -= 2 ;
		else 
		spr1y -= 3 ;

        if ( zoomwidth > 320 ) { zoomwidth -- ; cx++; }

		if ( (spr1x+tol1) < (spr2x+tol2) ) spr1x += 2;

		else

		if ( spr2x < (limd+cx) ) { spr1x++ ; spr2x++ ;}

		else

                if( cx < (bkgdlim+320-zoomwidth) ){ cx++;spr1x++ ; spr2x++ ;}


	    
	}

}

 else
 if ( (actual == 40) && (limp1>0) )  // RUSH
 {
	flag++ ;

	if ( flag > ( (p1.rush.hit-1)*p1.rush.spd ) )
	{
		if ( (spr1x+tol1) < (spr2x+tol2) ) spr1x += p1.rush.pix ;

		if ( zoomwidth > 320 ) { zoomwidth -- ; cx ++ ; }

		if ( flag > p1.rush.spd*(p1.rush.hit + (p1.rush.nbf-p1.rush.hit)/2) )
		
		if ( (spr1x+p1.rush.offx) > (spr2x + p2.width - p2.limdefx) )	  
		{
			damag2 = p1.rush.dmg ;

			p1.limoffx = spr1x + p1.rush.offx ;
			p1.limoffy = spr1y + p1.rush.offy ;

			    if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right]|| com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
		
				flag2 = 0 ; 
				fball1 = 0 ;
				autho_ht1 = 0 ;
				rush_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;

		}


		if ( flag > (p1.rush.spd*p1.rush.nbf) )
		{
			flag = (p1.rush.hit-1)*p1.rush.spd ;

			rush_stop1++ ;
			if ( rush_stop1+1 > p1.rush.rot )
			{
				fball1 = 0 ;
				autho_ht1 = 0 ;
				rush_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;
			}

		}

	}
	
 }

 else
 if ( (actual == 50) && (limp1>0) )  // SMOVE
 {
	flag++ ;

	if ( flag > ( (p1.smove.hit-1)*p1.smove.spd ) )
	{

		if ( flag > p1.smove.spd*(p1.smove.hit + (p1.smove.nbf-p1.smove.hit)/2) )
		
		if ( (spr1x+p1.smove.offx) > (spr2x + p2.width - p2.limdefx) )	  
		{
			damag2 = p1.smove.dmg ;

			p1.limoffx = spr1x + p1.smove.offx ;
			p1.limoffy = spr1y + p1.smove.offy ;

			    if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right]|| com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
		
				flag2 = 0 ; 
				fball1 = 0 ;
				autho_ht1 = 0 ;
				smove_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;

		}


		if ( flag > (p1.smove.spd*p1.smove.nbf) )
		{
			flag = (p1.smove.hit-1)*p1.smove.spd ;

			smove_stop1++ ;
			if ( smove_stop1+1 > p1.smove.rot )
			{
				fball1 = 0 ;
				autho_ht1 = 0 ;
				smove_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;
			}

		}

	}
	
 }

 else
 if ( (actual == 30) && (limp1>0) ) // FIREB
 {
	flag++ ;

	if ( flag == ( (p1.fireb.hit-1)*p1.fireb.spd ) )
	{
		fball1 = 1 ; 
		fball_flg1 = 0 ;
		fb1x = spr1x + fball_cx1 ;
		fb1y = spr1y + fball_cy1 - (fball_h1/2) ;
	}

	if ( flag > ((p1.fireb.spd*p1.fireb.nbf)+10) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 //SPECIALS
 else
 if ( (actual == 41) && (limp1>0) )  
 {
	flag++ ;

	if ( flag > ( (p1.rushx.hit-1)*p1.rushx.spd ) )
	{
		if ( (spr1x+tol1) < (spr2x+tol2) ) spr1x += p1.rushx.pix ;

		if ( zoomwidth > 320 ) { zoomwidth -- ; cx ++ ; }

		if ( flag > p1.rushx.spd*(p1.rushx.hit + (p1.rushx.nbf-p1.rushx.hit)/2) )
		
		if ( (spr1x+p1.rushx.offx) > (spr2x + p2.width - p2.limdefx) )	  
		{
			damag2 = p1.rushx.dmg ;

			p1.limoffx = spr1x + p1.rushx.offx ;
			p1.limoffy = spr1y + p1.rushx.offy ;

			    if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right]|| com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
		
				flag2 = 0 ; 
				fballx1 = 0 ;
				fball1 = 0 ;
				autho_ht1 = 0 ;
				rushx_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;

		}


		if ( flag > (p1.rushx.spd*p1.rushx.nbf) )
		{
			flag = (p1.rushx.hit-1)*p1.rushx.spd ;

			rushx_stop1++ ;
			if ( rushx_stop1+1 > p1.rushx.rot )
			{
				fball1 = 0 ;
				fballx1 = 0 ;
				autho_ht1 = 0 ;
				rushx_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;
			}

		}

	}
	
 }

 else
 if ( (actual == 51) && (limp1>0) )
 {
	flag++ ;

	if ( flag > ( (p1.smovex.hit-1)*p1.smovex.spd ) )
	{

		if ( flag > p1.smovex.spd*(p1.smovex.hit + (p1.smovex.nbf-p1.smovex.hit)/2) )
		
		if ( (spr1x+p1.smovex.offx) > (spr2x + p2.width - p2.limdefx) )	  
		{
			damag2 = p1.smovex.dmg ;

			p1.limoffx = spr1x + p1.smovex.offx ;
			p1.limoffy = spr1y + p1.smovex.offy ;

			    if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right]|| com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
		
				flag2 = 0 ; 
				fballx1 = 0 ;
				fball1 = 0 ;
				autho_ht1 = 0 ;
				smovex_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;

		}


		if ( flag > (p1.smovex.spd*p1.smovex.nbf) )
		{
			flag = (p1.smovex.hit-1)*p1.smovex.spd ;

			smovex_stop1++ ;
			if ( smovex_stop1+1 > p1.smovex.rot )
			{
				fballx1 = 0 ;
				fball1 = 0 ;
				autho_ht1 = 0 ;
				smovex_stop1 = 0 ;

				flag = 0 ;
				actual = 0 ;
				limp1 = -delayer ;
			}

		}

	}
	
 }

 else
 if ( (actual == 31) && (limp1>0) )
 {
	flag++ ;

	if ( flag == ( (p1.firebx.hit-1)*p1.firebx.spd ) )
	{
		fballx1 = 1 ; 
		fballx_flg1 = 0 ;
		fbx1x = spr1x + fballx_cx1 ;
		fbx1y = spr1y + fballx_cy1 - (fballx_h1/2) ;
	}

	if ( flag > ((p1.firebx.spd*p1.firebx.nbf)+10) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 //END OF SPECIALS

 else
 if ( (actual == 6) && (limp1>0) )
 {
	flag++ ;

	if ( flag > ((p1.spunch.spd*p1.spunch.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }
 
 else
 if ( (actual == 61) && (limp1>0) )

 {
	flag++ ;

	if ( flag > ((p1.wpunch.spd*p1.wpunch.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 else
 if ( (actual == 7) && (limp1>0) )

 {
	flag++ ;

	if ( flag > ((p1.skick.spd*p1.skick.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 else
 if ( (actual == 71) && (limp1>0) )

 {
	flag++ ;

	if ( flag > ((p1.wkick.spd*p1.wkick.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 else
 if ( (actual == 10) && (limp1>0) )

 {
	flag++ ;

	if ( flag > ((p1.gkick.spd*p1.gkick.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 else
 if ( (actual == 13) && (limp1>0) )

 {
	flag++ ;

	if ( flag > ((p1.gpunch.spd*p1.gpunch.nbf)-1) )
	{
		flag = 0 ;
		actual = 0 ;
        limp1 = -delayer ;
	}
 }

 else
 if ( actual == 8 )

  {
	if (fx) { Block() ; fx = 0 ; }

	if ( spr1y < (sol1-1) ) spr1y = spr1y + 3 ;

	flag++ ;

	if ( (flag > (damag1*10) ) && (spr1y>(sol1-2)) ) actual = 0 ;
  }

 else
 if ( actual == 18 )

  {
	if (fx) { life1 = life1 -2 ; Block() ; fx = 0 ; }

	if ( spr1y < (sol1-1) ) spr1y = spr1y + 3 ;

	flag++ ;

	if ( (flag > (damag1*10) ) && (spr1y>(sol1-2)) ) actual = 2 ;
  }

 else
 if ( actual == 9 )

  {
	if ( ak2 == (p2.akick.spd*p2.akick.hit)-3 ) Hurt(2);

	else
	if ( apch2 == (p2.apunch.spd*p2.apunch.hit)-3 ) Hurt(2);

	if ( (fx) && (flag2 > hitfram2) ) { Hurt(2) ; fx = 0 ; }

	if ( spr1y < (sol1-1) ) spr1y = spr1y + 3 ;

	hflag++ ;

	if ( hflag < 17 ) life1 = life1 - damag1 ;

	if ( hflag > 25 ) { actual = 0 ; hflag = 0 ;}

        if ( hflag < 7 )

	if ( (spr1x > (limg+cx) ) && (spr2x < (limd+cx) ) ) { spr1x-- ; spr2x += 2 ; }

	else

        {
                if ( spr1x > (limg+cx) )  spr1x -- ;

                else

                if( cx > 1) { cx--;}

                if ( spr2x < (limd+cx) ) spr2x++ ;
        }

  }

 else
 if ( actual == 19 )

  {
    if ( ak2 == (p2.akick.spd*p2.akick.hit)-3 ) Hurt(2);

	else
	if ( apch2 == (p2.apunch.spd*p2.apunch.hit)-3 ) Hurt(2);

	if ( (fx) && (flag2 > hitfram2) ) { Hurt(2) ; fx = 0 ; }

	if ( spr1y < (sol1-1) ) spr1y = spr1y + 3 ;

	hflag++ ;

	if ( hflag < 17 ) life1 = life1 - damag1 ; ;

	if ( hflag > 25 ) { actual = 2 ; hflag = 0 ;}

        if ( hflag < 7 )

	if ( (spr1x > (limg+cx) ) && (spr2x < (limd+cx) ) ) { spr1x-- ; spr2x += 2 ; }

	else

        {
                if ( spr1x > (limg+cx) )  spr1x -- ;

                else

                if( cx > 1) { cx--;}

                if ( spr2x < (limd+cx) ) spr2x++ ;
        }

  }

  else
  if ( actual == 20 )

  {
	if ( ak2 == (p2.akick.spd*p2.akick.hit)-3 ) Hurt(2);

	else
	if ( apch2 == (p2.apunch.spd*p2.apunch.hit)-3 ) Hurt(2);

	if ( (fx) && (flag2 > hitfram2) ) { Hurt(2) ; fx = 0 ; }

	if ( spr1y < (sol1-1) ) spr1y = spr1y + 2 ;

	hflag++ ;

	if ( hflag < 25 ) life1 = life1 - damag1 ;

	if ( hflag > 42 ) { actual = 0 ; hflag = 0 ;}

        if ( hflag < 15 )

	if ( (spr1x > (limg+cx) ) && (spr2x < (limd+cx) ) ) { spr1x-- ; spr2x += 2 ; }

	else

        {
                if ( spr1x > (limg+cx) )  spr1x -- ;

                else

                if( cx > 1) { cx--;}

                if ( spr2x < (limd+cx) ) spr2x++ ;
        }

  }

/******** PLAYER 2 ***********/

 if ( actual2 == 3 )    // SIMPLE JUMP

 {
	if (spr2y < jump_how2) sens2 = 1 ;

	if ( sens2 )
	{
		if ( spr2y < (jump_how2+4) ) spr2y++ ;
		else
		if ( spr2y < (jump_how2+17) ) spr2y += 2 ;
		else 
		spr2y += 3 ;

		if (spr2y > sol2) { actual2 = 0 ; spr2y = sol2 ; sens2 = 0 ;limak2 = 1; }
	}

	else

	{
		if ( spr2y < (jump_how2+4) ) spr2y-- ;
		else
		if ( spr2y < (jump_how2+17) ) spr2y -= 2 ;
		else 
		spr2y -= 3 ;
	}

 }

 else
 if ( actual2 == 4 )    // JUMP FORWARD
 {
	if (spr2y < jump_how2) sens2 = 1 ;

	if ( sens2 )
	{
		if ( spr2y < (jump_how2+4) ) spr2y++ ;
		else
		if ( spr2y < (jump_how2+30) ) spr2y += 2 ;
		else 
		spr2y += 3 ;


        if ( zoomwidth > 320 ) zoomwidth -- ;

		if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= 2 ;

		else

		if ( spr1x > (limg+cx) ) { spr1x-- ; spr2x-- ;}

		else

        if( cx > 1) { cx--; spr1x-- ; spr2x-- ;}

		if (spr2y > sol2) { actual2 = 0 ; spr2y = sol2 ; sens2 = 0 ;limak2 = 1;}
	}

	else

	{
		if ( spr2y < (jump_how2+4) ) spr2y-- ;
		else
		if ( spr2y < (jump_how2+30) ) spr2y -= 2 ;
		else 
		spr2y -= 3 ;

        if ( zoomwidth > 320 ) zoomwidth -- ;

		if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= 2 ;

		else

		if ( spr1x > (limg+cx) ) { spr1x-- ; spr2x-- ;}

		else

        if( cx > 1) { cx--;spr1x-- ; spr2x-- ;}

	}

 }

 else
 if ( actual2 == 5 )    // JUMP BACKWARD
 {
	if (spr2y < jump_how2) sens2 = 1 ;

	if ( sens2 )
	{
		if ( spr2y < (jump_how2+4) ) spr2y++ ;
		else
		if ( spr2y < (jump_how2+30) ) spr2y += 2 ;
		else 
		spr2y += 3 ;

		if ( spr2x < (limd+cx) ) spr2x+= 2;

		else

		if ( spr1x > (limg+cx) )

                for( i = 0; i < s; i++ )

        	{ if( cx < (bkgdlim+320-zoomwidth) ) { cx++; spr2x++; } }

                else

                for( i = 0; i < s; i++ )
	        {
                        if( cx < (bkgdlim+320-zoomwidth) )

                        if (zoomwidth< ((200+zoomsup)*1.6) )
                        {
                         zoomwidth++;

                         spr2x++;
                        }
                }

	    
		if (spr2y > sol2) { actual2 = 0 ; spr2y = sol2 ; sens2 = 0 ;limak2 = 1;}
	}

	else

	{
		if ( spr2y < (jump_how2+4) ) spr2y-- ;
		else
		if ( spr2y < (jump_how2+30) ) spr2y -= 2 ;
		else 
		spr2y -= 3 ;

		if ( spr2x < (limd+cx) ) spr2x += 2;

		else

		if ( spr1x > (limg+cx) )

                for( i = 0; i < s; i++ )

                { if( cx < (bkgdlim+320-zoomwidth) ) { cx++; spr2x++; }  }

                else

                for( i = 0; i < s; i++ )
	        {
                        if( cx < (bkgdlim+320-zoomwidth) )

                        if (zoomwidth< ((200+zoomsup)*1.6) )
                        {
                         zoomwidth++;

                         spr2x++;
                        }
            }
	   
	}

}
 
 else
 if ( (actual2 == 40) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 > ( (p2.rush.hit-1)*p2.rush.spd ) )
	{
		if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= p2.rush.pix ;

		if ( zoomwidth > 320 ) zoomwidth -- ;

		if ( flag2 > p2.rush.spd*(p2.rush.hit + (p2.rush.nbf-p2.rush.hit)/2) )

		if ( ( spr2x-(p2.rush.offx - p2.width) ) < (spr1x+p1.limdefx) )
		{
			damag1 = p2.rush.dmg ;

			p2.limoffx = spr2x-(p2.rush.offx - p2.width) ;
			p2.limoffy = spr2y + p2.rush.offy ;

			    if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fball2 = 0 ;
				autho_ht2 = 0 ;
				rush_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
	

		}


		if ( flag2 > (p2.rush.spd*p2.rush.nbf) )
		{
			flag2 = (p2.rush.hit-1)*p2.rush.spd ;

			rush_stop2++ ;
			if ( rush_stop2+1 > p2.rush.rot )
			{
				fball2 = 0 ;
				autho_ht2 = 0 ;
				rush_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
			}

		}
	}
	
 }

 else
 if ( (actual2 == 50) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 > ( (p2.smove.hit-1)*p2.smove.spd ) )
	{

		if ( flag2 > p2.smove.spd*(p2.smove.hit + (p2.smove.nbf-p2.smove.hit)/2) )

		if ( ( spr2x-(p2.smove.offx - p2.width) ) < (spr1x+p1.limdefx) )
		{
			damag1 = p2.smove.dmg ;

			p2.limoffx = spr2x-(p2.smove.offx - p2.width) ;
			p2.limoffy = spr2y + p2.smove.offy ;

			    if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fball2 = 0 ;
				autho_ht2 = 0 ;
				smove_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
	

		}


		if ( flag2 > (p2.smove.spd*p2.smove.nbf) )
		{
			flag2 = (p2.smove.hit-1)*p2.smove.spd ;

			smove_stop2++ ;
			if ( smove_stop2+1 > p2.smove.rot )
			{
				fball2 = 0 ;
				autho_ht2 = 0 ;
				smove_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
			}

		}
	}
	
 }


 else
 if ( (actual2 == 30) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 == ( (p2.fireb.hit-1)*p2.fireb.spd ) )
	{
		fball2 = 1 ; 
		fball_flg2 = 0 ;
		fb2x = spr2x + ( p2.width - fball_cx2 )- p2.fball.w ;
		fb2y = spr2y + fball_cy2 - (fball_h2/2) ;
	}

	if ( flag2 > ((p2.fireb.spd*p2.fireb.nbf)+10) )
	{
		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;
	}
 }

 // SPECIALS

 else
 if ( (actual2 == 41) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 > ( (p2.rushx.hit-1)*p2.rushx.spd ) )
	{
		if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= p2.rushx.pix ;

		if ( zoomwidth > 320 ) zoomwidth -- ;

		if ( flag2 > p2.rushx.spd*(p2.rushx.hit + (p2.rushx.nbf-p2.rushx.hit)/2) )

		if ( ( spr2x-(p2.rushx.offx - p2.width) ) < (spr1x+p1.limdefx) )
		{
			damag1 = p2.rushx.dmg ;

			p2.limoffx = spr2x-(p2.rushx.offx - p2.width) ;
			p2.limoffy = spr2y + p2.rushx.offy ;

			    if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fball2 = 0 ;
				fballx2 = 0 ;
				autho_ht2 = 0 ;
				rushx_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
	

		}


		if ( flag2 > (p2.rushx.spd*p2.rushx.nbf) )
		{
			flag2 = (p2.rushx.hit-1)*p2.rushx.spd ;

			rushx_stop2++ ;
			if ( rushx_stop2+1 > p2.rushx.rot )
			{
				fball2 = 0 ;
				fballx2 = 0 ;
				autho_ht2 = 0 ;
				rushx_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
			}

		}
	}
	
 }

 else
 if ( (actual2 == 51) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 > ( (p2.smovex.hit-1)*p2.smovex.spd ) )
	{

		if ( flag2 > p2.smovex.spd*(p2.smovex.hit + (p2.smovex.nbf-p2.smovex.hit)/2) )

		if ( ( spr2x-(p2.smovex.offx - p2.width) ) < (spr1x+p1.limdefx) )
		{
			damag1 = p2.smovex.dmg ;

			p2.limoffx = spr2x-(p2.smovex.offx - p2.width) ;
			p2.limoffy = spr2y + p2.smovex.offy ;

			    if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fball2 = 0 ;
				fballx2 = 0 ;
				autho_ht2 = 0 ;
				smovex_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
	

		}


		if ( flag2 > (p2.smovex.spd*p2.smovex.nbf) )
		{
			flag2 = (p2.smovex.hit-1)*p2.smovex.spd ;

			smovex_stop2++ ;
			if ( smovex_stop2+1 > p2.smovex.rot )
			{
				fball2 = 0 ;
				fballx2 = 0 ;
				autho_ht2 = 0 ;
				smovex_stop2 = 0 ;

				flag2 = 0 ;
				actual2 = 0 ;
				limp2 = -delayer ;
			}

		}
	}
	
 }


 else
 if ( (actual2 == 31) && (limp2>0) )
 {
	flag2++ ;

	if ( flag2 == ( (p2.firebx.hit-1)*p2.firebx.spd ) )
	{
		fballx2 = 1 ; 
		fballx_flg2 = 0 ;
		fbx2x = spr2x + ( p2.width - fballx_cx2 )- p2.fballx.w ;
		fbx2y = spr2y + fballx_cy2 - (fballx_h2/2) ;
	}

	if ( flag2 > ((p2.firebx.spd*p2.firebx.nbf)+10) )
	{
		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;
	}
 }

 // END OF SPECIALS
 else
 if ( (actual2 == 6) && (limp2>0) )
 {
		flag2++ ;

		if ( flag2 > ((p2.spunch.spd*p2.spunch.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( (actual2 == 61) && (limp2>0) )
 {
		flag2++ ;

		if ( flag2 > ((p2.wpunch.spd*p2.wpunch.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( (actual2 == 7) && (limp2>0) )

 {
		flag2++ ;

		if ( flag2 > ((p2.skick.spd*p2.skick.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( (actual2 == 71) && (limp2>0) )

 {
		flag2++ ;

		if ( flag2 > ((p2.wkick.spd*p2.wkick.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( (actual2 == 10) && (limp2>0) )
 {
		flag2++ ;

		if ( flag2 > ((p2.gkick.spd*p2.gkick.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( (actual2 == 13) && (limp2>0) )
 {
		flag2++ ;

		if ( flag2 > ((p2.gpunch.spd*p2.gpunch.nbf)-1) )
		{

		flag2 = 0 ;
		actual2 = 0 ;
        limp2 = -delayer ;

		}

 }

 else
 if ( actual2 == 8 )
 {
	if (fy) { Block() ; fy = 0 ; }

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

	flag2++ ;

	if ( (flag2 > (damag2*10) ) && (spr2y>(sol2-2)) ) actual2 = 0 ;
 }

 else
 if ( actual2 == 18 )
 {
	if (fy) { life2 = life2 -2 ; Block() ; fy = 0 ; }

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

	flag2++ ;

	if ( (flag2 > (damag2*10) ) && (spr2y>(sol2-2)) ) actual2 = 2 ;
 }

 else
 if ( actual2 == 9 )
 {
	if ( ak == (p1.akick.hit*p1.akick.spd) -3 ) Hurt(2);

	else
	if ( apch == (p1.apunch.hit*p1.apunch.spd) -3 ) Hurt(2);

    if ( (fy) && (flag > hitfram1) ) { Hurt(2) ; fy = 0 ; }

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

	hflag2++ ;

	if ( hflag2 < 17 ) life2 = life2 - damag2 ;

	if ( hflag2 > 25 ) { actual2 = 0 ; hflag2 = 0 ;}

        if ( hflag2 < 7 )

	if ( (spr2x < (limd+cx)) && (spr1x > (limg+cx)) ) { spr2x++ ; spr1x -= 2 ;}

	else

        {
                if (spr2x < (limd+cx) ) spr2x++ ;

                else

	            if( cx < (bkgdlim+320-zoomwidth) ) { cx++;  }

                if ( spr1x > (cx+limg) )  spr1x-- ;
        }

  }

  else
  if ( actual2 == 19 )
  {
    if ( ak == (p1.akick.hit*p1.akick.spd) -3 ) Hurt(2);

    else
	if ( apch == (p1.apunch.hit*p1.apunch.spd) -3 ) Hurt(2);

    if ( (fy) && (flag > hitfram1) ) { Hurt(2) ; fy = 0 ; }

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

	hflag2++ ;

	if ( hflag2 < 17 ) life2 = life2 - damag2 ;

	if ( hflag2 > 25 ) { actual2 = 2 ; hflag2 = 0 ;}

    if ( hflag2 < 7 )

	if ( (spr2x < (limd+cx)) && (spr1x > (limg+cx)) ) { spr2x++ ; spr1x -= 2 ;}

	else

        {
                if (spr2x < (limd+cx) ) spr2x ++ ;

                else

	            if( cx < (bkgdlim+320-zoomwidth) ) { cx++;  }

                if ( spr1x > (cx+limg) )  spr1x -- ;
        }

  }

  else
  if ( actual2 == 20 )
  {
	if ( ak == (p1.akick.hit*p1.akick.spd) -3 ) Hurt(2);

	else
	if ( apch == (p1.apunch.hit*p1.apunch.spd) -3 ) Hurt(2);

    if ( (fy) && (flag > hitfram1) ) { Hurt(2) ; fy = 0 ; }

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 2 ;

	hflag2++ ;

	if ( hflag2 < 25 ) life2 = life2 - damag2 ;

	if ( hflag2 > 42 ) { actual2 = 0 ; hflag2 = 0 ;}

        if ( hflag2 < 15 )

	if ( (spr2x < (limd+cx)) && (spr1x > (limg+cx)) ) { spr2x++ ; spr1x -= 2 ;}

	else

        {
                if (spr2x < (limd+cx) ) spr2x ++ ;

                else

	            if( cx < (bkgdlim+320-zoomwidth) ) { cx++;  }

                if ( spr1x > (cx+limg) )  spr1x -- ;
        }
  }

/************** UPDATE FIREBALLS **************/

  if (fball1)
  {
	  fb1x += p1.fball.pix ; fball_flg1++ ;

	  if (fball_flg1 > (p1.fball.nbf * p1.fball.spd)-1 ) fball_flg1 = 0 ;

	  if (fball2)
	  {
		  if ( (fb1x + p1.fball.w) > (fb2x +(p2.fball.w/3)) )
		  {
			  fball1 = 0 ;
			  fball2 = 0 ;
			  p1.limoffx = fb1x + p1.fball.offx ;
			  p1.limoffy = fb1y + p1.fball.offy ;
			  p1.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else
	  if (fballx2)
	  {
		  if ( (fb1x + p1.fball.w) > (fbx2x +(p2.fballx.w/3)) )
		  {
			  fball1 = 0 ;
			  fballx2 = 0 ;
			  p1.limoffx = fb1x + p1.fball.offx ;
			  p1.limoffy = fb1y + p1.fball.offy ;
			  p1.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else

	  if ( (fb1x + p1.fball.w) > (spr2x+(p2.width/3)) )
	  {
		  if ( fb1x < (spr2x+(p2.width/2)) )
		  {
			  if ( autho_ht1 && (spr2y > (sol2-30)) && (fb1x>(spr1x+fball_cx1+10)) ) 
			  {
				damag2 = p1.fireb.dmg ;

				p1.limoffx = fb1x + p1.fball.offx ;
				p1.limoffy = fb1y + p1.fball.offy ;

				if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if  (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right] || com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
				
				flag2 = 0 ; 
				fball1 = 0 ;
				autho_ht1 = 0 ;

			  }

		  }
		  else

		  if (fb1x>(cx+zoomwidth) ) fball1 = 0 ;
	  }

  }


  if (fball2)
  {
	  fb2x -= p2.fball.pix ; fball_flg2++ ;

	  if (fball_flg2 > (p2.fball.nbf * p2.fball.spd)-1 ) fball_flg2 = 0 ;

	  if (fball1)
	  {
		  if ( fb2x  < (fb1x +(2*p1.fball.w/3)) )
		  {
			  fball1 = 0 ;
			  fball2 = 0 ;
			  p2.limoffx = fb2x-(p2.fball.w - p2.fball.offx ) ;
			  p2.limoffy = fb2y + p2.fball.offy ;
			  p2.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else
	  if (fballx1)
	  {
		  if ( fb2x  < (fbx1x +(2*p1.fballx.w/3)) )
		  {
			  fballx1 = 0 ;
			  fball2 = 0 ;
			  p2.limoffx = fb2x-(p2.fball.w - p2.fball.offx ) ;
			  p2.limoffy = fb2y + p2.fball.offy ;
			  p2.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else

	  if ( fb2x < (spr1x+(2*p1.width/3)) )
	  {
		  if ( (fb2x+p2.fball.w) > (spr1x-(p1.width/2)) )
		  {
			  if ( autho_ht2 && (spr1y > (sol1-30)) 
				  
				  && (fb2x < (spr2x + (p2.width - fball_cx2)- p2.fball.w - 10)) ) 
			  {
				damag1 = p2.fireb.dmg ;

				p2.limoffx = fb2x-(p2.fball.w - p2.fball.offx ) ;
				p2.limoffy = fb2y + p2.fball.offy ;

				if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fball2 = 0 ;
				autho_ht2 = 0 ;

			  }

		  }
		  else

		  if (fb2x<(cx-20) ) fball2 = 0 ;
	  }

  }

  // NOW FOR SPECIALS

  if (fballx1)
  {
	  fbx1x += p1.fballx.pix ; fballx_flg1++ ;

	  if (fballx_flg1 > (p1.fballx.nbf * p1.fballx.spd)-1 ) fballx_flg1 = 0 ;

	  if (fball2)
	  {
		  if ((fbx1x + p1.fballx.w) > (fb2x +(p2.fball.w/3)))
		 
		  {
			  fballx1 = 0 ;
			  fball2 = 0 ;
			  p1.limoffx = fbx1x + p1.fballx.offx ;
			  p1.limoffy = fbx1y + p1.fballx.offy ;
			  p1.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else
	  if (fballx2)
	  {
		  if ((fbx1x + p1.fballx.w) > (fbx2x +(p2.fballx.w/3)))
		 
		  {
			  fballx1 = 0 ;
			  fballx2 = 0 ;
			  p1.limoffx = fbx1x + p1.fballx.offx ;
			  p1.limoffy = fbx1y + p1.fballx.offy ;
			  p1.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else

	  if ( (fbx1x + p1.fballx.w) > (spr2x+(p2.width/3)) )
	  {
		  if ( fbx1x < (spr2x+(p2.width/2)) )
		  {
			  if ( autho_ht1 && (spr2y > (sol2-30)) && (fbx1x>(spr1x+fballx_cx1+10)) ) 
			  {
				damag2 = p1.firebx.dmg ;

				p1.limoffx = fbx1x + p1.fballx.offx ;
				p1.limoffy = fbx1y + p1.fballx.offy ;

				if ( spr2y<(sol2-5) ) { p1.simpact_ct = 10 ; actual2 = 20 ; Hurt(1); } 

			    else

			    if  (key[p2_down])

				{
					if  (key[p2_right]|| com_gblock) { p1.bimpact_ct = 10 ; actual2 = 18 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 19 ; Hurt(1); }
				}

				else

				{
					if (key[p2_right] || com_block) { p1.bimpact_ct = 10 ; actual2 = 8 ; fy = 1 ; }

					else { p1.simpact_ct = 10 ; actual2 = 9 ; Hurt(1); }
				}
				
				flag2 = 0 ; 
				fballx1 = 0 ;
				autho_ht1 = 0 ;

			  }

		  }
		  else

		  if (fb1x>(cx+zoomwidth) ) fballx1 = 0 ;
	  }

  }


  if (fballx2)
  {
	  fbx2x -= p2.fballx.pix ; fballx_flg2++ ;

	  if (fballx_flg2 > (p2.fballx.nbf * p2.fballx.spd)-1 ) fballx_flg2 = 0 ;

	  if (fball1)
	  {
		  if ( fbx2x  < (fb1x +(2*p1.fball.w/3)) )
		  {
			  fball1 = 0 ;
			  fballx2 = 0 ;
			  p2.limoffx = fbx2x-(p2.fballx.w - p2.fballx.offx ) ;
			  p2.limoffy = fbx2y + p2.fballx.offy ;
			  p2.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else
	  if (fballx1)
	  {
		  if ( fbx2x  < (fbx1x +(2*p1.fballx.w/3)) )
		  {
			  fballx1 = 0 ;
			  fballx2 = 0 ;
			  p2.limoffx = fbx2x-(p2.fballx.w - p2.fballx.offx ) ;
			  p2.limoffy = fbx2y + p2.fballx.offy ;
			  p2.bimpact_ct = 10 ;
			  Hurt(1);
		  }
	  }

	  else

	  if ( fbx2x < (spr1x+(2*p1.width/3)) )
	  {
		  if ( (fbx2x+p2.fballx.w) > (spr1x-(p1.width/2)) )
		  {
			  if ( autho_ht2 && (spr1y > (sol1-30)) 
				  
				  && (fbx2x < (spr2x + (p2.width - fballx_cx2)- p2.fballx.w - 10)) ) 
			  {
				damag1 = p2.firebx.dmg ;

				p2.limoffx = fbx2x-(p2.fballx.w - p2.fballx.offx ) ;
				p2.limoffy = fbx2y + p2.fballx.offy ;

				if ( spr1y<(sol1-5) ) { p2.simpact_ct = 10 ; actual = 20 ; Hurt(1); } 

			    else

			    if (P1_DN )

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 19 ; Hurt(1); }
				}

				else

				{
					if (P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ; fx = 1 ; }

					else { p2.simpact_ct = 10 ; actual = 9 ; Hurt(1); }
				}
				
				flag = 0 ; 
				fballx2 = 0 ;
				autho_ht2 = 0 ;

			  }

		  }
		  else

		  if (fb2x<(cx-20) ) fballx2 = 0 ;
	  }

  }

	  
		  
/************** CHECKING PLAYER 1 INPUTS*****************/

if ( (actual < 3) || (actual > 100) )     // if actual position not prioritary

{
      actual = 0 ;

	  poll_joystick();

      if( P1_LT && P1_UP )
      {
	actual = 4 ;
      }

      else

      if( P1_RT && P1_UP )
      {
	actual = 5 ;
      }

      else

      if( P1_UP )
      {
	actual = 3 ;
      }

      else

      if( P1_DN )
      {
	actual = 2 ;
      }

      else

      if( P1_LT )
      {
	 if ( spr1x > (limg+cx) ) spr1x -= 2 ;

	 else

	 if ( spr2x < (limd+cx) )

	 for( i = 0; i < s; i++ )
	 {
	    if( cx > 1)
	      { cx--;spr1x--;}

	 }

     else

     for( i = 0; i < s; i++ )
	 {
	    if( cx > 1)

            if (zoomwidth< ((200+zoomsup)*1.6))
            {
                cx--;

                spr1x--;

                zoomwidth++;
            }

	 }

	 flag-- ;

	 if ( flag < 0 ) flag = ((p1.walk.spd*p1.walk.nbf)-1) ;

	 actual = 111 ;

      }

      else

      if( P1_RT )
      {
         if (zoomwidth>320) { zoomwidth-- ; cx++ ;}

		 if ( (spr1x+tol1) < (spr2x+tol2) ) spr1x += 2 ;

		 else

		 if ( spr2x < (limd+cx) ) { spr1x++ ; spr2x++ ;}

		 else

			if( cx < (bkgdlim+320-zoomwidth) )

			  { cx++; spr1x++ ; spr2x++ ;}


		 flag++ ;

		 if ( flag > ((p1.walk.spd*p1.walk.nbf)-1) ) flag = 0 ;

		 actual = 1 ;

      }

      if ( ( (actual < 2) || (actual >100) ) && (limp1>0) 
			&& !p1.fireb.succ && !p1.rush.succ && !p1.smove.succ
			&& !p1.firebx.succ && !p1.rushx.succ && !p1.smovex.succ )
      {

        if ( P1_SP )

        {
	        Paf(2);

                actual = 6 ;
                flag = 0 ;

                fy = 1 ;
        }

        else

		if ( P1_WP )

        {
	        Paf(1);

                actual = 61 ;
                flag = 0 ;

                fy = 1 ;
        }

		else

        if ( P1_SK )

        {

                Paf(4) ;

                actual = 7 ;
                flag = 0 ;

                fy = 1 ;
        }

		else

        if ( P1_WK )

        {

                Paf(3) ;

                actual = 71 ;
                flag = 0 ;

                fy = 1 ;
        }

      }

      if ( (actual == 2) && (limp1>0) )
	  {
        if ( P1_WK || P1_SK )

        {
	        Paf(1);

                actual = 10 ;
                flag = 0 ;

                fy = 1 ;
        }

		else

		if ( P1_SP || P1_WP )

        {
	        Paf(4);

                actual = 13 ;
                flag = 0 ;

                fy = 1 ;
        }

	  }

if ( p1.fireb.flg )
{
	  if ( check_kstatus1( p1.fireb.seq[0] ) && !p1.fireb.succ )

	  { p1.fireb.succ = 1 ; p1.fireb.nb = 0 ; }

	  else
	  if (p1.fireb.succ)
	  {
		  if ( (last_status1 == p1.fireb.seq[p1.fireb.nb]) ||  (last_status1 == p1.fireb.seq[p1.fireb.nb+1]) )
		  {
			  if (check_kstatus1(p1.fireb.seq[p1.fireb.nb+1]) )
			  {
				  p1.fireb.nb++ ; p1.fireb.succ = p1.fireb.nb + 1 ;

				  if (p1.fireb.succ == p1.fireb.end) 
				  { 
					  play_sample( hadoken1 , snd_vol, pan, pitch, 0 );
					  p1.fireb.succ = 0;
					  actual = 30 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.fireb.succ = 0 ;
		  }

		  else p1.fireb.succ = 0 ;
	  }

	  else p1.fireb.succ = 0 ;
}

else p1.fireb.succ = 0 ;


if ( p1.rush.flg )
{
	  if ( check_kstatus1( p1.rush.seq[0] ) && !p1.rush.succ )

	  { p1.rush.succ = 1 ; p1.rush.nb = 0 ; }

	  else
	  if (p1.rush.succ)
	  {
		  if ( (last_status1 == p1.rush.seq[p1.rush.nb]) ||  (last_status1 == p1.rush.seq[p1.rush.nb+1]) )
		  {
			  if (check_kstatus1(p1.rush.seq[p1.rush.nb+1]) )
			  {
				  p1.rush.nb++ ; p1.rush.succ = p1.rush.nb + 1 ;

				  if (p1.rush.succ == p1.rush.end) 
				  { 
					  play_sample( copter1 , snd_vol, pan, pitch, 0 );
					  p1.rush.succ = 0;
					  actual = 40 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.rush.succ = 0 ;
		  }

		  else p1.rush.succ = 0 ;
	  }

	  else p1.rush.succ = 0 ;
}

else p1.rush.succ = 0 ;


if ( p1.smove.flg )
{
	  if ( check_kstatus1( p1.smove.seq[0] ) && !p1.smove.succ )

	  { p1.smove.succ = 1 ; p1.smove.nb = 0 ; }

	  else
	  if (p1.smove.succ)
	  {
		  if ( (last_status1 == p1.smove.seq[p1.smove.nb]) ||  (last_status1 == p1.smove.seq[p1.smove.nb+1]) )
		  {
			  if (check_kstatus1(p1.smove.seq[p1.smove.nb+1]) )
			  {
				  p1.smove.nb++ ; p1.smove.succ = p1.smove.nb + 1 ;

				  if (p1.smove.succ == p1.smove.end) 
				  { 
					  play_sample( dragon1 , snd_vol, pan, pitch, 0 );
					  p1.smove.succ = 0;
					  actual = 50 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.smove.succ = 0 ;
		  }

		  else p1.smove.succ = 0 ;
	  }

	  else p1.smove.succ = 0 ;
}

else p1.smove.succ = 0 ;

// NOW FOR SPECIALS


if ( p1.firebx.flg && p1.firebx.sflg)
{
	  if ( check_kstatus1( p1.firebx.seq[0] ) && !p1.firebx.succ )

	  { p1.firebx.succ = 1 ; p1.firebx.nb = 0 ; }

	  else
	  if (p1.firebx.succ)
	  {
		  if ( (last_status1 == p1.firebx.seq[p1.firebx.nb]) ||  (last_status1 == p1.firebx.seq[p1.firebx.nb+1]) )
		  {
			  if (check_kstatus1(p1.firebx.seq[p1.firebx.nb+1]) )
			  {
				  p1.firebx.nb++ ; p1.firebx.succ = p1.firebx.nb + 1 ;

				  if (p1.firebx.succ == p1.firebx.end) 
				  { 
					  if (p1.firebx.spec) 
					  {
						  p1.firebx.sflg = 0 ;
						  spower1 = 0 ;
					  }
					  play_sample( hadokenx1 , snd_vol, pan, pitch, 0 );
					  p1.firebx.succ = 0;
					  actual = 31 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.firebx.succ = 0 ;
		  }

		  else p1.firebx.succ = 0 ;
	  }

	  else p1.firebx.succ = 0 ;
}

else p1.firebx.succ = 0 ;


if ( p1.rushx.flg && p1.rushx.sflg)
{
	  if ( check_kstatus1( p1.rushx.seq[0] ) && !p1.rushx.succ )

	  { p1.rushx.succ = 1 ; p1.rushx.nb = 0 ; }

	  else
	  if (p1.rushx.succ)
	  {
		  if ( (last_status1 == p1.rushx.seq[p1.rushx.nb]) ||  (last_status1 == p1.rushx.seq[p1.rushx.nb+1]) )
		  {
			  if (check_kstatus1(p1.rushx.seq[p1.rushx.nb+1]) )
			  {
				  p1.rushx.nb++ ; p1.rushx.succ = p1.rushx.nb + 1 ;

				  if (p1.rushx.succ == p1.rushx.end) 
				  { 
					  if (p1.rushx.spec) 
					  {
						  p1.rushx.sflg = 0 ;
						  spower1 = 0 ;
					  }
					  play_sample( copterx1 , snd_vol, pan, pitch, 0 );
					  p1.rushx.succ = 0;
					  actual = 41 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.rushx.succ = 0 ;
		  }

		  else p1.rushx.succ = 0 ;
	  }

	  else p1.rushx.succ = 0 ;
}

else p1.rushx.succ = 0 ;


if ( p1.smovex.flg && p1.smovex.sflg )
{
	  if ( check_kstatus1( p1.smovex.seq[0] ) && !p1.smovex.succ )

	  { p1.smovex.succ = 1 ; p1.smovex.nb = 0 ; }

	  else
	  if (p1.smovex.succ)
	  {
		  if ( (last_status1 == p1.smovex.seq[p1.smovex.nb]) ||  (last_status1 == p1.smovex.seq[p1.smovex.nb+1]) )
		  {
			  if (check_kstatus1(p1.smovex.seq[p1.smovex.nb+1]) )
			  {
				  p1.smovex.nb++ ; p1.smovex.succ = p1.smovex.nb + 1 ;

				  if (p1.smovex.succ == p1.smovex.end) 
				  { 
					  if (p1.smovex.spec) 
					  {
						  p1.smovex.sflg = 0 ;
						  spower1 = 0 ;
					  }
					  play_sample( dragonx1 , snd_vol, pan, pitch, 0 );
					  p1.smovex.succ = 0;
					  actual = 51 ;
					  flag = 0 ; autho_ht1 = 1 ;
					  spower1 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus1(last_status1) ) p1.smovex.succ = 0 ;
		  }

		  else p1.smovex.succ = 0 ;
	  }

	  else p1.smovex.succ = 0 ;
}

else p1.smovex.succ = 0 ;





      /* static updating  */
      if (actual == 0)
      {
		flag++;
		if (flag > ((p1.statik.spd*p1.statik.nbf)-1) ) flag = 0 ;

		if ( spr1y < (sol1-1) ) spr1y = spr1y + 3 ;

      }

      if ( !P1_WK && !P1_SK && !P1_SP && !P1_WP && (limp1<1) )

      limp1++;

}

// UPDATE AIR-KICK ***********/

if ( ( actual > 2 ) && ( actual < 6 ) && (apch==0) )

{
  poll_joystick();

  if (ak && limak) { ak++ ; if (ak>25) {ak = 0 ; limak = 0 ;} }

  else

  if (limak) if ( P1_WK || P1_SK ) { ak = 1 ; Paf(3) ; fy = 1 ; }
}

else ak = 0 ;

// UPDATE AIR-PUNCH ***********/

if ( ( actual > 2 ) && ( actual < 6 ) && (ak==0) )

{
  poll_joystick();

  if (apch && limak) { apch++ ; if (apch>25) {apch = 0 ; limak = 0 ;} }

  else

  if (limak) if ( P1_SP || P1_WP ) { apch = 1 ; Paf(1) ; fy = 1 ; }
}

else apch = 0 ;

//***** END OF CHECK/UPDATE PLAYER 1 ******************************/

//***** CHECK PLAYER 2 INPUT ************************/

if ( ((actual2 < 3) || (actual2 >100)) && gmode )     // if actual position not prioritary

{
	i = rand()%100 ;
	g = 0 ;
	h = rand()%100 ;
	com_block = 0 ;
	com_gblock = 0 ;
	
	if ( (actual2 == 2) && ( (spr1x+p1.limoffx) > (spr2x+p2.width-p2.limdefx ) ) )
	{
		if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

		if ( (actual == 6) || (actual == 7) || (actual==61)
				|| (actual==71) || ak || apch 
				|| (actual == 10) || (actual==13) || (actual==40)
				|| ( (fball1==1)&&((fb1x + p1.fball.w)>spr2x) ) )

		{ 
			if ( h < defense )
			{
				actual2 = 18 ; 
				flag2 = 0 ; 
				com_gblock = 1 ;
			}

			else com_gblock = 0 ;

		}


	}

	
	if ( (actual2 == 2) && (limp2>0) 	
		&& (( spr2x-(p2.gpunch.offx - p2.width) ) < (spr1x+p1.limdefx))
		&& (com_gblock==0) )
	{
		if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

        if (h < offense) 
		{
			if (i<50)
			{

					Paf(4) ;
					actual2 = 10 ;
					flag2 = 0 ;
					fx = 1 ;
			}

			else
			{

					Paf(1) ;
					actual2 = 13 ;
					flag2 = 0 ;
					fx = 1 ;
			}  

		}

	}

	else
	if (actual2 == 2) actual2 = 0 ;

	else
	if ( (actual2<2) || (actual2>100) )
	{

		if ( (spr1x+p1.limoffx) > (spr2x+p2.width-p2.limdefx ) ) // if reachable
		{
			if ( (actual == 6) || (actual == 7) || (actual==61)
				|| (actual==71) || ak || apch || (actual==40) 
				|| ( (fball1==1)&&((fb1x + p1.fball.w)>spr2x) ) )

			{
				if ( h < defense )
				{
					com_gblock = 0 ;
					com_block = 1 ; 
				}

				else com_block = 0 ;
			}

			else
			if ( (actual == 10) || (actual==13) ) 
			{
				if ( h < defense )
				{
					com_gblock = 1 ; 
				}

				else com_gblock = 0 ;
			}

		}

		h = rand()%100 ;
		g = 0 ;
		if ( ( ( ( spr2x-(p2.spunch.offx - p2.width) ) < (spr1x+p1.limdefx) )
			&& ( ( spr2x-(p2.wpunch.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
		&& ( ( ( spr2x-(p2.skick.offx - p2.width) ) < (spr1x+p1.limdefx) )
			&& ( ( spr2x-(p2.wkick.offx - p2.width) ) < (spr1x+p1.limdefx) ) ) )

		{
			// if offensive
		if (h < offense)
		{
			g = 0 ;

			if ( (limp2>0) && !p2.fireb.succ && !p2.rush.succ 
				&& !p2.smove.succ && !p2.firebx.succ && !p2.rushx.succ 
				&& !p2.smovex.succ )
			{

				if ( i < 5 )
				{
					actual2 = 0 ; 
					ai_static=100;
					g=1;
				}
					
				else
				if ( ( i < 20 )
				&& ( ( spr2x-(p2.spunch.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
				{
						Paf(2) ;
						actual2 = 6 ;
						flag2 = 0 ;
						fx = 1 ;
				}

				else
				if ( ( i < 35 )  
				&& ( ( spr2x-(p2.wpunch.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
				{
						Paf(1) ;
						actual2 = 61 ;
						flag2 = 0 ;
						fx = 1 ;
				}

				else
				if ( ( i < 60 ) 
				&& ( ( spr2x-(p2.skick.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
				{
						Paf(4) ;
						actual2 = 7 ;
						flag2 = 0 ;
						fx = 1 ;
				}

				else 
				if ( ( i < 80 ) 
				&& ( ( spr2x-(p2.wkick.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
				{
						Paf(3) ;
						actual2 = 71 ;
						flag2 = 0 ;
						fx = 1 ;
				}

				else 
				if ( i < 95 )
				{
					if ( ( i < 85 ) && p2.smovex.flg && p2.smovex.sflg )
					{ 
					  if (p2.smovex.spec) 
					  {
						  p2.smovex.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( dragonx2 , snd_vol, pan, pitch, 0 );
					  p2.smovex.succ = 0;
					  actual2 = 51 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
					}

					else
					if ( ( p2.smove.flg )
					&& ( ( spr2x-(p2.smove.offx - p2.width) ) < (spr1x+p1.limdefx) ) )
					{ 
						play_sample( dragon2 , snd_vol, pan, pitch, 0 );
						p2.smove.succ = 0;
						actual2 = 50 ;
						flag2 = 0 ; autho_ht2 = 1 ;
						spower2 += 10 ;
					}

					else 
					{
						g=1;
						ai_walkb = 100 ;
					}
               
				}

				else 
				{
					g=1;
					ai_walkb = 100 ;
				}


			}

			else 
			if ( actual == 111 ) 
			{
				g=0;
				actual2 = 2 ;
				//ai_walkb = 100 ;
			
			}

		}

		else // if not offensive
		{
			i = rand()%100 ;

			if ( i < 97 )
			{
				ai_walkb = 100 ;
				actual2 = 111 ;
				g = 1 ;

			}

			else actual2 = 5 ;


		}

		}

		else g = 1 ;

		i = rand()%100 ;
		h = rand()%100 ;

		// if not reachable
		if (g)
		{

		if ( ai_static ) // if walking
		{		
			ai_static-- ;

			actual2 = 0 ;

		}

		else
		if ( ai_walkf ) // if walking
		{		
			ai_walkf-- ;

			if (zoomwidth>320) zoomwidth-- ;

			if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= 2 ;

			else

			if ( spr1x > (limg+cx) ) { spr1x-- ; spr2x-- ;}

			else

			if( cx > 1) { cx--; spr1x-- ; spr2x-- ;}


			flag2++ ;

			if ( flag2 > ((p2.walk.spd*p2.walk.nbf)-1) ) flag2 = 0 ;

			actual2 = 1 ;

		}

		else
		if ( ai_walkb )
		{	
			ai_walkb-- ;

					 if ( spr2x < (limd+cx) ) spr2x += 2 ;

					 else

					 if ( spr1x > (limg+cx) )

					 for( i = 0; i < s; i++ )
					 {
						if( cx < (bkgdlim+320-zoomwidth) )

						{ cx++; spr2x++;}

					 }

					 else

					 for( i = 0; i < s; i++ )
					 {
						if( cx < (bkgdlim+320-zoomwidth) )

							if (zoomwidth< ((200+zoomsup)*1.6) )

							{
								zoomwidth++;

								spr2x++;
							}

					 }

					 flag2-- ;

					 if ( flag2 < 0 ) flag2 = ((p2.walk.spd*p2.walk.nbf)-1) ;

					 actual2 = 111 ;

		}

		else // if not walking		
		if (h < offense ) // if offensive
		{
			
			if ( i < 25)		
			{
					 actual2 = 1 ;
					 ai_walkf= 100 ;
					 
			}
			else 
			
			if ( i < 30) actual2 = 4 ;

			else
			if ( (limp2>0) && !p2.fireb.succ && !p2.rush.succ )
			{

				if ( i < 65 ) 
				{
					if ( ( i < 45 ) && p2.firebx.flg && p2.firebx.sflg )
					{ 
					  if (p2.firebx.spec) 
					  {
						  p2.firebx.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( hadokenx2 , snd_vol, pan, pitch, 0 );
					  p2.firebx.succ = 0;
					  actual2 = 31 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
					}

					else
					if ( p2.fireb.flg )
					{ 
						play_sample( hadoken2 , snd_vol, pan, pitch, 0 );
						p2.fireb.succ = 0;
						actual2 = 30 ;
						flag2 = 0 ; autho_ht2 = 1 ;
						spower2 += 10 ;
					}

					else actual2 = 4 ;
               
				}

				else
				{
					if ( ( i < 80 ) && p2.rushx.flg && p2.rushx.sflg )
					{ 
					  if (p2.rushx.spec) 
					  {
						  p2.rushx.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( copterx2 , snd_vol, pan, pitch, 0 );
					  p2.rushx.succ = 0;
					  actual2 = 41 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
					}

					else
					if ( p2.rush.flg )
					{ 
						play_sample( copter2 , snd_vol, pan, pitch, 0 );
						p2.rush.succ = 0;
						actual2 = 40 ;
						flag2 = 0 ; autho_ht2 = 1 ;
						spower2 += 10 ;
					}

					else actual2 = 4 ;
                
				}


			 }

		}

		else
		//if not offensive
		{
			i = rand()%100 ;
	
			if ( i < 50 ) 
			{
				actual2 = 0 ; 
				ai_static=100;
			}

			else
			if ( i < 65 ) 
			{
			actual2 = 5 ;
			}

			else
			if ( i < 80 ) 
			{
			actual2 = 3 ;
			}

			else
			if ( i < 85 ) 
			{
				actual2 = 1 ;
				ai_walkf = 100 ;
			}

			else
			{		 
				 actual2 = 111 ;
				 ai_walkb = 100 ;

			}
	
		}

	}
     
}
}


// 2 players game

if ( ((actual2 < 3) || (actual2 >100)) && !gmode )	
{
      actual2 = 0 ;

      //poll_joystick() ;


      if( (key[p2_right]) && (key[p2_up])  )
      {
	  actual2 = 5 ;
      }

      else

      if( (key[p2_left]) && (key[p2_up]) )
      {
	  actual2 = 4 ;
      }

      else

      if (key[p2_up]) 
      {
	  actual2 = 3 ;
      }

      else

      if (key[p2_down]) 
      {
	  actual2 = 2 ;
      }

      else

      if (key[p2_left]) 
      {
         if (zoomwidth>320) zoomwidth-- ;

		 if ( (spr2x+tol2) > (spr1x+tol1) ) spr2x -= 2 ;

		 else

		 if ( spr1x > (limg+cx) ) { spr1x-- ; spr2x-- ;}

		 else

		 if( cx > 1) { cx--; spr1x-- ; spr2x-- ;}


		 flag2++ ;

		 if ( flag2 > ((p2.walk.spd*p2.walk.nbf)-1) ) flag2 = 0 ;

		 actual2 = 1 ;

      }

      else

      if (key[p2_right])
      {
		 if ( spr2x < (limd+cx) ) spr2x += 2 ;

		 else

		 if ( spr1x > (limg+cx) )

		 for( i = 0; i < s; i++ )
		 {
			if( cx < (bkgdlim+320-zoomwidth) )

			{ cx++; spr2x++;}

		 }

		 else

		 for( i = 0; i < s; i++ )
		 {
			if( cx < (bkgdlim+320-zoomwidth) )

				if (zoomwidth< ((200+zoomsup)*1.6) )

				{
					zoomwidth++;

					spr2x++;
				}

		 }

		 flag2-- ;

		 if ( flag2 < 0 ) flag2 = ((p2.walk.spd*p2.walk.nbf)-1) ;

		 actual2 = 111 ;

      }

      if ( ((actual2 < 2) || (actual2 >100)) && (limp2>0) 
			&& !p2.fireb.succ && !p2.rush.succ && !p2.smove.succ
			&& !p2.firebx.succ && !p2.rushx.succ && !p2.smovex.succ )
      {

        if (key[p2_spunch]) 
        {

                Paf(2) ;

                actual2 = 6 ;

                flag2 = 0 ;

                fx = 1 ;
        }

        else

		if (key[p2_wpunch]) 
        {

                Paf(1) ;

                actual2 = 61 ;

                flag2 = 0 ;

                fx = 1 ;
        }

		else

        if (key[p2_skick]) 
        {

                Paf(4) ;

                actual2 = 7 ;

                flag2 = 0 ;

                fx = 1 ;
        }

		else

        if (key[p2_wkick]) 
        {

                Paf(3) ;

                actual2 = 71 ;

                flag2 = 0 ;

                fx = 1 ;
        }

      }

      if ( (actual2 == 2) && (limp2>0) )
	  {
        if( (key[p2_wkick]) || (key[p2_skick]))
        {

                Paf(4) ;

                actual2 = 10 ;

                flag2 = 0 ;

                fx = 1 ;
        }

		else

		if( (key[p2_wpunch]) || (key[p2_spunch]))
        {

                Paf(1) ;

                actual2 = 13 ;

                flag2 = 0 ;

                fx = 1 ;
        }

	  }
	  
if ( p2.fireb.flg )
{
	  if ( check_kstatus2( p2.fireb.seq[0] ) && !p2.fireb.succ )

	  { p2.fireb.succ = 1 ; p2.fireb.nb = 0 ; }

	  else
	  if (p2.fireb.succ)
	  {
		  if ( (last_status2 == p2.fireb.seq[p2.fireb.nb]) ||  (last_status2 == p2.fireb.seq[p2.fireb.nb+1]) )
		  {
			  if (check_kstatus2(p2.fireb.seq[p2.fireb.nb+1]) )
			  {
				  p2.fireb.nb++ ; p2.fireb.succ = p2.fireb.nb + 1 ;

				  if (p2.fireb.succ == p2.fireb.end) 
				  { 
					  play_sample( hadoken2 , snd_vol, pan, pitch, 0 );
					  p2.fireb.succ = 0;
					  actual2 = 30 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.fireb.succ = 0 ;
		  }

		  else p2.fireb.succ = 0 ;
	  }

	  else p2.fireb.succ = 0 ;
}

else p2.fireb.succ = 0 ;



if ( p2.rush.flg )
{
	  if ( check_kstatus2( p2.rush.seq[0] ) && !p2.rush.succ )

	  { p2.rush.succ = 1 ; p2.rush.nb = 0 ; }

	  else
	  if (p2.rush.succ)
	  {
		  if ( (last_status2 == p2.rush.seq[p2.rush.nb]) || (last_status2 == p2.rush.seq[p2.rush.nb+1]) )
		  {
			  if (check_kstatus2(p2.rush.seq[p2.rush.nb+1]) )
			  {
				  p2.rush.nb++ ; p2.rush.succ = p2.rush.nb + 1 ;

				  if (p2.rush.succ == p2.rush.end) 
				  { 
					  play_sample( copter2 , snd_vol, pan, pitch, 0 );
					  p2.rush.succ = 0;
					  actual2 = 40 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.rush.succ = 0 ;
		  }

		  else p2.rush.succ = 0 ;
	  }

	  else p2.rush.succ = 0 ;
}
	
else p2.rush.succ = 0 ;



if ( p2.smove.flg )
{
	  if ( check_kstatus2( p2.smove.seq[0] ) && !p2.smove.succ )

	  { p2.smove.succ = 1 ; p2.smove.nb = 0 ; }

	  else
	  if (p2.smove.succ)
	  {
		  if ( (last_status2 == p2.smove.seq[p2.smove.nb]) || (last_status2 == p2.smove.seq[p2.smove.nb+1]) )
		  {
			  if (check_kstatus2(p2.smove.seq[p2.smove.nb+1]) )
			  {
				  p2.smove.nb++ ; p2.smove.succ = p2.smove.nb + 1 ;

				  if (p2.smove.succ == p2.smove.end) 
				  { 
					  play_sample( dragon2 , snd_vol, pan, pitch, 0 );
					  p2.smove.succ = 0;
					  actual2 = 50 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.smove.succ = 0 ;
		  }

		  else p2.smove.succ = 0 ;
	  }

	  else p2.smove.succ = 0 ;
}
	
else p2.smove.succ = 0 ;

// NOW FOR SPECIALS

if ( p2.firebx.flg && p2.firebx.sflg )
{
	  if ( check_kstatus2( p2.firebx.seq[0] ) && !p2.firebx.succ )

	  { p2.firebx.succ = 1 ; p2.firebx.nb = 0 ; }

	  else
	  if (p2.firebx.succ)
	  {
		  if ( (last_status2 == p2.firebx.seq[p2.firebx.nb]) ||  (last_status2 == p2.firebx.seq[p2.firebx.nb+1]) )
		  {
			  if (check_kstatus2(p2.firebx.seq[p2.firebx.nb+1]) )
			  {
				  p2.firebx.nb++ ; p2.firebx.succ = p2.firebx.nb + 1 ;

				  if (p2.firebx.succ == p2.firebx.end) 
				  { 
					  if (p2.firebx.spec) 
					  {
						  p2.firebx.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( hadokenx2 , snd_vol, pan, pitch, 0 );
					  p2.firebx.succ = 0;
					  actual2 = 31 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.firebx.succ = 0 ;
		  }

		  else p2.firebx.succ = 0 ;
	  }

	  else p2.firebx.succ = 0 ;
}

else p2.firebx.succ = 0 ;



if ( p2.rushx.flg && p2.rushx.sflg )
{
	  if ( check_kstatus2( p2.rushx.seq[0] ) && !p2.rushx.succ )

	  { p2.rushx.succ = 1 ; p2.rushx.nb = 0 ; }

	  else
	  if (p2.rushx.succ)
	  {
		  if ( (last_status2 == p2.rushx.seq[p2.rushx.nb]) || (last_status2 == p2.rushx.seq[p2.rushx.nb+1]) )
		  {
			  if (check_kstatus2(p2.rushx.seq[p2.rushx.nb+1]) )
			  {
				  p2.rushx.nb++ ; p2.rushx.succ = p2.rushx.nb + 1 ;

				  if (p2.rushx.succ == p2.rushx.end) 
				  { 
					  if (p2.rushx.spec) 
					  {
						  p2.rushx.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( copterx2 , snd_vol, pan, pitch, 0 );
					  p2.rushx.succ = 0;
					  actual2 = 41 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.rushx.succ = 0 ;
		  }

		  else p2.rushx.succ = 0 ;
	  }

	  else p2.rushx.succ = 0 ;
}
	
else p2.rushx.succ = 0 ;



if ( p2.smovex.flg && p2.smovex.sflg )
{
	  if ( check_kstatus2( p2.smovex.seq[0] ) && !p2.smovex.succ )

	  { p2.smovex.succ = 1 ; p2.smovex.nb = 0 ; }

	  else
	  if (p2.smovex.succ)
	  {
		  if ( (last_status2 == p2.smovex.seq[p2.smovex.nb]) || (last_status2 == p2.smovex.seq[p2.smovex.nb+1]) )
		  {
			  if (check_kstatus2(p2.smovex.seq[p2.smovex.nb+1]) )
			  {
				  p2.smovex.nb++ ; p2.smovex.succ = p2.smovex.nb + 1 ;

				  if (p2.smovex.succ == p2.smovex.end) 
				  { 
					  if (p2.smovex.spec) 
					  {
						  p2.smovex.sflg = 0 ;
						  spower2 = 0 ;
					  }
					  play_sample( dragonx2 , snd_vol, pan, pitch, 0 );
					  p2.smovex.succ = 0;
					  actual2 = 51 ;
					  flag2 = 0 ; autho_ht2 = 1 ;
					  spower2 += 10 ;
				  }
			  }

			  else
			  if ( !check_kstatus2(last_status2) ) p2.smovex.succ = 0 ;
		  }

		  else p2.smovex.succ = 0 ;
	  }

	  else p2.smovex.succ = 0 ;
}
	
else p2.smovex.succ = 0 ;


}

/* static updating  */

if (actual2 == 0)

{
	flag2++;

	if (flag2 > ((p2.statik.spd*p2.statik.nbf)-1) ) flag2 = 0 ;

	if ( spr2y < (sol2-1) ) spr2y = spr2y + 3 ;

}

if ( !(key[p2_wpunch]) && !(key[p2_wkick]) 
	&& !(key[p2_spunch]) && !(key[p2_skick]) && (limp2<1) )

limp2++ ;


// UPDATE AIR-KICK ***********/

if ( ( actual2 > 2 ) && ( actual2 < 6 ) && ( apch2==0) )

{
  if (ak2 && limak2) { ak2++ ; if (ak2>25) {ak2 = 0 ; limak2 = 0 ;} }

  else

  {
     if (limak2) if (  (key[p2_wkick]) ||  (key[p2_skick]) ) { ak2 = 1 ; Paf(3) ; fx = 1 ; }
  }
}

else ak2 = 0 ;

// UPDATE AIR-PUNCH ***********/

if ( ( actual2 > 2 ) && ( actual2 < 6 ) && (ak2==0) )

{
  if (apch2 && limak2) { apch2++ ; if (apch2>25) {apch2 = 0 ; limak2 = 0 ;} }

  else

  {
     if (limak2) if ( (key[p2_spunch]) ||  (key[p2_wpunch]) ) { apch2 = 1 ; Paf(1) ;  fx = 1 ; }
  }
}

else apch2 = 0 ;

// AIR AI !!!

if ( ( actual2 > 2 ) && ( actual2 < 6 ) && gmode && (ak2==0) && (apch2==0))     
{
	i = rand()%100 ;
	h = rand()%100 ;
	com_block = 0 ;
	com_gblock = 0 ;
	
	if ( (spr1x+p1.akick.w) > spr2x )
	{
		if ( (actual == 6) || (actual == 7) || (actual==61)
				|| (actual==71) || ak || apch || (actual==40) )
		{ 
		
			if ( h < defense )
			{
				com_gblock = 0 ;
				com_block = 1 ; 
			}

			else com_block = 0 ;
		}

	}

	if ( (( spr2x-(p2.akick.w - p2.width) ) < (spr1x+p1.width)) )
	{
		// if offensive
		if (i < offense)
		{
			ak2 = 1 ;
		}

		else ak2 = 0 ;

	}

}



/**** CHECK PLAYER 1 ATTACKS*******/

if( (actual == 6) && ( flag == ( (p1.spunch.hit-1)*p1.spunch.spd) ) )// PUNCH
{
   if ( ( (spr1x+p1.spunch.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.spunch.offy+10) > (spr2y+p2.limdefy) ) )
   {
		poll_joystick() ;

		damag2 = p1.spunch.dmg ;
		
		p1.limoffx = spr1x + p1.spunch.offx ;
		p1.limoffy = spr1y + p1.spunch.offy ;

        if ( spr2y<(sol2-5) )

        { actual2 = 20 ; hitfram1 = (p1.spunch.hit-1)*p1.spunch.spd ; 
		  p1.wimpact_ct = 10 ;
		}
   
        else

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else { actual2 = 19 ; hitfram1 = (p1.spunch.hit-1)*p1.spunch.spd ;
				   p1.wimpact_ct = 10 ;
			     }
        }

        else

        {
			if (key[p2_right] || com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ;}

	        else { actual2 = 9 ; hitfram1 = (p1.spunch.hit-1)*p1.spunch.spd ;
				   p1.wimpact_ct = 10 ;
			     }

        }

	flag2 = 0 ;
   }
}

else

if( (actual == 61) && ( flag == ( (p1.wpunch.hit-1)*p1.wpunch.spd) ) )// WPUNCH
{
   if ( ( (spr1x+p1.wpunch.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.wpunch.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.wpunch.dmg ;

		p1.limoffx = spr1x + p1.wpunch.offx ;
		p1.limoffy = spr1y + p1.wpunch.offy ;

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.wpunch.hit-1)*p1.wpunch.spd ; }
        }

        else

        {
			if (key[p2_right] || com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 9 ; hitfram1 = (p1.wpunch.hit-1)*p1.wpunch.spd ; }

        }

	flag2 = 0 ;
   }
}

else

if( (actual == 7) && ( flag == ( (p1.skick.hit-1)*p1.skick.spd) ) )// KICK
{
   if ( ( (spr1x+p1.skick.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.skick.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.skick.dmg ;

		p1.limoffx = spr1x + p1.skick.offx ;
		p1.limoffy = spr1y + p1.skick.offy ;

        if ( spr2y<(sol2-5) )

        { p1.wimpact_ct = 10 ; actual2 = 20 ; hitfram1 = (p1.skick.hit-1)*p1.skick.spd ; }

        else

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.skick.hit-1)*p1.skick.spd ; }
        }

        else

        {
			if (key[p2_right]|| com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 9 ; hitfram1 = (p1.skick.hit-1)*p1.skick.spd ; }
        }

	flag2 = 0 ;
   }
}

else

if( (actual == 71) && ( flag == ( (p1.wkick.hit-1)*p1.wkick.spd) ) )// WKICK
{
   if ( ( (spr1x+p1.wkick.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.wkick.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.wkick.dmg ;

		p1.limoffx = spr1x + p1.wkick.offx ;
		p1.limoffy = spr1y + p1.wkick.offy ;

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.wkick.hit-1)*p1.wkick.spd ; }
        }

        else

        {
			if (key[p2_right]|| com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 9 ; hitfram1 = (p1.wkick.hit-1)*p1.wkick.spd ; }
        }

	flag2 = 0 ;
   }
}

else

if( (actual == 10)

        && ( flag == ( (p1.gkick.hit-1)*p1.gkick.spd) )

        && ( (actual2 > 5) || (actual2 < 3) ) )   // GKICK
{
   
   if ( ( (spr1x+p1.gkick.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.gkick.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.gkick.dmg ;

		p1.limoffx = spr1x + p1.gkick.offx ;
		p1.limoffy = spr1y + p1.gkick.offy ;

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

                else { p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.gkick.hit-1)*p1.gkick.spd ; }
        }

        else if ( flag < (p1.gkick.hit*p1.gkick.spd) )

        { p1.wimpact_ct = 10 ; actual2 = 20 ; hitfram1 = (p1.gkick.hit-1)*p1.gkick.spd ; }

	flag2 = 0 ;
   }
}

else

if( (actual == 13)

        && ( flag == ( (p1.gpunch.hit-1)*p1.gpunch.spd) )

        && ( (actual2 > 5) || (actual2 < 3) ) )   // GPUNCH

{
   if ( ( (spr1x+p1.gpunch.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.gpunch.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.gpunch.dmg ;

		p1.limoffx = spr1x + p1.gpunch.offx ;
		p1.limoffy = spr1y + p1.gpunch.offy ;

        if (key[p2_down]|| com_gblock)

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

                else { p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.gpunch.hit-1)*p1.gpunch.spd ; }
        }

        else if ( flag < (p1.gpunch.hit*p1.gpunch.spd) )

        { p1.wimpact_ct = 10 ; actual2 = 9 ; hitfram1 = (p1.gpunch.hit-1)*p1.gpunch.spd ; }

	flag2 = 0 ;
   }
}

else

if ( ak == (p1.akick.hit-1)*p1.akick.spd ) // AIR-KICK
{
   if ( ( (spr1x+p1.akick.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.akick.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.akick.dmg ;

		p1.limoffx = spr1x + p1.akick.offx ;
		p1.limoffy = spr1y + p1.akick.offy ;

        if ( spr2y<(sol2-5) )

        { p1.wimpact_ct = 10 ; actual2 = 20 ; hitfram1 = (p1.akick.hit-1)*p1.akick.spd ; }

        else

        if (key[p2_down]|| com_gblock) 

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else 
			
			if (spr1y>(sol1-20)) 
			{ p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.akick.hit-1)*p1.akick.spd ; }
        }

        else

        {
			if (key[p2_right]|| com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ;}

	        else { p1.wimpact_ct = 10 ; actual2 = 9 ; /*hitfram1 = (p1.akick.hit-1)*p1.akick.spd ;*/ }
        }

	flag2 = 0 ;
   }
}

else

if ( apch == (p1.apunch.hit-1)*p1.apunch.spd ) // AIR-PUNCH
{
   if ( ( (spr1x+p1.apunch.offx) > (spr2x + p2.width - p2.limdefx) )
	 && ( (spr1y+p1.apunch.offy+10) > (spr2y+p2.limdefy) ) )
   {
		damag2 = p1.apunch.dmg ;

		p1.limoffx = spr1x + p1.apunch.offx ;
		p1.limoffy = spr1y + p1.apunch.offy ;

        if ( spr2y<(sol2-5) )

        { p1.wimpact_ct = 10 ; actual2 = 20 ; hitfram1 = (p1.apunch.hit-1)*p1.apunch.spd ; }

        else

        if (key[p2_down]|| com_gblock) 

        {
			if (key[p2_right] || com_gblock ) { p1.bimpact_ct = 10 ; actual2 = 18 ;}

	        else 
			
			if (spr1y>(sol1-20))
			{ p1.wimpact_ct = 10 ; actual2 = 19 ; hitfram1 = (p1.apunch.hit-1)*p1.apunch.spd ; }
        }

        else

        {
			if (key[p2_right]|| com_block ) { p1.bimpact_ct = 10 ; actual2 = 8 ; }

			else { p1.wimpact_ct = 10 ; actual2 = 9 ;/*hitfram1 = (p1.apunch.hit-1)*p1.apunch.spd ; */}
        }

	flag2 = 0 ;
   }
}

/**** CHECK PLAYER 2 ATTACKS *******/

if( (actual2 == 6) && ( flag2 == ( (p2.spunch.hit-1)*p2.spunch.spd) ) ) // PUNCH
{
   if ( ( ( spr2x-(p2.spunch.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.spunch.offy+10) > (spr1y+p1.limdefy) ) )
   {
	    damag1 = p2.spunch.dmg ;

		p2.limoffx = spr2x-(p2.spunch.offx - p2.width) ;
		p2.limoffy = spr2y + p2.spunch.offy ;

        if ( spr1y<(sol1-5) )

        {  p2.wimpact_ct = 10 ; actual = 20 ; hitfram2 = (p2.spunch.hit-1)*p2.spunch.spd ; }

        else

        if ( P1_DN )

        {
			if ( P1_LT )  { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.spunch.hit-1)*p2.spunch.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

                else { p2.wimpact_ct = 10 ; actual = 9 ; hitfram2 = (p2.spunch.hit-1)*p2.spunch.spd ; }
        }

	flag = 0 ;
   }
}

else

if( (actual2 == 61) && ( flag2 == ( (p2.wpunch.hit-1)*p2.wpunch.spd) ) ) // WPUNCH
{
   if ( ( ( spr2x-(p2.wpunch.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.wpunch.offy+10) > (spr1y+p1.limdefy) ) )
   {
	    damag1 = p2.wpunch.dmg ;

		p2.limoffx = spr2x-(p2.wpunch.offx - p2.width) ;
		p2.limoffy = spr2y + p2.wpunch.offy ;
		
        if ( P1_DN)

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.wpunch.hit-1)*p2.wpunch.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

                else { p2.wimpact_ct = 10 ; actual = 9 ; hitfram2 = (p2.wpunch.hit-1)*p2.wpunch.spd ; }
        }

	flag = 0 ;
   }
}

else

if( (actual2 == 7) && ( flag2 == ( (p2.skick.hit-1)*p2.skick.spd) ) )// KICK
{
   if ( ( ( spr2x-(p2.skick.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.skick.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.skick.dmg ;

	   p2.limoffx = spr2x-(p2.skick.offx - p2.width) ; 
	   p2.limoffy = spr2y + p2.skick.offy ;

        if ( spr1y<(sol1-5) )

        { p2.wimpact_ct = 10 ; actual = 20 ; hitfram2 = (p2.skick.hit-1)*p2.skick.spd ; }

        else

        if ( P1_DN )

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.skick.hit-1)*p2.skick.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

	        else { p2.wimpact_ct = 10 ; actual = 9 ; hitfram2 = (p2.skick.hit-1)*p2.skick.spd ; }
        }

	flag = 0 ;
   }
}


else

if( (actual2 == 71) && ( flag2 == ( (p2.wkick.hit-1)*p2.wkick.spd) ) )// WKICK
{
   if ( ( ( spr2x-(p2.wkick.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.wkick.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.wkick.dmg ;

	   p2.limoffx = spr2x-(p2.wkick.offx - p2.width) ; 
	   p2.limoffy = spr2y + p2.wkick.offy ;

        if ( P1_DN )

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.wkick.hit-1)*p2.wkick.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

	        else { p2.wimpact_ct = 10 ; actual = 9 ; hitfram2 = (p2.wkick.hit-1)*p2.wkick.spd ; }
        }

	flag = 0 ;
   }
}

else

if( (actual2 == 10)

        && ( flag2 == ( (p2.gkick.hit-1)*p2.gkick.spd) )

        && ( (actual > 5) || (actual < 3) ) )// GKICK

{

   if ( ( ( spr2x-(p2.gkick.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.gkick.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.gkick.dmg ;

	   p2.limoffx = spr2x-(p2.gkick.offx - p2.width) ;
	   p2.limoffy = spr2y + p2.gkick.offy ;

        if ( P1_DN )

        {

			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.gkick.hit-1)*p2.gkick.spd ; }

        }

        else if ( flag2 < (p2.gkick.hit*p2.gkick.spd) )

        { p2.wimpact_ct = 10 ; actual = 20 ; hitfram2 = (p2.gkick.hit-1)*p2.gkick.spd ; }

	flag = 0 ;
   }
}

else

if( (actual2 == 13)

        && ( flag2 == ( (p2.gpunch.hit-1)*p2.gpunch.spd) )

        && ( (actual > 5) || (actual < 3) ) )// GPUNCH

{

   if ( ( ( spr2x-(p2.gpunch.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.gpunch.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.gpunch.dmg ;

	   p2.limoffx = spr2x-(p2.gpunch.offx - p2.width) ;
	   p2.limoffy = spr2y + p2.gpunch.offy ;

        if ( P1_DN )

        {

			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else { p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.gpunch.hit-1)*p2.gpunch.spd ; }

        }

        else if ( flag2 < (p2.gpunch.hit*p2.gpunch.spd) )

        { p2.wimpact_ct = 10 ; actual = 9 ; hitfram2 = (p2.gpunch.hit-1)*p2.gpunch.spd ; }

	flag = 0 ;
   }
}

else

if (ak2 == (p2.akick.hit-1)*p2.akick.spd) // AIR-KICK
{
   if ( ( ( spr2x-(p2.akick.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.akick.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.akick.dmg ;

	   p2.limoffx = spr2x-(p2.akick.offx - p2.width) ;
	   p2.limoffy = spr2y + p2.akick.offy ;

        if ( spr1y<(sol1-5) )

        { p2.wimpact_ct = 10 ; actual = 20 ; hitfram2 = (p2.akick.hit-1)*p2.akick.spd ; }

        else

        if ( P1_DN )

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else 
			
			if (spr2y>(sol2-20))
			{ p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.akick.hit-1)*p2.akick.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

			else { p2.wimpact_ct = 10 ; actual = 9 ; /*hitfram2 = (p2.akick.hit-1)*p2.akick.spd ;*/ }

        }

	flag = 0 ;
   }
}


else

if (apch2 == (p2.apunch.hit-1)*p2.apunch.spd) // AIR-PUNCH
{
   if ( ( ( spr2x-(p2.apunch.offx - p2.width) ) < (spr1x+p1.limdefx) )
     && ( ( spr2y+p2.apunch.offy+10) > (spr1y+p1.limdefy) ) )
   {
	   damag1 = p2.apunch.dmg ;

	   p2.limoffx = spr2x-(p2.apunch.offx - p2.width) ;
	   p2.limoffy = spr2y + p2.apunch.offy ;

        if ( spr1y<(sol1-5) )

        { p2.wimpact_ct = 10 ; actual = 20 ; hitfram2 = (p2.apunch.hit-1)*p2.apunch.spd ; }

        else

        if ( P1_DN )

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 18 ;}

	        else 
			
			if (spr2y>(sol2-20))
			{ p2.wimpact_ct = 10 ; actual = 19 ; hitfram2 = (p2.apunch.hit-1)*p2.apunch.spd ; }
        }

        else

        {
			if ( P1_LT ) { p2.bimpact_ct = 10 ; actual = 8 ;}

			else { p2.wimpact_ct = 10 ; actual = 9 ; /*hitfram2 = (p2.apunch.hit-1)*p2.apunch.spd ; */}        
        }

	flag = 0 ;
   }
}


/***** CHECK AND UPDATE LIFEBARS **********/

   draw_sprite( virtscreen , LBbanner ,0 ,0 ) ;

   blit ( Bar, virtscreen, (125-(life1/4)), 0, 20+(125-(life1/4)), 12,(life1/4)+2, 7 ) ;
                
   blit ( Bar2, virtscreen, 0, 0, 175, 12,(life2/4)+1, 7 ) ;
    
   if (spower1>=60) 
   {
	   spower1 = 60 ;
	   p1.firebx.sflg = p1.rushx.sflg = p1.smovex.sflg = 1 ;
   }
   if (spower2>=60) 
   {
	   spower2 = 60 ;
	   p2.firebx.sflg = p2.rushx.sflg = p2.smovex.sflg = 1 ;
   }

   draw_sprite( virtscreen , Power , 4,172 ) ;

   draw_sprite( virtscreen , Power , 249,172 ) ;

   blit ( PBar, virtscreen, 0, 0, 7, 190,spower1, 6 ) ;
                
   blit ( PBar, virtscreen, 0, 0, 312-spower2, 190,spower2, 6 ) ;


/*** TIME OVER ***/
#if TCLOCK == 1
	if (clock_frame++ >= t_decrement) {
	    clock_frame = 0;
	    if (t_clock-- <= 1) {
/*            t_clock = t_clock_ini; 
*/
		if (life1 < life2)
		    life1 = 0;
		else
		    life2 = 0;
	    }
	    sprintf(s_cron, "%d", t_clock);
	    c_dez = s_cron[0] - 48;
	    c_uni = s_cron[1] - 48;
	}

 	if (s_cron[1] != NULL) {
	    if (t_clock <= t_short)
		set_trans_blender(255, 0, 0, 50);
	    else
		set_trans_blender(255, 255, 255, 0);
	    draw_lit_sprite(virtscreen, B_clock[c_dez], 147, 22, 100);
	    draw_lit_sprite(virtscreen, B_clock[c_uni], 162, 22, 100);
	} else {
	    if (t_clock <= t_short)
		set_trans_blender(255, 0, 0, 100);
	    else
		set_trans_blender(255, 255, 255, 0);
	    draw_lit_sprite(virtscreen, B_clock[c_dez], 154, 22, 100);
	}
#endif


/**** VICTORY FLAGS ***/

if (vic_flag>20)

{
   if (vic1) draw_sprite( virtscreen , avic1 ,10 ,24 ) ;
	
   else draw_sprite( virtscreen , novic1 ,10 ,24 ) ;

   if (vic2) draw_sprite( virtscreen , avic1 ,294 ,24 ) ;

   else draw_sprite( virtscreen , novic1 ,294 ,24 ) ;
}

else

{
   if (vic1) draw_sprite( virtscreen , avic0 ,10 ,24 ) ;
	
   else draw_sprite( virtscreen , novic0 ,10 ,24 ) ;

   if (vic2) draw_sprite( virtscreen , avic0 ,294 ,24 ) ;

   else draw_sprite( virtscreen , novic0 ,294 ,24 ) ;
}

/**** END OF ROUND ****/

if ( ( life1 <= 0 ) || ( life2 <= 0 ) )
{

//	if (!nbpar) t2 = time( 0 );

	nbpar++ ;

    checkint = 0 ;

    fx = fy = 1 ;

    flag = flag2 = 1 ;

	cy = (200+zoomsup) - ZMRATIO ;

	if (cy<1) cy =1 ;
	
	if (retrace_count>2) retrace_count = 0 ;

	poll_joystick() ;

	if ( life1 <= 0 )
	{
		vic2++;

                if ( female1 )

                play_sample( female_snd , snd_vol, pan, pitch, 0 );

                else

                play_sample( argh_snd , snd_vol, pan, pitch, 0 );

                for ( flag = 1 ; flag < 160 ; ++flag )

                {
					if (animated) bkgd_counter++;

					retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
						blit_Bkgd() ;

                        if ( spr2y < sol2 )

                        GJump2 ( 5 , spr2x , spr2y );

                        else

		                GStatic2 ( (flag2/p2.statik.spd), spr2x , spr2y );

                        GDead ( (flag/20) , spr1x , spr1y );

						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						cy = (200+zoomsup) - ZMRATIO ;

						if (cy<1) cy =1 ;       

				   	    if ( (cy<zoomsup) && (cy>0) )

					    stretch_blit ( zoomscreen , virtscreen ,

					    cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					    else

                        blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;

                        //vsync() ;

                        blit_KOF91();

						while(!retrace_count);

					}

	                    flag2++ ;

                        if (flag2 > ((p2.statik.spd*p2.statik.nbf)-1) ) flag2 = 0 ;

                        if ( spr2y < sol2 ) spr2y++ ;

                        if ( spr1y < sol1 ) spr1y++ ;

                        if ( spr1x > (limg+cx) )  spr1x -- ;

						else

						 if ( spr2x < (limd+cx) )

						 for( i = 0; i < s; i++ )
						 {
							if( cx > 1)
							  { cx--;spr1x--;}

						 }

						 else

						 for( i = 0; i < s; i++ )
						 {
							if( cx > 1)

								if (zoomwidth< ((200+zoomsup)*1.6))
								{
									cx--;

									spr1x--;

									zoomwidth++;
								}

						 }
                      

                }

#if TIMEOVER == 1
		if (t_clock <= 1) {
		    masked_blit(Carton_TO, virtscreen, 0, 0,
				(320 - Carton_TO->w) / 2,
				(200 - Carton_TO->h) / 2, Carton_TO->w,
				Carton_TO->h);
		    blit_KOF91();
		} else {
#endif

				masked_blit ( Carton_KO , virtscreen , 0 , 0
						 , (320-Carton_KO->w)/2 
						 , (200-Carton_KO->h)/2 
						 , Carton_KO->w ,Carton_KO->h) ;

                blit_KOF91();

#if TIMEOVER == 1
		}
		if (t_clock <= 1)
		{
		    play_sample(to_snd, snd_vol, pan, pitch, 0);
			rest(30);
		}
		else
#endif
                play_sample( ko_snd , snd_vol, pan, pitch, 0 );

				rest(30);

				load_win2 ();

				retrace_count = 0 ;

                for ( flag = 1 ; flag < (p2.outwin.nbf*p2.outwin.spd) ; ++flag )

                {                

					if (animated) bkgd_counter++;

					retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
                        blit_Bkgd() ;

                        GDead ( 7 , spr1x , spr1y ) ;

                        GKick2 ( (flag/p2.outwin.spd) , ( spr2x - (p2.outwin.w - p2.width) ) , spr2y , 3) ;

						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						cy = (200+zoomsup) - ZMRATIO ;

						if (cy<1) cy =1 ;  

				   	    if ( (cy<zoomsup) && (cy>0) )

					    stretch_blit ( zoomscreen , virtscreen ,

					    cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					    else

                        blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
                
						blit_KOF91();

						while (!retrace_count);
					
					}

				}


				if ( (cy<zoomsup) && (cy>0) )

			    stretch_blit ( zoomscreen , virtscreen ,

			    cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

				else

                blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;


				angle = (Carton_WN->w/Carton_WN->h) ;

		 		for ( i = Carton_WN->w ; i > (Carton_WN->w/3) ; i-- )
				{

#if PERFECT == 1
		    if (life2 >= 500) 
			{
				draw_sprite(virtscreen, B_prfct,
						160 - B_prfct->w / 2,
						100 - B_prfct->h);
				if (i == Carton_WN->w)
					play_sample(prfct_snd, snd_vol, pan, pitch, 0);
		    }
#endif
					blit_KOF91();

					if ( (cy<zoomsup) && (cy>0) )

					stretch_blit ( zoomscreen , virtscreen ,

					cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					else

					blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
				
					stretch_sprite (  virtscreen ,Carton_WN , 290-i, 30 
						 
									, i , i/angle);

				}
				
	}

	else
	{
		vic1++;

                if (female2)

                play_sample( female_snd , snd_vol, pan, pitch, 0 );

                else

                play_sample( argh_snd , snd_vol, pan, pitch, 0 );

                for ( flag2 = 1 ; flag2 < 160 ; ++flag2 )

                {

					if (animated) bkgd_counter++;

					retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
                        blit_Bkgd() ;

                        if ( spr1y < sol1 )

                        GJump ( 5 , spr1x , spr1y );

                        else

		                GStatic ( (flag/p1.statik.spd), spr1x , spr1y );

                        GDead2 ( (flag2/20) , ( spr2x - (p2.ko.w - p2.width) ) , spr2y );

						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						cy = (200+zoomsup) - ZMRATIO ;

						if (cy<1) cy =1 ;    

						if ( (cy<zoomsup) && (cy>0) )

					    stretch_blit ( zoomscreen , virtscreen ,

					    cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					    else

                        blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;

                        vsync() ;

                        blit_KOF91();

						while (!retrace_count);

					}

	                    flag++ ;

                        if (flag > ((p1.statik.spd*p1.statik.nbf)-1) ) flag = 0 ;

                        if ( spr2y < sol2 ) spr2y++ ;

                        if ( spr1y < sol1 ) spr1y++ ;

                        if (spr2x < (limd+cx) ) spr2x ++ ;

						else

						 if ( spr1x > (limg+cx) )

						 for( i = 0; i < s; i++ )
						 {
							if( cx < (bkgdlim+320-zoomwidth) )

							{ cx++; spr2x++;}

						 }

						 else

						 for( i = 0; i < s; i++ )
						 {
							if( cx < (bkgdlim+320-zoomwidth) )

								if (zoomwidth< ((200+zoomsup)*1.6) )

								{
									zoomwidth++;

									spr2x++;
								}

						 }


                }

#if TIMEOVER == 1
		if (t_clock <= 1) {
		    masked_blit(Carton_TO, virtscreen, 0, 0,
				(320 - Carton_TO->w) / 2,
				(200 - Carton_TO->h) / 2, Carton_TO->w,
				Carton_TO->h);
		    blit_KOF91();
		} else {
#endif
				masked_blit ( Carton_KO , virtscreen , 0 , 0
						 , (320-Carton_KO->w)/2 
						 , (200-Carton_KO->h)/2 
						 , Carton_KO->w ,Carton_KO->h) ;

                blit_KOF91();

#if TIMEOVER == 1
		}

		if (t_clock <= 1)
		{
		    play_sample(to_snd, snd_vol, pan, pitch, 0);
			rest(30);
		}
		else
#endif
                play_sample( ko_snd , snd_vol, pan, pitch, 0 );

				rest(30);

				load_win1 ();

				retrace_count = 0 ;

                for ( flag = 1 ; flag < (p1.outwin.nbf*p1.outwin.spd) ; ++flag )

                {                              
	               
					if (animated) bkgd_counter++;

					retrace_count-- ;

					if (retrace_count>0) ;

					else
					{
						blit_Bkgd() ;

                        GDead2 ( 7 , (spr2x- (p2.ko.w - p2.width)) , spr2y ) ;

                        GKick ( (flag/p1.outwin.spd) , spr1x , spr1y , 3) ;

						if (foreground)
						masked_blit ( Frgd, zoomscreen , 
						0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

						cy = (200+zoomsup) - ZMRATIO ;

						if (cy<1) cy =1 ;     

						if ( (cy<zoomsup) && (cy>0) )

					    stretch_blit ( zoomscreen , virtscreen ,

					    cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					    else

                        blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
                
						blit_KOF91();

						while (!retrace_count) ;
					}

				}

				angle = (Carton_WN->w/Carton_WN->h) ;

				for ( i = Carton_WN->w ; i > (Carton_WN->w/3) ; i-- )
				{

#if PERFECT == 1
		    if (life1 >= 500) {
			draw_sprite(virtscreen, B_prfct,
				    160 - B_prfct->w / 2,
				    100 - B_prfct->h);
			if (i == Carton_WN->w)
			    play_sample(prfct_snd, snd_vol, pan, pitch, 0);
		    }
#endif

					blit_KOF91();

					if ( (cy<zoomsup) && (cy>0) )

					stretch_blit ( zoomscreen , virtscreen ,

					cx , cy, zoomwidth, ZMRATIO , 0, 0, 320, 200 ) ;

					else

					blit ( zoomscreen , virtscreen , cx , zoomsup, 0, 0, 320, 200 ) ;
				
					stretch_sprite (  virtscreen ,Carton_WN , 30, 30 
						 
									, i , i/angle);

				}
              
	}


    blit_KOF91();

	rest(1500) ;

	poll_joystick() ;
	clear_keybuf();

	if (vic1 > 1)
	{

        blit_KOF91();

		rest(500) ;
           
        rectfill ( virtscreen , 0 , 0 , 320 , 200 , 0 ) ;  

		draw_sprite (  virtscreen ,Carton_WN , (320-Carton_WN->w)/2, 20 );

		if ( Face1 != NULL )
            
	    draw_sprite( virtscreen , Face1,  (320-Face1->w)/2, 30+Carton_WN->h ) ;

		blit_KOF91();

		poll_joystick() ;
		clear_keybuf();

		while ( !(key[p2_spunch]) && !(key[p2_skick]) 
		  && !(key[p2_wpunch]) && !(key[p2_wkick])
          && !( key[KEY_ENTER]) 
		  && !P1_WK && !P1_SK && !P1_WP && !P1_SP )
		{
			poll_joystick();
		}
             
		//destroy_midi (music) ;

        if (animated) close_fli() ;

        destroy_bitmap (zoomscreen) ;

		if (gmode==2) { defeated[last_sel2] = 1 ; story = 1 ; }

		goto StartMenu;
	}

	else

	if (vic2 > 1)
	{

        blit_KOF91();

		rest(500) ;

		if (gmode) Carton_WN = load_bitmap ( "sprites\\lose.pcx" , 0 ) ;

        rectfill ( virtscreen , 0 , 0 , 320 , 200 , 0 ) ;  

		draw_sprite (  virtscreen ,Carton_WN , (320-Carton_WN->w)/2, 20 );

		if ( Face2 != NULL )
            
	    draw_sprite( virtscreen , Face2,  (320-Face1->w)/2, 30+Carton_WN->h ) ;

		blit_KOF91();

		poll_joystick() ;
		clear_keybuf();

		while ( !(key[p2_spunch]) && !(key[p2_skick]) 
		  && !(key[p2_wpunch]) && !(key[p2_wkick])
          && !( key[KEY_ENTER]) 
		  && !P1_WK && !P1_SK && !P1_WP && !P1_SP )
		{
			poll_joystick();
		}


		//destroy_midi (music) ;

        close_fli() ;

        destroy_bitmap (zoomscreen) ;

		if (gmode==2) story = 1 ;

		goto StartMenu;

	}
	round++;

	life1 = life2 = 500 ;

	actual = actual2 = 0 ;

	zoomwidth = 320 ;


#if TCLOCK == 1
	    clock_frame = 0;
	    t_clock = t_clock_ini;
	    sprintf(s_cron, "%d", t_clock);
	    c_dez = s_cron[0] - 48;
	    c_uni = s_cron[1] - 48;
#endif

	p1.wimpact_ct = p2.wimpact_ct = 0 ;
    p1.simpact_ct = p2.simpact_ct = 0 ;
    p1.bimpact_ct = p2.bimpact_ct = 0 ;

	flag = flag2 = 1 ;

    fball1 = fball2 = 0 ;

    p1.fireb.succ = p2.fireb.succ = 0 ;
    p1.rush.succ = p2.rush.succ = 0 ;
    rush_stop1 = rush_stop2 = 0 ;
    smove_stop1 = smove_stop2 = 0 ;


	spr1x = (cx + 75 - (p1.width/2)) , spr1y = sol1 ;
	spr2x = (cx + 250 -(p2.width/2)) , spr2y = sol2 ;

	deb = 1 ;
   }

/****** END OF THE 'REAL GAME' LOOP ******/

}

/******** SMALL ENDING STUFF *****************/

   fade_out( 2 );

   destroy_bitmap (zoomscreen) ;

   if (animated) close_fli() ;

   checkint = 0 ;

   goto Demoniac;

   return 0;

}


END_OF_MAIN();

void Intro(void)

{

char x ,y ,bab ; // dumb variables

int mvb = 20;

	bab = 1 ;

   Title = load_pcx ( "title.pcx" , Pal ) ;

   clear (virtscreen) ;
   clear (screen);

   if ( open_fli ("open.flc") != FLI_OK )
   {
	     allegro_message("open.flc is corrupted or missing\n");
		 allegro_exit();
		 exit(0);
   }

   if(!MP3_Init())
   {
	     allegro_message("Error initialising MP3 player\n");
   }
    
   y = MP3_Open ("title.mp3") ;

   rest (400) ;

   if (y) MP3_Play() ;

   retrace_count = 0 ;

   do

   {
	   if(mvb++>1) mvb = 0 ;

	   if (y) MP3_Play() ;

	   if(!mvb)
	   {

		retrace_count -= 2;

        reset_fli_variables() ;

        if ( next_fli_frame ( 0 ) != FLI_OK )

        bab = 0 ;

		if (keypressed()) bab = 0 ;

        set_palette ( fli_palette ) ;

		blit ( fli_bitmap , virtscreen , 0 , 0 , 0 , 0 , 320 , 200 ) ;

		blit_KOF91();

		if (!mp3) rest(15);

		while (!retrace_count);;

	   }
                

   }

   while ( bab ) ;


   close_fli () ;

//   rest (900) ;


   clear_keybuf () ;

   clear (virtscreen) ;

//   set_palette ( black_palette ) ;

   mvb = 20 ;
   retrace_count = 0 ;

   if ( open_fli ("intro.flc") == FLI_OK )

   do

   {
	   if(mvb++>0) mvb = 0 ;

	   if(mvb)
	   {
			retrace_count -= 2;

			if ( next_fli_frame ( 1 ) != FLI_OK )
			   simulate_keypress (KEY_SPACE) ;

			set_palette ( fli_palette ) ;

			blit ( fli_bitmap , virtscreen , 0 , 0 , 0 , 0 , 320 , 200 ) ;

			blit_KOF91();

			reset_fli_variables() ;

			if (!mp3) rest(15);

			while (!retrace_count);
	   }

	   else
	   {
		   if (y) MP3_Play() ;		
	   }

   }

   while ( !keypressed() ) ;



   close_fli () ;

   set_palette ( black_palette ) ;

   draw_sprite ( virtscreen , Title , 0 , 0 ) ;  

   blit_KOF91();   
    
   clear_keybuf() ;

   x = 1 ;

   while ( x )
   {
	   if (y) MP3_Play() ;

	   if (keypressed()) x = 0 ;
   }

   MP3_Close();

   MP3_Exit() ;
   
   //Done() ;

}

void blit_KOF91 (void)

{

int y ;

  if ((gfmode==1)||(gfmode>2))
  {
	  stretch_blit ( virtscreen , scanliner , 0, 0, 320, 200, 
											  0, 0,screen_width, screen_height) ;

	  if ((gfmode==1)||(gfmode==4))
      for ( y = 0 ; y < 400 ; y += 2 ) hline( scanliner , 0 , y ,screen_width , 0 ) ;

	  acquire_screen();
      blit ( scanliner , screen , 0, 0, 0, 0, screen_width, screen_height ) ;
	  release_screen();

  }
  
  else
  {
	  acquire_screen();

	  blit ( virtscreen , screen , 0, 0, start_x, start_y, 320, 200 ) ;

	  release_screen();

  }

}


void blit_Bkgd (void)
{
	if (animated)
    {
			if (bkgd_counter>bkgd_delay)
            {
					reset_fli_variables() ;

                    next_fli_frame(1) ;

                    bkgd_counter = 0 ;
            }

            blit ( fli_bitmap, zoomscreen , 0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;
    }

    else

	{
		blit ( Bkgd, zoomscreen , 0 , 0, 0, 0, (bkgdlim+320), (200+zoomsup) ) ;

	}

}


void loadsounds(void)
{
//        hit_alwa = load_sample (DIR_WAV "hit_alwa.wav") ;


        if ( (rd1 = load_sample (DIR_WAV "rd1.wav"))==NULL)
		{
			allegro_message("can't load sounds\n"
						"You probably did not unzip the archive correctly");
			exit(0);
		}
        rd2 = load_sample (DIR_WAV "rd2.wav") ;
        rd3 = load_sample (DIR_WAV "rd3.wav") ;

        fight = load_sample (DIR_WAV "fight.wav") ;

//        hit_good = load_sample (DIR_WAV "hit_good.wav") ;

		wp_alwa = load_sample (DIR_WAV "wp_alwa.wav") ;
		wk_alwa = load_sample (DIR_WAV "wk_alwa.wav") ;
		sp_alwa = load_sample (DIR_WAV "sp_alwa.wav") ;
		sk_alwa = load_sample (DIR_WAV "sk_alwa.wav") ;

		hit1 = load_sample (DIR_WAV "hit1.wav") ;
		hit2 = load_sample (DIR_WAV "hit2.wav") ;

		block_snd = load_sample (DIR_WAV "block.wav") ;

        ko_snd = load_sample (DIR_WAV "ko.wav") ;
        argh_snd = load_sample (DIR_WAV "argh1.wav") ;
        female_snd = load_sample (DIR_WAV "argh2.wav") ;

#if TIMEOVER ==1
        to_snd = load_sample (DIR_WAV "timeup.wav") ;
#endif                
        
#if PERFECT == 1
		prfct_snd = load_sample(DIR_WAV "perfect.wav" );
#endif

        done = load_sample (DIR_WAV "done.wav") ;
        buzz = load_sample (DIR_WAV "buzz.wav") ;
        bing = load_sample (DIR_WAV "bing.wav") ;

}

void freesounds(void)
{

        destroy_sample ( rd1 ) ;
        destroy_sample ( rd2 ) ;
        destroy_sample ( rd3 ) ;

        destroy_sample ( fight ) ;

//        destroy_sample ( hit_alwa ) ;
        destroy_sample ( wp_alwa ) ;
		destroy_sample ( wk_alwa ) ;
		destroy_sample ( sp_alwa ) ;
		destroy_sample ( sk_alwa ) ;

        destroy_sample ( ko_snd ) ;
        destroy_sample ( argh_snd ) ;
        destroy_sample ( female_snd ) ;
        destroy_sample ( done ) ;
        destroy_sample ( bing ) ;
        destroy_sample ( buzz ) ;
/*
#if TIMEOVER ==1
        destroy_sample ( to_snd ) ;
#endif                
        
#if PERFECT == 1
		destroy_sample ( prfct_snd ) ;
#endif
*/
}



void Buzz (void)

{

stop_sample( buzz ) ;

play_sample( buzz , snd_vol, pan, pitch, 0 );

}

void Bing (void)

{

play_sample( bing , snd_vol, pan, pitch, 0 );

}

void Done (void)

{

play_sample( done , snd_vol, pan, pitch, 0 );

}

void Block (void)

{

play_sample( block_snd , snd_vol, pan, pitch, 0 );

}

void Paf (char gana)

{

	switch (gana)
	{
		case 1 : play_sample( wp_alwa , snd_vol, pan, pitch, 0 ); break ;

		case 2 : play_sample( sp_alwa , snd_vol, pan, pitch, 0 ); break ;

		case 3 : play_sample( wk_alwa , snd_vol, pan, pitch, 0 ); break ;

		case 4 : play_sample( sk_alwa , snd_vol, pan, pitch, 0 ); break ;

	}

}

void Hurt (char gana)

{

	switch (gana)
	{
		case 1 : play_sample( hit2 , snd_vol, pan, pitch, 0 ); break ;

		case 2 : play_sample( hit1 , snd_vol, pan, pitch, 0 ); break ;

		default: play_sample( hit1 , snd_vol, pan, pitch, 0 ); break ;

	}

}


void Round (char which)

{

        switch ( which )
        {
          case 1 : play_sample( rd1 , snd_vol, pan, pitch, 0 ); break ;
          case 2 : play_sample( rd2 , snd_vol, pan, pitch, 0 ); break ;
          case 3 : play_sample( rd3 , snd_vol, pan, pitch, 0 ); break ;

          default : play_sample( rd1 , snd_vol, pan, pitch, 0 ); break ;

        }

}

void Fight (void)

{

play_sample( fight , snd_vol, pan, pitch, 0 );

}

void GStatic ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.statik.nbf ) || ((gueu+1)< 1) ) gueu = p1.statik.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.Static[gueu+1] ,x , y) ;
                
}

void GSingle ( int gueu , int x , int y )

{

draw_rle_sprite( zoomscreen, Player1.Single[gueu] ,x ,y ) ;
                 
}

void GKO ( int gueu , int x , int y )

{

x = x - 20 ;

switch (gueu)

        {

        case 1 : draw_rle_sprite( zoomscreen, Player1.KO[1] ,x ,y ) ;
                 break ;
        case 2 : draw_rle_sprite( zoomscreen, Player1.KO[2] ,x ,y ) ;
                 break ;
        case 3 : draw_rle_sprite( zoomscreen, Player1.KO[3] ,x ,y ) ;
                 break ;
        case 4 : draw_rle_sprite( zoomscreen, Player1.KO[4] ,x ,y ) ;
                 break ;
        case 5 :
        case 6 :
        default: draw_rle_sprite( zoomscreen, Player1.Single[1] ,(x+15) ,y ) ;
                 break ;
        }
}

void GDead ( int gueu , int x , int y )

{

x = x - 10 ;

switch (gueu)

    {
		case 0 : draw_rle_sprite( zoomscreen, Player1.KO[1] ,x ,y ) ;
                 break ;
        case 1 : draw_rle_sprite( zoomscreen, Player1.KO[1] ,x ,y ) ;
                 break ;
        case 2 : draw_rle_sprite( zoomscreen, Player1.KO[2] ,x ,y ) ;
                 break ;
        case 3 : draw_rle_sprite( zoomscreen, Player1.KO[3] ,x ,y ) ;
                 break ;
        case 4 : draw_rle_sprite( zoomscreen, Player1.KO[4] ,x ,y ) ;
                 break ;
        case 5 : draw_rle_sprite( zoomscreen, Player1.KO[3] ,x ,y ) ;
                 break ;
        case 6 : draw_rle_sprite( zoomscreen, Player1.KO[4] ,x ,y ) ;
                 break ;
        case 7 :
        default: draw_rle_sprite( zoomscreen, Player1.KO[5] ,x ,y ) ;
                 break ;
    }
}

void GJump ( int gueu , int x , int y )

{

draw_rle_sprite( zoomscreen, Player1.Jump[gueu] ,x ,y ) ;
                
}

void GHurt ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.hurt.nbf ) || ((gueu+1)< 1) ) gueu = p1.hurt.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.Hurt[gueu+1] ,x , y) ;
                 
}

void GGHurt ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.ghurt.nbf ) || ((gueu+1)< 1) ) gueu = p1.ghurt.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.GHurt[gueu+1] ,x , y) ;
                 
}


void GWalk ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.walk.nbf ) || ((gueu+1)< 1) ) gueu = p1.walk.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.Walk[gueu+1] ,x , y) ;

}

void GKick ( int gueu , int x , int y , int z )

{

switch (z)

        {

        case  1 : if ( ((gueu+1) > p1.skick.nbf ) || ((gueu+1)< 1) )
                  gueu = p1.skick.nbf - 1 ;
                  break ;

        case  2 : if ( ((gueu+1) > p1.intro.nbf ) || ((gueu+1)< 1) )
                  gueu = p1.intro.nbf - 1 ;
                  break ;

        case  3 : if ( ((gueu+1) > p1.outwin.nbf ) || ((gueu+1)< 1) )
                  gueu = p1.outwin.nbf - 1 ;
                  break ;
/*
        case  4 : if ( ((gueu+1) > outlose_nbf1 ) || ((gueu+1)< 1) )
                  gueu = outlose_nbf1 - 1 ;
                  break ;
*/
        }

draw_rle_sprite( zoomscreen, Player1.Kick[gueu+1] ,x , y) ;
                 
}

void GGKick ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.gkick.nbf ) || ((gueu+1)< 1) ) gueu = p1.gkick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.GKick[gueu+1] ,x , y) ;
                
}

void GWKick ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.wkick.nbf ) || ((gueu+1)< 1) ) gueu = p1.wkick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.WKick[gueu+1] ,x , y) ;
                
}

void GPunch ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.spunch.nbf ) || ((gueu+1)< 1) ) gueu = p1.spunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.Punch[gueu+1] ,x , y) ;

}

void GWPunch ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.wpunch.nbf ) || ((gueu+1)< 1) ) gueu = p1.wpunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.WPunch[gueu+1] ,x , y) ;

}

void GAKick ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.akick.nbf ) || ((gueu+1)< 1) ) gueu = p1.akick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.AKick[gueu+1] ,x , y) ;
                 
}

void GAPunch ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.apunch.nbf ) || ((gueu+1)< 1) ) gueu = p1.apunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.APunch[gueu+1] ,x , y) ;
                 
}

void GGPunch ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.gpunch.nbf ) || ((gueu+1)< 1) ) gueu = p1.gpunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.GPunch[gueu+1] ,x , y) ;
                 
}

void GFireB ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.fireb.nbf ) || ((gueu+1)< 1) ) gueu = p1.fireb.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.FireB[gueu+1] ,x , y) ;
                 
}


void GFireB2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.fireb.nbf ) || ((gueu+1)< 1) ) gueu = p2.fireb.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.FireB[gueu+1] ,x , y) ;
                 
}

void GFBall ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.fball.nbf ) || ((gueu+1)< 1) ) gueu = p1.fball.nbf - 1 ;


 B_Temp = create_bitmap( Player1.FBall[gueu+1]->w, Player1.FBall[gueu+1]->h );
 rectfill(B_Temp,0,0,Player1.FBall[gueu+1]->w,Player1.FBall[gueu+1]->h,makecol(255,0,255));
 draw_rle_sprite(B_Temp,Player1.FBall[gueu+1],0,0);
 set_trans_blender(255,255,255,170);
 draw_trans_sprite( zoomscreen, B_Temp ,x , y) ;

// draw_rle_sprite( zoomscreen, Player1.FBall[gueu+1] ,x , y) ;
}

void GFBall2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.fball.nbf ) || ((gueu+1)< 1) ) gueu = p2.fball.nbf - 1 ;

 B_Temp = create_bitmap( Player2.FBall[gueu+1]->w, Player2.FBall[gueu+1]->h );
 rectfill(B_Temp,0,0,Player2.FBall[gueu+1]->w,Player2.FBall[gueu+1]->h,makecol(255,0,255));
 draw_rle_sprite(B_Temp,Player2.FBall[gueu+1],0,0);
 set_trans_blender(255,255,255,170);
 draw_trans_sprite( zoomscreen, B_Temp ,x , y) ;

// draw_rle_sprite( zoomscreen, Player2.FBall[gueu+1] ,x , y) ;

}

void GRush ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.rush.nbf ) || ((gueu+1)< 1) ) gueu = p1.rush.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.Rush[gueu+1] ,x , y) ;
                 
}

void GRush2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.rush.nbf ) || ((gueu+1)< 1) ) gueu = p2.rush.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.Rush[gueu+1] ,x , y) ;
                 
}

void GSMove ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.smove.nbf ) || ((gueu+1)< 1) ) gueu = p1.smove.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.SMove[gueu+1] ,x , y) ;
                 
}

void GSMove2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.smove.nbf ) || ((gueu+1)< 1) ) gueu = p2.smove.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.SMove[gueu+1] ,x , y) ;
                 
}

// SPECIALS

void GFireBX ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.firebx.nbf ) || ((gueu+1)< 1) ) gueu = p1.firebx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.FireBX[gueu+1] ,x , y) ;
                 
}


void GFireBX2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.firebx.nbf ) || ((gueu+1)< 1) ) gueu = p2.firebx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.FireBX[gueu+1] ,x , y) ;
                 
}

void GFBallX ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.fballx.nbf ) || ((gueu+1)< 1) ) gueu = p1.fballx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.FBallX[gueu+1] ,x , y) ;
                 
}

void GFBallX2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.fballx.nbf ) || ((gueu+1)< 1) ) gueu = p2.fballx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.FBallX[gueu+1] ,x , y) ;
                 
}

void GRushX ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.rushx.nbf ) || ((gueu+1)< 1) ) gueu = p1.rushx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.RushX[gueu+1] ,x , y) ;
                 
}

void GRushX2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.rushx.nbf ) || ((gueu+1)< 1) ) gueu = p2.rushx.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.RushX[gueu+1] ,x , y) ;
                 
}

void GSMoveX ( int gueu , int x , int y )

{

if ( ((gueu+1) > p1.smovex.nbf ) || ((gueu+1)< 1) ) gueu = p1.smovex.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player1.SMoveX[gueu+1] ,x , y) ;
                 
}

void GSMoveX2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.smovex.nbf ) || ((gueu+1)< 1) ) gueu = p2.smovex.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.SMoveX[gueu+1] ,x , y) ;
                 
}

// END OF SPECIALS

void GStatic2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.statik.nbf ) || ((gueu+1)< 1) ) gueu = p2.statik.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.Static[gueu+1] ,x , y) ;
                 
}


void GWalk2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.walk.nbf ) || ((gueu+1)< 1) ) gueu = p2.walk.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.Walk[gueu+1] ,x , y) ;
                 
}

void GHurt2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.hurt.nbf ) || ((gueu+1)< 1) ) gueu = p2.hurt.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.Hurt[gueu+1] ,x , y) ;
                 
}

void GGHurt2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.ghurt.nbf ) || ((gueu+1)< 1) ) gueu = p2.ghurt.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.GHurt[gueu+1] ,x , y) ;
                 
}

void GKO2 ( int gueu , int x , int y , int z )

{

x = x + 30 ;

switch (gueu)

        {
        case 1 : draw_rle_sprite( zoomscreen, Player2.KO[1] ,x ,y ) ;
                 break ;
        case 2 : draw_rle_sprite( zoomscreen, Player2.KO[2] ,x ,y ) ;
                 break ;
        case 3 : draw_rle_sprite( zoomscreen, Player2.KO[3] ,x ,y ) ;
                 break ;
        case 4 : draw_rle_sprite( zoomscreen, Player2.KO[4] ,x ,y ) ;
                 break ;
        case 5 :
        case 6 :
        default: draw_rle_sprite( zoomscreen, Player2.Single[1] ,(x+z-25) ,y ) ;
                 break ;

        }
}

void GDead2 ( int gueu , int x , int y )

{

x = x + 10 ;

switch (gueu)

        {
		case 0 : draw_rle_sprite( zoomscreen, Player2.KO[1] ,x ,y ) ;
                 break ;
        case 1 : draw_rle_sprite( zoomscreen, Player2.KO[1] ,x ,y ) ;
                 break ;
        case 2 : draw_rle_sprite( zoomscreen, Player2.KO[2] ,x ,y ) ;
                 break ;
        case 3 : draw_rle_sprite( zoomscreen, Player2.KO[3] ,x ,y ) ;
                 break ;
        case 4 : draw_rle_sprite( zoomscreen, Player2.KO[4] ,x ,y ) ;
                 break ;
        case 5 : draw_rle_sprite( zoomscreen, Player2.KO[3] ,x ,y ) ;
                 break ;
        case 6 : draw_rle_sprite( zoomscreen, Player2.KO[4] ,x ,y ) ;
                 break ;
        case 7 :
        default: draw_rle_sprite( zoomscreen, Player2.KO[5] ,x ,y ) ;
                 break ;
        }
}

void GKick2 ( int gueu , int x , int y , int z )

{

switch (z)

        {

        case  1 : if ( ((gueu+1) > p2.skick.nbf ) || ((gueu+1)< 1) )
                  gueu = p2.skick.nbf - 1 ;
                  break ;

        case  2 : if ( ((gueu+1) > p2.intro.nbf ) || ((gueu+1)< 1) )
                  gueu = p2.intro.nbf - 1 ;
                  break ;

        case  3 : if ( ((gueu+1) > p2.outwin.nbf ) || ((gueu+1)< 1) )
                  gueu = p2.outwin.nbf - 1 ;
                  break ;
/*
        case  4 : if ( ((gueu+1) > outlose_nbf2 ) || ((gueu+1)< 1) )
                  gueu = outlose_nbf2 - 1 ;
                  break ;
*/
        }

draw_rle_sprite( zoomscreen, Player2.Kick[gueu+1] ,x , y) ;
                 
}

void GGKick2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.gkick.nbf ) || ((gueu+1)< 1) ) gueu = p2.gkick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.GKick[gueu+1] ,x , y) ;
                 
}

void GWKick2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.wkick.nbf ) || ((gueu+1)< 1) ) gueu = p2.wkick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.WKick[gueu+1] ,x , y) ;
                 
}

void GPunch2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.spunch.nbf ) || ((gueu+1)< 1) ) gueu = p2.spunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.Punch[gueu+1] ,x , y) ;
                 
}

void GWPunch2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.wpunch.nbf ) || ((gueu+1)< 1) ) gueu = p2.wpunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.WPunch[gueu+1] ,x , y) ;
                 
}

void GJump2 ( int gueu , int x , int y )

{

draw_rle_sprite( zoomscreen, Player2.Jump[gueu] ,x ,y ) ;
                 
}

void GSingle2 ( int gueu , int x , int y )

{

draw_rle_sprite( zoomscreen, Player2.Single[gueu] ,x ,y ) ;
                 
}


void GAKick2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.akick.nbf ) || ((gueu+1)< 1) ) gueu = p2.akick.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.AKick[gueu+1] ,x , y) ;
                 
}

void GAPunch2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.apunch.nbf ) || ((gueu+1)< 1) ) gueu = p2.apunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.APunch[gueu+1] ,x , y) ;
                 
}

void GGPunch2 ( int gueu , int x , int y )

{

if ( ((gueu+1) > p2.gpunch.nbf ) || ((gueu+1)< 1) ) gueu = p2.gpunch.nbf - 1 ;

draw_rle_sprite( zoomscreen, Player2.GPunch[gueu+1] ,x , y) ;
                 
}


void load_sprites1 (void)

{

int i ;
char x = 1 ;

   strcpy ( passeur, DIR_SPRITES );
   strcat ( passeur, charname[sel1] );
   strcat ( passeur, DIR_BAR "" );

   for ( num=1 ; num<(p1.statik.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "static" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flStatic[num] = load_pcx ( file , 0 ) ;  
    Player1.Static[num] = get_rle_sprite( Player1.flStatic[num] ) ;
    destroy_bitmap(Player1.flStatic[num]);

   }


   for ( num=1 ; num<(p1.walk.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "walk" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flWalk[num] = load_pcx ( file , 0 ) ;  
    Player1.Walk[num] = get_rle_sprite( Player1.flWalk[num] ) ;
	destroy_bitmap (Player1.flWalk[num]);

   }


   for ( num=1 ; num<(p1.spunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "spunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flPunch[num] = load_pcx ( file , 0 ) ;  
    Player1.Punch[num] = get_rle_sprite(Player1.flPunch[num] ) ;
	destroy_bitmap(Player1.flPunch[num]);

   }


   for ( num=1 ; num<(p1.wpunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "wpunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flWPunch[num] = load_pcx ( file , 0 ) ;  
    Player1.WPunch[num] = get_rle_sprite(Player1.flWPunch[num] ) ;
	destroy_bitmap(Player1.flWPunch[num]);

   }

   for ( num=1 ; num<(p1.gpunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "gpunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flGPunch[num] = load_pcx ( file , 0 ) ;  
    Player1.GPunch[num] = get_rle_sprite(Player1.flGPunch[num] ) ;
	destroy_bitmap(Player1.flGPunch[num]);

   }

   for ( num=1 ; num<(p1.apunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "apunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flAPunch[num] = load_pcx ( file , 0 ) ;  
    Player1.APunch[num] = get_rle_sprite(Player1.flAPunch[num] ) ;
	destroy_bitmap(Player1.flAPunch[num]);

   }

   for ( num=1 ; num<(p1.intro.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "intro" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flKick[num] = load_pcx ( file , 0 ) ;  
    Player1.Kick[num] = get_rle_sprite(Player1.flKick[num] ) ;
	destroy_bitmap(Player1.flKick[num]);

   }

   

   for ( num=1 ; num<(p1.gkick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "gkick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flGKick[num] = load_pcx ( file , 0 ) ;  
    Player1.GKick[num]=get_rle_sprite(Player1.flGKick[num] ) ;
	destroy_bitmap(Player1.flGKick[num]);

   }

   for ( num=1 ; num<(p1.wkick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "wkick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flWKick[num] = load_pcx ( file , 0 ) ;  
    Player1.WKick[num]=get_rle_sprite(Player1.flWKick[num] ) ;
	destroy_bitmap(Player1.flWKick[num]);

   }


   for ( num=1 ; num<(p1.hurt.nbf+1) ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "hurt" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flHurt[num] = load_pcx ( file , 0 ) ;  
    Player1.Hurt[num]=get_rle_sprite(Player1.flHurt[num] ) ;
	destroy_bitmap(Player1.flHurt[num]);

   }


   for ( num=1 ; num<(p1.ghurt.nbf+1) ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "ghurt" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flGHurt[num] = load_pcx ( file , 0 ) ;
    Player1.GHurt[num]=get_rle_sprite(Player1.flGHurt[num] ) ;
	destroy_bitmap(Player1.flGHurt[num]);

   }

   for ( num=1 ; num<6 ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "jump" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flJump[num] = load_pcx ( file , 0 ) ;  
    Player1.Jump[num]=get_rle_sprite(Player1.flJump[num] ) ;
	destroy_bitmap(Player1.flJump[num]);

   }

   for ( num=1 ; num<6 ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "ko" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player1.flKO[num] = load_pcx ( file , 0 ) ;  
    Player1.KO[num]=get_rle_sprite(Player1.flKO[num] ) ;
	destroy_bitmap(Player1.flKO[num]);

   }


   for ( num=1 ; num<5 ; ++num )

   {
	strcpy ( file, passeur );

        switch ( (int)num )

        {
         case  1  : strcat ( file, "block1.pcx" );
                    Player1.flSingle[0] = load_pcx ( file , 0 ) ;  
                    Player1.Single[0]=get_rle_sprite(Player1.flSingle[0]) ;
					destroy_bitmap(Player1.flSingle[0]);
					break ;


         case  2  : strcat ( file, "crouch1.pcx" );
                    Player1.flSingle[1] = load_pcx ( file , 0 ) ;  
                    Player1.Single[1]=get_rle_sprite(Player1.flSingle[1]) ;
					destroy_bitmap(Player1.flSingle[1]);
					break ;


         case  3  : strcat ( file, "gblock1.pcx" );
                    Player1.flSingle[2] = load_pcx ( file , 0 ) ;  
                    Player1.Single[2]=get_rle_sprite(Player1.flSingle[2]) ;
					destroy_bitmap(Player1.flSingle[2]);
					break ;


         case  4  : strcat ( file, "hurt1.pcx" );
                    Player1.flSingle[3] = load_pcx ( file , 0 ) ;  
                    Player1.Single[3]=get_rle_sprite(Player1.flSingle[3]) ;
					destroy_bitmap(Player1.flSingle[3]);
					break ;

        }
   }

   for ( num=1 ; num<(p1.akick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "akick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flAKick[num] = load_pcx ( file , 0 ) ;  
    Player1.AKick[num]=get_rle_sprite(Player1.flAKick[num]) ;
	destroy_bitmap(Player1.flAKick[num]);

   }


   if ( p1.fireb.flg )
   {
	   for ( num=1 ; num<(p1.fireb.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fireb" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flFireB[num] = load_pcx ( file , 0 ) ;  
		Player1.FireB[num] = get_rle_sprite(Player1.flFireB[num] ) ;
		destroy_bitmap(Player1.flFireB[num]);

	   }

	   for ( num=1 ; num<(p1.fball.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fball" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flFBall[num] = load_pcx ( file , 0 ) ;  
		Player1.FBall[num] = get_rle_sprite(Player1.flFBall[num] ) ;
		destroy_bitmap(Player1.flFBall[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "fireb.wav" );
	   hadoken1 = load_sample (file) ;

   }


   if ( p1.rush.flg )
   {
	   for ( num=1 ; num<(p1.rush.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "rush" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flRush[num] = load_pcx ( file , 0 ) ;  
		Player1.Rush[num] = get_rle_sprite(Player1.flRush[num] ) ;
		destroy_bitmap(Player1.flRush[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "rush.wav" );
	   copter1 = load_sample (file) ;

   }

   if ( p1.smove.flg )
   {
	   for ( num=1 ; num<(p1.smove.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "smove" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flSMove[num] = load_pcx ( file , 0 ) ;  
		Player1.SMove[num] = get_rle_sprite(Player1.flSMove[num] ) ;
		destroy_bitmap(Player1.flSMove[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "smove.wav" );
	   dragon1 = load_sample (file) ;

   }

   // SPECIALS

   if ( p1.firebx.flg )
   {
	   for ( num=1 ; num<(p1.firebx.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "firebx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flFireBX[num] = load_pcx ( file , 0 ) ;  
		Player1.FireBX[num] = get_rle_sprite(Player1.flFireBX[num] ) ;
		destroy_bitmap(Player1.flFireBX[num]);

	   }

	   for ( num=1 ; num<(p1.fballx.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fballx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flFBallX[num] = load_pcx ( file , 0 ) ;  
		Player1.FBallX[num] = get_rle_sprite(Player1.flFBallX[num] ) ;
		destroy_bitmap(Player1.flFBallX[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "firebx.wav" );
	   hadokenx1 = load_sample (file) ;

   }


   if ( p1.rushx.flg )
   {
	   for ( num=1 ; num<(p1.rushx.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "rushx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flRushX[num] = load_pcx ( file , 0 ) ;  
		Player1.RushX[num] = get_rle_sprite(Player1.flRushX[num] ) ;
		destroy_bitmap(Player1.flRushX[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "rushx.wav" );
	   copterx1 = load_sample (file) ;

   }

   if ( p1.smovex.flg )
   {
	   for ( num=1 ; num<(p1.smovex.nbf+1) ; ++num )

	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "smovex" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player1.flSMoveX[num] = load_pcx ( file , 0 ) ;  
		Player1.SMoveX[num] = get_rle_sprite(Player1.flSMoveX[num] ) ;
		destroy_bitmap(Player1.flSMoveX[num]);

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "smovex.wav" );
	   dragonx1 = load_sample (file) ;

   }





}


void load_sprites2 (void)

{

int i ;
char x = 1 ;


   strcpy ( passeur, DIR_SPRITES );
   strcat ( passeur, charname[sel2] );
   strcat ( passeur,  DIR_BAR  );

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("check char2 path :\n%s", passeur );
#endif

   for ( num=1 ; num<(p2.statik.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
	else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "static" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flStatic[num] = load_pcx ( file , Pal ) ;
    Player1.flStatic[num] = create_bitmap ( p2.width , p2.height ) ;
    fill_sprite(Player1.flStatic[num] );

    draw_sprite_h_flip (Player1.flStatic[num], Player2.flStatic[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flStatic[num]);
      draw_lit_sprite(Player2.flStatic[num],Player1.flStatic[num]  ,0 , 0, 100);
      Player1.flStatic[num] = Player2.flStatic[num];
    }
#if GAME_DEBUG > 0
    allegro_message("\nload 1..\n");
#endif
    Player2.Static[num] =get_rle_sprite (Player1.flStatic[num] ) ;
    destroy_bitmap (Player2.flStatic[num]) ;

   }

#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<5 ; ++num )

   {
	strcpy ( file, passeur );

        switch ( (int)num )

        {
         case  1  : strcat ( file, "block1.pcx" );
                    Player2.flSingle[0] = load_pcx ( file , Pal ) ;
                    Player1.flSingle[0] = create_bitmap ( p2.width , p2.height ) ;
                    fill_sprite(Player1.flSingle[0]);
                    draw_sprite_h_flip (Player1.flSingle[0], Player2.flSingle[0], 0, 0);           
                    if (clone){
                      set_trans_blender(0, 255, 0, 0);
                      fill_sprite(Player2.flSingle[0]);
                      draw_lit_sprite(Player2.flSingle[0],Player1.flSingle[0]  ,0 , 0, 100);
                      Player1.flSingle[0] = Player2.flSingle[0];
                    }
                    #if GAME_DEBUG > 0
                    allegro_message("\nload 2..\n");
                    #endif
		    Player2.Single[0] = get_rle_sprite (Player1.flSingle[0]) ;
		    destroy_bitmap(Player1.flSingle[0]);
		    break ;

         case  2  : strcat ( file, "crouch1.pcx" );
                    Player2.flSingle[1] = load_pcx ( file , Pal ) ;
	    			Player1.flSingle[1] = create_bitmap ( p2.width , p2.height ) ;
                                fill_sprite(Player1.flSingle[1]);

                                draw_sprite_h_flip (Player1.flSingle[1], Player2.flSingle[1], 0, 0) ;
                                if (clone){
                                  set_trans_blender(0, 255, 0, 0);
                                  fill_sprite(Player2.flSingle[1]);
                                  draw_lit_sprite(Player2.flSingle[1],Player1.flSingle[1]  ,0 , 0, 100);
                                  Player1.flSingle[1] = Player2.flSingle[1];
                                }
#if GAME_DEBUG > 0

                                allegro_message("\nload 3..\n");
#endif
				Player2.Single[1] = get_rle_sprite (Player1.flSingle[1]) ;
				destroy_bitmap(Player1.flSingle[1]);
				break ;


         case  3  : strcat ( file, "gblock1.pcx" );
                    Player2.flSingle[2] = load_pcx ( file , Pal ) ;
					Player1.flSingle[2] = create_bitmap ( p2.width , p2.height ) ;
                                        fill_sprite(Player1.flSingle[2]);
                                        draw_sprite_h_flip (Player1.flSingle[2], Player2.flSingle[2], 0, 0) ;

                                        if (clone){
                                          set_trans_blender(0, 255, 0, 0);
                                          fill_sprite(Player2.flSingle[2]);
                                          draw_lit_sprite(Player2.flSingle[2],Player1.flSingle[2]  ,0 , 0, 100);
                                          Player1.flSingle[2] = Player2.flSingle[2];
                                        }
#if GAME_DEBUG > 0
                                        
                                            allegro_message("\nload 4..\n");
#endif

					Player2.Single[2] = get_rle_sprite (Player1.flSingle[2]) ;
					destroy_bitmap(Player1.flSingle[2]);
					break ;


         case  4  : strcat ( file, "hurt1.pcx" );
                    Player2.flSingle[3] = load_pcx ( file , Pal ) ;
					Player1.flSingle[3] = create_bitmap ( p2.width , p2.height ) ;
                                        fill_sprite(Player1.flSingle[3]);

                                        draw_sprite_h_flip (Player1.flSingle[3], Player2.flSingle[3], 0, 0) ;
                                        if (clone){
                                          set_trans_blender(0, 255, 0, 0);
                                          fill_sprite(Player2.flSingle[3]);
                                          draw_lit_sprite(Player2.flSingle[3],Player1.flSingle[3]  ,0 , 0, 100);
                                          Player1.flSingle[3] = Player2.flSingle[3];
                                        }
#if GAME_DEBUG > 0

                                        allegro_message("\nload 5..\n");
#endif
					Player2.Single[3] = get_rle_sprite (Player1.flSingle[3]) ;
					destroy_bitmap(Player1.flSingle[3]);
					break ;

        }
   }

#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<(p2.walk.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "walk" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player2.flWalk[num] = load_pcx ( file , Pal ) ;
    Player1.flWalk[num] = create_bitmap ( p2.width , p2.height ) ;
    fill_sprite(Player1.flWalk[num]);     
    
    draw_sprite_h_flip (Player1.flWalk[num], Player2.flWalk[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flWalk[num]);
      draw_lit_sprite(Player2.flWalk[num],Player1.flWalk[num]  ,0 , 0, 100);
      Player1.flWalk[num] = Player2.flWalk[num];
    } 
#if GAME_DEBUG > 0

    allegro_message("\nload 6..\n");
#endif
    
    Player2.Walk[num] =get_rle_sprite (Player1.flWalk[num] ) ;
    destroy_bitmap (Player2.flWalk[num]) ;

   }

#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<(p2.spunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "spunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flPunch[num] = load_pcx ( file , Pal ) ;
    Player1.flPunch[num] = create_bitmap ( p2.spunch.w , p2.height ) ;
    fill_sprite(Player1.flPunch[num]);

    draw_sprite_h_flip (Player1.flPunch[num], Player2.flPunch[num], 0, 0) ;        
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flPunch[num]);
      draw_lit_sprite(Player2.flPunch[num],Player1.flPunch[num]  ,0 , 0, 100);
      Player1.flPunch[num] = Player2.flPunch[num];
    } 
#if GAME_DEBUG > 0

    allegro_message("\nload 7..\n");
#endif
    
    Player2.Punch[num] =get_rle_sprite (Player1.flPunch[num] ) ;
    destroy_bitmap (Player2.flPunch[num]) ;

   }

#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<(p2.wpunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "wpunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flWPunch[num] = load_pcx ( file , Pal ) ;
    Player1.flWPunch[num] = create_bitmap ( p2.wpunch.w , p2.height ) ;
    fill_sprite(Player1.flWPunch[num]);

    draw_sprite_h_flip (Player1.flWPunch[num], Player2.flWPunch[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flWPunch[num]);
      draw_lit_sprite(Player2.flWPunch[num],Player1.flWPunch[num]  ,0 , 0, 100);
      Player1.flWPunch[num] = Player2.flWPunch[num];
    }   
#if GAME_DEBUG > 0

    allegro_message("\nload 8..\n");
#endif

    Player2.WPunch[num] =get_rle_sprite (Player1.flWPunch[num] ) ;
    destroy_bitmap (Player2.flWPunch[num]) ;

   }
   
#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<(p2.apunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "apunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flAPunch[num] = load_pcx ( file , Pal ) ;
    Player1.flAPunch[num] = create_bitmap ( p2.apunch.w , p2.height ) ;
    fill_sprite(Player1.flAPunch[num]);

    draw_sprite_h_flip (Player1.flAPunch[num], Player2.flAPunch[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flAPunch[num]);
      draw_lit_sprite(Player2.flAPunch[num],Player1.flAPunch[num]  ,0 , 0, 100);
      Player1.flAPunch[num] = Player2.flAPunch[num];
    }    
#if GAME_DEBUG > 0

    allegro_message("\nload 9..\n");
#endif

    Player2.APunch[num] =get_rle_sprite (Player1.flAPunch[num] ) ;
    destroy_bitmap (Player2.flAPunch[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.gpunch.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "gpunch" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flGPunch[num] = load_pcx ( file , Pal ) ;
    Player1.flGPunch[num] = create_bitmap ( p2.gpunch.w , p2.height ) ;
    fill_sprite(Player1.flGPunch[num]);

    draw_sprite_h_flip (Player1.flGPunch[num], Player2.flGPunch[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flGPunch[num]);
      draw_lit_sprite(Player2.flGPunch[num],Player1.flGPunch[num]  ,0 , 0, 100);
      Player1.flGPunch[num] = Player2.flGPunch[num];
    }    
#if GAME_DEBUG > 0

    allegro_message("\nload 10..\n");
#endif

    Player2.GPunch[num] =get_rle_sprite (Player1.flGPunch[num] ) ;
    destroy_bitmap (Player2.flGPunch[num]) ;

   }
#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.intro.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "intro" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.flKick[num] = create_bitmap ( p2.intro.w , p2.height ) ;
    fill_sprite(Player1.flKick[num]);

    draw_sprite_h_flip (Player1.flKick[num], Player2.flKick[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flKick[num]);
      draw_lit_sprite(Player2.flKick[num],Player1.flKick[num] ,0 , 0, 100);
      Player1.flKick[num] = Player2.flKick[num];
    }    
#if GAME_DEBUG > 0

    allegro_message("\nload 11..\n");
#endif

    Player2.Kick[num] =get_rle_sprite (Player1.flKick[num] ) ;
    destroy_bitmap (Player2.flKick[num]) ;

   }

#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.gkick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "gkick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flGKick[num] = load_pcx ( file , Pal ) ;
    Player1.flGKick[num] = create_bitmap ( p2.gkick.w , p2.height ) ;
    fill_sprite(Player1.flGKick[num]);

    draw_sprite_h_flip (Player1.flGKick[num], Player2.flGKick[num], 0, 0) ;
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flGKick[num]);
      draw_lit_sprite(Player2.flGKick[num],Player1.flGKick[num] ,0 , 0, 100);
      Player1.flGKick[num] = Player2.flGKick[num];
    }    
#if GAME_DEBUG > 0

    allegro_message("\nload 12(%d)..\n",num);
#endif
    

    Player2.GKick[num] =get_rle_sprite (Player1.flGKick[num] ) ;
    destroy_bitmap (Player2.flGKick[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.wkick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "wkick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flWKick[num] = load_pcx ( file , Pal ) ;
    Player1.flWKick[num] = create_bitmap ( p2.wkick.w , p2.height ) ;
    fill_sprite(Player1.flWKick[num]);
    
    draw_sprite_h_flip (Player1.flWKick[num], Player2.flWKick[num], 0, 0) ;        
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flWKick[num]);
      draw_lit_sprite(Player2.flWKick[num],Player1.flWKick[num] ,0 , 0, 100);
      Player1.flWKick[num] = Player2.flWKick[num];
    }    
#if GAME_DEBUG > 0

    allegro_message("\nload 13(%d)..\n",num);
#endif

    Player2.WKick[num] =get_rle_sprite (Player1.flWKick[num] ) ;
    destroy_bitmap (Player2.flWKick[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.hurt.nbf+1) ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "hurt" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flHurt[num] = load_pcx ( file , Pal ) ;
    Player1.flHurt[num] = create_bitmap ( p2.width , p2.height ) ;
    fill_sprite(Player1.flHurt[num]);
    
    draw_sprite_h_flip (Player1.flHurt[num], Player2.flHurt[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flHurt[num]);
      draw_lit_sprite(Player2.flHurt[num],Player1.flHurt[num] ,0 , 0, 100);
      Player1.flHurt[num] = Player2.flHurt[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 14..\n");
#endif

    Player2.Hurt[num] =get_rle_sprite (Player1.flHurt[num] ) ;
    destroy_bitmap (Player2.flHurt[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.ghurt.nbf+1) ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "ghurt" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flGHurt[num] = load_pcx ( file , Pal ) ;
    Player1.flGHurt[num] = create_bitmap ( p2.width , p2.height ) ;
    fill_sprite(Player1.flGHurt[num]);
    
    draw_sprite_h_flip (Player1.flGHurt[num], Player2.flGHurt[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flGHurt[num]);
      draw_lit_sprite(Player2.flGHurt[num],Player1.flGHurt[num] ,0 , 0, 100);
      Player1.flGHurt[num] = Player2.flGHurt[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 15..\n");
#endif

    Player2.GHurt[num] =get_rle_sprite (Player1.flGHurt[num] ) ;
    destroy_bitmap (Player2.flGHurt[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   for ( num=1 ; num<6 ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "jump" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flJump[num] = load_pcx ( file , Pal ) ;
    Player1.flJump[num] = create_bitmap ( p2.width , p2.height ) ;
    fill_sprite(Player1.flJump[num]);

    draw_sprite_h_flip (Player1.flJump[num], Player2.flJump[num], 0, 0) ;    
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flJump[num]);
      draw_lit_sprite(Player2.flJump[num],Player1.flJump[num] ,0 , 0, 100);
      Player1.flJump[num] = Player2.flJump[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 16..\n");
#endif

    Player2.Jump[num] =get_rle_sprite (Player1.flJump[num] ) ;
    destroy_bitmap (Player2.flJump[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<6 ; ++num )

   {
	gcvt ( num , 1 , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "ko" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flKO[num] = load_pcx ( file , Pal ) ;
    Player1.flKO[num] = create_bitmap ( p2.ko.w , p2.height ) ;
    fill_sprite(Player1.flKO[num]);

    draw_sprite_h_flip (Player1.flKO[num], Player2.flKO[num], 0, 0) ;        
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flKO[num]);
      draw_lit_sprite(Player2.flKO[num],Player1.flKO[num] ,0 , 0, 100);
      Player1.flKO[num] = Player2.flKO[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 17..\n");
#endif

    Player2.KO[num] =get_rle_sprite (Player1.flKO[num] ) ;
    destroy_bitmap (Player2.flKO[num]) ;

   }


#if GAME_DEBUG > 0
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   for ( num=1 ; num<(p2.akick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "akick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player2.flAKick[num] = load_pcx ( file , Pal ) ;
    Player1.flAKick[num] = create_bitmap ( p2.akick.w , p2.height ) ;
    fill_sprite(Player1.flAKick[num]);

    draw_sprite_h_flip (Player1.flAKick[num], Player2.flAKick[num], 0, 0) ;        
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flAKick[num]);
      draw_lit_sprite(Player2.flAKick[num],Player1.flAKick[num] ,0 , 0, 100);
      Player1.flAKick[num] = Player2.flAKick[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 18..\n");
#endif

    Player2.AKick[num] =get_rle_sprite (Player1.flAKick[num] ) ;
    destroy_bitmap (Player2.flAKick[num]) ;

   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   if ( p2.fireb.flg )
   {
	   for ( num=1 ; num<(p2.fireb.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fireb" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flFireB[num] = load_pcx ( file , Pal ) ;
		Player1.flFireB[num] = create_bitmap ( p2.fireb.w , p2.height ) ;
                fill_sprite(Player1.flFireB[num]);

		draw_sprite_h_flip (Player1.flFireB[num], Player2.flFireB[num], 0, 0) ;                
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flFireB[num]);
                  draw_lit_sprite(Player2.flFireB[num],Player1.flFireB[num]   ,0 , 0, 100);
                  Player1.flFireB[num] = Player2.flFireB[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 19..\n");
#endif

		Player2.FireB[num] =get_rle_sprite (Player1.flFireB[num] ) ;
		destroy_bitmap (Player2.flFireB[num]) ;

	   }
#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
	   for ( num=1 ; num<(p2.fball.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fball" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flFBall[num] = load_pcx ( file , Pal ) ;
		Player1.flFBall[num] = create_bitmap ( p2.fball.w , fball_h2 ) ;
                fill_sprite(Player1.flFBall[num]);

		draw_sprite_h_flip (Player1.flFBall[num], Player2.flFBall[num], 0, 0) ;                
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flFBall[num]);
                  draw_lit_sprite(Player2.flFBall[num],Player1.flFBall[num]   ,0 , 0, 100);
                  Player1.flFBall[num] = Player2.flFBall[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 20..\n");
#endif

		Player2.FBall[num] =get_rle_sprite (Player1.flFBall[num] ) ;
		destroy_bitmap (Player2.flFBall[num]) ;

	   }


	   strcpy ( file, passeur );
	   strcat ( file, "fireb.wav" );
	   hadoken2 = load_sample (file) ;

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
}


   if ( p2.rush.flg )
   {
	   for ( num=1 ; num<(p2.rush.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "rush" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flRush[num] = load_pcx ( file , Pal ) ;
		Player1.flRush[num] = create_bitmap ( p2.rush.w , p2.height ) ;
                fill_sprite(Player1.flRush[num]);

		draw_sprite_h_flip (Player1.flRush[num], Player2.flRush[num], 0, 0) ;
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flRush[num]);
                  draw_lit_sprite(Player2.flRush[num],Player1.flRush[num]   ,0 , 0, 100);
                  Player1.flRush[num] = Player2.flRush[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 21..\n");
#endif
                

		Player2.Rush[num] =get_rle_sprite (Player1.flRush[num] ) ;
		destroy_bitmap (Player2.flRush[num]) ;

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "rush.wav" );
	   copter2 = load_sample (file) ;

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   }

   if ( p2.smove.flg )
   {
	   for ( num=1 ; num<(p2.smove.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "smove" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flSMove[num] = load_pcx ( file , Pal ) ;
		Player1.flSMove[num] = create_bitmap ( p2.smove.w , p2.height ) ;
                fill_sprite(Player1.flSMove[num]);

		draw_sprite_h_flip (Player1.flSMove[num], Player2.flSMove[num], 0, 0) ;
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flSMove[num]);
                  draw_lit_sprite(Player2.flSMove[num],Player1.flSMove[num]   ,0 , 0, 100);
                  Player1.flSMove[num] = Player2.flSMove[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 22..\n");
#endif
                

		Player2.SMove[num] =get_rle_sprite (Player1.flSMove[num] ) ;
		destroy_bitmap (Player2.flSMove[num]) ;

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "smove.wav" );
	   dragon2 = load_sample (file) ;

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   }

   // SUPER SPECIALS

   if ( p2.firebx.flg )
   {
	   for ( num=1 ; num<(p2.firebx.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "firebx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flFireBX[num] = load_pcx ( file , Pal ) ;
		Player1.flFireBX[num] = create_bitmap ( p2.firebx.w , p2.height ) ;
                fill_sprite(Player1.flFireBX[num]);

		draw_sprite_h_flip (Player1.flFireBX[num], Player2.flFireBX[num], 0, 0) ;
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flFireBX[num]);
                  draw_lit_sprite(Player2.flFireBX[num],Player1.flFireBX[num]   ,0 , 0, 100);
                  Player1.flFireBX[num] = Player2.flFireBX[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 23..\n");
#endif
                

		Player2.FireBX[num] =get_rle_sprite (Player1.flFireBX[num] ) ;
		destroy_bitmap (Player2.flFireBX[num]) ;

	   }

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

	   for ( num=1 ; num<(p2.fballx.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "fballx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flFBallX[num] = load_pcx ( file , Pal ) ;
		Player1.flFBallX[num] = create_bitmap ( p2.fballx.w , fballx_h2 ) ;
                fill_sprite(Player1.flFBallX[num]);

		draw_sprite_h_flip (Player1.flFBallX[num], Player2.flFBallX[num], 0, 0) ;                
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flFBallX[num]);
                  draw_lit_sprite(Player2.flFBallX[num],Player1.flFBallX[num]   ,0 , 0, 100);
                  Player1.flFBallX[num] = Player2.flFBallX[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 24..\n");
#endif

		Player2.FBallX[num] =get_rle_sprite (Player1.flFBallX[num] ) ;
		destroy_bitmap (Player2.flFBallX[num]) ;

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "firebx.wav" );
	   hadokenx2 = load_sample (file) ;

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   }


   if ( p2.rushx.flg )
   {
	   for ( num=1 ; num<(p2.rushx.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "rushx" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flRushX[num] = load_pcx ( file , Pal ) ;
		Player1.flRushX[num] = create_bitmap ( p2.rushx.w , p2.height ) ;
                fill_sprite(Player1.flRushX[num]);
                
		draw_sprite_h_flip (Player1.flRushX[num], Player2.flRushX[num], 0, 0) ;                                
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flRushX[num]);
                  draw_lit_sprite(Player2.flRushX[num],Player1.flRushX[num]   ,0 , 0, 100);
                  Player1.flRushX[num] = Player2.flRushX[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 25..\n");
#endif

		Player2.RushX[num] =get_rle_sprite (Player1.flRushX[num] ) ;
		destroy_bitmap (Player2.flRushX[num]) ;

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "rushx.wav" );
	   copterx2 = load_sample (file) ;

#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif

   }

   if ( p2.smovex.flg )
   {
	   for ( num=1 ; num<(p2.smovex.nbf+1) ; ++num )
	   {
		if (num<10) i = 1 ;
		else i = 2 ;

		gcvt ( num , i , tilenum ) ;

		strcpy ( file, passeur );
		strcat ( file, "smovex" );
		strcat ( file, tilenum );
		strcat ( file, ".pcx" );

		Player2.flSMoveX[num] = load_pcx ( file , Pal ) ;
		Player1.flSMoveX[num] = create_bitmap ( p2.smovex.w , p2.height ) ;
                fill_sprite(Player1.flSMoveX[num]);
                
		draw_sprite_h_flip (Player1.flSMoveX[num], Player2.flSMoveX[num], 0, 0) ;                        
                if (clone){
                  set_trans_blender(0, 255, 0, 0);
                  fill_sprite(Player2.flSMoveX[num]);
                  draw_lit_sprite(Player2.flSMoveX[num],Player1.flSMoveX[num]   ,0 , 0, 100);
                  Player1.flSMoveX[num] = Player2.flSMoveX[num];
                }    

#if GAME_DEBUG > 0
                allegro_message("\nload 26..\n");
#endif

		Player2.SMoveX[num] =get_rle_sprite (Player1.flSMoveX[num] ) ;
		destroy_bitmap (Player2.flSMoveX[num]) ;

	   }

	   strcpy ( file, passeur );
	   strcat ( file, "smovex.wav" );
	   dragonx2 = load_sample (file) ;


#if GAME_DEBUG == 2
if (debug)
#endif
#if GAME_DEBUG > 0
allegro_message("%s loaded", file );
#endif
   }



}


void load_kick1 (void)

{

int i ;

strcpy ( passeur, DIR_SPRITES );
strcat ( passeur, charname[sel1] );
strcat ( passeur,  DIR_BAR  );

for ( num=1 ; num<(p1.skick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "skick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.Kick[num] = get_rle_sprite ( Player1.flKick[num] ) ;
	destroy_bitmap (Player1.flKick[num]) ;

   }

}

void load_win1 (void)

{
int i ;

strcpy ( passeur, DIR_SPRITES );
strcat ( passeur, charname[sel1] );
strcat ( passeur,  DIR_BAR  );

for ( num=1 ; num<(p1.outwin.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "outwin" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.Kick[num] = get_rle_sprite ( Player1.flKick[num] ) ;
	destroy_bitmap (Player1.flKick[num]) ;

   }

}
/*
void load_outlose1 (void)

{
int i ;

strcpy ( passeur, "sprites/" );
strcat ( passeur, charname[sel1] );
strcat ( passeur, "/" );

for ( num=1 ; num<(p1.outlose.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "outlose" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player1.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.Kick[num] = get_rle_sprite ( Player1.flKick[num] ) ;
	destroy_bitmap (Player1.flKick[num]) ;

   }

}
*/


void load_kick2 (void)

{

int i ;

strcpy ( passeur, DIR_SPRITES );
strcat ( passeur, charname[sel2] );
strcat ( passeur,  DIR_BAR  );

rectfill(zoomscreen, 0, 0,zoomscreen->w,zoomscreen->h, makecol(255,0,255));

//clear_to_color( virtscreen , makecol(255,0,255) ) ;

for ( num=1 ; num<(p2.skick.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "skick" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

    Player2.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.flKick[num] = create_bitmap ( p2.skick.w , p2.height ) ;
    fill_sprite(Player1.flKick[num]);
    
    draw_sprite_h_flip (Player1.flKick[num], Player2.flKick[num], 0, 0) ;        
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flKick[num]);
      draw_lit_sprite(Player2.flKick[num],Player1.flKick[num]   ,0 , 0, 100);
      Player1.flKick[num] = Player2.flKick[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 27..\n");
#endif

    Player2.Kick[num] =get_rle_sprite (Player1.flKick[num] ) ;
    destroy_bitmap (Player2.flKick[num]) ;

   }

}

void load_win2 (void)

{
int i ;

strcpy ( passeur, DIR_SPRITES );
strcat ( passeur, charname[sel2] );
strcat ( passeur,  DIR_BAR  );

rectfill(virtscreen, 0, 0,virtscreen->w,virtscreen->h, makecol(255,0,255));
   
//clear_to_color( virtscreen , makecol(255,0,255) ) ;

for ( num=1 ; num<(p2.outwin.nbf+1) ; ++num )

   {
	if (num<10) i = 1 ;
    else i = 2 ;

	gcvt ( num , i , tilenum ) ;

	strcpy ( file, passeur );
    strcat ( file, "outwin" );
	strcat ( file, tilenum );
	strcat ( file, ".pcx" );

	Player2.flKick[num] = load_pcx ( file , Pal ) ;
    Player1.flKick[num] = create_bitmap ( p2.outwin.w , p2.height ) ; 
    fill_sprite(Player1.flKick[num]);
    
    draw_sprite_h_flip (Player1.flKick[num], Player2.flKick[num], 0, 0) ;         
    if (clone){
      set_trans_blender(0, 255, 0, 0);
      fill_sprite(Player2.flKick[num]);
      draw_lit_sprite(Player2.flKick[num],Player1.flKick[num]   ,0 , 0, 100);
      Player1.flKick[num] = Player2.flKick[num];
    }    

#if GAME_DEBUG > 0
    allegro_message("\nload 28..\n");
#endif

    Player2.Kick[num] =get_rle_sprite (Player1.flKick[num] ) ;
    destroy_bitmap (Player2.flKick[num]) ; 

   }

}


void destroy_bitmaps (void)

{

unload_datafile ( creditflic ) ;

unload_datafile ( fonts_dat ) ;


destroy_bitmap(Title) ;
//destroy_bitmap(Selecteur) ;

//destroy_bitmap(Face1) ;
//destroy_bitmap(Face2) ;

destroy_bitmap(virtscreen) ;
//destroy_bitmap(scanliner) ;

//destroy_bitmap(Bkgd) ;

//destroy_bitmap(Blood) ; 

   for ( num=1 ; num<7 ; ++num )

		destroy_rle_sprite (Player1.Jump[num]) ;
                    

   for ( num=1 ; num<6 ; ++num )

		destroy_rle_sprite (Player1.KO[num]) ;
                    

   for ( num=1 ; num<(p1.statik.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.Static[num]) ;
                    

   for ( num=1 ; num<(p1.walk.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.Walk[num]) ;
                    

   for ( num=1 ; num<(p1.spunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.Punch[num]) ;


   for ( num=1 ; num<(p1.wpunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.WPunch[num]) ;


   for ( num=1 ; num<(p1.gpunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.GPunch[num]) ;


   for ( num=1 ; num<(p1.apunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.APunch[num]) ;
                    

   for ( num=1 ; num<(p1.skick.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.Kick[num]) ;


   for ( num=1 ; num<(p1.wkick.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.WKick[num]) ;
                    

   for ( num=1 ; num<(p1.gkick.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.GKick[num]) ;
                    

   for ( num=1 ; num<(p1.hurt.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.Hurt[num]) ;
                    
   for ( num=1 ; num<(p1.ghurt.nbf+1) ; ++num )

	    destroy_rle_sprite (Player1.GHurt[num]) ;

   for ( num=1 ; num<(p1.fireb.nbf+1) ; ++num )

	    destroy_rle_sprite (Player1.FireB[num]) ;

   for ( num=1 ; num<(p1.fball.nbf+1) ; ++num )

	    destroy_rle_sprite (Player1.FBall[num]) ;
                   
destroy_rle_sprite(Player1.Single[0]) ;
destroy_rle_sprite(Player1.Single[1]) ;
destroy_rle_sprite(Player1.Single[2]) ;
destroy_rle_sprite(Player1.Single[3]) ;

   for ( num=1 ; num<(p1.akick.nbf+1) ; ++num )

		destroy_rle_sprite (Player1.AKick[num]) ;
                
// PLAYER 2 SPRITES

   for ( num=1 ; num<(p2.statik.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.Static[num]) ;
                    

   for ( num=1 ; num<(p2.walk.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.Walk[num]) ;
                    

   for ( num=1 ; num<(p2.spunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.Punch[num]) ;
                    

   for ( num=1 ; num<(p2.wpunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.WPunch[num]) ;


   for ( num=1 ; num<(p2.gpunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.GPunch[num]) ;


   for ( num=1 ; num<(p2.apunch.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.APunch[num]) ;


   for ( num=1 ; num<6 ; ++num )

		destroy_rle_sprite (Player2.Jump[num]) ;
                    

   for ( num=1 ; num<6 ; ++num )

		destroy_rle_sprite (Player2.KO[num]) ;
                    

   for ( num=1 ; num<(p2.skick.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.Kick[num]) ;


   for ( num=1 ; num<(p2.wkick.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.WKick[num]) ;
                    

   for ( num=1 ; num<(p2.gkick.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.GKick[num]) ;

   for ( num=1 ; num<(p2.fireb.nbf+1) ; ++num )

	    destroy_rle_sprite (Player2.FireB[num]) ;

   for ( num=1 ; num<(p2.fball.nbf+1) ; ++num )

	    destroy_rle_sprite (Player2.FBall[num]) ;
                    

destroy_rle_sprite(Player2.Single[0]) ;
destroy_rle_sprite(Player2.Single[1]) ;
destroy_rle_sprite(Player2.Single[2]) ;
destroy_rle_sprite(Player2.Single[3]) ;

   for ( num=1 ; num<(p2.hurt.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.Hurt[num]) ;
                    

   for ( num=1 ; num<(p2.ghurt.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.GHurt[num]) ;
                    

   for ( num=1 ; num<(p2.akick.nbf+1) ; ++num )

		destroy_rle_sprite (Player2.AKick[num]) ;
                    

}

char get_kstatus ( char strg[] ) 

{

char rtn_value ;
char c ;

	c = strg[0] ;

	switch ( c )
	{
		case 'U' : if ( c == strg[1] )
				   {
					   if (c == 'L') rtn_value = 7 ;
					   else
					   if (c == 'R') rtn_value = 8 ;
				   }		   
				   else rtn_value = 1 ; break ;

		case 'D' : if ( c == strg[1] )
				   {
					   if (c == 'L') rtn_value = 5 ;
					   else
					   if (c == 'R') rtn_value = 6 ;
				   }		   
				   else rtn_value = 2 ; break ;

		case 'L' : rtn_value = 3 ; break ;
		case 'R' : rtn_value = 4 ; break ;

		case 'P' : rtn_value = 9 ; break ;
		case 'K' : rtn_value = 10 ; break ;

		case 'N' : rtn_value = 0 ; break ;

		default   : rtn_value = 10 ; break ;
	}

return rtn_value ;

}


char check_kstatus1 ( char ctrl_key ) 

{

char rtnv ;

poll_joystick() ;

	switch(ctrl_key)
	{
	case 0 : if ( !P1_UP && !P1_DN && !P1_LT && !P1_RT
						 && !P1_WP && !P1_SP && !P1_WK && !P1_SK ) 

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 1 : if ( P1_UP && !P1_DN && !P1_LT && !P1_RT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 2 : if ( P1_DN && !P1_UP && !P1_LT && !P1_RT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 3 : if ( P1_LT && !P1_UP && !P1_DN && !P1_RT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 4 : if ( P1_RT && !P1_UP && !P1_DN && !P1_LT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 5 : if ( P1_DN && !P1_UP && !P1_RT && P1_LT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 6 : if ( P1_DN && !P1_UP && P1_RT && !P1_LT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 7 : if ( !P1_DN && P1_UP && !P1_RT && P1_LT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 8 : if ( !P1_DN && P1_UP && P1_RT && !P1_LT 
				 && !P1_WP && !P1_SP && !P1_WK && !P1_SK)

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 9 : if ( (P1_WP || P1_SP) && (!P1_WK && !P1_SK) )

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 10: if ( (!P1_WP && !P1_SP) && (P1_WK || P1_SK) )

			 rtnv = 1 ; else rtnv = 0 ; break ;

	default: allegro_message ("problem in check_kstatus1() : %c",ctrl_key);exit(0);break;

	}

	if (rtnv) last_status1 = ctrl_key ;

return rtnv ;

}


char check_kstatus2 ( char ctrl_key ) 

{

char rtnv ;

	switch(ctrl_key)
	{
	case 0 : if ( !key[p2_up] && !key[p2_down] && !key[p2_left] && !key[p2_right]
							  && !key[p2_wpunch] && !key[p2_spunch]
							  && !key[p2_wkick] && !key[p2_skick] ) 

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 1 : if ( key[p2_up] && !key[p2_down] && !key[p2_left] 
							 && !key[p2_right]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 2 : if ( key[p2_down] && !key[p2_up] && !key[p2_left] 
							 && !key[p2_right]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 3 : if ( !key[p2_left] && !key[p2_up] && !key[p2_down] 
							 && key[p2_right]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 4 : if ( !key[p2_right] && !key[p2_up] && !key[p2_down] 
							 && key[p2_left]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 5 : if ( key[p2_down] && !key[p2_up] && key[p2_right] 
							 && !key[p2_left]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 6 : if ( key[p2_down] && !key[p2_up] && !key[p2_right] 
							 && key[p2_left]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 7 : if ( !key[p2_down] && key[p2_up] && key[p2_right] 
							 && !key[p2_left]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 8 : if ( !key[p2_down] && key[p2_up] && !key[p2_right] 
							 && key[p2_left]
							 && !key[p2_wpunch] && !key[p2_spunch]
							 && !key[p2_wkick] && !key[p2_skick])

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 9 : if ( (key[p2_wpunch] || key[p2_spunch]) && (!key[p2_wkick] 
							 && !key[p2_skick]) )

			 rtnv = 1 ; else rtnv = 0 ; break ;

	case 10: if ( (!key[p2_wpunch] && !key[p2_spunch]) && (key[p2_wkick] 
							 || key[p2_skick]) )

			 rtnv = 1 ; else rtnv = 0 ; break ;

	default: allegro_message ("problem in check_kstatus2() : %c",ctrl_key);exit(0);break;

	}

	if (rtnv) last_status2 = ctrl_key ;

return rtnv ;

}



int scan_files(char *name, int attrib, char what ) 
{
   char dta_name[512], buf[512];
   void *dta;
   int dta_attrib;
   int c = 0;

   if (ustrchr(name, '#')) {
      *allegro_errno = ENOTDIR;
      return 0;
   }

   if (!_al_file_isok(name)) 
      return 0;

   /*
   dta = al_findfirst(name, attrib, dta_name, &dta_attrib);

   if (!dta)
      return 0;

   do {
      replace_filename(buf, name, dta_name, sizeof(buf));
      //get the filename buffer here !!!!
	  if (what==0)
	  {
		  if (!ustrchr(buf, '.'))
		  {
			ustrncpy( charname[c], buf+8, 30 );
			c++ ;
		  }
	  }

	  else
	  if (what==1)
	  {
		ustrncpy( bkgdname[c], buf+6, 30 );
		c++ ;
	  }

	  else
	  if (what==2)
	  {
		ustrncpy( flcname[c], buf+6, 30 );
		c++ ;
	  }

      if (*allegro_errno != 0)
	 break;
   } while (al_findnext(dta, dta_name, &dta_attrib) == 0);

   al_findclose(dta);
   */

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
//            printf("%s\n", dir->d_name);
            sprintf(dta_name, "%s", dir->d_name);

            replace_filename(buf, name, dta_name, sizeof(buf));
            //get the filename buffer here !!!!
            if (what==0)
            {
                if (!ustrchr(buf, '.'))
                {
                    ustrncpy( charname[c], buf+8, 30 );
                    c++ ;
                }
            }

            else
            if (what==1)
            {
                ustrncpy( bkgdname[c], buf+6, 30 );
                c++ ;
            }

            else
            if (what==2)
            {
                ustrncpy( flcname[c], buf+6, 30 );
                c++ ;
            }

            if (*allegro_errno != 0)
                break;

        }
        closedir(d);
    }


   errno = *allegro_errno = 0;
   return c;
}


//static XA_DecoderInfo *
//cmdline_init(void)
//{
//    XA_DecoderInfo *decoder;
//
//    /* the decoder init has to be done first */
//    int status;
//    status = decoder_new(&decoder);
//    if (status != XA_SUCCESS) {
//        exit(1);
//    }
//
//    /* register the input modules (the registration order is important) */
//    {
//        XA_InputModule module;
//
//        file_input_module_register(&module);
//        decoder_input_module_register(decoder, &module);
//    }
//
//    /* register the output modules (the registration order is important) */
//    {
//        XA_OutputModule module;
//
//        audio_output_module_register(&module);
//        decoder_output_module_register(decoder, &module);
//    }
//
//    /* set the notification function */
//    decoder->notification_client.client          = decoder;
//    decoder->notification_client.notify_progress = NULL;//cmdline_notify_progress;
//    decoder->notification_client.notify_debug    = NULL;//cmdline_notify_debug;
//    decoder->notification_client.notify_error    = NULL;//cmdline_notify_error;
//
//    /* output parameters */
//    Options.output.port                 = 0;
//    Options.output.volume               = -1;
//    Options.output.name                 = NULL;
//
//    return decoder;
//}


int MP3_Init (void)
{
if(mp3)
{
//    /* instanciate a decoder */
//    decoder = cmdline_init();
//
//    if (Options.output.port == 0) {
//        Options.output.port =
//            XA_DECODER_CONTROL_OUTPUT_SPEAKER |
//            XA_DECODER_CONTROL_OUTPUT_LINE;
//    };
//
//    // open output
//    decoder_output_new(decoder,
//                       Options.output.name,
//                       XA_DECODER_OUTPUT_AUTOSELECT);
//
//    mp3status = decoder_output_open(decoder);
//    if (mp3status != XA_SUCCESS) return 0 ;
//	else
//	return 1 ;
    return 1;
}
else return 1 ;
}




void MP3_Exit (void) 
{
if(mp3)
{
//	decoder_delete(decoder);
}
}


int MP3_Open (char *mp3file) 
{
if(mp3)
{
//	decoder_input_new(decoder, mp3file, XA_DECODER_INPUT_AUTOSELECT);
//    mp3status = decoder_input_open(decoder);
//    if (mp3status == XA_SUCCESS)
//	{
//			return 1 ;
//    }
//	else
//	{
//			return 0 ;
//    }
return 1;

}
else	return 0 ;
}

int MP3_Play (void) 
{
if(mp3)
{
//	if ( mp3status == XA_SUCCESS
//	  || mp3status == XA_ERROR_INVALID_FRAME
//	  || mp3status == XA_ERROR_TIMEOUT
//	   )
//	{
//         mp3status = decoder_play(decoder);
//    }
//
//	else
//	{
//		MP3_Close() ;
//	    MP3_Open("title.mp3");
//	}

	return 0 ;
}
	return 0 ;
}

int MP3_Close (void) 
{
if(mp3)
{
//	decoder_output_set_control(decoder, NULL,
//                                       XA_DECODER_CONTROL_DRAIN);
//    decoder_input_close(decoder);
}
	return 0 ;
}
