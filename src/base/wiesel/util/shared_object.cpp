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



SharedObject::SharedObject() : references(0)
{
	return;
}


SharedObject::~SharedObject() {
	// there shouldn't exist any references to this object when deleting
	// when this assert fails, the object might be deleted by hand.
	assert(references == 0);

	return;
}



static ref<const SharedObject> autorelease_ptr;

void wiesel::autorelease(const SharedObject* obj) {
	autorelease_ptr = obj;
}
