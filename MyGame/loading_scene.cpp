//
//  loading_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#include "loading_scene.h"
#include "scene_manager.h"

namespace loading_scene {
	static int count = 0;
	void do_loading_scene(double dt) {
		printf("loading ... %i\n", count);
		if (count++ >= 5) {
			scene_manager::set_scene([](double dt) {
				printf("fick dich opfer!\n");
			});
		}
	}
}