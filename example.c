/*
 * Copyright 2024 Roger Feese
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "src/haptics.h"

// Pre-defined haptic effects
enum haptic_effects {
	HAPTIC_NUDGE = 1,
	HAPTIC_LASER,
	HAPTIC_EXPLODE,
	HAPTIC_LEFTRIGHT1,
	HAPTIC_LEFTRIGHT2,
};

void register_effects(){
	SDL_HapticEffect effect = {};

	// NUDGE
	effect.type = SDL_HAPTIC_SINE;
	effect.periodic.direction.type = SDL_HAPTIC_POLAR;
	effect.periodic.direction.dir[0] = 0;
	effect.periodic.period = 25;
	effect.periodic.magnitude = 15000;
	effect.periodic.length = 100;
	effect.periodic.attack_length = 0;
	effect.periodic.fade_length = 0;
	Haptics_register_effect_at(&effect, HAPTIC_NUDGE);

	// LASER
	effect.type = SDL_HAPTIC_TRIANGLE;
	effect.periodic.direction.type = SDL_HAPTIC_POLAR;
	effect.periodic.direction.dir[0] = 0;
	effect.periodic.period = 2;
	effect.periodic.magnitude = 12000;
	effect.periodic.length = 800;
	effect.periodic.attack_length = 10;
	effect.periodic.fade_length = 20;
	Haptics_register_effect_at(&effect, HAPTIC_LASER);

	// EXPLODE
	effect.type = SDL_HAPTIC_SINE;
	effect.periodic.direction.type = SDL_HAPTIC_POLAR;
	effect.periodic.direction.dir[0] = 0;
	effect.periodic.period = 80;
	effect.periodic.magnitude = 20000;
	effect.periodic.length = 600;
	effect.periodic.attack_length = 0;
	effect.periodic.fade_length = 550;
	Haptics_register_effect_at(&effect, HAPTIC_EXPLODE);

	// LEFTRIGHT1
	effect.type = SDL_HAPTIC_LEFTRIGHT;
	effect.leftright.length = 700;
	effect.leftright.large_magnitude = 32767;
	effect.leftright.small_magnitude = 0;
	Haptics_register_effect_at(&effect, HAPTIC_LEFTRIGHT1);

	// LEFTRIGHT2
	effect.type = SDL_HAPTIC_LEFTRIGHT;
	effect.leftright.length = 700;
	effect.leftright.large_magnitude = 0;
	effect.leftright.small_magnitude = 32767;
	Haptics_register_effect_at(&effect, HAPTIC_LEFTRIGHT2);
}

int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_GAMECONTROLLER|SDL_INIT_VIDEO);
	// We need a window to collect events from
	SDL_Window *screen = SDL_CreateWindow("haptics example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 200, 0);
	if(!screen){
		printf("Unable to create screen.\n");
	}

	Haptics_init();
	register_effects();

	int exit_signal = 0;
	while(!exit_signal){
		SDL_Event e = {};
		while(SDL_PollEvent(&e)){
			// exit events
			if(e.type == SDL_QUIT){
				exit_signal = 1;
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE){
				exit_signal = 1;
			}


			// controller add
			if(e.type == SDL_CONTROLLERDEVICEADDED){
				// We need to open the haptics device via the joystick, but all we have is joystick device index.
				// 	1. Get joystick instance_id from the device index using SDL_JoystickGetDeviceInstanceID
				// 	2. Get the joystick device from instance_id via SDL_JoystickFromInstanceID
				SDL_Joystick *joy = SDL_JoystickOpen(e.cdevice.which);
				Haptics_open_joystick_for_player(joy, 0);
				Haptics_player_set_enabled(0, 1);
				printf("Added controller for player %d.\n", 0);
			}

			// controller remove
			if(e.type == SDL_CONTROLLERDEVICEREMOVED){
				Haptics_controller_removed(0);
				printf("Removed controller for player %d.\n", 0);
			}

			// haptics playback
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_s){
				Haptics_stop_all();
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_1){
				Haptics_player_run_effect(0, HAPTIC_NUDGE, 1);
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_2){
				Haptics_player_run_effect(0, HAPTIC_LASER, 1);
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_3){
				Haptics_player_run_effect(0, HAPTIC_EXPLODE, 1);
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_4){
				Haptics_player_run_effect(0, HAPTIC_LEFTRIGHT1, 1);
			}
			if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_5){
				Haptics_player_run_effect(0, HAPTIC_LEFTRIGHT2, 1);
			}
		}
	}

	Haptics_close();
	SDL_Quit();
	return EXIT_SUCCESS;
}
