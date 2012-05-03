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
	HelloWiesel() {}
	virtual ~HelloWiesel() {}

public:
	virtual bool onInit() {
		Log::info << "start application HelloWiesel" << std::endl;
		
		// fix aspect ratio, until fixed by engine :-P
		float aspect_ratio = Engine::getCurrent()->getScreen()->getAspectRatio();

		const char src_vertex_shader[] =
			"attribute vec4 vPosition;\n"
			"attribute vec4 vColor;\n"
			"attribute vec2 vTexCoord0;\n"
			"varying vec4 my_color;\n"
			"varying vec2 my_texcoord0;\n"
			"uniform sampler2D texture0\n;"
			"void main() {\n"
			"  gl_Position = vPosition;\n"
			"  my_color = vColor;\n"
			"  my_texcoord0 = vTexCoord0;\n"
			"}\n";

		const char src_fragment_shader[] =
			"precision mediump float;\n"
			"varying vec4 my_color;\n"
			"varying vec2 my_texcoord0;\n"
			"uniform sampler2D texture0;\n"
			"void main() {\n"
			"  //gl_FragColor = my_color;\n"
			"  gl_FragColor = texture2D(texture0, my_texcoord0);\n"
			"}\n";

		Shader* vertex_shader   = Shader::compile(src_vertex_shader,   ShaderType_VertexShader);
		Shader* fragment_shader = Shader::compile(src_fragment_shader, ShaderType_FragmentShader);
		assert(vertex_shader);
		assert(fragment_shader);

		vertex_shader->attrib_vertex_position	= "vPosition";
		vertex_shader->attrib_vertex_color		= "vColor";
		vertex_shader->attrib_vertex_texcoords.push_back("vTexCoord0");
		fragment_shader->attrib_vertex_textures.push_back("texture0");

		program = new ShaderProgram();
		program->attach(vertex_shader);
		program->attach(fragment_shader);
		program->link();
		program->retain();

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
		vbo->setupVertexColors(4);
		vbo->setupTextureLayer(0);
		vbo->addVertex(-1.0f / aspect_ratio,  1.0f);
		vbo->addVertex(-1.0f / aspect_ratio, -1.0f);
		vbo->addVertex( 1.0f / aspect_ratio,  1.0f);
		vbo->addVertex( 1.0f / aspect_ratio, -1.0f);

		vbo->setVertexColor(0, 1.0f, 0.0f, 0.0f, 1.0f);
		vbo->setVertexColor(1, 0.0f, 1.0f, 0.0f, 1.0f);
		vbo->setVertexColor(2, 1.0f, 1.0f, 0.0f, 1.0f);
		vbo->setVertexColor(3, 0.0f, 0.0f, 1.0f, 1.0f);

		vbo->setVertexTextureCoordinate(0, 0, 0.0f, 0.0f);
		vbo->setVertexTextureCoordinate(1, 0, 0.0f, 1.0f);
		vbo->setVertexTextureCoordinate(2, 0, 1.0f, 0.0f);
		vbo->setVertexTextureCoordinate(3, 0, 1.0f, 1.0f);

		return true;
	}


	virtual void onRun() {
		return;
	}


	virtual void onRender() {
		vbo->bind(program, texture);
		vbo->render();
		vbo->unbind(program);
		return;
	}


	virtual void onShutdown() {
		logmsg(LogLevel_Error, WIESEL_LOG_TAG, "references before release: %p, %p", program, texture);

		safe_release(program);
		safe_release(texture);
		safe_release(vbo);

		return;
	}

private:
	ShaderProgram*	program;
	Texture*		texture;
	VertexBuffer*	vbo;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


