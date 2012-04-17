/*
 * engine.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_PLATFORM_ANDROID_ENGINE_H__
#define __WIESEL_PLATFORM_ANDROID_ENGINE_H__

#ifdef __ANDROID__

#include "../../../wiesel/engine.h"

#include <android_native_app_glue.h>


namespace wiesel {

	class GenericFileSystem;
	class AndroidAssetFileSystem;


	/**
	 * @brief The android engine implementation.
	 */
	class AndroidEngine
	: public Engine
	{
	public:
		AndroidEngine(struct android_app *app);
		virtual ~AndroidEngine();

	// to be called by callback functions
	public:
		void __cmd_init_window();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();

	public:
		virtual bool decodeImage(
				DataSource *data,
				unsigned char **pBuffer, size_t *pSize,
				unsigned int *pWidth, unsigned int *pHeight,
				unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
				int *pRbits, int *pGbits, int *pBbits, int *pAbits,
				bool as_texture
		);

	private:
		virtual bool decodeImage_PNG(
				DataSource *data,
				unsigned char **pBuffer, size_t *pSize,
				unsigned int *pWidth, unsigned int *pHeight,
				unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
				int *pRbits, int *pGbits, int *pBbits, int *pAbits,
				bool as_texture
		);

	protected:
		struct android_app*			app;
		GenericFileSystem*			root_fs;
		AndroidAssetFileSystem*		asset_fs;
	};

}

#endif /* __ANDROID__ */
#endif /* __WIESEL_PLATFORM_ANDROID_ENGINE_H__ */
