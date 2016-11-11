/**
*	@file input.c
*
*	@brief This file contains functions for input handling from a controller.
*
*	@author Sebastian Dine
*
*/

/**
*	This function contains the logic for the START button according to different
*	scenarios e.g. title screen, ingame, gameover.
*
*	@author Sebastian Dine
*
*/
void input_btn_start(void){

	if(gameover){
		current_level = 0;
		restart = 1;
		return;
	}

	if(titlescreen){
		titlescreen = 0;
		return;
	}

	pause = !pause;

}

/**
*	This function provides the main input handling functionalities for an 
*	controller on port 1.
*	It contains logic for input of the following buttons: UP, DOWN, LEFT, RIGHT, START.
*
*	@author Sebastian Dine
*
*/
void mainloop_handle_input(void){
	input = pad_trigger(0);		/* Reading controller 1 input */

	if((input&PAD_LEFT) && (direction!=DIR_RIGHT)){
		direction = DIR_LEFT;
		return;
	}
	if((input&PAD_RIGHT) && (direction!=DIR_LEFT)){
		direction = DIR_RIGHT;
		return;
	}
	if((input&PAD_UP) && (direction!=DIR_DOWN)){
		direction = DIR_UP;
		return;
	}
	if((input&PAD_DOWN) && (direction!=DIR_UP)){
		direction = DIR_DOWN;
		return;
	}
	if(input&PAD_START){
		input_btn_start();
		return;
	}
}


