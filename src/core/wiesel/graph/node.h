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
#ifndef __WIESEL_GRAPH_NODE_H__
#define	__WIESEL_GRAPH_NODE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/util/shared_object.h>
#include <wiesel/math/matrix.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>

#include <vector>


namespace wiesel {

	class Node;

	namespace video {
		class VideoDevice;
		class RenderContext;
	}


	/**
	 * @brief Alias type for a list of nodes.
	 */
	typedef std::vector<Node*>	NodeList;


	/**
	 * @brief Alias type for the node's sort key.
	 */
	typedef short int			NodeOrder;


	/**
	 * @brief Base-class for any nodes in the scene graph.
	 * Usually you won't ne use Node as a direct base class
	 * for your custom objects.
	 * Instead, you should be consider Node2D or Node3D.
	 */
	class WIESEL_CORE_EXPORT Node : public virtual SharedObject
	{
	public:
		Node();
		virtual ~Node();

	private:
		/// copy-constructor unavailable
		Node(const Node &other) {}

	// children access
	public:
		/**
		 * @brief Adds a new child.
		 * @return \c true, when the node was added, \c false otherwise.
		 */
		bool addChild(Node *child, NodeOrder order=0);

		/**
		 * @brief Adds a new child without sorting the children list.
		 * This can be useful, when adding a large amount of children at one.
		 * After adding all children, the application should call #sortChildren.
		 * @return \c true, when the node was added, \c false otherwise.
		 */
		bool addChildUnsorted(Node *child);

		/**
		 * @brief Removes the given child.
		 */
		void removeChild(Node *child);

		/**
		 * @brief Provides access to the children list.
		 * @return A const-list of all children, which cannot be manipulated.
		 */
		inline const NodeList *getChildren() {
			return &children;
		}

		/**
		 * @brief Get the node's parent.
		 */
		inline Node* getParent() {
			return parent;
		}

		/**
		 * @brief Get the node's order key.
		 */
		inline NodeOrder getOrderKey() const {
			return order;
		}
		
		/**
		 * @brief Sort all children by their sort-key.
		 */
		void sortChildren();

	// getter / setter
	public:
		/**
		 * @brief Set the visibility of this node and all of it's children.
		 */
		void setVisisble(bool visible);

		/**
		 * @brief Tells whether this node is visible or not.
		 */
		inline bool isVisible() const {
			return visible;
		}

	// matrix access and manipulation
	public:
		/**
		 * @brief Returns the local transformation matrix.
		 * The local transform matrix stores all transformations
		 * relative to it's parent.
		 */
		inline const matrix4x4& getLocalTransform() const {
			return local_transform;
		}

		/**
		 * @brief Returns the world transformation matrix.
		 * The world transform matrix stores all transformations
		 * relative to the scene root.
		 */
		inline const matrix4x4& getWorldTransform() const {
			return world_transform;
		}

		/**
		 * @brief Update the local and world transformation matrix, if necessary.
		 */
		void updateTransform();

		/**
		 * @brief Flags the transformation on this node and all it's children as dirty.
		 * This means, on the next update, the world transformation matrix will be updated.
		 */
		void setTransformDirty();

		/**
		 * @brief Check if the current transformation needs to be updated.
		 */
		bool isTransformDirty() const {
			return transform_dirty;
		}

	// coordinate conversion
	public:
		/**
		 * @brief Converts a screen-location (for example the location of a touch event) into
		 * the local coordinate system of this node.
		 */
		/*
		vector2d convertScreenToLocal(video::VideoDevice *video_device, const vector2d &screen);
		*/

		/**
		 * @brief Converts a position vector from world coordinates into
		 * the local coordinate system of this node.
		 */
		vector2d convertWorldToLocal(const vector2d &world) const;

		/**
		 * @brief Converts a position vector from world coordinates into
		 * the local coordinate system of this node.
		 */
		vector3d convertWorldToLocal(const vector3d &world) const;

	// public functions
	public:
		/**
		 * @brief Render this node including all it's children.
		 */
		virtual void render(video::RenderContext *render_context);

	// overridables
	protected:
		/**
		 * Compute the transformations relative to the node's parent.
		 * Can be overridden in subclasses.
		 */
		virtual void computeLocalTransform(matrix4x4 *transform);

		/**
		 * @brief Called to render this node.
		 * This function should be the only place to put rendering code.
		 */
		virtual void onDraw(video::RenderContext *render_context);

	// private functions
	private:
		/**
		 * @brief Render the current node.
		 */
		void render_this(video::RenderContext *render_context);

	// members available for subclasses
	protected:
		matrix4x4	local_transform;	//!< Local transformation, relative to it's parent.
		matrix4x4	world_transform;	//!< World transformation, relative to the scene.
		bool		transform_dirty;	//!< When true, the world transformation matrix has to be updated.

	private:
		bool		visible;

		/// the parent node of this one.
		Node*		parent;

		/// the sort order key
		NodeOrder	order;

		/// the list containing all children of this node.
		NodeList	children;
	};

}

#endif	/* __WIESEL_GRAPH_NODE_H__ */

