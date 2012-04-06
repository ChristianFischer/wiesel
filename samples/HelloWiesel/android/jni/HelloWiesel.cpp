
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
		Log::info << "start application HelloWiesel" << std::endl;

		const char src_vertex_shader[] =
			"attribute vec4 vPosition;\n"
			"attribute vec4 vColor;\n"
			"varying vec4 my_color;\n"
			"void main() {\n"
			"  gl_Position = vPosition;\n"
			"  my_color = vColor;\n"
			"}\n";

		const char src_fragment_shader[] =
			"precision mediump float;\n"
			"varying vec4 my_color;\n"
			"void main() {\n"
			"  gl_FragColor = my_color;\n"
			"}\n";

		vertex_shader   = Shader::compile(src_vertex_shader,   ShaderType_VertexShader);
		fragment_shader = Shader::compile(src_fragment_shader, ShaderType_FragmentShader);
		assert(vertex_shader);
		assert(fragment_shader);

		vertex_shader->attrib_vertex_position	= "vPosition";
		vertex_shader->attrib_vertex_color		= "vColor";

		program = new ShaderProgram();
		program->attach(vertex_shader);
		program->attach(fragment_shader);
		program->link();

		vbo.setupVertexPositions(2);
		vbo.setupVertexColors(4);
		vbo.addVertex(-0.5f,  0.5f);
		vbo.addVertex(-0.5f, -0.5f);
		vbo.addVertex( 0.5f,  0.5f);
		vbo.addVertex( 0.5f, -0.5f);

		vbo.setVertexColor(0, 1.0f, 0.0f, 0.0f, 1.0f);
		vbo.setVertexColor(1, 0.0f, 1.0f, 0.0f, 1.0f);
		vbo.setVertexColor(2, 1.0f, 1.0f, 0.0f, 1.0f);
		vbo.setVertexColor(3, 0.0f, 0.0f, 1.0f, 1.0f);

		return true;
	}


	virtual void onRun() {
		return;
	}


	virtual void onRender() {
		vbo.bind(program);
		vbo.render();
		vbo.unbind(program);
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


