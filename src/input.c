
void input_btn_start(void){

	if(gameover){
		gameover = 0;
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


