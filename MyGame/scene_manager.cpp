//
//  scene_manager.cpp
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#include "scene_manager.h"
#include "input_manager.h"
#include "camera.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

namespace scene_manager {
	static scene next_scene;
	static scene current_scene;
	static ALLEGRO_TRANSFORM identity_transform;
	


	void set_scene(scene scene) {
		next_scene = scene;
	}

	void tick(double dt) {
		//change to scene which was set in set_scene
		if (next_scene.tick_scene != nullptr) {
			if (current_scene.destroy_scene)
				current_scene.destroy_scene();
			
			current_scene = next_scene;
			next_scene.tick_scene = nullptr;
			
			if (current_scene.init_scene)
				current_scene.init_scene();
			camera::reset();
			input_manager::init();
		}

		if (current_scene.tick_scene != nullptr)
			current_scene.tick_scene(dt);
	}
	
	void draw(double dt) {
		al_identity_transform(&identity_transform);
		al_use_transform(&identity_transform);
		al_clear_to_color(al_map_rgb(0,0,0));
		camera::apply();
		if (current_scene.draw_scene) {
			current_scene.draw_scene(dt);
		}
	}
	
	void destroy() {
		current_scene.destroy_scene();
		scene dummy;
		current_scene = dummy;
	}
}