
#include <wiesel/application_main.h>
#include <wiesel/gl/shaders.h>
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
		const char src_vertex_shader[] =
			"attribute vec4 vPosition;\n"
			"void main() {\n"
			"  gl_Position = vPosition;\n"
			"}\n";

		const char src_fragment_shader[] =
			"precision mediump float;\n"
			"void main() {\n"
			"  gl_FragColor = vec4 (0.63671875, 0.76953125, 0.22265625, 1.0);\n"
			"}\n";

		vertex_shader   = Shader::compile(src_vertex_shader,   ShaderType_VertexShader);
		fragment_shader = Shader::compile(src_fragment_shader, ShaderType_FragmentShader);
		assert(vertex_shader);
		assert(fragment_shader);

		vertex_shader->attrib_vertex_position	= "vPosition";

		program = new ShaderProgram();
		program->attach(vertex_shader);
		program->attach(fragment_shader);
		program->link();

		vbo.addVertex( 0.0f,  0.5f);
		vbo.addVertex(-0.5f, -0.5f);
		vbo.addVertex( 0.5f, -0.5f);

		return true;
	}


	virtual void onRun() {
		return;
	}


	virtual void onRender() {
		vbo.bind(program);
		vbo.render();
		return;
	}


	virtual void onShutdown() {
		program->release();
		vertex_shader->release();
		fragment_shader->release();

		delete program;
		delete vertex_shader;
		delete fragment_shader;

		return;
	}

private:
	Shader*			vertex_shader;
	Shader*			fragment_shader;
	ShaderProgram*	program;

	VertexBuffer	vbo;
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


