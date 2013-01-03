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
#ifndef __WIESEL_GENERIC_PLATFORM_H__
#define __WIESEL_GENERIC_PLATFORM_H__

#include <wiesel/wiesel-common.def>

#include <wiesel/platform.h>


namespace wiesel {

	class GenericFileSystem;


	/**
	 * @brief A generic platform implementation which should fit
	 * most windows and unix like implementations.
	 */
	class WIESEL_COMMON_EXPORT GenericPlatform : public Platform
	{
	public:
		GenericPlatform();
		virtual ~GenericPlatform();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();

	protected:
		FileSystem*		root_fs;
		FileSystem*		asset_fs;
	};

}

#endif /* __WIESEL_GENERIC_PLATFORM_H__ */
