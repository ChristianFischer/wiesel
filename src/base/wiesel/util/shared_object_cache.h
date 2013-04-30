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
#ifndef __WIEEL_UTIL_SHARED_OBJECT_CACHE_H__
#define	__WIEEL_UTIL_SHARED_OBJECT_CACHE_H__

#include "shared_object.h"

#include <map>


namespace wiesel {

	/**
	 * @brief A class for caching any SharedObject instances.
	 */
	template <typename KEY, typename TYPE>
	class SharedObjectCache
	{
	public:
		/// alias type for lists storing the objects
		typedef std::map<KEY,TYPE*>	List;

		SharedObjectCache() {}
		virtual ~SharedObjectCache() {
			releaseAllObjects();
		}


		/**
		 * @brief Stores a new object in this cache list.
		 * There can be only one object with the same key in the cache.
		 * If there's already an object with that key in the list, the new object
		 * will NOT be added.
		 * @param key		The key of the object, which should be stored.
		 * @param object	The object, which should be stored.
		 * @return			\c true, if the object was stored, \c false if not.
		 */
		bool add(const KEY &key, TYPE *object) {
			if (object != NULL) {
				typename List::iterator it = cached_objects.find(key);
				if (it == cached_objects.end()) {
					cached_objects[key] = object;
					keep(object);
					return true;
				}
			}

			return false;
		}


		/**
		 * @brief Get the object, which was assigned to the given key.
		 * @return	The pointer to the object which was stored for the given key,
		 *			or \c NULL, if no object was found.
		 */
		TYPE *get(const KEY &key) {
			typename List::iterator it = cached_objects.find(key);
			if (it != cached_objects.end()) {
				return it->second;
			}

			return NULL;
		}


		/**
		 * @brief Drop the object with the given key from the cache.
		 */
		void drop(const KEY &key) {
			typename List::iterator it = cached_objects.find(key);
			if (it != cached_objects.end()) {
				release(it->second);
				cached_objects.erase(it);
			}
		}


		/**
		 * @brief Drop the object with the given key from the cache,
		 * only if the object is no longer referenced by other objects.
		 */
		void dropIfUnused(const KEY &key) {
			typename List::iterator it = cached_objects.find(key);
			if (it != cached_objects->end() && it->second->getRetainCount() == 1) {
				release(it->second);
				cached_objects.erase(it);
			}
		}


		/**
		 * @brief Removes all unused objects from this cache.
		 * Unused objects are objects with a retain count of one.
		 */
		void releaseUnusedObjects() {
			typename List::iterator it;
			for(it=cached_objects.begin(); it!=cached_objects.end();) {
				if (it->second->getReferenceCount() == 1) {
					release(it->second);
					cached_objects.erase(it++);
				}
				else {
					++it;
				}
			}

			return;
		}


		/**
		 * @brief Removes all objects from this cache.
		 */
		void releaseAllObjects() {
			typename List::iterator it;
			for(it=cached_objects.begin(); it!=cached_objects.end(); ++it) {
				release(it->second);
			}

			cached_objects.clear();

			return;
		}

	private:
		List	cached_objects;
	};

}

#endif	/* __WIEEL_UTIL_SHARED_OBJECT_CACHE_H__ */
