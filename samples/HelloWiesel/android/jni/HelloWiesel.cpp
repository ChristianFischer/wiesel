
#include <wiesel/application_main.h>
#include <wiesel/gl/shaders.h>
#include <wiesel/gl/textures.h>
#include <wiesel/gl/vbo.h>

using namespace wiesel;


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
			"uniform sampler2D texture0\n;"
			"void main() {\n"
			"  //gl_FragColor = my_color;\n"
			"  gl_FragColor = texture2D(texture0, my_texcoord0);\n"
			"}\n";

		vertex_shader   = Shader::compile(src_vertex_shader,   ShaderType_VertexShader);
		fragment_shader = Shader::compile(src_fragment_shader, ShaderType_FragmentShader);
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

		// note: we're loading this image from SDcard, it's currently not part of this sample application
		texture = Texture::fromFile("/mnt/sdcard/test.png");

		vbo.setupVertexPositions(2);
		vbo.setupVertexColors(4);
		vbo.setupTextureLayer(0);
		vbo.addVertex(-1.0f / aspect_ratio,  1.0f);
		vbo.addVertex(-1.0f / aspect_ratio, -1.0f);
		vbo.addVertex( 1.0f / aspect_ratio,  1.0f);
		vbo.addVertex( 1.0f / aspect_ratio, -1.0f);

		vbo.setVertexColor(0, 1.0f, 0.0f, 0.0f, 1.0f);
		vbo.setVertexColor(1, 0.0f, 1.0f, 0.0f, 1.0f);
		vbo.setVertexColor(2, 1.0f, 1.0f, 0.0f, 1.0f);
		vbo.setVertexColor(3, 0.0f, 0.0f, 1.0f, 1.0f);

		vbo.setVertexTextureCoordinate(0, 0, 0.0f, 0.0f);
		vbo.setVertexTextureCoordinate(1, 0, 0.0f, 1.0f);
		vbo.setVertexTextureCoordinate(2, 0, 1.0f, 0.0f);
		vbo.setVertexTextureCoordinate(3, 0, 1.0f, 1.0f);

		return true;
	}


	virtual void onRun() {
		return;
	}


	virtual void onRender() {
		vbo.bind(program, texture);
		vbo.render();
		vbo.unbind(program);
		return;
	}


	virtual void onShutdown() {
		program->release();
		vertex_shader->release();
		fragment_shader->release();
		texture->release();

		delete program;
		delete vertex_shader;
		delete fragment_shader;
		delete texture;

		return;
	}

private:
	Shader*			vertex_shader;
	Shader*			fragment_shader;
	ShaderProgram*	program;

	Texture*		texture;

	VertexBuffer	vbo;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


