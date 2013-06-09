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
#ifndef __WIESEL_GRAPH_LIGHTING_LIGHTING_MANAGER_H__
#define __WIESEL_GRAPH_LIGHTING_LIGHTING_MANAGER_H__

#include <wiesel/graph/node.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/wiesel-core.def>

#include "wiesel/video/shader_constantbuffer.h"
#include "wiesel/video/shader_target.h"

#include "light_source.h"


namespace wiesel {

	/**
	 * @brief The LightingManager is a helper class to manage light sources
	 * and lighting targets.
	 * The LightingManager needs to be part of the scene graph, either as a
	 * parent of all targets and lights, or a sibling within the tree. When
	 * added as a sibling, it should have a high priority in order to get
	 * invoked before rendering the targets itself.
	 */
	class WIESEL_CORE_EXPORT LightingManager :
			public Node,
			public video::ShaderConstantBufferWriter
	{
	friend class LightingManagerBuilder;

	private:
		LightingManager();

	public:
		virtual ~LightingManager();

	// managing lights
	public:
		/**
		 * @brief Adds a new \ref LightSource to this lighting manager.
		 */
		void addLightSource(LightSource *light);

		/**
		 * @brief Removes a light source from this manager.
		 */
		void removeLightSource(LightSource *light);

		/**
		 * @brief Removes all light sources from this manager.
		 */
		void removeAllLightSources();

	// managing objects
	public:
		/**
		 * @brief Adds a \ref ShaderTarget to this lighting manager, which will
		 * receive all data managed by this object.
		 */
		void addTarget(video::ShaderTarget *target);

		/**
		 * @brief Removes a shader target from this manager.
		 */
		void removeTarget(video::ShaderTarget *target);

		/**
		 * @brief Removes all targets from this manager.
		 */
		void removeAllTargets();

	// managing shaders
	public:
		/**
		 * @brief Set a shader, which should be used for all managed targets per default.
		 * When set, this shader will be set for all targets under the manager's control.
		 */
		void setDefaultLightingShader(video::Shader *shader);

		/**
		 * @brief Get the current default shader for all managed objects.
		 */
		video::Shader *getDefaultLightingShader();

		/**
		 * @brief Get the currently used shader, depending on the currently active lights.
		 * @return A shader, when a default shader was configured, or \c NULL, if not.
		 */
		video::Shader *getCurrentLightingShader();

	// properties
	public:
		/**
		 * @brief Set the maximum number of active light sources, which can be handled
		 * by the lighting manager and the according shaders at the same time.
		 */
		inline uint8_t getMaxLightSources() const {
			return max_light_sources;
		}

		/**
		 * @brief Get the name of the uniform member which contains the lights data.
		 */
		inline const std::string& getLightsUniformName() const {
			return membername_lights;
		}

		/**
		 * @brief Get the name of the uniform member which contains the number of currently active lights.
		 */
		inline const std::string& getLightsCountUniformName() const {
			return membername_lights_count;
		}

		/**
		 * @brief Get the name of the constant buffer, which contains the light-information
		 * managed by this lighting manager.
		 */
		inline const std::string& getConstantBufferName() const {
			return buffer_name;
		}

		/**
		 * @brief Get the template for constant buffers containing the light-information
		 * managed by this lighting manager.
		 */
		inline video::ShaderConstantBufferTemplate* getConstantBufferTemplate() {
			return buffer_template;
		}

		/**
		 * @brief Get the template for constant buffers containing the light-information
		 * managed by this lighting manager.
		 */
		inline const video::ShaderConstantBufferTemplate* getConstantBufferTemplate() const {
			return buffer_template;
		}

		/**
		 * @brief Get the constant buffer containing the light-information
		 * manager by this lighting manager.
		 */
		inline video::ShaderConstantBuffer* getConstantBuffer() {
			return buffer;
		}

	// ShaderConstantBufferWriter
	public:
		virtual const data_t getShaderDataPointer(const std::string &name, video::ValueType type, size_t elements) const;

	// ShaderConstantBufferWriter
	protected:
		virtual bool doSetShaderValue(const std::string &name, video::ValueType type, size_t elements, const void *pValue);


	public:
		virtual void render(video::RenderContext *render_context);

	protected:
		virtual void preRender();

	private:
		struct LightInfoMember {
			LightSource::Attribute		attribute;
			video::ValueType			type;
			std::string					name;

			std::vector<video::ShaderConstantBufferTemplate::index_t>		member_index_per_light;
		};

		typedef std::vector<LightInfoMember>	LightInfoMembers;

	private:
		uint8_t											max_light_sources;

		std::string										membername_lights_count;
		std::string										membername_lights;
		video::ShaderConstantBufferTemplate::index_t	memberindex_lights_count;
		LightInfoMembers								light_info_members;

		std::string										buffer_name;
		ref<video::ShaderConstantBufferTemplate>		buffer_template;
		ref<video::ShaderConstantBuffer>				buffer;

		ref<video::Shader>								default_lighting_shader;

		std::set<LightSource*>							managed_light_sources;
		std::set<video::ShaderTarget*>					managed_targets;
		std::set<Node2D*>								managed_2d_nodes;
	};

} // namespace wiesel

#endif // __WIESEL_GRAPH_LIGHTING_LIGHTING_MANAGER_H__
