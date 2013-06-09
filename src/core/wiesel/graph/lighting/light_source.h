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
#ifndef __WIESEL_GRAPH_LIGHTING_LIGHT_SOURCE_H__
#define __WIESEL_GRAPH_LIGHTING_LIGHT_SOURCE_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/math/vector3d.h>
#include <wiesel/util/shared_object.h>


namespace wiesel {

	/**
	 * @brief Abstract base class for any lightsources, which should be handled
	 * by the \ref LightingManager.
	 */
	class WIESEL_CORE_EXPORT LightSource : public virtual SharedObject
	{
	public:
		/**
		 * @brief An enumeration of various light attributes.
		 */
		enum Attribute {
			Attribute_Position,
			Attribute_ColorAmbient,
			Attribute_ColorDiffuse,
			Attribute_ColorSpecular,
			Attribute_Strength,
		};


	public:
		LightSource();
		virtual ~LightSource();

	// properties
	public:
		/**
		 * @brief Set whether this light is enabled or not.
		 */
		void setLightEnabled(bool enabled);

		/**
		 * @brief Get whether this light is enabled or not.
		 */
		inline bool isLightEnabled() const {
			return enabled;
		}


		/**
		 * @brief Set the light's ambient color.
		 */
		void setLightColorAmbient(float r, float g, float b);

		/**
		 * @brief Set the light's ambient color.
		 */
		void setLightColorAmbient(const vector3d& color);

		/**
		 * @brief Get the light's ambient color.
		 */
		inline const vector3d& getLightColorAmbient() const {
			return color_ambient;
		}


		/**
		 * @brief Set the light's diffuse color.
		 */
		void setLightColorDiffuse(float r, float g, float b);

		/**
		 * @brief Set the light's diffuse color.
		 */
		void setLightColorDiffuse(const vector3d& color);

		/**
		 * @brief Get the light's diffuse color.
		 */
		inline const vector3d& getLightColorDiffuse() const {
			return color_diffuse;
		}


		/**
		 * @brief Set the light's specular color.
		 */
		void setLightColorSpecular(float r, float g, float b);

		/**
		 * @brief Set the light's specular color.
		 */
		void setLightColorSpecular(const vector3d& color);

		/**
		 * @brief Get the light's specular color.
		 */
		inline const vector3d& getLightColorSpecular() const {
			return color_specular;
		}


		/**
		 * @brief Set the light's strength.
		 */
		void setLightStrength(float strength);

		/**
		 * @brief Get the light's strength.
		 */
		inline float getLightStrength() const {
			return strength;
		}

	// overridables
	public:
		/**
		 * @brief Computes the location of this light source in world space.
		 */
		virtual vector3d getWorldLocation() = 0;

	private:
		bool		enabled;
		vector3d	color_ambient;
		vector3d	color_diffuse;
		vector3d	color_specular;
		float		strength;
	};

} // namespace wiesel

#endif // __WIESEL_GRAPH_LIGHTING_LIGHT_SOURCE_H__
