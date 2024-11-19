/*
 * Copyright 2024 Roger Feese
*/
#include <SDL2/SDL.h>
#include "haptics.h"

#define HAPTICS_MAX_EFFECTS 32
#define HAPTICS_MAX_GAIN 9

// Haptics data associated with a player
struct _SDL_Haptic;
typedef struct HapticsPlayer {
	int enabled; // is haptics enabled preference
	int gain; // haptics intensity / 9
	struct _SDL_Haptic *device; // device associated with the player
	int effect[HAPTICS_MAX_EFFECTS]; // per-playerdevice effect identifiers
} HapticsPlayer;

#define HAPTICS_MAX_PLAYERS 4

// Overall settings

typedef struct Haptics {
	int enabled;
	union SDL_HapticEffect effectDefinitions[HAPTICS_MAX_EFFECTS]; // Pre-Defined effects, identified by index
	HapticsPlayer players[HAPTICS_MAX_PLAYERS]; // Haptic data, indexed by player
} Haptics;

Haptics haptics = { .enabled = 1, .effectDefinitions = {}, .players = {} };


// System management
// - Init
int Haptics_init(){
	if(SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0){
		return 0;
	}

	for(int p = 0; p < HAPTICS_MAX_PLAYERS; p++){
		for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
			haptics.players[p].effect[i] = -1;
		}
	}
	return 1;
}

// - Pause all
void Haptics_pause_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			SDL_HapticPause(haptics.players[i].device);
		}
	}
}

void Haptics_unpause_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			SDL_HapticUnpause(haptics.players[i].device);
		}
	}
}

void Haptics_player_pause_all(int player){
	SDL_HapticPause(haptics.players[player].device);
}

void Haptics_player_unpause_all(int player){
	SDL_HapticUnpause(haptics.players[player].device);
}

// - Stop all
void Haptics_stop_all(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			SDL_HapticStopAll(haptics.players[i].device);
		}
	}
}

void Haptics_player_stop_all(int player){
	SDL_HapticStopAll(haptics.players[player].device);
}

// - Cleanup
void Haptics_close(){
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			SDL_HapticStopAll(haptics.players[i].device);
			SDL_HapticClose(haptics.players[i].device);
			haptics.players[i].device = NULL;
		}
	}
}

// - Change settings
void Haptics_set_enabled(int value){
	haptics.enabled = value;
	// if haptics are disabled, make sure that they are stopped.
	if(!value){
		Haptics_stop_all();
	}
}

void Haptics_player_set_enabled(int player, int value){
	haptics.players[player].enabled = value;
	// if haptics are disabled, make sure that they are stopped.
	if(!value){
		Haptics_player_stop_all(player);
	}
}

void Haptics_player_set_gain(int player, int value){
	haptics.players[player].gain = value;
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
			haptics.players[i].enabled = value;
		}
		snprintf(configkey, 32, "haptics_player_%d_gain", i);
		if(get_int(&configkey[0], &value)){
			haptics.players[i].gain = value;
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
		set_int(&configkey[0], haptics.players[i].enabled);

		snprintf(configkey, 32, "haptics_player_%d_gain", i);
		set_int(&configkey[0], haptics.players[i].gain);
	}
}

// - Haptic Device Detection - call on device add / remove
// - Application of effects to devices - on device add
int Haptics_open_joystick_for_player(SDL_Joystick *joystick, int player){
	haptics.players[player].device = SDL_HapticOpenFromJoystick(joystick);
	if(!haptics.players[player].device){
		return 0;
	}

	// try to appply registered effects
	for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
		haptics.players[player].effect[i] = SDL_HapticNewEffect(haptics.players[player].device, &haptics.effectDefinitions[i]);
	}

	return 1;
}

int Haptics_close_for_player(int player){
	SDL_HapticClose(haptics.players[player].device);
	haptics.players[player].device = 0;

	// clear registered effects
	for(int i = 0; i < HAPTICS_MAX_EFFECTS; i++){
		haptics.players[player].effect[i] = -1;
	}

	return 1;
}


// Effect definition / management

// - Register and get reference for effect
int Haptics_register_effect(union SDL_HapticEffect *sdlHapticEffect){
	int effect = 0;
	while((effect < HAPTICS_MAX_EFFECTS) && haptics.effectDefinitions[effect].type){
		effect++;
	}

	if(effect >= HAPTICS_MAX_EFFECTS){
		return -1;
	}
	haptics.effectDefinitions[effect] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			haptics.players[i].effect[effect] = SDL_HapticNewEffect(haptics.players[i].device, sdlHapticEffect);
		}
	}
	return effect;
}

void Haptics_register_effect_at(union SDL_HapticEffect *sdlHapticEffect, int id){
	if(id >= HAPTICS_MAX_EFFECTS){
		return;
	}
	haptics.effectDefinitions[id] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			haptics.players[i].effect[id] = SDL_HapticNewEffect(haptics.players[i].device, sdlHapticEffect);
		}
	}
}

// - Delete an effect
void Haptics_remove_effect(int effect){
	if(haptics.effectDefinitions[effect].type){
		haptics.effectDefinitions[effect].type = 0;
	}

	// unregister effect from devices
	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device && (haptics.players[i].effect[effect] >= 0) ){
			SDL_HapticDestroyEffect(haptics.players[i].device, haptics.players[i].effect[effect]);
			haptics.players[i].effect[effect] = -1;
		}
	}
}

// - Modify an effect
void Haptics_set_effect(union SDL_HapticEffect *sdlHapticEffect, int effect){
	haptics.effectDefinitions[effect] = *sdlHapticEffect;

	for(int i = 0; i < HAPTICS_MAX_PLAYERS; i++){
		if(haptics.players[i].device){
			haptics.players[i].effect[effect] = SDL_HapticNewEffect(haptics.players[i].device, sdlHapticEffect);
			if(haptics.players[i].effect[effect] < 0){
				printf("Unable to register effect %d with player %d\n", effect, i);
			}
		}
	}
}


// Effect application / control

// - Apply an effect to player
void Haptics_player_run_effect(int player, int effect, Uint32 iterations){
	if(haptics.enabled && haptics.players[player].enabled && haptics.players[player].device && (haptics.players[player].effect[effect] >= 0) ){
		SDL_HapticRunEffect(haptics.players[player].device, haptics.players[player].effect[effect], iterations);
	}
}

// - Update an applied effect on a specific player
void Haptics_player_update_effect(int player, int effect, union SDL_HapticEffect *sdlHapticEffect){}

// - Stop effect on a player
void Haptics_player_stop_effect(int player, int effect){
	if(haptics.players[player].device && (haptics.players[player].effect[effect] >= 0) ){
		SDL_HapticStopEffect(haptics.players[player].device, haptics.players[player].effect[effect]);
	}
}

// Callback to clean up haptics when a controller is removed
void Haptics_controller_removed(int player){
	Haptics_player_stop_all(player);
	Haptics_close_for_player(player);
	Haptics_player_set_enabled(player, 0);
}
