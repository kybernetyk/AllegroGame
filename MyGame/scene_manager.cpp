//
//  scene_manager.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#include "scene_manager.h"

namespace scene_manager {

	static std::function<void(double)> current_scene = nullptr;
	static std::function<void(double)> next_scene = nullptr;

	void set_scene(std::function<void(double)> scene) {
		next_scene = scene;
	}

	void run_scene(double dt) {
		if (current_scene != nullptr)
			current_scene(dt);
		
		if (next_scene) {
			current_scene = next_scene;
			next_scene = nullptr;
		}
	}
}