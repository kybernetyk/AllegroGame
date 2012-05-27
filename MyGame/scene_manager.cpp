//
//  scene_manager.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#include "scene_manager.h"

namespace scene_manager {
	static scene next_scene;
	static scene current_scene;

	void set_scene(scene scene) {
		next_scene = scene;
	}

	void run(double dt) {
		if (current_scene.run_scene != nullptr)
			current_scene.run_scene(dt);
		
		if (current_scene.draw_scene)
			current_scene.draw_scene(dt);
		
		//change to scene which was set in set_scene
		if (next_scene.run_scene != nullptr) {
			if (current_scene.destroy_scene)
				current_scene.destroy_scene();
			
			current_scene = next_scene;
			next_scene.run_scene = nullptr;
			
			if (current_scene.init_scene)
				current_scene.init_scene(); 
		}
	}
}