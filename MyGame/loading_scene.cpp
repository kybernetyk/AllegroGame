//
//  loading_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#include "loading_scene.h"
#include "scene_manager.h"
#include "menu_scene.h"

namespace loading_scene {
	static int progress = 0;
	
	static void init_scene() {
		printf("initing ...\n");
		progress = 0;
	}
	
	static void do_scene(double dt) {
		printf("loading progress: %i\n", progress);
		if (progress++ >= 100) {
			/*
			scene_manager::scene s;
			
			s.init_scene = []() {
				printf("scene init!\n");
			};
			
			s.tick_scene = [](double dt) {
				printf("opfer! %f\n", dt);
			};
			
			s.draw_scene = [](double dt) {
				printf("draw: %f\n", dt);
			};
			 */
			
			scene_manager::set_scene(menu_scene::create());
			
		}
	}
	
	static void draw(double dt) {
		printf("drawing loading_scene!\n");
	}
	scene_manager::scene create() {
		scene_manager::scene scene;
		scene.init_scene = init_scene;
		scene.tick_scene = do_scene;
		scene.draw_scene = draw;
		return scene;
	}
}