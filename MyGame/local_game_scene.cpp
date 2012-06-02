//
//  local_game_scene.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 02/06/2012.
//
//

#include "local_game_scene.h"
#include "camera.h"
#include "sprite.h"
#include <vector>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

namespace local_game_scene {
	
	/* example of having distinct states for each scene
	 instance. this shouldn't be neccessary too often - 
	 most of the time one static module global state
	 struct should be enough. */
	struct state_t {
		sprite::sprite_t test;
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> states;
	
	
	static void init(size_t state_id) {
		auto state = states[state_id];
		state->test = sprite::create_sprite("arma.png");
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
	}
	
	static void draw(double dt, size_t state_id) {
		auto state = states[state_id];
		
		camera::apply();		
		ALLEGRO_COLOR col = {.r = 255};
		
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);
		col.g = 255;
		sprite::draw_sprite(state->test, 100, 100, 0.0);
	}
	
	
	scene_manager::scene create() {
		scene_manager::scene scene;
		state_t state;
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