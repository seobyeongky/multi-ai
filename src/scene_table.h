#pragma once

#include "scene.h"

enum scene_id_t {
	INVALID_SCENE = -1,
	INTRO_SCENE = 0,
	NAME_QUERY_SCENE = 1,
	MENU_SCENE = 2,
	PLAY_SCENE = 3,
	NR_SCENE = 4
};

typedef smap<scene_id_t, Scene *> scene_table_t;

extern scene_table_t SCENE_TABLE;

void CreateScenes();