/*
 * Copyright 2024 Roger Feese
*/
#include <SDL2/SDL.h>
#include "haptics.h"

// Overall settings
int Haptics_enabled = 1;

// Pre-Defined effects, identified by index
union SDL_HapticEffect HapticsEffectDefinitions[HAPTICS_MAX_EFFECTS] = {};

struct SDL_Haptic;
// Haptic data, indexed by player
HapticsPlayer HapticsPlayers[HAPTICS_MAX_PLAYERS] = {};

// System management
// - Init
int Haptics_init(){
	if(SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0){
		return 0;
	}

	for(int p = 0; p < HAPTICS_MAX_PLAYERS; p++){
		for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
			HapticsPlayers[p].effect[i] = -1;
		}
	}
	return 1;
}

// - Pause all
void Haptics_pause_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			SDL_HapticPause(HapticsPlayers[i].device);
		}
	}
}

void Haptics_unpause_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			SDL_HapticUnpause(HapticsPlayers[i].device);
		}
	}
}

void Haptics_player_pause_all(int player){
	SDL_HapticPause(HapticsPlayers[player].device);
}

void Haptics_player_unpause_all(int player){
	SDL_HapticUnpause(HapticsPlayers[player].device);
}

// - Stop all
void Haptics_stop_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			SDL_HapticStopAll(HapticsPlayers[i].device);
		}
	}
}

void Haptics_player_stop_all(int player){
	SDL_HapticStopAll(HapticsPlayers[player].device);
}

// - Cleanup
void Haptics_close(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			SDL_HapticStopAll(HapticsPlayers[i].device);
			SDL_HapticClose(HapticsPlayers[i].device);
			HapticsPlayers[i].device = NULL;
		}
	}
}

// - Change settings
void Haptics_set_enabled(int value){
	Haptics_enabled = value;
	// if haptics are disabled, make sure that they are stopped.
	if(!value){
		Haptics_stop_all();
	}
}

void Haptics_player_set_enabled(int player, int value){
	HapticsPlayers[player].enabled = value;
	// if haptics are disabled, make sure that they are stopped.
	if(!value){
		Haptics_player_stop_all(player);
	}
}

void Haptics_player_set_gain(int player, int value){
	HapticsPlayers[player].gain = value;
}

// - Load settings
void Haptics_settings_load(config_get_int_t get_int){
	if(!get_int){
		return;
	}
	char configkey[32] = {'\0'};
	int value = 0;
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		snprintf(configkey, 32, "haptics_player_%d_enabled", i);
		if(get_int(&configkey[0], &value)){
			HapticsPlayers[i].enabled = value;
		}
		snprintf(configkey, 32, "haptics_player_%d_gain", i);
		if(get_int(&configkey[0], &value)){
			HapticsPlayers[i].gain = value;
		}
	}
}

// - Save settings
void Haptics_settings_save(config_set_int_t set_int){
	if(!set_int){
		return;
	}
	char configkey[32] = {'\0'};
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		snprintf(configkey, 32, "haptics_player_%d_enabled", i);
		set_int(&configkey[0], HapticsPlayers[i].enabled);

		snprintf(configkey, 32, "haptics_player_%d_gain", i);
		set_int(&configkey[0], HapticsPlayers[i].gain);
	}
}

// - Haptic Device Detection - call on device add / remove
// - Application of effects to devices - on device add
int Haptics_open_joystick_for_player(SDL_Joystick *joystick, int player){
	HapticsPlayers[player].device = SDL_HapticOpenFromJoystick(joystick);
	if(!HapticsPlayers[player].device){
		return 0;
	}

	// try to appply registered effects
	for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
		HapticsPlayers[player].effect[i] = SDL_HapticNewEffect(HapticsPlayers[player].device, &HapticsEffectDefinitions[i]);
	}

	return 1;
}

int Haptics_close_for_player(int player){
	SDL_HapticClose(HapticsPlayers[player].device);
	HapticsPlayers[player].device = 0;

	// clear registered effects
	for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
		HapticsPlayers[player].effect[i] = -1;
	}

	return 1;
}


// Effect definition / management

// - Register and get reference for effect
int Haptics_register_effect(union SDL_HapticEffect *sdlHapticEffect){
	int effect = 0;
	while((effect < HAPTICS_MAX_EFFECTS) && HapticsEffectDefinitions[effect].type){
		effect++;
	}

	if(effect >= HAPTICS_MAX_EFFECTS){
		return -1;
	}
	HapticsEffectDefinitions[effect] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			HapticsPlayers[i].effect[effect] = SDL_HapticNewEffect(HapticsPlayers[i].device, sdlHapticEffect);
		}
	}
	return effect;
}

void Haptics_register_effect_at(union SDL_HapticEffect *sdlHapticEffect, int id){
	if(id >= HAPTICS_MAX_EFFECTS){
		return;
	}
	HapticsEffectDefinitions[id] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			HapticsPlayers[i].effect[id] = SDL_HapticNewEffect(HapticsPlayers[i].device, sdlHapticEffect);
		}
	}
}

// - Delete an effect
void Haptics_remove_effect(int effect){
	if(HapticsEffectDefinitions[effect].type){
		HapticsEffectDefinitions[effect].type = 0;
	}

	// unregister effect from devices
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device && (HapticsPlayers[i].effect[effect] >= 0) ){
			SDL_HapticDestroyEffect(HapticsPlayers[i].device, HapticsPlayers[i].effect[effect]);
			HapticsPlayers[i].effect[effect] = -1;
		}
	}
}

// - Modify an effect
void Haptics_set_effect(union SDL_HapticEffect *sdlHapticEffect, int effect){
	HapticsEffectDefinitions[effect] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(HapticsPlayers[i].device){
			HapticsPlayers[i].effect[effect] = SDL_HapticNewEffect(HapticsPlayers[i].device, sdlHapticEffect);
			if(HapticsPlayers[i].effect[effect] < 0){
				printf("Unable to register effect %d with player %d\n", effect, i);
			}
		}
	}
}


// Effect application / control

// - Apply an effect to player
void Haptics_player_run_effect(int player, int effect, Uint32 iterations){
	if(Haptics_enabled && HapticsPlayers[player].enabled && HapticsPlayers[player].device && (HapticsPlayers[player].effect[effect] >= 0) ){
		SDL_HapticRunEffect(HapticsPlayers[player].device, HapticsPlayers[player].effect[effect], iterations);
	}
}

// - Update an applied effect on a specific player
void Haptics_player_update_effect(int player, int effect, union SDL_HapticEffect *sdlHapticEffect){}

// - Stop effect on a player
void Haptics_player_stop_effect(int player, int effect){
	if(HapticsPlayers[player].device && (HapticsPlayers[player].effect[effect] >= 0) ){
		SDL_HapticStopEffect(HapticsPlayers[player].device, HapticsPlayers[player].effect[effect]);
	}
}

// Callback to clean up haptics when a controller is removed
void Haptics_controller_removed(int player){
	Haptics_player_stop_all(player);
	Haptics_close_for_player(player);
	Haptics_player_set_enabled(player, 0);
}
