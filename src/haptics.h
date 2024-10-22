/*
 * Copyright 2024 Roger Feese
*/
#ifndef HAPTICS_H
#define HAPTICS_H

#define HAPTICS_MAX_EFFECTS 32


// Overall settings
extern int Haptics_enabled;

// Pre-Defined effects, identified by index
union SDL_HapticEffect;
extern union SDL_HapticEffect HapticsEffectDefinitions[HAPTICS_MAX_EFFECTS];

// Haptics data associated with a player
struct _SDL_Haptic;
typedef struct HapticsPlayer {
	int enabled; // is haptics enabled preference
	int gain; // haptics intensity preference
	struct _SDL_Haptic *device; // device associated with the player
	int effect[HAPTICS_MAX_EFFECTS]; // per-playerdevice effect identifiers
} HapticsPlayer;

#define HAPTICS_MAX_PLAYERS 4

// Haptic data, indexed by player
extern HapticsPlayer HapticsPlayers[HAPTICS_MAX_PLAYERS];

// System management
// - Init
int Haptics_init();

// - Pause all
void Haptics_pause_all();
void Haptics_unpause_all();
void Haptics_player_pause_all(int player);
void Haptics_player_unpause_all(int player);

// - Stop all
void Haptics_stop_all();
void Haptics_player_stop_all(int player);

// - Cleanup
void Haptics_close();

// - Change settings
void Haptics_set_enabled(int value);
void Haptics_player_set_enabled(int player, int value);
void Haptics_player_set_gain(int player, int value);

// - Load settings
void Haptics_load_int_setting(char *key, int value);

// - Export settings
// TODO  struct { char key[32]; int value }[] Haptics_settings_export();

// - Haptic Device Detection - call on device add / remove
// - Application of effects to devices - on device add
// jdevice_index: joystick device_index
typedef struct _SDL_Joystick SDL_Joystick;
int Haptics_open_joystick_for_player(SDL_Joystick *joystick, int player);
int Haptics_close_for_player(int player);


// Effect definition / management

// - Register and get reference for effect
union SDL_HapticEffect;
int Haptics_register_effect(union SDL_HapticEffect *sdlHapticEffect);
void Haptics_register_effect_at(union SDL_HapticEffect *sdlHapticEffect, int id);

// - Delete an effect
void Haptics_remove_effect(int effect);

// - Modify an effect
void Haptics_set_effect(union SDL_HapticEffect *sdlHapticEffect, int effect);


// Effect application / control

// - Apply an effect to player
void Haptics_player_run_effect(int player, int effect, Uint32 iterations);

// - Update an applied effect on a specific player
void Haptics_player_update_effect(int player, int effect, union SDL_HapticEffect *sdlHapticEffect);

// - Stop effect on a player
void Haptics_player_stop_effect(int player, int effect);

// Callback to clean up haptics when a controller is removed
void Haptics_controller_removed(int player);

#endif // HAPTICS_H
