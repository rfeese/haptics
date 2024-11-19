/*
 * Copyright 2024 Roger Feese
*/
#ifndef HAPTICS_H
#define HAPTICS_H

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
// Will call a function to obtain key value pairs
typedef int (config_get_int_t)(const char *key, int *value);
void Haptics_settings_load(config_get_int_t get_int);

// - Save settings
// Will call a function to send key value pairs
typedef int (config_set_int_t)(const char *key, int value);
void Haptics_settings_save(config_set_int_t set_int);

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
