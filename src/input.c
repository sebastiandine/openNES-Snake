void mainloop_handle_input(void){
	input = pad_poll(0);													/* Reading controller poll */
	if((input&PAD_LEFT) && (direction!=DIR_RIGHT)) 	direction = DIR_LEFT;	/* Snake is not allowed to flip to the opposite direction. */
	if((input&PAD_RIGHT) && (direction!=DIR_LEFT)) 	direction = DIR_RIGHT;
	if((input&PAD_UP) && (direction!=DIR_DOWN)) 	direction = DIR_UP;
	if((input&PAD_DOWN) && (direction!=DIR_UP)) 	direction = DIR_DOWN;

	if(input&PAD_START){
		if(!gameover){										/* Ingame, Controller-button 'Start' is responsible for pausing the game */
			pause = (pause == 1)? 0 : 1;
			delay(5);										/* Without a delay, a pause-command is not recognized properly */
		}
		else{												/* In case of game over, 'Start' is responsible for restart */
			gameover = 0;
			restart = 1;
		}
	}
}
