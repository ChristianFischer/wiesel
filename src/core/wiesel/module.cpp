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
#include "module.h"
#include "module_registry.h"

using namespace wiesel;



ModuleRegistry *ModuleRegistry::getInstance() {
	static ModuleRegistry instance = ModuleRegistry();
	return &instance;
}



bool wiesel::SortModuleLoadersPredicate(IModuleLoader *a, IModuleLoader *b) {
	// priority is the most important value
	if (a->getPriority() != b->getPriority()) {
		return a->getPriority() > b->getPriority();
	}

	// when the API on both modules are the same, order by API version
	if (
			a->getApi() == b->getApi()
		&&	a->getApiVersion() != b->getApiVersion()
	) {
		return a->getApiVersion() > b->getApiVersion();
	}

	// default order is API name
	return a->getApi() > b->getApi();
}

