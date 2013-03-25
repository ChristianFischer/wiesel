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
#include "dx11_video_driver.h"
#include "dx11_video_loader.h"
#include "wiesel/engine.h"

#include "dx11_indexbuffer_content.h"
#include "dx11_vertexbuffer_content.h"
#include "dx11_shader_content.h"
#include "dx11_texture_content.h"

#include <wiesel/util/log.h>
#include <wiesel/ui/touchhandler.h>
#include <assert.h>

#include <algorithm>


using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


#define WND_CLASS_NAME		L"wiesel"


Dx11VideoDeviceDriver::Dx11VideoDeviceDriver(Screen *screen) : VideoDeviceDriver(screen) {
	render_context	= NULL;
	hInstance		= NULL;
	hWnd			= NULL;

	return;
}


Dx11VideoDeviceDriver::~Dx11VideoDeviceDriver() {
	Engine::getInstance()->unregisterUpdateable(this);

	if (render_context) {
		render_context->release();
		render_context = NULL;
	}

	return;
}


bool Dx11VideoDeviceDriver::init(const dimension &size, unsigned int flags) {
	hInstance = GetModuleHandle(0);

	if (!initWindow(size, SW_SHOW)) {
		return false;
	}

	// try to initialize the OpenGL part
	if (!initDirectX()) {
		return false;
	}

	// update screen size and projection
	updateScreenSize(size.width, size.height);

	// register for updates
	Engine::getInstance()->registerUpdateable(this);

	// video device is ready
	setState(Video_Active);

	return true;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc;

			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);

			break;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}


bool Dx11VideoDeviceDriver::initWindow(const dimension &size, int nCmdShow) {
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"wiesel";
	wcex.hIconSm		= 0;

	if (!RegisterClassEx(&wcex)) {
		return false;
	}

	// Create window
	RECT rc = { 0, 0, size.width, size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindow(
					WND_CLASS_NAME,
					L"Direct3D 11",
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					rc.right  - rc.left,
					rc.bottom - rc.top,
					NULL,
					NULL,
					hInstance,
					NULL
	);

	if (!hWnd) {
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return true;
}


bool Dx11VideoDeviceDriver::initDirectX() {
	// remove the old context, if any
	if (render_context) {
		render_context->release();
		render_context = NULL;
	}

	// initialize the new render context
	render_context = DirectX11RenderContext::createContextWithWindowHandle(hWnd, getScreen(), &info);
	if (render_context) {
		render_context->initContext();
		render_context->retain();
	}
	else {
		return false;
	}

	return true;
}


void Dx11VideoDeviceDriver::updateScreenSize(float w, float h) {
	VideoDeviceDriver::updateScreenSize(w, h);
	return;
}


bool Dx11VideoDeviceDriver::shutdown() {
	// Remove the window.
	if (hWnd) {
		DestroyWindow(hWnd);
		hWnd = NULL;
	}

	// Remove the application instance.
	UnregisterClass(WND_CLASS_NAME, hInstance);
	hInstance = NULL;	
	
	return true;
}


void Dx11VideoDeviceDriver::update(float dt) {
	if (hWnd) {
		MSG msg = {0};

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch(msg.message) {
				case WM_QUIT: {
					Engine::getInstance()->requestExit();
					break;
				}
			}
		}
	}

	return;
}


vector2d Dx11VideoDeviceDriver::convertScreenToWorld(const vector2d &screen) const {
	// at first, convert into OpenGL coordinate space
	vector2d transformed = vector2d(
			+(screen.x / getResolution().width  - 0.5f) * 2,
			-(screen.y / getResolution().height - 0.5f) * 2
	);

	// then use the projection- and modelview matrix to get the world coordinate system
	return
			transformed
		/	getProjectionMatrix()
	;

}


void Dx11VideoDeviceDriver::preRender() {
	if (render_context) {
		render_context->preRender();
		render_context->setProjectionMatrix(projection);
	}

	return;
}


void Dx11VideoDeviceDriver::postRender() {
	if (render_context) {
		render_context->postRender();
	}

	return;
}



RenderContext *Dx11VideoDeviceDriver::getCurrentRenderContext() {
	return render_context;
}


IndexBufferContent *Dx11VideoDeviceDriver::createIndexBufferContent(IndexBuffer *index_buffer) {
	return Dx11IndexBufferContent::createContentFor(this->render_context, index_buffer);
}

VertexBufferContent *Dx11VideoDeviceDriver::createVertexBufferContent(VertexBuffer *vertex_buffer) {
	return Dx11VertexBufferContent::createContentFor(this->render_context, vertex_buffer);
}

ShaderContent *Dx11VideoDeviceDriver::createShaderContent(Shader *shader) {
	return Dx11ShaderContent::createContentFor(this->render_context, shader);
}

TextureContent *Dx11VideoDeviceDriver::createTextureContent(Texture *texture) {
	return Dx11TextureContent::createContentFor(this->render_context, texture);
}

