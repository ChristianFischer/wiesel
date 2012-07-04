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
#ifndef __WIESEL_UI_TOUCH_H__
#define	__WIESEL_UI_TOUCH_H__

#include <wiesel/util/shared_object.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>

#include <inttypes.h>


namespace wiesel {

	// predeclarations
	
	class Node;
	class TouchHandler;
	class TouchReceiver;


	/// alias type for an unique touch-identifier.
	typedef uint64_t	TouchID;



	enum TouchState {
		TouchState_Started,
		TouchState_Holding,
		TouchState_Moving,
		TouchState_Released,
	};



	class Touch
	{
	friend class TouchHandler;

	private:
		Touch();

	public:
		Touch(TouchID id, float x, float y);
		~Touch();

		/// get the touch ID
		inline TouchID getTouchID() const {
			return id;
		}

		/// get the age of this touch object
		inline float getAge() const {
			return age;
		}

		inline TouchState getState() const {
			return state;
		}

		/// get the owner of this touch object
		inline TouchReceiver* getOwner() const {
			return owner;
		}

		/// get the untransformed screen location of this event.
		inline const vector2d& getScreenLocation() const {
			return current_screen_location;
		}

		/// get the original screen location, where the touch hit the screen for the first time
		inline const vector2d& getOriginalLocation() const {
			return original_screen_location;
		}

		/// get the screen location in the previous frame
		inline const vector2d& getPreviousScreenLocation() const {
			return previous_screen_location;
		}

	public:
		/**
		 * @brief Claims this event for an owner.
		 * The event can be claimed only one time. All other tries to claim this
		 * event by an other owner will be rejected.
		 * After the event was claimed by a specific owner, all upcoming events
		 * will be transformed into the coordinate space of it's owner.
		 * @param owner The new owner of this event.
		 * @return \c true on success, \c false otherwise.
		 */
		bool claim(TouchReceiver *owner);

	// for the TouchHandler
	public:
		/**
		 * @brief Timed update for this object
		 */
		void update(float dt);

		/**
		 * @brief Switch the state of this touch
		 * @param state	the new state.
		 */
		void setState(TouchState state);

		/**
		 * @brief Set the new screen-location of this touch.
		 * It will automatically change the transformed location,
		 * when it has a valid owner object.
		 */
		void updateLocation(float new_x, float new_y);

	private:
		TouchID			id;
		float			age;
		TouchState		state;
		vector2d		original_screen_location;
		vector2d		previous_screen_location;
		vector2d		current_screen_location;

		TouchReceiver*	owner;
		Node*			node;
	};



	class TouchReceiver : public virtual SharedObject
	{
	public:
		TouchReceiver();
		virtual ~TouchReceiver();

		virtual void onTouchStarted(Touch *touch);
		virtual void onTouchMoved(const Touch *touch);
		virtual void onTouchEnded(const Touch *touch);
	};

}
#endif	/* __WIESEL_UI_TOUCH_H__ */

