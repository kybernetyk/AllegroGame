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
		cm::point_t pos;
		float r;
		float z;
	};
	
	struct orientation_t {
		cm::point_t pos;
		cm::vector_t vel;
		float speed;
		float rot;
	};
	
	struct ship_t {
		orientation_t orientation;
		cm::sprite_t sprite;
	};
	
	struct player_t {
		ship_t ship;
		cm::point_t crosshair_pos;
		int xp;
	};
	
	struct state_t {
		player_t player;
		
		cm::point_t world_pos = {0,0};
		star_t stars[255] = {};
	};
	
	//we're using a vector of shared_ptrs here. though we could
	//have used just a vector of state_t removing those
	//makes it difficult as all ids following the removed
	//state would be invalid. thus we use a shared_ptr
	//that we set to nullptr to trigger object destruction
	std::vector <std::shared_ptr<state_t>> g_states;
	
	
	static void init(std::shared_ptr<state_t> state) {
		state->player.ship.sprite = cm::sprite::create_sprite("Triangle.png");
		
		for (int i = 0; i < 255; i++) {
			state->stars[i] = star_t {.pos = cm::point_t{static_cast<float>(rand() % 1000), static_cast<float>(rand() % 1000)},
									  .r = static_cast<float>(rand()%2),
				.z = static_cast<float>((3.0/(float)(rand()%3+1)))};
		}
	}
	
	static void destroy(std::shared_ptr<state_t> state) {
		cm::sprite::destroy_sprite(state->player.ship.sprite); //actually not needed because sprites get auto destroyed due to RAII but that's to show how to dealloc sprites if needed
	}
	
	
	static void handle_user_input(double dt, std::shared_ptr<state_t> state) {
		if (cm::input_manager::key_pressed(ALLEGRO_KEY_W)) {
			state->player.ship.orientation.speed += 100.0 * dt;
			if (state->player.ship.orientation.speed > 200.0)
				state->player.ship.orientation.speed = 200.0;
		}
		if (cm::input_manager::key_pressed(ALLEGRO_KEY_A)) {
			state->player.ship.orientation.rot += dt * 90.0;
		}
		if (cm::input_manager::key_pressed(ALLEGRO_KEY_S)) {
			state->player.ship.orientation.speed -= 100.0 * dt;
			if (state->player.ship.orientation.speed < -25.0)
				state->player.ship.orientation.speed= -25.0;
		}
		if (cm::input_manager::key_pressed(ALLEGRO_KEY_D)) {
			state->player.ship.orientation.rot -= dt * 90.0;
		}
		
	}
	
	static void update_player(double dt, std::shared_ptr<state_t> state) {
		auto x = cm::input_manager::mouse_x() + state->world_pos.x;
		auto y = cm::input_manager::mouse_y() + state->world_pos.y;
		
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
		
		cm::camera::translate_to(state->world_pos.x, state->world_pos.y);
	}
	
	
	static void draw_stars(double dt, std::shared_ptr<state_t> state) {
		for (auto &s : state->stars) {
			auto visible_world_rect = cm::camera::visible_area();
			if (cm::trig::is_point_inside_rect(s.pos, visible_world_rect)) {
				ALLEGRO_COLOR col = ALLEGRO_COLOR{static_cast<float>(1.0/s.z),static_cast<float>(1.0/s.z),static_cast<float>(1.0/s.z),1.0};
				al_draw_filled_circle(s.pos.x, s.pos.y, s.r, col);
			}
		}
	}
	
	static void draw_hud(double dt, std::shared_ptr<state_t> state) {
		al_draw_filled_rectangle(0, 0, cm::cfg::screen_width, 100, ALLEGRO_COLOR {0.1,0.1,0.1, .a = 0.2});
	}
	
	static void draw(double dt, std::shared_ptr<state_t> state) {
		draw_stars(dt, state);
		
		ALLEGRO_COLOR col = {.r = 1.0,.a = 1.0};
		al_draw_filled_rectangle(0.0, 0.0, 100.0, 100.0, col);
		
		col = ALLEGRO_COLOR{.r= 1.0, .a = 1.0};
		
		cm::sprite::draw_sprite(state->player.ship.sprite,
								state->player.ship.orientation.pos.x,
								state->player.ship.orientation.pos.y,
								state->player.ship.orientation.rot);
		
		al_draw_circle(state->player.crosshair_pos.x, state->player.crosshair_pos.y, 16.0, col, 2.0);
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
		
		
		cm::camera::apply_identity();
		draw_hud(dt, state);
	}
	
	
	cm::scene_t create() {
		cm::scene_t scene;
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