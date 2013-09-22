#pragma once

#include "scene_table.h"

namespace Director
{
	void SwitchScene(scene_id_t sid);

	void Begin(wstring title, size_t width, size_t height, scene_id_t first_scene);
}