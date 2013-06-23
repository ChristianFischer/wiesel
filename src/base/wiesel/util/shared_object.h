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
#ifndef __WIESEL_UTIL_SHARED_OBJECT_H__
#define __WIESEL_UTIL_SHARED_OBJECT_H__

#include <wiesel/wiesel-base.def>

#include <assert.h>
#include <stddef.h>
#include <list>
#include <vector>


namespace wiesel {

	/**
	 * @brief A base-class for any objects, which want to use reference-counting.
	 * Usage:
	 * Each time, a reference to a \ref SharedObject is referenced in a global object,
	 * this object should call \ref retain() to increment the reference counter.
	 * When the object is no longer required, \ref release() should be used to decrement
	 * the reference counter. When the reference counter hits zero, the object will be deleted.
	 * Objects which are never retained will be deleted next time,
	 * when \ref SharedObject::purgeDeadObjects() is called.
	 *
	 * By design, this class should be used as a virtual base class only.
	 */
	class WIESEL_BASE_EXPORT SharedObject
	{
	protected:
		SharedObject();

	public:
		virtual ~SharedObject();
		
	public:
		friend void _keep(const SharedObject* obj);
		friend void _release(const SharedObject* obj);

	// reference-counting
	public:
		/**
		 * @brief get the current value of the reference counter.
		 */
		inline int getReferenceCount() const {
			return references;
		}

	private:
		mutable int		references;
	};
	
	
	
	
	inline void _keep(const SharedObject *obj) {
		assert(obj != NULL);
		++obj->references;
	}

	inline void _release(const SharedObject *obj) {
		assert(obj != NULL);
		assert(obj->references > 0);
		
		if ((--obj->references) <= 0) {
			delete obj;
		}
	}



	/**
	 * @brief Takes a pointer of a \ref SharedObject to retain it.
	 * When the pointer is \ref NULL, nothing happens.
	 * @return the pointer of the object with an incremented reference counter.
	 */
	template <class OBJ>
	inline OBJ* keep(OBJ* obj) {
		if (obj != NULL) {
			_keep(obj);
		}

		return obj;
	}
	
	/**
	 * @brief Releases a pointer to a \ref SharedObject.
	 * When the reference counter of this object reaches zero, the object will be destroyed.
	 */
	inline void release(const SharedObject *obj) {
		if (obj != NULL) {
			_release(obj);
		}
	}

	/**
	 * @brief Takes a pointer of a \ref SharedObject to release it.
	 * When the pointer is \ref NULL, nothing happens.
	 * The pointer will be set to \ref NULL after releasing the object.
	 */
	template <class OBJ>
	inline void safe_release(OBJ*& obj) {
		if (obj != NULL) {
			_release(obj);
			obj  = NULL;
		}

		return;
	}
	
	/**
	 * @brief Marks an object to be autoreleased.
	 * The object will be kept for a short time.
	 * There can be only one object stored to autorelease at the same time.
	 */
	void WIESEL_BASE_EXPORT autorelease(const SharedObject *obj);





	/**
	 * @brief A smartpointer type which wraps a \ref SharedObject 
	 * and automatically increases and decreases the reference counter, 
	 * when the pointer is created or destroyed.
	 */
	template <class T>
	class ref
	{
	public:
		ref() : pointer(NULL) {
		}

		ref(T* p) : pointer(keep(p)) {
		}

		ref(const ref<T>& other) : pointer(keep(other.pointer)) {
		}

		~ref() {
			safe_release(pointer);
		}
		
	public:
		inline const ref<T> operator=(T* p) {
			if (this->pointer != p) {
				safe_release(this->pointer);
				this->pointer = keep(p);
			}

			return *this;
		}

		inline const ref<T> operator=(const ref<T>& other) {
			if (this->pointer != other.pointer) {
				safe_release(this->pointer);
				this->pointer = keep(other.pointer);
			}

			return *this;
		}

	public:
		inline operator T*() {
			return pointer;
		}

		inline operator const T*() const {
			return pointer;
		}
		
	public:
		inline T* operator*() {
			return pointer;
		}

		inline const T* operator*() const {
			return pointer;
		}

	public:
		inline T* operator->() {
			return pointer;
		}

		inline const T* operator->() const {
			return pointer;
		}
		
	public:
		/**
		 * @brief Alias type for a vector, which stores smartpointers to class \ref T.
		 */
		typedef std::vector< wiesel::ref<T> >		vector;
		
		/**
		 * @brief Alias type for a vector, which stores smartpointers to class \ref T.
		 */
		typedef std::list< wiesel::ref<T> >			list;
	
	private:
		T* pointer;
	};

} /* namespace wiesel */
#endif /* __WIESEL_UTIL_SHARED_OBJECT_H__ */
