//
//  types.h
//  MyGame
//
//  Created by Leon Szpilewski on 20/01/2013.
//
//

#ifndef __MyGame__types__
#define __MyGame__types__

namespace cm {
	struct rect_t {
		float x;
		float y;
		float w;
		float h;
	};
	
	struct point_t {
		float x;
		float y;
	};
	
	struct vector_t {
		float x;
		float y;
	};

	namespace trig {
		bool is_point_inside_rect(point_t p, rect_t &r);
	};
}
#endif /* defined(__MyGame__types__) */
