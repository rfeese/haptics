#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL_haptic.h>
#include "../../Unity/src/unity.h"
#include "../src/haptics.h"

struct _SDL_Haptic {
};

struct _SDL_Joystick {
};

int _SDL_InitSubSystem_called = 0;
int SDL_InitSubSystem(Uint32 flags){
	_SDL_InitSubSystem_called = 1;
	return 0;
}

int _SDL_HapticPause_called = 0;
int SDL_HapticPause(SDL_Haptic * haptic){
	_SDL_HapticPause_called = 1;
	return 0;
}

int _SDL_HapticUnpause_called = 0;
int SDL_HapticUnpause(SDL_Haptic * haptic){
	_SDL_HapticUnpause_called = 1;
	return 0;
}

int _SDL_HapticStopAll_called = 0;
int SDL_HapticStopAll(SDL_Haptic * haptic){
	_SDL_HapticStopAll_called = 1;
	return 0;
}

int _SDL_HapticClose_called = 0;
void SDL_HapticClose(SDL_Haptic * haptic){
	_SDL_HapticClose_called = 1;
}

SDL_Haptic haptic1 = {};
int _SDL_HapticOpenFromJoystick_called = 0;
SDL_Haptic *SDL_HapticOpenFromJoystick(SDL_Joystick *joystick){
	_SDL_HapticOpenFromJoystick_called = 1;
	return &haptic1;
}

int _SDL_HapticNewEffect_called = 0;
int SDL_HapticNewEffect(SDL_Haptic * haptic, SDL_HapticEffect * effect){
	_SDL_HapticNewEffect_called = 1;
	return 0;
}

int _SDL_HapticDestroyEffect_called = 0;
void SDL_HapticDestroyEffect(SDL_Haptic * haptic, int effect){
	_SDL_HapticDestroyEffect_called = 1;
}

int _SDL_HapticRunEffect_called = 0;
int SDL_HapticRunEffect(SDL_Haptic * haptic, int effect, Uint32 iterations){
	_SDL_HapticRunEffect_called = 1;
	return 0;
}

int _SDL_HapticStopEffect_called = 0;
int SDL_HapticStopEffect(SDL_Haptic * haptic, int effect){
	_SDL_HapticStopEffect_called = 1;
	return 0;
}

SDL_JoystickID SDL_JoystickGetDeviceInstanceID(int device_index){
	return (SDL_JoystickID)1;
}

SDL_Joystick joystick1 = {};
SDL_Joystick *SDL_JoystickFromInstanceID(SDL_JoystickID joyid){
	return &joystick1;
}


// runs before each test
void setUp(void){
	_SDL_InitSubSystem_called = 0;
	_SDL_HapticPause_called = 0;
	_SDL_HapticUnpause_called = 0;
	_SDL_HapticStopAll_called = 0;
	_SDL_HapticClose_called = 0;
	_SDL_HapticOpenFromJoystick_called = 0;
	_SDL_HapticNewEffect_called = 0;
	_SDL_HapticDestroyEffect_called = 0;
	_SDL_HapticRunEffect_called = 0;
	_SDL_HapticStopEffect_called = 0;
}

//runs after each test
void tearDown(void){
}

void test_Haptics_init(){
	TEST_ASSERT_EQUAL_INT(1, Haptics_init());
	TEST_ASSERT_EQUAL_INT(1, _SDL_InitSubSystem_called);
}

void test_Haptics_pause_all(){
	Haptics_pause_all();
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticPause_called);
}

void test_Haptics_unpause_all(){
	Haptics_unpause_all();
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticUnpause_called);
}

void test_Haptics_player_pause_all(){
	Haptics_player_pause_all(0);
	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticPause_called);
}

void test_Haptics_player_unpause_all(){
	Haptics_player_unpause_all(0);
	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticUnpause_called);
}

void test_Haptics_stop_all(){
	Haptics_stop_all();
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticStopAll_called);
}

void test_Haptics_player_stop_all(){
	Haptics_player_stop_all(0);
	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticStopAll_called);
}

void test_Haptics_close(){
	Haptics_close();
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticStopAll_called);
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticClose_called);
}

void test_Haptics_set_enabled(){
	Haptics_set_enabled(1);
	Haptics_set_enabled(0);
}

void test_Haptics_player_set_enabled(){
	Haptics_player_set_enabled(0, 1);
	Haptics_player_set_enabled(0, 0);
}

void test_Haptics_player_set_gain(){
	Haptics_player_set_gain(0, 1);
}

// typedef int (config_get_int_t)(const char *key, int *value);
int _config_get_int_called = 0;
int config_get_int(const char *key, int *value){
	*value = 1;
	_config_get_int_called = 1;
	return 1;
}

void test_Haptics_settings_load(){
	_config_get_int_called = 0;
	Haptics_settings_load(config_get_int);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _config_get_int_called, "config_get_int should have been called.");
}

// typedef int (config_set_int_t)(const char *key, int value);
int _config_set_int_called = 0;
int config_set_int(const char *key, int value){
	_config_set_int_called = 1;
	return 1;
}
void test_Haptics_settings_save(){
	_config_set_int_called = 0;
	Haptics_settings_save(config_set_int);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _config_set_int_called, "config_set_int should have been called.");
}

void test_Haptics_open_joystick_for_player(){
	SDL_Joystick joystick = {};
	TEST_ASSERT_EQUAL_INT(1, Haptics_open_joystick_for_player(&joystick, 0));
	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticOpenFromJoystick_called);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _SDL_HapticNewEffect_called, "Effect should be added to the device.");
}

void test_Haptics_register_effect(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };

	SDL_HapticEffect effect2 = { .type = SDL_HAPTIC_TRIANGLE };
	SDL_Haptic device2 = {};

	TEST_ASSERT_EQUAL_INT(1, Haptics_register_effect(&effect2));
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _SDL_HapticNewEffect_called, "Effect should be added to the device.");
}

void test_Haptics_register_effect_at(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };

	SDL_HapticEffect effect2 = { .type = SDL_HAPTIC_TRIANGLE };
	SDL_Haptic device2 = {};

	Haptics_register_effect_at(&effect2, 1);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _SDL_HapticNewEffect_called, "Effect should be added to the device.");
}

void test_Haptics_remove_effect(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };
	SDL_Haptic device1 = {};

	Haptics_remove_effect(0);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, _SDL_HapticDestroyEffect_called, "Effect should be removed from device.");
}

void test_Haptics_set_effect(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };

	SDL_HapticEffect effect2 = { .type = SDL_HAPTIC_TRIANGLE };

	Haptics_set_effect(&effect2, 0);

	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticNewEffect_called);
}

void test_Haptics_player_run_effect(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };
	SDL_Haptic device1 = {};

	Haptics_player_run_effect(0, 0, 1);
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticRunEffect_called);

	// overall enabled flag is honored
	Haptics_player_run_effect(0, 0, 1);
	TEST_ASSERT_EQUAL_INT(0, _SDL_HapticRunEffect_called);

	// player enabled flag is honored
	Haptics_player_run_effect(0, 0, 1);
	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticRunEffect_called);
}

void test_Haptics_player_update_effect(){
}

void test_Haptics_player_stop_effect(){
	SDL_HapticEffect effect1 = { .type = SDL_HAPTIC_SINE };
	SDL_Haptic device1 = {};

	Haptics_player_stop_effect(0, 0);

	TEST_ASSERT_EQUAL_INT(1, _SDL_HapticStopEffect_called);
}


int main(){
	UNITY_BEGIN();
	RUN_TEST(test_Haptics_init);
	RUN_TEST(test_Haptics_pause_all);
	RUN_TEST(test_Haptics_unpause_all);
	RUN_TEST(test_Haptics_player_pause_all);
	RUN_TEST(test_Haptics_player_unpause_all);
	RUN_TEST(test_Haptics_stop_all);
	RUN_TEST(test_Haptics_player_stop_all);
	RUN_TEST(test_Haptics_close);
	RUN_TEST(test_Haptics_set_enabled);
	RUN_TEST(test_Haptics_player_set_enabled);
	RUN_TEST(test_Haptics_player_set_gain);
	RUN_TEST(test_Haptics_settings_load);
	RUN_TEST(test_Haptics_settings_save);
	RUN_TEST(test_Haptics_open_joystick_for_player);
	RUN_TEST(test_Haptics_register_effect);
	RUN_TEST(test_Haptics_register_effect_at);
	RUN_TEST(test_Haptics_remove_effect);
	RUN_TEST(test_Haptics_set_effect);
	RUN_TEST(test_Haptics_player_run_effect);
	RUN_TEST(test_Haptics_player_update_effect);
	RUN_TEST(test_Haptics_player_stop_effect);

	return UNITY_END();
}
