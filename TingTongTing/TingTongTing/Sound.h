#pragma once
#pragma once



#include <string>
#include "fmod.hpp"			// FMOD(음악재생)

#pragma comment(lib, "fmodex_vc.lib") // 음악재생 FMOD

using namespace FMOD;				// fmod.hpp

enum SB_SOUNDKIND_ { SD_A = 0, SD_B, SD_C, SD_D, SD_E, SD_F, SD_G, SD_H, SD_END };					// FMOD 인자
enum SB_SOUND_KIND_ { SOUND_LOOP_PLAY = 0, SOUND_MUSIC_PLAY, SOUND_EFFECT_PLAY, SOUND_SYNC_PLAY };	// SOUND_SYSTEM_ 인자

#define Sound_ SOUND_SYSTEM_::get()

class SOUND_SYSTEM_ {
private:
	System * pSystem, *pSystem1, *pSystem2, *pSystem3;
	Channel*	pChannel, *pChannel1, *pChannel2, *pChannel3;
	Sound*		pSound[SD_END];
	bool pause_controller;
	bool auto_sync_music;
	bool auto_create;
	char recent_file[256];
	int game_sound, effect_sound;
	double loop_sound;

	static SOUND_SYSTEM_* instance;

	/*  *  *  *  *  *  *  *  *  *  생성자  *  *  *  *  *  *  *  *  *  *  *  */
	SOUND_SYSTEM_();
	~SOUND_SYSTEM_();
public:

	/*  *  *  *  *  *  *  *  *  *  접근자  *  *  *  *  *  *  *  *  *  *  *  */
	static SOUND_SYSTEM_* get();

	System* get_pSystem();
	System* get_pSystem1();
	System* get_pSystem2();
	System* get_pSystem3();
	System** get_add_pSystem();
	System** get_add_pSystem1();
	System** get_add_pSystem2();
	System** get_add_pSystem3();
	Channel* get_pChannel();
	Channel* get_pChannel1();
	Channel* get_pChannel2();
	Channel* get_pChannel3();
	Channel** get_add_pChannel();
	Channel** get_add_pChannel1();
	Channel** get_add_pChannel2();
	Channel** get_add_pChannel3();
	Sound* get_pSound(int index);
	Sound** get_add_pSound(int index);
	bool get_pause_controller();
	bool get_auto_sync_music();
	bool get_auto_create();
	char* get_recent_file();
	int get_game_sound();
	int get_effect_sound();

	/*  *  *  *  *  *  *  *  *  *  설정자  *  *  *  *  *  *  *  *  *  *  *  */
	void set_pSystem(System* pSystem);
	void set_pSystem1(System* pSystem1);
	void set_pSystem2(System* pSystem2);
	void set_pSystem3(System* pSystem3);
	void set_pChannel(Channel* pChannel);
	void set_pChannel1(Channel* pChannel1);
	void set_pChannel2(Channel* pChannel2);
	void set_pChannel3(Channel* pChannel3);
	void set_pSound(Sound* pSound, int index);
	void set_pause_controller(bool pause_controller);
	void set_auto_sync_music(bool auto_sync_music);
	void set_auto_create(bool auto_create);
	void set_recent_file(char* str);
	void set_game_sound(int game_sound);
	void set_effect_sound(int effect_sound);
	void set_loop_sound(double loop_sound);

	/*  *  *  *  *  *  *  *  *  *  함수  *  *  *  *  *  *  *  *  *  *  *  */
	void generate_system(char* str, int index);
	void set_volume();
	void set_effect_volume();
	void set_sync_volume(int volume_);
	void update();
	void pause(bool flag);
};