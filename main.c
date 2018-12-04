#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "display.h"
#include "io.h"

// . /opt/mcb32tools/environment

char enabledPlayers[4] = {'t','f','f','f'};
// PORTE är leds
int timeoutcount = 0;
int timeoutcount2 = 0;

int TURN = 2; // Degree of player turning

struct Player{
	int x;
	int y;
	int angle;
};

struct Pair {
    int index;
    int bitIn;
};

struct Direction{
	char x_axis;
	char y_axis;
};

uint8_t frame [] = {
	//PAGE 0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//PAGE 1
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//PAGE 2
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//PAGE 3
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void setup(void);
void gameLoop(void);
void menuLoop(void);
void displayMenu();
void draw (struct Player player);
void playerInput( struct Player players[]);
void movePlayers( struct Player players[]);
void drawPlayers( struct Player players[]);
void spawnPlayers( struct Player players[]);
void getSettings();
void displaySettings();
void collision(struct Player players[]);
void playerCollision(const int playerNr, const int x, const int y);
struct Pair getPlayerIndex(const int x, const int y);
struct Direction getDirection(int a);


int main(void) {
	setup();
	menuLoop();
	gameLoop();

	return 0;
}

void setup(void){
	// D - knappar och switches
	// E - LED-lampor

	//Kanske behövs
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	TRISESET = 0xFFFF;

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;

	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;
	/* Turn on SPI */
	SPI2CONSET = 0x8000;

	// Init TRISD that is used for buttons and swithces.
	TRISDSET = (1 << 8);
	TRISD = TRISD & 0xFE0;

	T2CON = 0x70;       //Set prescaling 1:256.
    T2CONSET = 0x8000;  //Starts clock.
    PR2 = 80000000/256/100; // Divide by 256 to match prescale.
                            // Divide by 100 since we want periods of 1/100s.
	TMR2 = 0;

	init();
}

void spawnPlayers( struct Player players[4] ){
	struct Player p1;
	p1.x = 10;
	p1.y = 8;
	p1.angle = 90;
	players[0] = p1

	if(enabledPlayers[1] == 't'){
		struct Player p2;
		p2.x = 118;
		p2.y = 22;
		p2.angle = 270;
		players[1] = p2;
	}

	if(enabledPlayers[2] == 't'){
		struct Player p3;
		p3.x = 10;
		p3.y = 22;
		p3.angle = 90;
		players[2] = p3;
	}

	if(enabledPlayers[3] == 't'){
		struct Player p4;
		p4.x = 118;
		p4.y = 8;
		p4.angle = 270;
		players[3] = p4;
	}
}

void gameLoop(){
	// General logic for movement, collision, rendering, input and timing.
	struct Player players[4];
	spawnPlayers(players);

	int i;
	for ( i = 0; i < 4; i++ ){
		if(enabledPlayers[i] == 't')
			draw(players[i]);
	}

	renderDisplay(0, frame);
	while(1){
		if((IFS(0)&0x100) == 256){
        	timeoutcount++;
			timeoutcount2++;
        	IFSCLR(0) = 0x100; // Clears bit 9.
		}

		if(timeoutcount == 6){
			movePlayers( players );
			collision( players );
			drawPlayers( players );
			renderDisplay(0, frame);
			timeoutcount = 0;
		}

		if (timeoutcount2 == 1){
			playerInput( players );
			timeoutcount2 = 0;
		}
	}
}

void collision ( struct Player players[] ){
	// Checks detection for all enabled players.
	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){
			playerCollision(i, players[i].x, players[i].y);
		}
	}
}

void playerCollision( const int playerNr, const int x, const int y ){
	// Detects player's collision with any objects in the frame.
	// Disables player once collision occurs.
	struct Pair t = getPlayerIndex(x,y);

	int index = t.index;
	int bitIn = t.bitIn;

	uint8_t p = frame[index];
	if( p > 0 ){
		int collisionMask = p & (1 << bitIn);
		if ( collisionMask ){
			enabledPlayers[playerNr] = 'f';
		}
	}
}

struct Pair getPlayerIndex( const int x, const int y ){
	// Returns calculated index position of a player in the frame array.
	int page = y / 8;
	int bitIn = y % 8;

	int index = x + page * 128;

	struct Pair t = { index, bitIn };
	return t;
}

void playerInput( struct Player players[4] ){
	// Fetches all enabled players' joystick input.
	// Both X and Y values of joystick are positive (1) when idle.
	// X = 1 and Y = 0  -->   left turn
	// X = 0 and Y = 1  -->   right turn
	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){

			int valueX = getJyStckX(i+1);
			int valueY = getJyStckY(i+1);

			if( valueX == 1 && valueY == 1)
				// No turning
				continue;
			if( valueX == 1 && valueY == 0){

				int a = players[i].angle;
				if (TURN > a){
					players[i].angle = a + 360 - TURN;
				} else {
					players[i].angle = a - TURN;
				}

			}
			if( valueX == 0 && valueY == 1){
				players[i].angle =  (players[i].angle + TURN) % 360;
			}
		}
	}
}

void movePlayers( struct Player players[4] ){
	// Fetches players' direction (NSWE), which depends on their angles.
	// Changes players' position depending on direction.
	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){

			int a = players[i].angle;
			struct Direction dir = getDirection(a);
			if(dir.y_axis == 'n'){
				players[i].y = players[i].y - 1;
			} else if(dir.y_axis == 's'){
				players[i].y = players[i].y + 1;
			}

			if(dir.x_axis == 'e'){
				players[i].x = players[i].x + 1;
			} else if(dir.x_axis == 'w'){
				players[i].x = players[i].x - 1;
			}
		}
	}
}

struct Direction getDirection(int a){
	// Returns a direction depending on angle a.
	struct Direction dir;

	if( a > 23 && a <= 150){
		dir.x_axis = 'e';
	} else if ( a > 203 && a <= 338 ) {
		dir.x_axis = 'w';
	}
	if (a > 293 || a <= 68){
		dir.y_axis = 'n';
	} else if (a <= 248 && a > 105 ) {
		dir.y_axis = 's';
	}
	return dir;
}

void drawPlayers( struct Player players[4] ){
	// Draws each enabled player.
	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){
			draw(players[i]);
		}
	}
}

void draw(struct Player player){
	struct Pair t = getPlayerIndex(player.x,player.y);
	int index = t.index;
	int bitIn = t.bitIn;
	int a = player.angle;

	frame[index] = frame[index] | (1 << bitIn);
}

void displaySettings(){
	// Shows settings for toggling multiple players

	displayString(0,"Back, BTN 4");
	//displayString(1,"Player 2 [ ]");
	//displayString(2,"Player 3 [ ]");
	//displayString(3,"Player 4 [ ]");
	//update();

	while(1){
		if(enabledPlayers[1] == 't'){
			displayString(1,"Player 2 [X]");
		} else{
			displayString(1,"Player 2 [ ]");
		}

		if(enabledPlayers[2] == 't'){
			displayString(2,"Player 3 [X]");
		} else{
			displayString(2,"Player 3 [ ]");
		}
		if(enabledPlayers[3] == 't'){
			displayString(3,"Player 4 [X]");
		} else{
			displayString(3,"Player 4 [ ]");
		}
		update();
		getSettings();
		if ( getBtns() == 1 ){
			break;
		}
	}
}

void getSettings(){
	// Fetches toggle input from board, enabling/disabling players.
	int swInput = getSw();

	if( (swInput & 0x2) == 2){
		enabledPlayers[1] = 't';
	}else {
		enabledPlayers[1] = 'f';
	}


	if( (swInput & 0x4) == 4){
		enabledPlayers[2] = 't';
		//displayString(2,"Player 3 [X]");
	} else{
		enabledPlayers[2] = 'f';
		//isplay_string(2,"Player 3 [ ]");
	}

	if( (swInput & 0x8) == 8){
		enabledPlayers[3] = 't';
		//displayString(3,"Player 4 [X]");
	} else{
		enabledPlayers[3] = 'f';
		//displayString(3,"Player 4 [ ]");
	}
}

void displayMenu(){
	displayString(0,"--Menu--");
	displayString(1,"");
	displayString(2,"Play (BTN4)");
	displayString(3,"Settings (BTN3)");
}

void menuLoop(){

	displayMenu();
	update();

	while (1){
		int inptBtn = getBtns();
		getSettings();
		if ( inptBtn == 4 ){
			break;
		}

		if ( inptBtn == 2 ){
			displaySettings();
			displayMenu();
			update();
		}
	}
}
