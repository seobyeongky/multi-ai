#pragma once

#include "scene.h"

namespace Director
{
	void SwitchScene(Scene * next_scene);

	void Begin(Scene * first_scene);
}