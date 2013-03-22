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
#ifndef __WIESEL_ANDROID_PLATFORM_H__
#define __WIESEL_ANDROID_PLATFORM_H__

#include <wiesel/wiesel-android.def>

#include <wiesel/io/filesystem.h>
#include <wiesel/platform.h>
#include <android_native_app_glue.h>


namespace wiesel {
namespace android {

	class AndroidAssetFileSystem;
	class AndroidVideoDeviceDriver;
	class IAndroidMessageReceiver;



	int32_t platform_handle_input(struct android_app* app, AInputEvent* event);
	void platform_handle_cmd(struct android_app* app, int32_t cmd);


	/**
	 * @brief The android engine implementation.
	 */
	class WIESEL_ANDROID_EXPORT AndroidPlatform : public Platform
	{
	friend int32_t platform_handle_input(struct android_app* app, AInputEvent* event);
	friend void platform_handle_cmd(struct android_app* app, int32_t cmd);

	public:
		AndroidPlatform();
		virtual ~AndroidPlatform();

	// android stuff
	public:
		/// initialize the platform with the native android app object
		void initAndroidApp(struct android_app* app);

		/// get the android app object
		inline struct android_app *getAndroidApp() {
			return app;
		}

	// message receiver
	public:
		void addReceiver(IAndroidMessageReceiver *receiver);
		void removeReceiver(const IAndroidMessageReceiver *receiver);

	// to be called by callback functions
	private:
		void onCommand(struct android_app* app, int32_t cmd);
		int32_t onInputEvent(struct android_app* app, AInputEvent* event);

/*
	protected:
		bool initWindow();
		void closeWindow();
*/

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();
		virtual FileSystem *getDataFileSystem(const std::string &subdir);
		virtual FileSystem *getExternalDataFileSystem(const std::string &subdir);

	protected:
		struct android_app*			app;
		AndroidAssetFileSystem*		asset_fs;
		FileSystem*					data_fs;
		FileSystem*					data_ext_fs;
		bool						window_initialized;

		std::vector<IAndroidMessageReceiver*>	receivers;
	};



	/**
	 * @brief An interface receiving events from the android platform.
	 */
	class IAndroidMessageReceiver : public virtual SharedObject
	{
	public:
		IAndroidMessageReceiver();
		virtual ~IAndroidMessageReceiver();

	public:
		virtual void onAndroidCommand(int32_t cmd) = 0;
		virtual int32_t onAndroidInputEvent(AInputEvent *event) = 0;
	};
}
}

#endif /* __WIESEL_ANDROID_PLATFORM_H__ */
