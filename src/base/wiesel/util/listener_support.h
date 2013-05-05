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
#ifndef __WIESEL_UTIL_LISTENER_SUPPORT_H__
#define	__WIESEL_UTIL_LISTENER_SUPPORT_H__

#include "shared_object.h"

#include <algorithm>
#include <list>

namespace wiesel {

	/**
	 * @brief A baseclass to add listener support for any classes.
	 */
	template <class LISTENER>
	class ListenerSupport : public virtual SharedObject
	{
	protected:
		ListenerSupport() { }
		virtual ~ListenerSupport() { }

	public:
		typedef std::list<LISTENER*>	Listeners;

	public:
		/**
		 * @brief Add a new listener to this object.
		 * The class does not take ownage of the listener object.
		 * @param listener	The listener object to be added.
		 */
		void addListener(LISTENER *listener) {
			typename Listeners::iterator it=std::find(listeners.begin(), listeners.end(), listener);
			if (it == listeners.end()) {
				listeners.push_back(listener);
			}

			return;
		}

		/**
		 * @brief Removes a listener from this object.
		 * @param listener	The listener object to be removed.
		 */
		void removeListener(const LISTENER *listener) {
			typename Listeners::iterator it=std::find(listeners.begin(), listeners.end(), listener);
			if (it != listeners.end()) {
				listeners.erase(it);
			}

			return;
		}

	protected:
		/**
		 * @brief Returns the list of all registered listeners.
		 */
		inline const Listeners* getListeners() const{
			return &listeners;
		}

		/**
		 * @brief Iterator to the first registered listener.
		 */
		inline typename Listeners::const_iterator listeners_begin() const {
			return listeners.begin();
		}

		/**
		 * @brief Iterator to the end of the listeners list.
		 */
		inline typename Listeners::const_iterator listeners_end() const {
			return listeners.end();
		}

	private:
		Listeners		listeners;
	};
}

#endif // __WIESEL_UTIL_LISTENER_SUPPORT_H__

