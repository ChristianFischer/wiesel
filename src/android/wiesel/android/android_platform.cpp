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
#ifdef __ANDROID__

#include "android_platform.h"
#include "io/android_asset_fs.h"
#include "video/android_video_driver.h"

#include "wiesel/io/datasource.h"
#include "wiesel/io/directory_filesystem.h"
#include "wiesel/io/generic_root_fs.h"
#include "wiesel/io/file.h"
#include "wiesel/ui/touchhandler.h"
#include "wiesel/util/log.h"
#include "wiesel/video/video_driver.h"
#include "wiesel/engine.h"

#include <algorithm>
#include <assert.h>
#include <inttypes.h>
#include <android_native_app_glue.h>


using namespace wiesel;
using namespace wiesel::android;
using namespace std;


AndroidPlatform::AndroidPlatform() {
	this->app					= NULL;
	this->asset_fs				= NULL;
	this->data_fs				= NULL;
	this->data_ext_fs			= NULL;
	this->window_initialized	= false;

	return;
}

AndroidPlatform::~AndroidPlatform() {
	if (asset_fs) {
		delete asset_fs;
		asset_fs = NULL;
	}

	if (data_fs) {
		delete data_fs;
		data_fs = NULL;
	}

	if (data_ext_fs) {
		delete data_ext_fs;
		data_ext_fs = NULL;
	}

	return;
}






/**
 * Process the next input event.
 */
int32_t wiesel::android::platform_handle_input(struct android_app* app, AInputEvent* event) {
	AndroidPlatform* platform = static_cast<AndroidPlatform*>(app->userData);
	if (platform) {
		return platform->onInputEvent(app, event);
	}

	return 0;
}

/**
 * Process the next main command.
 */
void wiesel::android::platform_handle_cmd(struct android_app* app, int32_t cmd) {
	AndroidPlatform* platform = static_cast<AndroidPlatform*>(app->userData);
	if (platform) {
		platform->onCommand(app, cmd);
	}

	return;
}








void AndroidPlatform::initAndroidApp(struct android_app *app) {
	this->app		= app;
	this->asset_fs	= new AndroidAssetFileSystem(app->activity->assetManager);

	// try to fetch application data via JNI
	fetchApplicationInfoFromJNI();

	if (app && app->activity) {
		// NOTE: There's a known bug, that internalDataPath and externalDataPath are both NULL
		//       on Android 2.3 and we need a workaround for this case.
		if (app->activity->internalDataPath) {
			data_fs = new GenericFileSystem(app->activity->internalDataPath);
		}
		else
		if (this->data_dir.empty() == false) {
			if (data_dir.empty() == false) {
				data_fs = new GenericFileSystem(data_dir);
			}
		}

		if (app->activity->externalDataPath) {
			data_ext_fs = new GenericFileSystem(app->activity->externalDataPath);
		}
	}

	app->userData     = this;
	app->onAppCmd     = platform_handle_cmd;
	app->onInputEvent = platform_handle_input;

	// wait until window was created
	do {
		onRun();
	}
	while(!window_initialized);

	return;
}


void AndroidPlatform::addReceiver(IAndroidMessageReceiver* recv) {
	receivers.push_back(recv);
	return;
}

void AndroidPlatform::removeReceiver(const IAndroidMessageReceiver* recv) {
	std::vector<IAndroidMessageReceiver*>::iterator it;

	while((it = std::find(receivers.begin(), receivers.end(), recv)) != receivers.end()) {
		receivers.erase(it);
	}

	return;
}


bool AndroidPlatform::onInit() {
	return true;
}


void AndroidPlatform::onCommand(android_app* app, int32_t cmd) {
	/*
	string cmd_name = "<unknown>";
	switch(cmd) {
		case APP_CMD_INPUT_CHANGED:				cmd_name = "APP_CMD_INPUT_CHANGED";				break;
		case APP_CMD_INIT_WINDOW:				cmd_name = "APP_CMD_INIT_WINDOW";				break;
		case APP_CMD_TERM_WINDOW:				cmd_name = "APP_CMD_TERM_WINDOW";				break;
		case APP_CMD_WINDOW_RESIZED:			cmd_name = "APP_CMD_WINDOW_RESIZED";			break;
		case APP_CMD_WINDOW_REDRAW_NEEDED:		cmd_name = "APP_CMD_WINDOW_REDRAW_NEEDED";		break;
		case APP_CMD_CONTENT_RECT_CHANGED:		cmd_name = "APP_CMD_CONTENT_RECT_CHANGED";		break;
		case APP_CMD_GAINED_FOCUS:				cmd_name = "APP_CMD_GAINED_FOCUS";				break;
		case APP_CMD_LOST_FOCUS:				cmd_name = "APP_CMD_LOST_FOCUS";				break;
		case APP_CMD_CONFIG_CHANGED:			cmd_name = "APP_CMD_CONFIG_CHANGED";			break;
		case APP_CMD_LOW_MEMORY:				cmd_name = "APP_CMD_LOW_MEMORY";				break;
		case APP_CMD_START:						cmd_name = "APP_CMD_START";						break;
		case APP_CMD_RESUME:					cmd_name = "APP_CMD_RESUME";					break;
		case APP_CMD_SAVE_STATE:				cmd_name = "APP_CMD_SAVE_STATE";				break;
		case APP_CMD_PAUSE:						cmd_name = "APP_CMD_PAUSE";						break;
		case APP_CMD_STOP:						cmd_name = "APP_CMD_STOP";						break;
		case APP_CMD_DESTROY:					cmd_name = "APP_CMD_DESTROY";					break;
	}

	Log::info << endl << cmd_name << endl;
	*/
	

	// check, if the app object is valid
	assert(app == this->app);
	if (app != this->app) {
		return;
	}

	switch (cmd) {
		case APP_CMD_SAVE_STATE: {
			// The system has asked us to save our current state.  Do so.
			break;
		}

		case APP_CMD_INIT_WINDOW: {
			// The window is being shown, get it ready.
			window_initialized = true;
			break;
		}

		case APP_CMD_CONFIG_CHANGED: {
			break;
		}

		case APP_CMD_WINDOW_RESIZED: {
			break;
		}

		case APP_CMD_TERM_WINDOW: {
			window_initialized = false;
			break;
		}

		case APP_CMD_GAINED_FOCUS: {
			break;
		}

		case APP_CMD_LOST_FOCUS: {
			break;
		}

		case APP_CMD_PAUSE:
		case APP_CMD_STOP: {
			break;
		}

		case APP_CMD_START:
		case APP_CMD_RESUME: {
			break;
		}

		case APP_CMD_DESTROY: {
			break;
		}
	}

	// send this event to all registered message receivers
	for(std::vector<IAndroidMessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
		(*it)->onAndroidCommand(cmd);
	}

	return;
}


int32_t AndroidPlatform::onInputEvent(struct android_app* app, AInputEvent* event) {
	int32_t ret = 0;

	// check, if the app object is valid
	assert(app == this->app);
	if (app != this->app) {
		return 0;
	}

	// send this event to all registered message receivers
	for(std::vector<IAndroidMessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
		int32_t r = (*it)->onAndroidInputEvent(event);
		if (r) {
			ret = r;
		}
	}

	return ret;
}


/*
bool AndroidPlatform::initWindow() {
	if (app->window != NULL) {
		// when there's still an existing screen, try to re-attach to it's EGL context
		if (video_device != NULL) {
			bool success = false;

			if (video_device->reattachContext()) {
				success = true;
			}

			if (!success) {
				closeWindow();
				assert(screen == NULL);
			}
		}

		if (video_device == NULL) {
			video_device = new AndroidVideoDevice(this, app);
			video_device->retain();

			if (video_device->init() == false) {
				closeWindow();
			}
		}
	}

	return video_device != NULL;
}


void AndroidPlatform::closeWindow() {
	if (video_device) {
		video_device->releaseContext();
		video_device->release();
		video_device = NULL;
	}

	return;
}
*/


void AndroidPlatform::onShutdown() {
	// close the java activity
	ANativeActivity_finish(app->activity);
	return;
}


void AndroidPlatform::onRunFirst() {
	return;
}


bool AndroidPlatform::onRun() {
	// Read all pending events.
	int ident;
	int events;
	struct android_poll_source* source;

	// loop until all events are read, then continue
	while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
		// Process this event.
		if (source != NULL) {
			source->process(app, source);
		}

		// Check if we are exiting.
		if (app->destroyRequested != 0) {
			Engine::getInstance()->requestExit();
			return true;
		}
	}

	/*
	if (video_device) {
		switch(video_device->getState()) {
			case Engine_Background:
			case Engine_Suspended: {
				// no need to waste CPU time while in suspend state...
				sleep(1);
				break;
			}

			default: {
				break;
			}
		}
	}
	*/

	return false;
}


FileSystem *AndroidPlatform::getRootFileSystem() {
	return NULL;
}


FileSystem *AndroidPlatform::getAssetFileSystem() {
	return asset_fs;
}


FileSystem *AndroidPlatform::getDataFileSystem(const std::string &subdir) {
	return data_fs;
}


FileSystem *AndroidPlatform::getExternalDataFileSystem(const std::string &subdir) {
	if (data_ext_fs) {
		return data_ext_fs;
	}

	return getDataFileSystem(subdir);
}



void AndroidPlatform::fetchApplicationInfoFromJNI() {
	JNIEnv* env = NULL;
	jint initenv_result = app->activity->vm->AttachCurrentThread(&env, NULL);
	if (initenv_result != JNI_OK || env == NULL) {
		Log::info << "Could not get JNI Env (env=" << env << ", result=" << initenv_result << ")" << std::endl;
		return;
	}

	// gett all the classes we need
	jclass activity_class			= env->FindClass("android/app/Activity");
	jclass pkgmgr_class				= env->FindClass("android/content/pm/PackageManager");
	jclass pkginfo_class			= env->FindClass("android/content/pm/PackageInfo");
	jclass appinfo_class			= env->FindClass("android/content/pm/ApplicationInfo");
	jclass file_class				= env->FindClass("java/io/File");

	if (
			activity_class			== NULL
		||	pkgmgr_class			== NULL
		||	pkginfo_class			== NULL
		||	appinfo_class			== NULL
		||	file_class				== NULL
	) {
		return;
	}

	jmethodID activity_getpkgmgr	= env->GetMethodID(activity_class,		"getPackageManager",	"()Landroid/content/pm/PackageManager;");
	jmethodID activity_getpkgname	= env->GetMethodID(activity_class,		"getPackageName",		"()Ljava/lang/String;");
	jmethodID activity_getfilesdir	= env->GetMethodID(activity_class,		"getFilesDir",			"()Ljava/io/File;");
	jmethodID pkgmgr_getpkginfo		= env->GetMethodID(pkgmgr_class,		"getPackageInfo",		"(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
	jmethodID file_getabspath		= env->GetMethodID(file_class,			"getAbsolutePath",		"()Ljava/lang/String;");

	if (
			activity_getpkgmgr		== NULL
		||	activity_getpkgname		== NULL
		||	activity_getfilesdir	== NULL
		||	pkgmgr_getpkginfo		== NULL
		||	file_getabspath			== NULL
	) {
		return;
	}

	jfieldID pkginfo_appinfo		= env->GetFieldID(pkginfo_class,		"applicationInfo",		"Landroid/content/pm/ApplicationInfo;");
	jfieldID appinfo_datadir		= env->GetFieldID(appinfo_class,		"dataDir",				"Ljava/lang/String;");

	if (
			pkginfo_appinfo			== NULL
		||	appinfo_datadir			== NULL
	) {
		return;
	}

	// get the packagename
	jstring pkgname_jstr = (jstring)env->CallObjectMethod(app->activity->clazz, activity_getpkgname);
	if (pkgname_jstr) {
		// store the package name
		const char *chr_pkgname = env->GetStringUTFChars(pkgname_jstr, NULL);
		this->package_name = chr_pkgname;
		env->ReleaseStringUTFChars(pkgname_jstr, chr_pkgname);
		chr_pkgname = NULL;
	}
	else {
		return;
	}

	// get the package manager (and all information we need from it)
	jobject pkgmgr = env->CallObjectMethod(app->activity->clazz, activity_getpkgmgr);
	if (pkgmgr) {
		// get the package info object
		jobject pkginfo = env->CallObjectMethod(pkgmgr, pkgmgr_getpkginfo, pkgname_jstr, 0);
		if (pkginfo) {
			jobject appinfo = env->GetObjectField(pkginfo, pkginfo_appinfo);
			if (appinfo) {
				env->DeleteLocalRef(appinfo);
			}

			env->DeleteLocalRef(pkginfo);
		}

		env->DeleteLocalRef(pkgmgr);
	}

	jobject filedir_file = env->CallObjectMethod(app->activity->clazz, activity_getfilesdir);
	if (filedir_file) {
		jstring filedir_jstr = (jstring)env->CallObjectMethod(filedir_file, file_getabspath);
		if (filedir_jstr) {
			const char *filedir_chr = env->GetStringUTFChars(filedir_jstr, NULL);
			this->data_dir = filedir_chr;
			env->ReleaseStringUTFChars(filedir_jstr, filedir_chr);
			env->DeleteLocalRef(filedir_jstr);
		}

		env->DeleteLocalRef(filedir_file);
	}

	env->DeleteLocalRef(pkgname_jstr);

	return;
}



IAndroidMessageReceiver::IAndroidMessageReceiver() {
	return;
}

IAndroidMessageReceiver::~IAndroidMessageReceiver() {
	return;
}


#endif // __ANDROID__
