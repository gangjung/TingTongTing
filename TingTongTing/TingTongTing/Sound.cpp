#pragma warning(disable : 4996)
#include "Sound.h"

SOUND_SYSTEM_* SOUND_SYSTEM_::instance;

/*  *  *  *  *  *  *  *  *  *  생성자  *  *  *  *  *  *  *  *  *  *  *  */
SOUND_SYSTEM_::SOUND_SYSTEM_() {
	strcpy(recent_file, "");
	game_sound = 30;
	pause_controller = false;
	loop_sound = 1;

	System_Create(&this->pSystem);//사운드 시스템 생성
	this->pSystem->init(10, FMOD_INIT_NORMAL, 0);
	System_Create(&this->pSystem1);//사운드 시스템 생성
	set_volume();

	set_effect_volume();
}
SOUND_SYSTEM_::~SOUND_SYSTEM_() {
	delete instance;
}

/*  *  *  *  *  *  *  *  *  *  접근자  *  *  *  *  *  *  *  *  *  *  *  */
SOUND_SYSTEM_* SOUND_SYSTEM_::get() {
	if (instance == NULL) {
		instance = new SOUND_SYSTEM_();
	}

	return instance;
}
System* SOUND_SYSTEM_::get_pSystem() {
	return pSystem;
}
System* SOUND_SYSTEM_::get_pSystem1() {
	return pSystem1;
}
System* SOUND_SYSTEM_::get_pSystem2() {
	return pSystem2;
}
System* SOUND_SYSTEM_::get_pSystem3() {
	return pSystem3;
}
System** SOUND_SYSTEM_::get_add_pSystem() {
	return &pSystem;
}
System** SOUND_SYSTEM_::get_add_pSystem1() {
	return &pSystem1;
}
System** SOUND_SYSTEM_::get_add_pSystem2() {
	return &pSystem2;
}
System** SOUND_SYSTEM_::get_add_pSystem3() {
	return &pSystem3;
}
Channel* SOUND_SYSTEM_::get_pChannel() {
	return pChannel;
}
Channel* SOUND_SYSTEM_::get_pChannel1() {
	return pChannel1;
}
Channel* SOUND_SYSTEM_::get_pChannel2() {
	return pChannel2;
}
Channel* SOUND_SYSTEM_::get_pChannel3() {
	return pChannel3;
}
Channel** SOUND_SYSTEM_::get_add_pChannel() {
	return &pChannel;
}
Channel** SOUND_SYSTEM_::get_add_pChannel1() {
	return &pChannel1;
}
Channel** SOUND_SYSTEM_::get_add_pChannel2() {
	return &pChannel2;
}
Channel** SOUND_SYSTEM_::get_add_pChannel3() {
	return &pChannel3;
}
Sound* SOUND_SYSTEM_::get_pSound(int index) {
	return pSound[index];
}
Sound** SOUND_SYSTEM_::get_add_pSound(int index) {
	return &(pSound[index]);
}
bool SOUND_SYSTEM_::get_pause_controller() {
	return this->pause_controller;
}
bool SOUND_SYSTEM_::get_auto_sync_music() {
	return this->auto_sync_music;
}
bool SOUND_SYSTEM_::get_auto_create() {
	return this->auto_create;
}
char* SOUND_SYSTEM_::get_recent_file() {
	return this->recent_file;
}
int SOUND_SYSTEM_::get_game_sound() {
	return this->game_sound;
}
int SOUND_SYSTEM_::get_effect_sound() {
	return this->effect_sound;
}

/*  *  *  *  *  *  *  *  *  *  설정자  *  *  *  *  *  *  *  *  *  *  *  */
void SOUND_SYSTEM_::set_pSystem(System* pSystem) {
	this->pSystem = pSystem;
}
void SOUND_SYSTEM_::set_pSystem1(System* pSystem1) {
	this->pSystem1 = pSystem1;
}
void SOUND_SYSTEM_::set_pSystem2(System* pSystem2) {
	this->pSystem2 = pSystem2;
}
void SOUND_SYSTEM_::set_pSystem3(System* pSystem3) {
	this->pSystem3 = pSystem3;
}
void SOUND_SYSTEM_::set_pChannel(Channel* pChannel) {
	this->pChannel = pChannel;
}
void SOUND_SYSTEM_::set_pChannel1(Channel* pChannel1) {
	this->pChannel1 = pChannel1;
}
void SOUND_SYSTEM_::set_pChannel2(Channel* pChannel2) {
	this->pChannel2 = pChannel2;
}
void SOUND_SYSTEM_::set_pChannel3(Channel* pChannel3) {
	this->pChannel3 = pChannel3;
}
void SOUND_SYSTEM_::set_pSound(Sound* pSound, int index) {
	this->pSound[index] = pSound;
}
void SOUND_SYSTEM_::set_pause_controller(bool pause_controller) {
	this->pause_controller = pause_controller;
}
void SOUND_SYSTEM_::set_auto_sync_music(bool auto_sync_music) {
	this->auto_sync_music = auto_sync_music;
}
void SOUND_SYSTEM_::set_auto_create(bool auto_create) {
	this->auto_create = auto_create;
}
void SOUND_SYSTEM_::set_recent_file(char* str) {
	strcpy(recent_file, str);
}
void SOUND_SYSTEM_::set_game_sound(int game_sound) {
	this->game_sound = game_sound;
}
void SOUND_SYSTEM_::set_effect_sound(int effect_sound) {
	this->effect_sound = effect_sound;
}
void SOUND_SYSTEM_::set_loop_sound(double loop_sound) {
	this->loop_sound = loop_sound;
	set_volume();
}

/*  *  *  *  *  *  *  *  *  *  함수  *  *  *  *  *  *  *  *  *  *  *  */
void SOUND_SYSTEM_::generate_system(char* str, int index) {
	if (strcmp(str, "")) {		// char* str에 인자 "" 전달시 사운드 시스템 해제 
		switch (index) {
		case SOUND_LOOP_PLAY:
			System_Create(&this->pSystem);//사운드 시스템 생성
			this->pSystem->init(10, FMOD_INIT_NORMAL, 0);
			this->pSystem->createStream(str, FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &pSound[0]);
			this->pSystem->playSound(FMOD_CHANNEL_FREE, pSound[0], 0, &pChannel);
			break;
		case SOUND_MUSIC_PLAY:
			set_recent_file(str);
			System_Create(&this->pSystem1);//사운드 시스템 생성
			this->pSystem1->init(10, FMOD_INIT_NORMAL, 0);
			this->pSystem1->createStream(str, FMOD_HARDWARE, 0, &pSound[1]);
			this->pSystem1->playSound(FMOD_CHANNEL_FREE, pSound[1], 0, &pChannel1);
			break;
		case SOUND_EFFECT_PLAY:
			System_Create(&this->pSystem2);//사운드 시스템 생성
			this->pSystem2->init(10, FMOD_INIT_NORMAL, 0);
			this->pSystem2->createStream(str, FMOD_HARDWARE, 0, &pSound[2]);
			this->pSystem2->playSound(FMOD_CHANNEL_FREE, pSound[2], 0, &pChannel2);
			break;
		case SOUND_SYNC_PLAY:
			System_Create(&this->pSystem3);//사운드 시스템 생성
			this->pSystem3->init(10, FMOD_INIT_NORMAL, 0);
			this->pSystem3->createStream(str, FMOD_HARDWARE, 0, &pSound[3]);
			this->pSystem3->playSound(FMOD_CHANNEL_FREE, pSound[3], 0, &pChannel3);
			break;
		}
	}
	else {
		switch (index) {
		case SOUND_LOOP_PLAY:
			if (this->pSystem) {
				this->pSystem->release();
				this->pSystem->close();
			}
			break;
		case SOUND_MUSIC_PLAY:
			if (this->pSystem1) {
				this->pSystem1->release();
				this->pSystem1->close();
			}
			break;
		case SOUND_EFFECT_PLAY:
			if (this->pSystem2) {
				this->pSystem2->release();
				this->pSystem2->close();
			}
			break;
		case SOUND_SYNC_PLAY:
			if (this->pSystem3) {
				this->pSystem3->release();
				this->pSystem3->close();
			}
			break;
		}
	}
	switch (index) {
	case SOUND_LOOP_PLAY: case SOUND_MUSIC_PLAY:
		set_volume();
		break;
	case SOUND_SYNC_PLAY:
		pChannel3->setVolume((float)game_sound / 100);
		break;
	}
}
void SOUND_SYSTEM_::set_volume() {
	float volume = (float)game_sound / 100;

	if (auto_create) {
		pChannel->setVolume(volume / 1000);
		pChannel1->setVolume(volume / 1000);
		pChannel3->setVolume(volume);
	}
	else {
		pChannel->setVolume(volume * loop_sound);
		pChannel1->setVolume(volume);
	}
}
void SOUND_SYSTEM_::set_effect_volume() {
	float volume = (float)effect_sound / 300 * (float)game_sound / 100 * 2;
	pChannel2->setVolume(volume);
}
void SOUND_SYSTEM_::set_sync_volume(int volume_) {
	float volume = (float)volume_ / 300 * (float)game_sound / 100 * 2;
	pChannel3->setVolume(volume);
}
void SOUND_SYSTEM_::update() {
	this->pSystem->update();
	this->pSystem1->update();
	this->pSystem2->update();
	this->pSystem3->update();
}
void SOUND_SYSTEM_::pause(bool flag) {
	pause_controller = flag;
	pChannel1->setPaused(flag);
	pChannel3->setPaused(flag);
}