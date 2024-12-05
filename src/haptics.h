/*
 * Copyright 2024 Roger Feese
*/
#ifndef HAPTICS_H
#define HAPTICS_H

/**
 * Initialize the haptics system.
 *
 * \return 1 if successful.
 */
int Haptics_init();

/**
 * Pause haptics for all players.
 */
void Haptics_pause_all();

/**
 * Resume all player haptics.
 */
void Haptics_unpause_all();

/**
 * Pause haptics for the specified player.
 *
 * \param player Player index.
 */
void Haptics_player_pause_all(int player);

/**
 * Resume haptics for the specified player.
 *
 * \param player Player index.
 */
void Haptics_player_unpause_all(int player);

/**
 * Stop haptics for all players.
 */
void Haptics_stop_all();

/**
 * Stop haptics for specified player.
 *
 * \param player Player index.
 */
void Haptics_player_stop_all(int player);

/**
 * Close haptics system and all devices.
 */
void Haptics_close();

/**
 * Set haptics system enabled.
 *
 * \param value Enabled setting value 0 or 1.
 */
void Haptics_set_enabled(int value);

/**
 * Set haptics enabled for specified player.
 *
 * \param player Player index.
 * \param value Enabled setting value 0 or 1.
 */
void Haptics_player_set_enabled(int player, int value);

/**
 * Set haptics gain/intensity for specified player.
 *
 * \param player Player index.
 * \param value Gain setting value 0 to 9.
 */
void Haptics_player_set_gain(int player, int value);

/**
 * Prototype function for obtaining configuration values.
 */
typedef int (config_get_int_t)(const char *key, int *value);

/**
 * Load haptics system settings.
 *
 * \param get_int Function pointer for obtaining configuration values.
 */
void Haptics_settings_load(config_get_int_t get_int);

/**
 * Prototype function for saving configuration values.
 */
typedef int (config_set_int_t)(const char *key, int value);

/**
 * Save haptics system settings.
 *
 * \param set_int Function pointer for saving configuration values.
 */
void Haptics_settings_save(config_set_int_t set_int);

// prototype
typedef struct _SDL_Joystick SDL_Joystick;

/**
 * Open haptics device on joystick for specified player.
 *
 * \param joystick SDL Joystick.
 * \param player Player index.
 */
int Haptics_open_joystick_for_player(SDL_Joystick *joystick, int player);

/**
 * Close haptics device for player.
 *
 * \param player Player index.
 */
int Haptics_close_for_player(int player);


// prototype
union SDL_HapticEffect;

/**
 * Register a haptics effect.
 *
 * \param sdlHapticsEffect SDL Haptics effect to register.
 * \return Effect index.
 */
int Haptics_register_effect(union SDL_HapticEffect *sdlHapticEffect);

/**
 * Register a haptics effect at a specified index.
 *
 * \param sdlHapticsEffect SDL Haptics effect to register.
 * \param id Index to register effect at.
 */
void Haptics_register_effect_at(union SDL_HapticEffect *sdlHapticEffect, int id);

/**
 * Remove/unregister the haptics effect at the specified index.
 *
 * \param id Effect index.
 */
void Haptics_remove_effect(int effect);

/**
 * Modify effect at specified index.
 *
 * \param effect Effect index.
 */
void Haptics_set_effect(union SDL_HapticEffect *sdlHapticEffect, int effect);

/**
 * Run a haptic effect on the specified player.
 *
 * \param player Player index.
 * \param effect Effect index.
 * \param iterations Number of times to repeat the effect.
 */
void Haptics_player_run_effect(int player, int effect, Uint32 iterations);

/**
 * Update an effect for the specified player.
 *
 * \param player Player index.
 * \param effect Effect index.
 * \param sdlHapticEffect Updated SDL Haptic Effect.
 */
void Haptics_player_update_effect(int player, int effect, union SDL_HapticEffect *sdlHapticEffect);

/**
 * Stop a specified effect for specified player.
 *
 * \param player Player index.
 * \param effect Effect index.
 */
void Haptics_player_stop_effect(int player, int effect);

/**
 * Open haptics device for player when a device is added.
 *
 * \param device_index SDL Joystick device index.
 * \param player Player index.
 */
void Haptics_controller_added(int device_index, int player);

/**
 * Clean up haptics device for player when a device is removed.
 *
 * \param player Player index.
 */
void Haptics_controller_removed(int player);

#endif // HAPTICS_H
