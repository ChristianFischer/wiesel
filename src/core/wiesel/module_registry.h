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
#ifndef __WIESEL_MODULE_REGISTRY_H__
#define __WIESEL_MODULE_REGISTRY_H__

#include "module.h"

#include <algorithm>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>


#include <wiesel/util/log.h>
#include <ios>


/**
 * @brief Registers a new module class.
 * Each time the module is requested, a new instance will be created.
 * @param interface_class		Class of the interface, the new module belongs to.
 * @param implementation_class	Class which implements the intterface above.
 * @param factory				Function-pointer to a function, which creates an instance of the implementation_class.
 *								The factory may return \c NULL, when a module cannot be created.
 * @param api					(string) name of the module-API (for example: OpenGL, DirectX, ...)
 *								When multiple modules with the same API are registered, they will be ordered by their API version.
 * @param api_version			Version of the API as unsigned hex-number. For example, version 7.1.25 could be 0x07012500.
 * @param priotiry				An priority value for ordering the modules.
 *								Default values are: \ref IModuleLoader::PriorityNull, \ref IModuleLoader::PriorityLow,
 *								\ref IModuleLoader::PriorityNormal, \ref IModuleLoader::PriorityHigh.
 */
#define REGISTER_MODULE(interface_class, implementation_class, factory, api, api_version, priotiry) \
	::wiesel::ModuleLoaderImpl<interface_class, implementation_class> implementation_class##_Loader(factory, api, api_version, priotiry);

/**
 * @brief Registers a new module class.
 * Each time the module is requested, a new instance will be created.
 * @param interface_class		Class of the interface, the new module belongs to.
 * @param implementation_class	Class which implements the intterface above.
 * @param factory				Function-pointer to a function, which creates an instance of the implementation_class.
 *								The factory may return \c NULL, when a module cannot be created.
 * @param api					(string) name of the module-API (for example: OpenGL, DirectX, ...)
 *								When multiple modules with the same API are registered, they will be ordered by their API version.
 * @param api_version			Version of the API as unsigned hex-number. For example, version 7.1.25 could be 0x07012500.
 * @param priotiry				An priority value for ordering the modules.
 *								Default values are: \ref IModuleLoader::PriorityNull, \ref IModuleLoader::PriorityLow,
 *								\ref IModuleLoader::PriorityNormal, \ref IModuleLoader::PriorityHigh.
 */
#define REGISTER_MODULE_SINGLETON(interface_class, implementation_class, factory, api, api_version, priotiry) \
	::wiesel::ModuleLoaderSingletonImpl<interface_class, implementation_class> implementation_class##_Loader(factory, api, api_version, priotiry);


namespace wiesel {


	class WIESEL_CORE_EXPORT IModuleLoader;

	template <class INTERFACE_CLASS>
	class ModuleLoader;

	bool WIESEL_CORE_EXPORT SortModuleLoadersPredicate(IModuleLoader *a, IModuleLoader *b);



	/**
	 * @brief A generic template function to create modules.
	 * Can be used as generator function when registering a module class.
	 */
	template <class MODULE>
	MODULE* generic_create() {
		return new MODULE();
	}



	/**
	 * @brief The module registry remembers all modules, which are known in the current engine's context.
	 * It's possible to register new modules via \ref REGISTER_MODULE and query for implementations of
	 * a specific interface.
	 */
	class WIESEL_CORE_EXPORT ModuleRegistry
	{
	private:
		ModuleRegistry() {}
		~ModuleRegistry() {}

	public:
		static ModuleRegistry *getInstance();

	public:
		/**
		 * @brief Registering a new module implementation.
		 * @param loader	A loader object, which loads an implementation for a specific interface.
		 *					The loader will automatically be registered for the loader's interface type.
		 */
		template <class INTERFACE_CLASS>
		void registerModule(ModuleLoader<INTERFACE_CLASS> *loader) {
			module_loader_registry.insert(std::make_pair(
							std::string(typeid(INTERFACE_CLASS).name()),
							static_cast<IModuleLoader*>(loader)
			));
		}

		/**
		 * @brief Removes a module loader from the registry.
		 * The given loader will be removed from the whole registry,
		 * independent of which, or how many, interface(s) it was registered to.
		 */
		void unregisterModule(IModuleLoader *loader) {
			std::multimap<std::string, IModuleLoader*>::iterator it=module_loader_registry.begin();
			while(it != module_loader_registry.end()) {
				std::multimap<std::string, IModuleLoader*>::iterator current = it++;
				if (current->second == loader) {
					module_loader_registry.erase(current);
				}
			}

			return;
		}

		/**
		 * @brief Finds all module implemenations for a specific interface class.
		 * The resulting list will be sorted by priority and API version.
		 */
		template <class INTERFACE_CLASS>
		std::vector<ModuleLoader<INTERFACE_CLASS>*> findModules() const {
			std::vector<ModuleLoader<INTERFACE_CLASS>*> loaders;

			// find all loaders and put them into our vector
			std::multimap<std::string, IModuleLoader*>::const_iterator it;
			std::string if_name = std::string(typeid(INTERFACE_CLASS).name());
			for(it=module_loader_registry.find(if_name); it!=module_loader_registry.end(); it++) {
				ModuleLoader<INTERFACE_CLASS> *loader = dynamic_cast<ModuleLoader<INTERFACE_CLASS>*>(it->second);
				if (loader) {
					loaders.push_back(loader);
				}
			}

			// sort the list by priority and API version.
			std::sort(loaders.begin(), loaders.end(), &SortModuleLoadersPredicate);

			return loaders;
		}

		/**
		 * @brief Find the first registered interface implementation.
		 * The result will be the first implementation after sorting by priority and API version.
		 * The result may be \c NULL, if no implementation was found.
		 * @return The first valid interface implementaion or \c NULL, if no module is available.
		 */
		template <class INTERFACE_CLASS>
		ModuleLoader<INTERFACE_CLASS>* findFirst() const {
			std::vector<ModuleLoader<INTERFACE_CLASS>*> loaders = findModules<INTERFACE_CLASS>();
			return loaders.empty() ? NULL : loaders[0];
		}

		/**
		 * @brief Finds the first registered interface implementation and create amn instance of it.
		 * The result will be a new instance of the first implementation class in a list ordered by priority and API version.
		 * The result may be \c NULL, if no implementation was found.
		 * @return An instance of the first valid interface implementation or \c NULL, if no module is available.
		 */
		template <class INTERFACE_CLASS>
		INTERFACE_CLASS* createFirst() const {
			std::vector<ModuleLoader<INTERFACE_CLASS>*> loaders = findModules<INTERFACE_CLASS>();
			for(typename std::vector<ModuleLoader<INTERFACE_CLASS>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
				INTERFACE_CLASS *module = (*it)->create();
				if (module) {
					return module;
				}
			}

			return NULL;
		}

	private:
		std::multimap<std::string, IModuleLoader*>	module_loader_registry;
	};





	/**
	 * @brief Basic interface for module loaders with no concrete type.
	 */
	class WIESEL_CORE_EXPORT IModuleLoader
	{
	public:
		/**
		 * @brief Determines the version of the module's API.
		 * Should be defined in hexadecimal form like 0x03020800u for version 3.2.8
		 */
		typedef unsigned int	ApiVersion;

		/**
		 * @brief A priority value to configure how important a specigic module would be.
		 * Default value for normal priority would be \ref PriorityNormal.
		 */
		typedef unsigned short	Priority;

		enum {
			PriorityNull	=     0,	//!< Special priority value for fallback modules, which does nothing.
			PriorityLow		=    10,	//!< Modules with priority lower than default modules.
			PriorityNormal	=   100,	//!< Default modules.
			PriorityHigh	=  1000,	//!< Priority for modules, which should be preferred.
		};

	private:
		IModuleLoader() {}

	protected:
		IModuleLoader(const std::string &api, ApiVersion version, Priority priority) :
			api(api),
			version(version),
			priority(priority)
		{ }

		virtual ~IModuleLoader() {}

	public:
		/**
		 * @brief Get the API name, which is implemented by this module.
		 */
		inline const std::string& getApi() const {
			return api;
		}

		/**
		 * @brief Get the version number of the module's API implementation.
		 * Modules with the same API will be ordered by their version number.
		 * The version number will be displayed as hexadecimal numeric value,
		 * for example version number 3.1.17 could be 0x03011700u.
		 */
		inline ApiVersion getApiVersion() const {
			return version;
		}

		/**
		 * @brief Get the priority value of this module.
		 * Modules with a higher priority value should be preferred.
		 */
		inline Priority getPriority() const {
			return priority;
		}

	private:
		std::string		api;
		ApiVersion		version;
		Priority		priority;
	};



	/**
	 * @brief A class for loading a module of a specific interface.
	 */
	template <class INTERFACE_CLASS>
	class ModuleLoader : public IModuleLoader
	{
	protected:
		/**
		 * @brief Initialize the module loader.
		 */
		ModuleLoader(const std::string &api, ApiVersion version, Priority priority) :
		IModuleLoader(api, version, priority) {
			ModuleRegistry::getInstance()->registerModule<INTERFACE_CLASS>(this);
			return;
		}

		virtual ~ModuleLoader() {
			ModuleRegistry::getInstance()->unregisterModule(this);
		}

	public:
		/**
		 * @brief Creates the instance of the module interface class.
		 * This may be a shared instance.
		 */
		virtual INTERFACE_CLASS* create() = 0;
	};



	/**
	 * @brief Implements the module loader for a concrete implementation class.
	 * This class would be usually be instanciated via macro \ref REGISTER_MODULE.
	 */
	template <class INTERFACE_CLASS, class IMPLEMENTATION_CLASS>
	class ModuleLoaderImpl : public ModuleLoader<INTERFACE_CLASS>
	{
	public:
		/**
		 * @brief Alias type for the factory function, which creates the concrete instance.
		 */
		typedef IMPLEMENTATION_CLASS* (*Factory)();

		/**
		 * @brief Creates the module loader object.
		 * The loader will be automatically registered into module registry.
		 * @see REGISTER_MODULE
		 */
		ModuleLoaderImpl<INTERFACE_CLASS,IMPLEMENTATION_CLASS>(Factory factory, const std::string &api, unsigned int version, unsigned short priority=IModuleLoader::PriorityNormal) :
		ModuleLoader<INTERFACE_CLASS>(api, version, priority) {
			this->module_factory = factory;
			return;
		}

		virtual ~ModuleLoaderImpl() {
			return;
		}

	public:
		/**
		 * @brief Creates the instance of the module interface class.
		 * Each call creates a new instance.
		 */
		virtual INTERFACE_CLASS* create() {
			return (*module_factory)();
		}

	private:
		Factory module_factory;
	};



	/**
	 * @brief Implements the module loader for a concrete implementation class.
	 * This class would be usually be instanciated via macro \ref REGISTER_MODULE.
	 */
	template <class INTERFACE_CLASS, class IMPLEMENTATION_CLASS>
	class ModuleLoaderSingletonImpl : public ModuleLoader<INTERFACE_CLASS>
	{
	public:
		/**
		 * @brief Alias type for the factory function, which creates the concrete instance.
		 */
		typedef IMPLEMENTATION_CLASS* (*Factory)();

		/**
		 * @brief Creates the module loader object.
		 * The loader will be automatically registered into module registry.
		 * @see REGISTER_MODULE
		 */
		ModuleLoaderSingletonImpl(Factory factory, const std::string &api, IModuleLoader::ApiVersion version, IModuleLoader::Priority priority=IModuleLoader::PriorityNormal) :
		ModuleLoader<INTERFACE_CLASS>(api, version, priority) {
			this->module_factory = factory;
			this->instance       = NULL;
			return;
		}

		virtual ~ModuleLoaderSingletonImpl() {
			release();
			return;
		}

	public:
		/**
		 * @brief Creates the instance of the module interface class.
		 * The instance will be shared and will be released on application end.
		 */
		virtual INTERFACE_CLASS* create() {
			if (instance == NULL) {
				instance = (*module_factory)();
				keep(instance);
			}

			return instance;
		}

		/**
		 * @brief Release the instance, if any.
		 * If the instance is retained by other parts of the application, the object will not be destroyed,
		 * but the module loader will create a new instance next time calling \ref create().
		 */
		virtual void release() {
			safe_release(instance);
		}

	private:
		IMPLEMENTATION_CLASS*	instance;
		Factory					module_factory;
	};
}

#endif /* __WIESEL_MODULE_REGISTRY_H__ */
