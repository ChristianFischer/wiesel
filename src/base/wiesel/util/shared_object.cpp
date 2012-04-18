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
#include "shared_object.h"
#include <algorithm>

using namespace wiesel;



SharedObject::List SharedObject::living_objects = List();



SharedObject::SharedObject() : references(0)
{
	// add to living objects list
	living_objects.push_back(this);
	return;
}


SharedObject::~SharedObject() {
	// there shouldn't exist any references to this object when deleting
	// when this assert fails, the object might be deleted by hand.
	assert(references == 0);

	// remove from living objects list
	List::iterator it=std::find(living_objects.begin(), living_objects.end(), this);
	if (it != living_objects.end()) {
		living_objects.erase(it);
	}

	return;
}


void SharedObject::purgeDeadObjects() {
	// we're using a separate list to store which objects should be deleted,
	// instead of deleting them immediately, because deleting an object may cause
	// other objects will also be deleted and thereby the living_objects list will change.
	static List objects_to_delete;
	objects_to_delete.clear();

	for(List::iterator it=living_objects.begin(); it!=living_objects.end(); it++) {
		SharedObject *obj = *it;

		if (obj->getReferenceCount() <= 0) {
			objects_to_delete.push_back(obj);
		}
	}

	// now delete each objects in the "delete" list.
	for(List::iterator it=objects_to_delete.begin(); it!=objects_to_delete.end(); it++) {
		delete *it;
	}

	return;
}

