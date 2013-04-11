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
#include "render_context.h"
#include "shaders.h"

using namespace wiesel;
using namespace wiesel::video;



RenderContext::RenderContext() {
	this->screen = NULL;
	return;
}


RenderContext::RenderContext(Screen *screen) {
	this->screen = screen;

	// create default constant buffers
	ShaderConstantBufferTemplate*	cb_tpl_modelview;
	cb_tpl_modelview				= Shaders::instance()->getModelviewMatrixBufferTemplate();
	this->cb_modelview				= new ShaderConstantBuffer(cb_tpl_modelview);
	this->cb_modelview->retain();

	ShaderConstantBufferTemplate*	cb_tpl_projection;
	cb_tpl_projection				= Shaders::instance()->getProjectionMatrixBufferTemplate();
	this->cb_projection				= new ShaderConstantBuffer(cb_tpl_projection);
	this->cb_projection->retain();

	return;
}


RenderContext::~RenderContext() {
	safe_release(cb_modelview);
	safe_release(cb_projection);
	return;
}
