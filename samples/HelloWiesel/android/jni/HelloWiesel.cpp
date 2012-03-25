
#include <wiesel/application_main.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

using namespace wiesel;



class HelloWiesel
: public wiesel::Application
{
public:
	HelloWiesel() {}
	virtual ~HelloWiesel() {}

public:
	virtual void onRun() {
		return;
	}

	virtual void onRender() {
		return;
	}
};



// define the application entry point
WIESEL_APPLICATION_SETUP( HelloWiesel );


