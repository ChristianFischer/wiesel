/*
 * application.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_APPLICATION_H__
#define __WIESEL_APPLICATION_H__

namespace wiesel {

	/**
	 * @brief An abstract class implementing the application logic.
	 */
	class Application {
	public:
		Application();
		virtual ~Application();

		/**
		 * @brief called each frame to process application data.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRun() = 0;

		/**
		 * @brief called each frame to render graphics.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRender() = 0;
	};

}

#endif /* __WIESEL_APPLICATION_H__ */
