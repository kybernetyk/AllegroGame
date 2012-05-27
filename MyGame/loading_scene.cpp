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
	
	static void init_scene() {
		printf("initing ...\n");
	}
	
	static int count = 0;
	static void do_scene(double dt) {
		printf("loading ... %i\n", count);
		if (count++ >= 5) {
			scene_manager::scene s;
			
			s.init_scene = []() {
				printf("scene init!\n");
			};
			
			s.run_scene = [](double dt) {
				printf("opfer! %f\n", dt);
			};
			
			s.draw_scene = [](double dt) {
				printf("draw: %f\n", dt);
			};
			
			scene_manager::set_scene(s);
			
		}
	}
	
	static void draw(double dt) {
		printf("drawing loading_scene!\n");
	}
	scene_manager::scene create() {
		scene_manager::scene scene;
		scene.init_scene = init_scene;
		scene.run_scene = do_scene;
		scene.draw_scene = draw;
		return scene;
	}
}