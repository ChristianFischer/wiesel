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
#ifndef __WIESEL_APPLICATION_ENTRY_H__
#define __WIESEL_APPLICATION_ENTRY_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/application.h>
#include <wiesel/module.h>
#include <wiesel/module_registry.h>

/// macro to register an application class, so it can be loaded from the engine
#define WIESEL_APPLICATION_SETUP(APPLICATION_CLASS)								\
				static APPLICATION_CLASS* __##APPLICATION_CLASS##_factory() {	\
					return new APPLICATION_CLASS();								\
				}																\
																				\
				REGISTER_MODULE(												\
							wiesel::Application,								\
							APPLICATION_CLASS,									\
							&__##APPLICATION_CLASS##_factory,					\
							"Application",										\
							0x01000000u,										\
							wiesel::IModuleLoader::PriorityNormal				\
				)


#endif /* __WIESEL_APPLICATION_H__ */
