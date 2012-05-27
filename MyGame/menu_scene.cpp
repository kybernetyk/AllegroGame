//
//  menu_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "menu_scene.h"
#include <vector>

namespace menu_scene {
	struct state_t {
		int foo = 0;
	};
	
	std::vector <state_t> states;
	
	 void tick(double dt, size_t state_id) {
		state_t &state = states[state_id];
		printf("menu scene tick: %f = %i\n", dt, state.foo);
		state.foo++;
		 
		 if (state.foo > 30) {
			 scene_manager::set_scene(create());
		 }
	}
	
	scene_manager::scene create() {
		scene_manager::scene scene;
		state_t state;
		state.foo = 23;
		states.push_back(state);
		size_t i = states.size()-1;
		
		scene.tick_scene = [i](double dt) {
			menu_scene::tick(dt, i);
		};
		return scene;
	}
}