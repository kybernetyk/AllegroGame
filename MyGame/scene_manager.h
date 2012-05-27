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

namespace scene_manager {
	struct scene {
		std::function<void()> init_scene = nullptr;
		std::function<void()> destroy_scene = nullptr;
		std::function<void(double)> run_scene = nullptr;
		std::function<void(double)> draw_scene = nullptr;
	};
	
	void set_scene(scene next_scene);
	void run(double dt);
}

#endif /* defined(__MyGame__scene_manager__) */
