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
#include <wiesel/math/matrix.h>
#include <wiesel/gl/shaders.h>
#include <wiesel/gl/textures.h>
#include <wiesel/gl/vbo.h>
#include <wiesel/io/filesystem.h>
#include <wiesel/graph/2d/sprite_node.h>
#include <wiesel.h>

using namespace wiesel;
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
		
		// get the screen centre
		const dimension &screen = Engine::getCurrent()->getScreen()->getSize();
		float center_x = screen.width  / 2;
		float center_y = screen.height / 2;
		float size     = min(screen.width, screen.height);

		// note: we're loading this image from SDcard, it's currently not part of this sample application
		File *tex_file = Engine::getCurrent()->getAssetFileSystem()->findFile("/images/wiesel.png");
		if (tex_file) {
			Log::info << "load texture from: " << tex_file->getFullPath() << std::endl;
			texture = Texture::fromFile(tex_file);
			texture->retain();
		}
		else {
			Log::info << "texture not found" << endl;
			texture = NULL;
		}

		sprite = new SpriteNode(texture);
		sprite->setPosition(center_x, center_y);
		sprite->setScale(size / sprite->getContentSize().getMin());
		sprite->retain();

		Scene *scene = new Scene();
		scene->addChild(sprite);
		pushScene(scene);

		return true;
	}


	virtual void onRun() {
		Application::onRun();
		return;
	}


	virtual void onRender() {
		Application::onRender();
		return;
	}


	virtual void onShutdown() {
		safe_release(texture);
		safe_release(sprite);
		return;
	}

private:
	Texture*		texture;
	SpriteNode*		sprite;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


