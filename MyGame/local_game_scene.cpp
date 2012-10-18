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
#include "input_manager.h"
#include <vector>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "sys_config.h"
#include <math.h>

namespace local_game_scene {
	
	struct star_t {
		float x;
		float y;
		float r;
		float z;
	};
	
	/* example of having distinct states for each scene
	 instance. this shouldn't be neccessary too often - 
	 most of the time one static module global state
	 struct should be enough. */
	struct state_t {
		sprite::sprite_t player_sprite;
		float player_rot = 0;
		float player_x = 0.0;
		float player_y = 0.0;
		float player_speed = 0.0;
		float player_vx = 0.0;
		float player_vy = 0.0;
		
		float crosshair_x = 0.0;
		float crosshair_y = 0.0;
		
		float world_x = 0.0;
		float world_y = 0.0;
		
		star_t stars[255];
		
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> g_states;
	
	
	static void init(std::shared_ptr<state_t> state) {
		state->player_sprite = sprite::create_sprite("Triangle.png");
		
		for (int i = 0; i < 255; i++) {
			state->stars[i] = star_t {.x = static_cast<float>(rand() % 1000), .y = static_cast<float>(rand() % 1000), .r = static_cast<float>(rand()%4), .z = static_cast<float>(rand() % 3)};
		}
	}
	
	static void destroy(std::shared_ptr<state_t> state) {
		sprite::destroy_sprite(state->player_sprite); //actually not needed because sprites get auto destroyed due to RAII but that's to show how to dealloc sprites if needed
	}
	
	
	static void handle_user_input(double dt, std::shared_ptr<state_t> state) {
		if (input_manager::key_pressed(ALLEGRO_KEY_W)) {

			float speed = 3.0 * dt;
			
			state->player_vy += speed * cos((M_PI/180.0)*state->player_rot);
			state->player_vx += -speed * sin((M_PI/180.0)*state->player_rot);
			
			if (state->player_vy > 3.0)
				state->player_vy = 3.0;
			if (state->player_vx > 3.0)
				state->player_vx = 3.0;
			if (state->player_vy < -3.0)
				state->player_vy = -3.0;
			if (state->player_vx < -3.0)
				state->player_vx = -3.0;
			

		}
		if (input_manager::key_pressed(ALLEGRO_KEY_A)) {
			state->player_rot += dt * 90.0;
		}
		if (input_manager::key_pressed(ALLEGRO_KEY_S)) {
			state->player_speed -= 25.0 * dt;
			if (state->player_speed < -25.0)
				state->player_speed = -25.0;
			
			float speed = state->player_speed * dt;
			state->player_vy += speed * cos((M_PI/180.0)*state->player_rot);
			state->player_vx += speed * sin((M_PI/180.0)*state->player_rot);

		}
		if (input_manager::key_pressed(ALLEGRO_KEY_D)) {
			state->player_rot -= dt * 90.0;
		}
		
	}

	
	static void tick(double dt, std::shared_ptr<state_t> state) {
		handle_user_input(dt, state);

		state->world_x = state->player_x;
		state->world_y = state->player_y;
		
		auto x = state->player_x;
		auto y = state->player_y;
		
		printf("player x: %f, player y: %f\n", x,y);
		
		x = input_manager::mouse_x() + state->world_x;
		y = input_manager::mouse_y() + state->world_y;
		
		state->crosshair_x = x;
		state->crosshair_y = y;

		printf("mouse x: %f, mouse y: %f\n", x,y);

		camera::translate_to(state->world_x, state->world_y);
		
		if (state->player_speed > 0.0) {
			state->player_speed -= dt * 25.0;
		}
		if (state->player_speed < 0.0) {
			state->player_speed += dt * 25.0;
		}
		

		state->player_x += state->player_vx;
		state->player_y += state->player_vy;
	}
	
	static void draw_stars(double dt, std::shared_ptr<state_t> state) {
		for (auto &p : state->stars) {
			ALLEGRO_COLOR col = ALLEGRO_COLOR{255/p.z,255/p.z,255/p.z};

			al_draw_filled_circle(p.x, p.y, p.z, col);
		}
	}
	
	static void draw(double dt, std::shared_ptr<state_t> state) {
		camera::apply();		
		draw_stars(dt, state);
		
		ALLEGRO_COLOR col = {.r = 255};
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);

		col.g = 255;
		sprite::draw_sprite(state->player_sprite, state->player_x, state->player_y, state->player_rot);
		
		al_draw_circle(state->crosshair_x, state->crosshair_y, 16.0, ALLEGRO_COLOR{.r=255}, 4.0);
		
		//al_draw_filled_circle(300, 400, 50, ALLEGRO_COLOR {.r = 129, .b = 255});
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