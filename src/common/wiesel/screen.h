/*
 * screen.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_SCREEN_H__
#define __WIESEL_SCREEN_H__

#include <wiesel/geometry.h>

namespace wiesel {

	/**
	 * @brief An abstract interface to the current screen.
	 */
	class Screen {
	public:
		Screen();
		virtual ~Screen();

		/// get the current screen size in pixels
		inline const dimension& getSize() const {
			return size;
		}

		/// get the current aspect ratio of the screen
		inline float getAspectRatio() const {
			return size.width / size.height;
		}

	// overridables
	public:
		/// called before rendering a frame
		virtual void preRender() = 0;

		/// called after rendering a frame
		virtual void postRender() = 0;

	protected:
		dimension	size;
	};

}

#endif /* __WIESEL_SCREEN_H__ */
