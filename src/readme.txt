/*****************************************************************
IMPORTANT: You need the complete package od KOF 1.1 for windows to play it
download it at MOAH's site.
You need Allegro and Xaudio installed on your system to compile it
KOF Linux source code
http://sourceforge.net/projects/koflinux
Download it at
http://www2.netexplora.com/koflinux
originally written for Win**** OS by Nicolas Sevez , aka Moah (moah@online.fr)
Ported to linux (and maintained) by:
- Duncan Mac-Vicar Prett (duncan@linuxfan.com)
With the help of:
- Crusader (crusader@linuxgames.com)
Orignial MOAH's KOF91 page:
http://perso.club-internet.fr/moah_/KOF91/
  
  === Required software ===
   
   - Allegro WIP 3.9.31 (http://www.talula.demon.co.uk/allegro/)
    - Xaudio SDK 3.0.7 (http://www.xaudio.com/)
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
*****************************************************************/

