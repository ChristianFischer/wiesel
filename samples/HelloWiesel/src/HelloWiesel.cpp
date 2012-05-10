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
#include <wiesel.h>

using namespace wiesel;
using namespace std;


class HelloWiesel
: public wiesel::Application
{
public:
	HelloWiesel() : m(matrix4x4::identity) {}
	virtual ~HelloWiesel() {}

public:
	virtual bool onInit() {
		Log::info << "start application HelloWiesel" << std::endl;
		
		// get the screen centre
		const dimension &screen = Engine::getCurrent()->getScreen()->getSize();
		float center_x = screen.width  / 2;
		float center_y = screen.height / 2;
		float size     = min(screen.width, screen.height) / 2;

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

		vbo = new VertexBuffer();
		vbo->retain();
		vbo->setupVertexPositions(2);
	//	vbo->setupVertexColors(4);
		vbo->setupTextureLayer(0);
		vbo->addVertex(center_x - size, center_y + size);
		vbo->addVertex(center_x - size, center_y - size);
		vbo->addVertex(center_x + size, center_y + size);
		vbo->addVertex(center_x + size, center_y - size);

		vbo->setVertexColor(0, 1.0f, 0.0f, 0.0f, 1.0f);
		vbo->setVertexColor(1, 0.0f, 1.0f, 0.0f, 1.0f);
		vbo->setVertexColor(2, 1.0f, 1.0f, 0.0f, 1.0f);
		vbo->setVertexColor(3, 0.0f, 0.0f, 1.0f, 1.0f);

		vbo->setVertexTextureCoordinate(0, 0, 0.0f, 0.0f);
		vbo->setVertexTextureCoordinate(1, 0, 0.0f, 1.0f);
		vbo->setVertexTextureCoordinate(2, 0, 1.0f, 0.0f);
		vbo->setVertexTextureCoordinate(3, 0, 1.0f, 1.0f);
		
		Shader *vertex_shader = Shaders::instance()->getVertexShaderFor(vbo);
		Shader *fragment_shader = Shaders::instance()->getFragmentShaderFor(vbo);
		assert(vertex_shader);
		assert(fragment_shader);

		program = new ShaderProgram();
		program->attach(vertex_shader);
		program->attach(fragment_shader);
		program->link();
		program->retain();

		return true;
	}


	virtual void onRun() {
		return;
	}


	virtual void onRender() {
		program->bind();
		program->setProjectionMatrix(Engine::getCurrent()->getScreen()->getProjectionMatrix());
		program->setModelviewMatrix(matrix4x4::identity);

		vbo->bind(program, texture);
		vbo->render();
		vbo->unbind(program);

		return;
	}


	virtual void onShutdown() {
		safe_release(program);
		safe_release(texture);
		safe_release(vbo);

		return;
	}

private:
	ShaderProgram*	program;
	Texture*		texture;
	VertexBuffer*	vbo;
	matrix4x4		m;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


