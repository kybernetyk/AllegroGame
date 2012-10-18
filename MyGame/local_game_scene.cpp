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
		float rot;
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> g_states;
	
	
	static void init(std::shared_ptr<state_t> state) {
		state->test = sprite::create_sprite("Triangle.png");
	}
	
	static void destroy(std::shared_ptr<state_t> state) {
		sprite::destroy_sprite(state->test); //actually not needed because sprites get auto destroyed due to RAII but that's to show how to dealloc sprites if needed
	}
	
	static void tick(double dt, std::shared_ptr<state_t> state) {
		state->rot += 0.01;
		printf("rot: %f\n", state->rot);
	}
	
	static void draw(double dt, std::shared_ptr<state_t> state) {
		camera::apply();		
		ALLEGRO_COLOR col = {.r = 255};
		
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);
		col.g = 255;
		sprite::draw_sprite(state->test, 400, 300, state->rot);
	}
	
	
	scene_manager::scene create() {
		scene_manager::scene scene;
		state_t state;
		g_states.push_back(std::make_shared<state_t>(state));
		size_t i = g_states.size()-1;
		
		scene.init_scene = [i]() {
			auto state = g_states[i];
			init(state);
		};
		
		scene.destroy_scene = [i]() {
			auto state = g_states[i];
			destroy(state);
			g_states[i] = nullptr;
		};
		
		scene.tick_scene = [i](double dt) {
			auto state = g_states[i];
			tick(dt, state);
		};
		
		scene.draw_scene = [i](double dt) {
			auto state = g_states[i];
			draw(dt, state);
		};
		
		return scene;
	}
}