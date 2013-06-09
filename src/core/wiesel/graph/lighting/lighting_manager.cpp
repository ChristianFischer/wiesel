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
#include "lighting_manager.h"

using namespace wiesel;
using namespace wiesel::video;


LightingManager::LightingManager() {
	this->max_light_sources			= 4;
	this->memberindex_lights_count	= -1;
	this->buffer_template			= new ShaderConstantBufferTemplate();
}


LightingManager::~LightingManager() {
	removeAllLightSources();
	removeAllTargets();
}



void LightingManager::addLightSource(LightSource* light) {
	if (light) {
		std::set<LightSource*>::iterator it = managed_light_sources.find(light);
		if (it == managed_light_sources.end()) {
			managed_light_sources.insert(keep(light));
		}
	}

	return;
}


void LightingManager::removeLightSource(LightSource* light) {
	if (light) {
		std::set<LightSource*>::iterator it = managed_light_sources.find(light);
		if (it != managed_light_sources.end()) {
			release(*it);
			managed_light_sources.erase(it);
		}
	}

	return;
}


void LightingManager::removeAllLightSources() {
	for(std::set<LightSource*>::iterator it=managed_light_sources.begin(); it!=managed_light_sources.end(); it++) {
		release(*it);
	}

	managed_light_sources.clear();

	return;
}



void LightingManager::addTarget(ShaderTarget* target) {
	if (target) {
		{
			std::set<ShaderTarget*>::iterator it = managed_targets.find(target);
			if (it != managed_targets.end()) {
				return;
			}
		}

		managed_targets.insert(keep(target));

		Node2D *node2d = dynamic_cast<Node2D*>(target);
		if (node2d) {
			std::set<Node2D*>::iterator it = managed_2d_nodes.find(node2d);
			if (it == managed_2d_nodes.end()) {
				managed_2d_nodes.insert(keep(node2d));
			}
		}

		// assign all shader data to this target
		Shader* current_shader = getCurrentLightingShader();
		if (current_shader) {
			target->setShader(current_shader);
		}
		else {
			// when no default shader provided by the lighting manager,
			// the target must already contain a valid shader.
			assert(target->getShader() != NULL);
		}

		target->assignShaderConstantBuffer(
				getConstantBufferName(),
				getConstantBuffer()
		);
	}

	return;
}


void LightingManager::removeTarget(ShaderTarget* target) {
	if (target) {
		{
			std::set<ShaderTarget*>::iterator it = managed_targets.find(target);
			if (it != managed_targets.end()) {
				release(*it);
				managed_targets.erase(it);
			}
		}

		Node2D *node2d = dynamic_cast<Node2D*>(target);
		if (node2d) {
			std::set<Node2D*>::iterator it = managed_2d_nodes.find(node2d);
			if (it != managed_2d_nodes.end()) {
				release(*it);
				managed_2d_nodes.erase(it);
			}
		}
	}

	return;
}


void LightingManager::removeAllTargets() {
	for(std::set<ShaderTarget*>::iterator it=managed_targets.begin(); it!=managed_targets.end(); it++) {
		release(*it);
	}

	for(std::set<Node2D*>::iterator it=managed_2d_nodes.begin(); it!=managed_2d_nodes.end(); it++) {
		release(*it);
	}

	managed_targets.clear();
	managed_2d_nodes.clear();

	return;
}



void LightingManager::setDefaultLightingShader(Shader* shader) {
	if (shader) {
		// chec, if the shader contains the correct constant buffer
		assert(shader->findConstantBufferTemplate(getConstantBufferName()) == getConstantBufferTemplate());

		this->default_lighting_shader = shader;
	}

	return;
}


Shader* LightingManager::getDefaultLightingShader() {
	return default_lighting_shader;
}


Shader* LightingManager::getCurrentLightingShader() {
	if (default_lighting_shader) {
		return default_lighting_shader;
	}

	return NULL;
}



const ShaderConstantBufferWriter::data_t LightingManager::getShaderDataPointer(const std::string &name, ValueType type, size_t elements) const {
	return buffer->getShaderDataPointer(name, type, elements);
}


bool LightingManager::doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue) {
	return buffer->setShaderValue(name, type, elements, pValue);
}



void LightingManager::render(RenderContext *render_context) {
	preRender();

	Node::render(render_context);

	return;
}


void LightingManager::preRender() {
	int current_light_index = 0;

	for(
			std::set<LightSource*>::iterator
			light_it=managed_light_sources.begin();
			light_it!=managed_light_sources.end();
			light_it++
	) {
		LightSource *light = *light_it;

		if (light->isLightEnabled() == false) {
			continue;
		}

		// set all light members
		for(LightInfoMembers::iterator it=light_info_members.begin(); it!=light_info_members.end(); it++) {
			assert(current_light_index < static_cast<int>(it->member_index_per_light.size()));
			ShaderConstantBufferTemplate::index_t var_index = it->member_index_per_light[current_light_index];

			switch(it->attribute) {
				case LightSource::Attribute_Position: {
					const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(var_index);
					vector3d world_location = light->getWorldLocation();

					if (world_location != *(reinterpret_cast<const vector3d*>(data))) {
						buffer->setShaderValueAt(var_index, world_location);
					}

					break;
				}

				case LightSource::Attribute_ColorAmbient: {
					const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(var_index);

					if (light->getLightColorAmbient() != *(reinterpret_cast<const vector3d*>(data))) {
						buffer->setShaderValueAt(var_index, light->getLightColorAmbient());
					}

					break;
				}

				case LightSource::Attribute_ColorDiffuse: {
					const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(var_index);

					if (light->getLightColorDiffuse() != *(reinterpret_cast<const vector3d*>(data))) {
						buffer->setShaderValueAt(var_index, light->getLightColorDiffuse());
					}

					break;
				}

				case LightSource::Attribute_ColorSpecular: {
					const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(var_index);

					if (light->getLightColorSpecular() != *(reinterpret_cast<const vector3d*>(data))) {
						buffer->setShaderValueAt(var_index, light->getLightColorSpecular());
					}

					break;
				}

				case LightSource::Attribute_Strength: {
					const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(var_index);

					if (light->getLightStrength() != *(reinterpret_cast<const float*>(data))) {
						buffer->setShaderValueAt(var_index, light->getLightStrength());
					}

					break;
				}
			}
		}

		++current_light_index;
	}

	// update the number of active lights
	if (memberindex_lights_count != -1) {
		const ShaderConstantBuffer::data_t data = buffer->getShaderDataPointerAt(memberindex_lights_count);

		if (current_light_index != *(reinterpret_cast<const int*>(data))) {
			buffer->setShaderValueAt(memberindex_lights_count, current_light_index);
		}
	}

	return;
}
