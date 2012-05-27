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
	void set_scene(std::function<void(double)> scene);
	void run_scene(double dt);
}

#endif /* defined(__MyGame__scene_manager__) */
