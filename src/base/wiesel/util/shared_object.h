/*
 * managed_object.h
 *
 *  Created on: 10.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_UTIL_MANAGED_OBJECT_H__
#define __WIESEL_UTIL_MANAGED_OBJECT_H__

#include <assert.h>
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
	 */
	class SharedObject
	{
	protected:
		SharedObject();

	public:
		virtual ~SharedObject();

	// reference-counting
	public:
		/// An alias type for a list of managed objects.
		typedef std::vector<SharedObject*> List;

		/**
		 * @brief increment the reference-counter.
		 */
		inline void retain() {
			++references;
		}

		/**
		 * @brief decrement the reference-counter.
		 * When the reference-counter hits zero,
		 * the object will be deleted immediately.
		 */
		inline void release() {
			// checks, if there are any references for this object.
			// when this condition fails, the object may be release()'d too often
			// or there's a retain() missing.
			assert(references > 0);

			if (--references <= 0) {
				delete this;
			}
		}

		/**
		 * @brief get the current value of the reference counter.
		 */
		inline int getReferenceCount() const {
			return references;
		}

	// handling living / dead objects
	public:
		/**
		 * @brief Provides a read-only list of all currently living objects.
		 */
		static inline const List *getLivingObjects() {
			return &living_objects;
		}

		/**
		 * @brief Deletes all objects with a reference-count of 0.
		 * This would be all objects, where the reference-counter never was increased.
		 */
		static void purgeDeadObjects();

	private:
		static List	living_objects;
		int			references;
	};



	/**
	 * @brief Takes a pointer of a \ref SharedObject to retain it.
	 * When the pointer is \ref NULL, nothing happens.
	 */
	inline void safe_retain(SharedObject* obj) {
		if (obj != NULL) {
			obj->retain();
		}

		return;
	}

	/**
	 * @brief Takes a pointer of a \ref SharedObject to release it.
	 * When the pointer is \ref NULL, nothing happens.
	 * The pointer will be set to \ref NULL after releasing the object.
	 */
	template <class OBJ>
	inline void safe_release(OBJ*& obj) {
		if (obj != NULL) {
			obj->release();
			obj  = NULL;
		}

		return;
	}


} /* namespace wiesel */
#endif /* __WIESEL_UTIL_MANAGED_OBJECT_H__ */
