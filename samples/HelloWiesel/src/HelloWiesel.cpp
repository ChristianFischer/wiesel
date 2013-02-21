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
#include <wiesel/application_main.h>
#include <wiesel/graph/2d/sprite_node.h>
#include <wiesel/io/filesystem.h>
#include <wiesel/math/matrix.h>
#include <wiesel/resources/graphics/spritesheet.h>
#include <wiesel/ui/bitmapfont.h>
#include <wiesel/ui/label_node.h>
#include <wiesel/video/render_context.h>
#include <wiesel/video/screen.h>
#include <wiesel/video/shader.h>
#include <wiesel/video/texture.h>
#include <wiesel/video/vertexbuffer.h>
#include <wiesel/video/video_driver.h>
#include <wiesel.h>

using namespace wiesel;
using namespace wiesel::video;
using namespace std;


class HelloWiesel
: public wiesel::Application
{
public:
	HelloWiesel() {}
	virtual ~HelloWiesel() {}

public:
	virtual bool onInit() {
		Log::info << "start application HelloWiesel" << std::endl;

		// at first, we create our screen object
		screen = new Screen();
		screen->retain();

		// the screen needs to be initialized
		screen->loadVideoDevice(dimension(640, 480), 0);
		
		// successful?
		assert(screen->getVideoDeviceDriver());
		if (!screen->getVideoDeviceDriver()) {
			return false;
		}

		// print video information
		Log::info << *(screen->getVideoDeviceDriver()->getVideoInfo()) << std::endl;

		// get the screen centre
		const dimension &screen_size = screen->getVideoDeviceDriver()->getResolution();
		float center_x = screen_size.width  / 2;
		float center_y = screen_size.height / 2;
		float size     = min(screen_size.width, screen_size.height);

		// note: we're loading this image from SDcard, it's currently not part of this sample application
		File *tex_file = Engine::getInstance()->getAssetFileSystem()->findFile("/images/wiesel.png");
		if (tex_file) {
			Log::info << "load texture from: " << tex_file->getFullPath() << std::endl;
			texture = Texture::fromFile(tex_file);
			texture->retain();
			texture->loadContentFrom(screen);
		}
		else {
			Log::info << "texture not found" << endl;
			texture = NULL;
		}

		sprite = new SpriteNode(texture);
		sprite->setPosition(center_x, center_y);
		sprite->setScale(size / sprite->getContentSize().getMin());
		sprite->retain();

		// load the bitmap font
		File*			font_file		= Engine::getInstance()->getAssetFileSystem()->findFile("/images/font.png");
		Texture*		font_texture	= Texture::fromFile(font_file);
		SpriteSheet*	font_ss			= new SpriteSheet(font_texture);

		// prepare the texture
		font_texture->loadContentFrom(screen);

		// configure the font
		font_ss->createRasterFrames(16, 8,  0, 26, 'A');
		font_ss->createRasterFrames(16, 8, 32, 26, 'a');
		font_ss->createRasterFrames(16, 8, 97, 31, '!');
		BitmapFont *font = new BitmapFont(font_ss);

		label = new LabelNode();
		label->setFont(font);
		label->setText("Hello Wiesel");
		label->setPosition(center_x, 0.0f);
		label->setPivot(0.5f, 0.0f);
		label->setScale(screen_size.width / label->getContentSize().width * 0.95f);
		label->retain();

		Scene *scene = new Scene();
		scene->addChild(sprite);
		scene->addChild(label);
		pushScene(scene);

		return true;
	}


	virtual void onRun(float dt) {
		Application::onRun(dt);

		switch(screen->getState()) {
			case Video_Uninitialized: {
				// do nothing
				break;
			}

			case Video_Suspended: {
				// do nothing
				break;
			}

			case Video_Background:
			case Video_Active: {
				if (screen && screen->getVideoDeviceDriver()) {
					screen->getVideoDeviceDriver()->preRender();
					this->onRender(screen->getVideoDeviceDriver()->getCurrentRenderContext());
					screen->getVideoDeviceDriver()->postRender();
				}

				break;
			}
		}

		return;
	}


	virtual void onRender(RenderContext *render_context) {
		Application::onRender(render_context);
		return;
	}


	virtual void onShutdown() {
		safe_release(texture);
		safe_release(sprite);
		safe_release(label);

		safe_release(screen);

		return;
	}

private:
	Screen*			screen;

	Texture*		texture;

	SpriteNode*		sprite;
	LabelNode*		label;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


