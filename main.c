#include <pic32mx.h>
#include <stdint.h>
#include <math.h>
#include "display.h"
#include "io.h"


char enabledPlayers[4] = {'t','f','f','f'};
// PORTE är leds

struct player{
	int x;
	int y;
	int angle;
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
void game_loop(void);
void menu_loop(void);
void draw (int x, int y);
void playerInput( struct player players[]);
void movePlayers( struct player players[]);
void drawPlayers( struct player players[]);

int main(void) {
	setup();
	menu_loop();
	game_loop();

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

	//Kanske behövs
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

	init();
}

void game_loop(){

	struct player players[4];

	struct player p1;
	p1.x = 10;
	p1.y = 8;
	p1.angle = 90;
	players[0] = p1;

	if(enabledPlayers[1] == 't'){
		struct player p2;
		p2.x = 118;
		p2.y = 22;
		p2.angle = 270;
		players[1] = p2;
	}

	if(enabledPlayers[2] == 't'){
		struct player p3;
		p3.x = 10;
		p3.y = 22;
		p3.angle = 90;
		players[2] = p3;
	}

	if(enabledPlayers[3] == 't'){
		struct player p4;
		p4.x = 118;
		p4.y = 8;
		p4.angle = 270;
		players[3] = p4;
	}

	int i;
	for ( i = 0; i < 4; i++ ){
		if(enabledPlayers[i] == 't')
			draw(players[i].x, players[i].y);
	}

	render_display(0, frame);
	while(1){
		render_display(0, frame);
		playerInput( players );
		movePlayers( players );
		drawPlayers( players );
		//tick();
		int i;
		for ( i = 0; i < 1000000; i++){}
	}
}

void playerInput( struct player players[4] ){

	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){

			int valueX = getJyStckX(i+1);
			int valueY = getJyStckY(i+1);

			if( valueX == 1 && valueY == 1)
				continue;
			if( valueX == 1 && valueY == 0){

				int a = players[i].angle;
				if (9 > a){
					players[i].angle = a + 360 - 9;
				} else {
					players[i].angle = a - 9;
				}

			}
			if( valueX == 0 && valueY == 1){
				players[i].angle =  (players[i].angle + 9) % 360;
			}
		}
	}

}

void movePlayers( struct player players[4] ){

	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){

			int a = players[i].angle;
			if( a < 180){
				if( a < 90 ){
					int dif1 = 90 - a;
					int dif2  = 45 - a;
					if ( dif2 < 0 ){
						dif2 = 0 - dif2;
					}
					int dif3 = a - 0;

					if(dif3 > dif2){
							if(dif2 > dif1){
								players[i].x = players[i].x + 1;
							} else {
								players[i].x = players[i].x + 1;
								players[i].y = players[i].y - 1;
							}
					} else{
						if( dif3 < dif1 ){
							players[i].y = players[i].y - 1;
						} else {
							players[i].x = players[i].x + 1;
						}
					}
				} else {
					int dif1 = 180 - a;
					int dif2  = 120 - a;
					if ( dif2 < 0 ){
						dif2 = 0 - dif2;
					}
					int dif3 = a - 0;

					if(dif3 > dif2){
							if(dif2 > dif1){
								players[i].y = players[i].y + 1;
							} else {
								players[i].x = players[i].x + 1;
								players[i].y = players[i].y + 1;
							}
					} else{
						if( dif3 < dif1 ){
							players[i].x = players[i].x + 1;
						} else {
							players[i].y = players[i].y + 1;
						}
					}
				}
			} else{
				if( a < 270 ){
					int dif1 = 270 - a;
					int dif2  = 225 - a;
					if ( dif2 < 0 ){
						dif2 = 0 - dif2;
					}
					int dif3 = a - 0;

					if(dif3 > dif2){
							if(dif2 > dif1){
								players[i].x = players[i].x - 1;
							} else {
								players[i].x = players[i].x - 1;
								players[i].y = players[i].y + 1;
							}
					} else{
						if( dif3 < dif1 ){
							players[i].y = players[i].y + 1;
						} else {
							players[i].x = players[i].x - 1;
						}
					}
				} else {
					int dif1 = 360 - a;
					int dif2  = 315 - a;
					if ( dif2 < 0 ){
						dif2 = 0 - dif2;
					}
					int dif3 = a - 0;

					if(dif3 > dif2){
						if(dif2 > dif1){
								players[i].y = players[i].y + 1;
							} else {
								players[i].x = players[i].x + 1;
								players[i].y = players[i].y + 1;
							}
					} else{
						if( dif3 < dif1 ){
							players[i].x = players[i].x + 1;
						} else {
							players[i].y = players[i].y + 1;
						}
					}					
				}
			}

		}
	}

}

void drawPlayers( struct player players[4] ){
	int i;
	for ( i = 0; i < 4; i++ ){
		if ( enabledPlayers[i] == 't' ){
			draw(players[i].x,players[i].y);
		}
	}
}

void draw(int x, int y){
	int page = y / 8;
	int bitIn = y % 8;

	int index = x + page * 128;
	int w;
	for(w = 0; w < 2; w++){
		frame[index + w] = 3 << bitIn;
	}
}

void settings(){

	display_string(0,"Back, BTN 4");
	display_string(1,"Player 2 [ ]");
	display_string(2,"Player 3 [ ]");
	display_string(3,"Player 4 [ ]");
	update();
	while(1){

		int swInput = getSw();

		if( (swInput & 0x2) == 2){
			enabledPlayers[1] = 't';
			display_string(1,"Player 2 [X]");
		} else{
			enabledPlayers[1] = 'f';
			display_string(1,"Player 2 [ ]");
		}


		if( (swInput & 0x4) == 4){
			enabledPlayers[2] = 't';
			display_string(2,"Player 3 [X]");
		} else{
			enabledPlayers[2] = 'f';
			display_string(2,"Player 3 [ ]");
		}

		if( (swInput & 0x8) == 8){
			enabledPlayers[3] = 't';
			display_string(3,"Player 4 [X]");
		} else{
			enabledPlayers[3] = 'f';
			display_string(3,"Player 4 [ ]");
		}
		update();

		if ( getBtns() == 1 ){
			return;
		}
	}
}

void menu_loop(){

	display_string(0,"--Menu--");
	display_string(2,"Play (BTN4)");
	display_string(3,"Settings (BTN3)");
	update();


	while (1){

		int inptBtn = getBtns();
		if ( inptBtn == 4 ){
			break;
		}
		if ( inptBtn == 2 ){
			settings();
			display_string(0,"--Menu--");
			display_string(1,"");
			display_string(2,"Play (BTN4)");
			display_string(3,"Settings (BTN3)");
			update();
		}
	}
}
