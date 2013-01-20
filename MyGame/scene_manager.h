//
//  scene_manager.h
//  MyGame
//
//  Created by Leon Szpilewski on 5/27/12.
//
//

#ifndef __MyGame__scene_manager__
#define __MyGame__scene_manager__

#include <iostream>
#include <functional>

namespace cm {
	struct scene_t {
		std::function<void()> init_scene = nullptr;
		std::function<void()> destroy_scene = nullptr;
		std::function<void(double)> tick_scene = nullptr;
		std::function<void(double)> draw_scene = nullptr;
	};

	namespace scene_manager {
		void set_scene(scene_t next_scene);
		void tick(double dt);
		void draw(double dt);
		void destroy();
	}
}
#endif /* defined(__MyGame__scene_manager__) */
