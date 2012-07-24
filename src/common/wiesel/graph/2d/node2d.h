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
#ifndef __WIESEL_GRAPH_2D_NODE2D_H__
#define	__WIESEL_GRAPH_2D_NODE2D_H__

#include "wiesel/graph/node.h"
#include "wiesel/math/vector2d.h"
#include "wiesel/geometry.h"


namespace wiesel {

	class Node2D : public Node
	{
	public:
		Node2D();
		virtual ~Node2D();

	// getter/setter
	public:
		/// set the content size of this node
		void setContentSize(const dimension &size);

		/// set the content size of this node
		void setContentSize(float width, float height);

		/// set the pivot point of this node.
		/// usually this value would be between 0,0 (bottom left) and 1,1 (top right).
		void setPivot(const vector2d &pivot);

		/// set the pivot point of this node.
		/// usually this value would be between 0,0 (bottom left) and 1,1 (top right).
		void setPivot(float x, float y);

		/// get the content size of this node.
		inline const dimension& getContentSize() const {
			return bounds.size;
		}

		/// get the pivot point of this node.
		/// usually this value would be between 0,0 (bottom left) and 1,1 (top right).
		inline const vector2d& getPivot() const {
			return pivot;
		}

		/// get the bounding rect of this node.
		inline const rect& getBounds() const {
			return bounds;
		}

	protected:
		/// update the bounding rect after content size or pivot point has changed.
		void updateBounds();

	// transformation setter/getter
	public:
		/// set the position of this node, relative to it's parent.
		void setPosition(const vector2d &v);

		/// set the position of this node.
		void setPosition(float x, float y);

		/// set the rotation of this node.
		/// unless other transformation methods,
		/// this function will take values in degrees
		void setRotation(float a);

		/// set the scale value of this node.
		void setScale(float s);

		/// set the scale value of this node.
		void setScaleX(float sx);

		/// set the scale value of this node.
		void setScaleY(float sy);

		/// get the position of this node, relative to it's parent.
		inline const vector2d& getPosition() const {
			return position;
		}

		/// get the rotation of this node.
		inline float getRotation() const {
			return rotation;
		}

		/// get the scale value of this node.
		inline float getScaleX() const {
			return scale_x;
		}

		/// get the scale value of this node.
		inline float getScaleY() const {
			return scale_y;
		}

		/// get the scale value of this node.
		inline float getScale() const {
			return scale_x;
		}

		/// update the transform matrices
		void updateTransform();


	public:
		/// Tests, if a point is within this node.
		/// The point should already be transformed into the node's coordinate system.
		virtual bool hitBy(const vector2d &local) const;


	// overridables
	protected:


	private:
		rect		bounds;
		vector2d	pivot;

		vector2d	position;
		float		rotation;
		float		scale_x;
		float		scale_y;
	};

}

#endif	/* __WIESEL_GRAPH_2D_NODE2D_H__ */

