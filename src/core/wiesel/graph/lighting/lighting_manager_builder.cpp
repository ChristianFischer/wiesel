/**
 * Copyright (C) 2013
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
#include "lighting_manager_builder.h"
#include "lighting_manager.h"

#include "wiesel/video/shader_constantbuffer_builder.h"

#include <sstream>

using namespace wiesel;
using namespace wiesel::video;


LightingManagerBuilder::LightingManagerBuilder() {
	this->manager				= new LightingManager();
	this->finished				= false;
	this->lightinfo_created		= false;
}


LightingManagerBuilder::~LightingManagerBuilder() {
	assert(this->manager == NULL);
	assert(this->finished);
	
	// delete the manager, when the builder was destructed,
	// before the construction was finished.
	if (this->manager) {
		assert(this->manager->getReferenceCount() == 0);
		delete this->manager;
		this->manager = NULL;
	}

	return;
}


void LightingManagerBuilder::setMaxLightSources(uint8_t max_light_sources) {
	assert(manager);
	if (manager) {
		manager->max_light_sources = max_light_sources;
	}

	return;
}


uint8_t LightingManagerBuilder::getMaxLightSources() const {
	if (manager) {
		return manager->getMaxLightSources();
	}

	return 0;
}


void LightingManagerBuilder::setConstantBufferName(const std::string& name) {
	assert(manager);
	if (manager) {
		manager->buffer_name = name;
	}

	return;
}


std::string LightingManagerBuilder::getConstantBufferName() const {
	if (manager) {
		return manager->getConstantBufferName();
	}

	return std::string();
}


void LightingManagerBuilder::setLightsUniformName(const std::string& name) {
	if (manager) {
		manager->membername_lights = name;
	}

	return;
}


std::string LightingManagerBuilder::getLightsUniformName() const {
	if (manager) {
		return manager->getLightsUniformName();
	}

	return std::string();
}


void LightingManagerBuilder::setLightsCountUniformName(const std::string& name) {
	if (manager) {
		manager->membername_lights_count = name;
	}

	return;
}


std::string LightingManagerBuilder::getLightsCountUniformName() const {
	if (manager) {
		return manager->getLightsCountUniformName();
	}

	return std::string();
}




void LightingManagerBuilder::addLightInfoMember(LightSource::Attribute attribute, ValueType type, const std::string& name) {
	LightInfoMember member;
	member.attribute	= attribute;
	member.type			= type;
	member.name			= name;

	lightinfo_struct.push_back(member);
}


void LightingManagerBuilder::addDefaultLightInfoMember(LightSource::Attribute attribute) {
	switch(attribute) {
		case LightSource::Attribute_Position: {
			addLightInfoMember(LightSource::Attribute_Position,			TypeVector3f,		"position"	);
			break;
		}

		case LightSource::Attribute_ColorAmbient: {
			addLightInfoMember(LightSource::Attribute_ColorAmbient,		TypeVector3f,		"ambient_color"		);
			break;
		}

		case LightSource::Attribute_ColorDiffuse: {
			addLightInfoMember(LightSource::Attribute_ColorDiffuse,		TypeVector3f,		"diffuse_color"		);
			break;
		}

		case LightSource::Attribute_ColorSpecular: {
			addLightInfoMember(LightSource::Attribute_ColorSpecular,	TypeVector3f,		"specular_color"		);
			break;
		}

		case LightSource::Attribute_Strength: {
			addLightInfoMember(LightSource::Attribute_Strength,			TypeFloat,			"strength"	);
			break;
		}
	}

	return;
}


void LightingManagerBuilder::finishLightInfoMembers() {
	assert(lightinfo_created == false);
	assert(finished == false);
	assert(manager);

	if (lightinfo_created == false && finished == false && manager) {
		assert(getLightsUniformName().empty() == false);
		if (getLightsUniformName().empty()) {
			return;
		}

		if (getLightsCountUniformName().empty() == false) {
			getConstantBufferTemplateBuilder()->addEntry(TypeInt32, 1, getLightsCountUniformName());
		}

		// fill the constant buffer template with all required members
		for(int light=0; light<getMaxLightSources(); light++) {
			for(LightInfoStruct::iterator it=lightinfo_struct.begin(); it!=lightinfo_struct.end(); it++) {
				std::stringstream ss;
				ss << getLightsUniformName();
				ss << '[' << light << ']';
				ss << '.' << it->name;

				getConstantBufferTemplateBuilder()->addEntry(it->type, 1, ss.str());
			}
		}

		// create a list containing indices for all light info members
		manager->light_info_members.reserve(lightinfo_struct.size());
		for(LightInfoStruct::iterator it=lightinfo_struct.begin(); it!=lightinfo_struct.end(); it++) {
			LightingManager::LightInfoMember info_member;
			info_member.attribute	= it->attribute;
			info_member.type		= it->type;
			info_member.name		= it->name;
			info_member.member_index_per_light.resize(getMaxLightSources());

			for(int light=0; light<getMaxLightSources(); light++) {
				std::stringstream ss;
				ss << getLightsUniformName();
				ss << '[' << light << ']';
				ss << '.' << it->name;

				ShaderConstantBufferTemplate::index_t idx;
				idx = getConstantBufferTemplateBuilder()->findShaderValueIndex(ss.str());
				assert(idx != -1);

				info_member.member_index_per_light[light] = idx;
			}

			manager->light_info_members.push_back(info_member);
		}

		// get the index of the lights count member
		if (getLightsCountUniformName().empty() == false) {
			ShaderConstantBufferTemplate::index_t idx;
			idx = getConstantBufferTemplateBuilder()->findShaderValueIndex(getLightsCountUniformName());
			manager->memberindex_lights_count = idx;
		}
		else {
			manager->memberindex_lights_count = -1;
		}

		lightinfo_created = true;
	}

	return;
}




ShaderConstantBufferTemplateBuilder* LightingManagerBuilder::getConstantBufferTemplateBuilder() {
	return &buffer_builder;
}


const ShaderConstantBufferTemplateBuilder* LightingManagerBuilder::getConstantBufferTemplateBuilder() const {
	return &buffer_builder;
}


LightingManager* LightingManagerBuilder::create() {
	assert(this->manager);

	if (this->manager) {
		if (this->lightinfo_created == false) {
			finishLightInfoMembers();
		}

		// create the constant buffer template
		manager->buffer_template = buffer_builder.create();

		// create the constant buffer based on this template
		manager->buffer = new ShaderConstantBuffer(manager->getConstantBufferTemplate());

		// initialize the buffer with zero active lights
		manager->getConstantBuffer()->setShaderValue(getLightsCountUniformName(), 0);

		// clear the object reference to the created manager
		// and set the builder's state to 'finished'
		LightingManager* _manager = this->manager;
		this->manager  = NULL;
		this->finished = true;

		return _manager;
	}

	return NULL;
}

