//
//  menu_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#include "menu_scene.h"
#include "camera.h"
#include <vector>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

namespace menu_scene {
	
	/* example of having distinct states for each scene
	 instance. this shouldn't be neccessary too often - 
	 most of the time one static module global state
	 struct should be enough. */
	struct state_t {
		int foo = 0;
		ALLEGRO_BITMAP *bmp;

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

	
	static void init(size_t state_id) {
		auto state = states[state_id];
		state->foo = 2;
		
		state->bmp = al_load_bitmap("arma.png");
		if (!state->bmp) {
			fprintf(stderr, "couldn't load arma.png!\n");
		}
	}

	static void destroy(size_t state_id) {
		//destroy scene at index i
		//this will be cumbersome with an vector
		//maybe get an array with N slots and mark as free
		//or whatever. states should be pretty small
		//so we can ignore this.
		states[state_id] = nullptr;
	}
	
	static void tick(double dt, size_t state_id) {
		auto state = states[state_id];
		
		//calculate our world
		state->foo++;
		printf("menu scene tick: %f = %i\n", dt, state->foo);
		camera::translate_by(0.5, 0.5);
//		if (state->foo > 40) {
//			scene_manager::scene scene;
//			scene.tick_scene = [](double dt) {
//				printf("dummy\n");
//			};
//			scene_manager::set_scene(scene);
//		}
	}
	
	static void draw(double dt, size_t state_id) {
		auto state = states[state_id];
		printf("drawing with foo %i\n", state->foo);

		camera::apply();		
		ALLEGRO_COLOR col = {.r = 255};
		
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);
		col.g = 255;
		
		al_draw_filled_rounded_rectangle(300, 300, 340, 340, 8, 8, col);
		al_draw_bitmap(state->bmp, 200, 200, 0);
		
		ALLEGRO_TRANSFORM T;
		al_identity_transform(&T);
		al_use_transform(&T);
	}
	
	
	scene_manager::scene create() {
		scene_manager::scene scene;
		state_t state;
		state.foo = 23;
		states.push_back(std::make_shared<state_t>(state));
		size_t i = states.size()-1;
		
		scene.init_scene = [i]() {
			init(i);
		};
		
		scene.destroy_scene = [i]() {
			destroy(i);
		};

		scene.tick_scene = [i](double dt) {
			tick(dt, i);
		};
		
		scene.draw_scene = [i](double dt) {
			draw(dt, i);
		};
		
		
		return scene;
	}
}