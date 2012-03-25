#ifndef __WIESEL_GEOMETRY_H__
#define __WIESEL_GEOMETRY_H__


namespace wiesel {

	struct dimension
	{
	// constructors
	public:
		/// constructs a new dimension object with zero width and height
		dimension();
		
		/// constructs a new dimension object with width and height
		dimension(float width, float height);
		
		/// copy-constructor
		dimension(const dimension &other);
		
	// operations
	public:
		/// scale width and height with factor \c s
		void scale(float s);
		
		/// scale width and height with separate factors
		void scale(float sx, float sy);

	// members
	public:
		float width;
		float height;
	};
	
	
	inline dimension operator*(const dimension &dim, float s) {
		return dimension(dim.width * s, dim.height * s);
	}
	
	inline dimension& operator*=(dimension &dim, float s) {
		dim.width  *= s;
		dim.height *= s;
		return dim;
	}

}

#endif
