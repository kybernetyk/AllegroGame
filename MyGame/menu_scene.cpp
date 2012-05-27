//
//  menu_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "menu_scene.h"

namespace menu_scene {
	static void tick(double dt) {
		printf("menu scene tick: %f\n", dt);
	}

	scene_manager::scene create() {
		scene_manager::scene scene;
		scene.tick_scene = tick;
		return scene;
	}
}