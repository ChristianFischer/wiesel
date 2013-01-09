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
#ifndef __WIESEL_ANDROID_VIDEO_DEVICE_H__
#define __WIESEL_ANDROID_VIDEO_DEVICE_H__

#include <wiesel/wiesel-android.def>

#ifdef __ANDROID__

#include <wiesel/video/screen.h>
#include <wiesel/video/video_device.h>
#include <wiesel/android/android_platform.h>

#include <wiesel/geometry.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <android_native_app_glue.h>


namespace wiesel {
namespace android {

	/**
	 * @brief The android VideoDevice implementation.
	 */
	class WIESEL_ANDROID_EXPORT AndroidVideoDevice :
			public wiesel::video::VideoDevice,
			public wiesel::android::IAndroidMessageReceiver
	{
	public:
		AndroidVideoDevice(AndroidPlatform *platform, wiesel::video::Screen *screen);
		virtual ~AndroidVideoDevice();

	public:
		bool init();

	private:
		bool initContext();
		bool resize();
		bool detachContext();
		bool reattachContext();
		bool releaseContext();

		virtual void onAndroidCommand(int32_t cmd);
		virtual int32_t onAndroidInputEvent(AInputEvent *event);

		virtual void preRender();
		virtual void postRender();

	private:
		AndroidPlatform*	platform;
		dimension			configured_size;

		EGLint     format;
		EGLConfig  config;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
	};

}
}

#endif // __ANDROID__

#endif /* __WIESEL_ANDROID_VIDEO_DEVICE_H__ */
