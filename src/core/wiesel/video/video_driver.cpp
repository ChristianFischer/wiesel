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
#include "video_driver.h"
#include "video_loader.h"
#include "render_context.h"

#include "wiesel/module_registry.h"

#include <iomanip>

using namespace wiesel;
using namespace wiesel::video;


VideoInfo::VideoInfo() {
	textures.max_size			= 0;
	textures.max_texture_units	= 0;
	textures.requires_pot		= true;

	return;
}


VideoInfo::~VideoInfo() {
	return;
}



std::ostream& wiesel::video::operator <<(std::ostream &o, const VideoInfo &video_info) {
	const int align = 16;

	o
			<< std::left
			<< std::setw(align) << "API:"
				<< video_info.api << " " << video_info.api_version << std::endl
			<< std::setw(align) << "Vendor:"
				<< video_info.api_vendor << std::endl
			<< std::setw(align) << "Renderer:"
				<< video_info.renderer << std::endl
			<< "[Shaders]" << std::endl
			<< std::setw(align) << " API:"
				<< video_info.shaders.api << " " << video_info.shaders.api_version << std::endl
			<< "[Textures]" << std::endl
			<< std::setw(align) << " max size:"
				<< video_info.textures.max_size << std::endl
			<< std::setw(align) << " max units:"
				<< video_info.textures.max_texture_units << std::endl
			<< std::setw(align) << " pot required:"
				<< (video_info.textures.requires_pot ? "yes" : "no") << std::endl
	;

	// print extensions
	/*
	o << "[Extensions]" << std::endl;
	for(std::vector<std::string>::const_iterator it=video_info.extensions.begin(); it!=video_info.extensions.end(); it++) {
		o << " - " << (*it) << std::endl;
	}
	*/

	return o;
}





VideoDeviceDriver::VideoDeviceDriver(Screen *screen) : DeviceDriver(screen) {
	this->projection = matrix4x4::identity;
	this->screen     = screen;
	return;
}

VideoDeviceDriver::~VideoDeviceDriver() {
	return;
}


void VideoDeviceDriver::setState(VideoState state) {
	if (this->state != state) {
		this->state  = state;
	}

	return;
}


void VideoDeviceDriver::updateScreenSize(float w, float h) {
	size = dimension(w, h);
	projection = matrix4x4::ortho(0, w, 0, h, 0, 1000);
	return;
}

