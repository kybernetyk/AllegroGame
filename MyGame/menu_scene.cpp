//
//  menu_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "menu_scene.h"
#include <vector>
#include <memory>

namespace menu_scene {
	
	/* example of having distinct states for each scene
	 instance. this shouldn't be neccessary too often - 
	 most of the time one static module global state
	 struct should be enough. */
	struct state_t {
		int foo;
	};

	static void tick(double dt, state_t &state) {
		printf("menu scene tick: %f = %i\n", dt, state.foo);
		state.foo++;
		
		if (state.foo > 40) {
			scene_manager::scene scene;
			scene.tick_scene = [](double dt) {
				printf("dummy\n");
			};
			scene_manager::set_scene(scene);
		}
	}
	
	static void destroy() {
		printf("BAM!\n");
	}

	scene_manager::scene create() {
		scene_manager::scene scene;
		state_t state;
		state.foo = 23;
		
		scene.tick_scene = [state](double dt) mutable {
			tick(dt, state);
		};
		
		scene.destroy_scene = destroy;
				
		return scene;
	}
}