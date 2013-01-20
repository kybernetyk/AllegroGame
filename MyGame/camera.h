//
//  camera.h
//  MyGame
//
//  Created by Leon Szpilewski on 5/28/12.
//
//

#ifndef __MyGame__camera__
#define __MyGame__camera__
#include "trig.h"

namespace cm {
	namespace camera {
		void translate_to(float x, float y);
		void translate_by(float x, float y);
		void apply();
		void apply_identity();
		void reset();
		
		const float x();
		const float y();
		const rect_t visible_area();
	}
}

#endif /* defined(__MyGame__camera__) */
