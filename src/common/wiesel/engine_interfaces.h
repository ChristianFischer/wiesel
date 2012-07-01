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
#ifndef __WIESEL_ENGINE_INTERFACES_H__
#define __WIESEL_ENGINE_INTERFACES_H__

#include <wiesel/util/shared_object.h>


namespace wiesel {

	/**
	 * @brief Interface for classes, which wants to receive periodically updates.
	 * To receive updates, the object has to be received on the engine object.
	 * The update will be invoked each frame by the engine.
	 */
	class IUpdateable : public virtual SharedObject
	{
	public:
		IUpdateable();
		virtual ~IUpdateable();

	public:
		/**
		 * @brief Called each frame by the engine.
		 * @param dt The time which has elapsed since the last update in seconds.
		 */
		virtual void update(float dt) = 0;
	};

}

#endif /* __WIESEL_ENGINE_INTERFACES_H__ */
