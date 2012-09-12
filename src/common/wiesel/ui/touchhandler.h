/**
 * Copyright (C) 2012
 * Christian Fischer
 *
 * https://bitbucket.org/baldur/wiesel/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#ifndef __WIESEL_UI_TOUCHHANDLER_H__
#define	__WIESEL_UI_TOUCHHANDLER_H__

#include <wiesel/wiesel-common.def>
#include <wiesel/engine_interfaces.h>

#include "touch.h"

#include <vector>


namespace wiesel {


	/// alias for a list of touches
	typedef std::vector<Touch*>	TouchList;


	/**
	 * @brief Manager class for handling touch events.
	 */
	class WIESEL_COMMON_EXPORT TouchHandler :
		public IUpdateable
	{
	public:
		TouchHandler();
		virtual ~TouchHandler();

	public:
		/**
		 * @brief Starts a new touch event.
		 * The id of each touch event has to be unique. When starting a new touch event
		 * with an existing ID, the old touch event will be closed first.
		 * @param id	The ID of the new touch event. The ID has to be unique.
		 * @param x		X position of this event on the screen.
		 * @param y		Y position of this event on the screen.
		 * @return \c true, when the event was accepted, \c false when it was rejected.
		 */
		void startTouch(TouchID id, float x, float y);

		/**
		 * @brief Updates the location of an existing touch event.
		 * When no touch event with the given ID was found, the update will be ignored.
		 */
		void updateTouchLocation(TouchID id, float x, float y);

		/**
		 * @brief Releases the touch with the given ID.
		 * When no touch event with the given ID was found, nothing happens.
		 */
		void releaseTouch(TouchID id);

		/**
		 * @brief Releases all touches.
		 */
		void releaseAllTouches();

		/**
		 * @brief Tries to find a touch object by it's ID.
		 * @return A pointer to the touch object or \c NULL, when no touch was found.
		 */
		Touch *findTouchById(TouchID id);
		
		/**
		 * @brief Get the immutable list of all active touch events.
		 */
		inline const TouchList* getActiveTouches() const {
			return &active_touches;
		}

	// IUpdateable
	public:
		virtual void update(float dt);

	private:
		TouchList		active_touches;
	};
}
#endif	/* __WIESEL_UI_TOUCHHANDLER_H__ */

