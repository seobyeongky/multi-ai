#pragma once

#include "internal.h"

struct global_t
{
	RenderWindow				window;

	Logger *					logger;

	Font						default_font;

	smap<wstring, Texture *>	texture_map;
	smap<wstring, Sprite *>		sprite_map;

	audiere::AudioDevicePtr		audio_device;
	audiere::OutputStreamPtr	bg_music;

	int							present_time;
	int							delta_time;

	wstring						my_name;
};

extern global_t G;