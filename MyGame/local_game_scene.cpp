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
	struct vector_t {
		float x;
		float y;
	};
	
	struct star_t {
		vector_t pos;
		float r;
		float z;
	};
	
	struct rect_t {
		float x;
		float y;
		float w;
		float h;
	};
	
	struct orientation_t {
		vector_t pos;
		vector_t vel;
		float speed;
		float rot;
	};
	
	struct ship_t {
		orientation_t orientation;
		sprite::sprite_t sprite;
	};
	
	struct player_t {
		ship_t ship;
		vector_t crosshair_pos;
		int xp;
	};
	
	struct state_t {
		player_t player;
		
		vector_t world_pos = {0,0};
		rect_t visible_world_rect = {0,0,SCREEN_W,SCREEN_W};
		
		star_t stars[255] = {};
		
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> g_states;
	
	
	static void init(std::shared_ptr<state_t> state) {
		state->player.ship.sprite = sprite::create_sprite("Triangle.png");
		
		for (int i = 0; i < 255; i++) {
			state->stars[i] = star_t {.pos = vector_t{static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)},
									  .r = static_cast<float>(rand()%4),
									  .z = static_cast<float>(rand() % 3)};
		}
	}
	
	static void destroy(std::shared_ptr<state_t> state) {
		sprite::destroy_sprite(state->player.ship.sprite); //actually not needed because sprites get auto destroyed due to RAII but that's to show how to dealloc sprites if needed
	}
	
	
	static void handle_user_input(double dt, std::shared_ptr<state_t> state) {
		if (input_manager::key_pressed(ALLEGRO_KEY_W)) {
			state->player.ship.orientation.speed += 100.0 * dt;
			if (state->player.ship.orientation.speed > 100.0)
				state->player.ship.orientation.speed = 100.0;
		}
		if (input_manager::key_pressed(ALLEGRO_KEY_A)) {
			state->player.ship.orientation.rot += dt * 90.0;
		}
		if (input_manager::key_pressed(ALLEGRO_KEY_S)) {
			state->player.ship.orientation.speed -= 100.0 * dt;
			if (state->player.ship.orientation.speed < -25.0)
				state->player.ship.orientation.speed= -25.0;
		}
		if (input_manager::key_pressed(ALLEGRO_KEY_D)) {
			state->player.ship.orientation.rot -= dt * 90.0;
		}
		
	}
	
	static void update_player(double dt, std::shared_ptr<state_t> state) {
		auto x = input_manager::mouse_x() + state->world_pos.x;
		auto y = input_manager::mouse_y() + state->world_pos.y;
		
		state->player.crosshair_pos.x = x;
		state->player.crosshair_pos.y = y;
		
		printf("mouse x: %f, mouse y: %f\n", x,y);
		
		if (state->player.ship.orientation.speed > 0.0) {
			state->player.ship.orientation.speed -= dt * 25.0;
		}
		if (state->player.ship.orientation.speed < 0.0) {
			state->player.ship.orientation.speed += dt * 25.0;
		}
		
		float speed = state->player.ship.orientation.speed * dt;
		printf("speed: %f m/s\n", state->player.ship.orientation.speed);
		state->player.ship.orientation.pos.x -= speed * sin((M_PI/180.0)*state->player.ship.orientation.rot);
		state->player.ship.orientation.pos.y += speed * cos((M_PI/180.0)*state->player.ship.orientation.rot);
	}
	
	
	static void tick(double dt, std::shared_ptr<state_t> state) {
		handle_user_input(dt, state);
		
		update_player(dt, state);
		
		state->world_pos.x = state->player.ship.orientation.pos.x;
		state->world_pos.y = state->player.ship.orientation.pos.y;
		
		state->visible_world_rect = rect_t{
			.x = state->world_pos.x - SCREEN_W/2,
			.y = state->world_pos.y - SCREEN_H/2,
			.w = SCREEN_W,
			.h = SCREEN_H
		};

		camera::translate_to(state->world_pos.x, state->world_pos.y);
	}
	
	static bool is_star_inside_rect(star_t &s, rect_t &r) {
		return (s.pos.x >= r.x &&
				s.pos.x <= r.x + r.w &&
				s.pos.y >= r.y &&
				s.pos.y <= r.y + r.h);
	}
	
	static void draw_stars(double dt, std::shared_ptr<state_t> state) {
		for (auto &p : state->stars) {
			if (is_star_inside_rect(p, state->visible_world_rect)) {
				ALLEGRO_COLOR col = ALLEGRO_COLOR{255/p.z,255/p.z,255/p.z,255};
				al_draw_filled_circle(p.pos.x, p.pos.y, p.z, col);
			}
		}
	}
	
	static void draw(double dt, std::shared_ptr<state_t> state) {
		camera::apply();
		draw_stars(dt, state);
		
		ALLEGRO_COLOR col = {.r = 255,.a = 255};
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);
		
		col.g = 255;
		sprite::draw_sprite(state->player.ship.sprite,
							state->player.ship.orientation.pos.x,
							state->player.ship.orientation.pos.y,
							state->player.ship.orientation.rot);
		
		al_draw_circle(state->player.crosshair_pos.x, state->player.crosshair_pos.y, 16.0, ALLEGRO_COLOR{.r=255, .a = 255}, 2.0);
		al_draw_line(state->player.crosshair_pos.x - 16.0,
					 state->player.crosshair_pos.y,
					 state->player.crosshair_pos.x + 16.0,
					 state->player.crosshair_pos.y,
					 col, 2.0);
		al_draw_line(state->player.crosshair_pos.x,
					 state->player.crosshair_pos.y - 16.0,
					 state->player.crosshair_pos.x,
					 state->player.crosshair_pos.y + 16.0,
					 col, 2.0);
		
		
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