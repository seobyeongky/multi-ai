#include "scene_table.h"
#include "intro_scene.h"
#include "menu_scene.h"
#include "name_query_scene.h"
//#include "play_scene.h"

scene_table_t SCENE_TABLE;

void CreateScenes()
{
	SCENE_TABLE[INTRO_SCENE] = new IntroScene();
	SCENE_TABLE[MENU_SCENE] = new MenuScene();
	SCENE_TABLE[NAME_QUERY_SCENE] = new NameQueryScene();
}