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
		int foo = 0;
		
		~state_t() {
			printf("state_t destroyed\n");
		}
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> states;
	
	static void tick(double dt, size_t state_id) {
		auto state = states[state_id];
		
		printf("menu scene tick: %f = %i\n", dt, state->foo);
		state->foo++;
		
		if (state->foo > 40) {
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
		states.push_back(std::make_shared<state_t>(state));
		size_t i = states.size()-1;
		
		scene.tick_scene = [i](double dt) {
			tick(dt, i);
		};
		
		scene.destroy_scene = [i]() {
			//destroy scene at index i
			//this will be cumbersome with an vector
			//maybe get an array with N slots and mark as free
			//or whatever. states should be pretty small
			//so we can ignore this.
			states[i] = nullptr;
			
			destroy();
		};
		
		return scene;
	}
}