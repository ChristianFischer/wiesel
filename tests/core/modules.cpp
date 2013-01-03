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
#include "gtest/gtest.h"

#include <wiesel/module_registry.h>


using namespace wiesel;



class ITestModule : public Module
{
public:
	ITestModule() {
		return;
	}

	virtual ~ITestModule() {
		return;
	}

public:
	virtual int getTestValue() = 0;
};




class TestMod17 : public ITestModule
{
public:
	TestMod17() {
		return;
	}

	virtual ~TestMod17() {
		return;
	}

	static TestMod17* create() {
		return new TestMod17();
	}

	virtual int getTestValue() {
		return 17;
	}
};


class TestMod23 : public ITestModule
{
public:
	TestMod23() {
		return;
	}

	virtual ~TestMod23() {
		return;
	}

	static TestMod23* create() {
		return new TestMod23();
	}

	virtual int getTestValue() {
		return 23;
	}
};


class TestMod42 : public ITestModule
{
public:
	TestMod42() {
		return;
	}

	virtual ~TestMod42() {
		return;
	}

	static TestMod42* create() {
		return new TestMod42();
	}

	virtual int getTestValue() {
		return 42;
	}
};




class ITrackingModule : public Module
{
};


class TrackingModuleImpl : public ITrackingModule
{
public:
	static TrackingModuleImpl *create() {
		return new TrackingModuleImpl();
	}

	TrackingModuleImpl() {
		++construction_count;
	}

	virtual ~TrackingModuleImpl() {
		++destruction_count;
	}

public:
	static int construction_count;
	static int destruction_count;
};

int TrackingModuleImpl::construction_count	= 0;
int TrackingModuleImpl::destruction_count	= 0;





/**
 * Test if the module loaders work.
 */
TEST(ModuleApi, ModuleLoaders) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITestModule>*> TestModList;

	// there shouldn't be any module in the registry
	TestModList list1 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list1.empty());

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE(ITestModule, TestMod17, &TestMod17::create, "TestMod", 0x01000000u, IModuleLoader::PriorityNormal);

		// now we should have exactly one module registered
		TestModList list2 = ModuleRegistry::getInstance()->findModules<ITestModule>();
		ASSERT_EQ(1u, list2.size());

		// get the module loader
		ModuleLoader<ITestModule> *test_loader = list2[0];

		// get the module
		ITestModule *testmod1 = test_loader->create();
		ASSERT_TRUE(NULL != testmod1);

		// compare the result of the module
		ASSERT_EQ(17, testmod1->getTestValue());
	}

	// after we left the scope, the registry should be empty
	TestModList list3 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list3.empty());

	return;
}


TEST(ModuleApi, ModuleConstruction) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITrackingModule>*> TrackingModList;

	// reset vars
	TrackingModuleImpl::construction_count	= 0;
	TrackingModuleImpl::destruction_count	= 0;

	ITrackingModule *module1 = NULL;
	ITrackingModule *module2 = NULL;

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE(ITrackingModule, TrackingModuleImpl, &TrackingModuleImpl::create, "TrackingModule", 0x01000000u, IModuleLoader::PriorityNormal);

		// no module should be created at this moment
		ASSERT_EQ(0, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// get the module loader
		TrackingModList list = ModuleRegistry::getInstance()->findModules<ITrackingModule>();
		ASSERT_EQ(1u, list.size());

		// there should still no module instance
		ASSERT_EQ(0, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// now create the first module...
		module1 = list[0]->create();
		module1->retain();

		// one creation, but no destruction.
		ASSERT_EQ(1, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// create the second module
		module2 = list[0]->create();
		module2->retain();

		// two creations, but still no destruction
		ASSERT_EQ(2, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// release the first module
		module1->release();
		module1 = NULL;

		// the first one should be destructed, but the first one should be still alive.
		ASSERT_EQ(2, TrackingModuleImpl::construction_count);
		ASSERT_EQ(1, TrackingModuleImpl::destruction_count);
	}

	// we left scope. the module loader should be destroyed
	ASSERT_TRUE(ModuleRegistry::getInstance()->findModules<ITrackingModule>().empty());

	// ... but the second module itself should still be alive.
	ASSERT_EQ(2, TrackingModuleImpl::construction_count);
	ASSERT_EQ(1, TrackingModuleImpl::destruction_count);

	// now release the second module
	module2->release();
	module2 = NULL;

	// ... and check, if it's really gone
	ASSERT_EQ(2, TrackingModuleImpl::construction_count);
	ASSERT_EQ(2, TrackingModuleImpl::destruction_count);

	return;
}


TEST(ModuleApi, ModuleConstructionSingleton) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITrackingModule>*> TrackingModList;

	// reset vars
	TrackingModuleImpl::construction_count	= 0;
	TrackingModuleImpl::destruction_count	= 0;

	ITrackingModule *module1 = NULL;
	ITrackingModule *module2 = NULL;

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE_SINGLETON(ITrackingModule, TrackingModuleImpl, &TrackingModuleImpl::create, "TrackingModule", 0x01000000u, IModuleLoader::PriorityNormal);

		// no module should be created at this moment
		ASSERT_EQ(0, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// get the module loader
		TrackingModList list = ModuleRegistry::getInstance()->findModules<ITrackingModule>();
		ASSERT_EQ(1u, list.size());

		// there should still no module instance
		ASSERT_EQ(0, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// now create the first module...
		module1 = list[0]->create();
		module1->retain();

		// one creation, but no destruction.
		ASSERT_EQ(1, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// the reference count of our module should be 2 (module1 + singleton reference)
		ASSERT_EQ(2, module1->getReferenceCount());

		// create the second module
		module2 = list[0]->create();
		module2->retain();

		// we should got the same object
		ASSERT_EQ(module1, module2);

		// since the module is registered as singleton, there should still be only one instance
		ASSERT_EQ(1, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// the reference count of our module should be 3 (module1, module2, singleton reference)
		ASSERT_EQ(3, module1->getReferenceCount());
		ASSERT_EQ(3, module2->getReferenceCount());

		// release the first module
		module1->release();
		module1 = NULL;

		// there should still be only one valid object, it should not be released
		ASSERT_EQ(1, TrackingModuleImpl::construction_count);
		ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

		// ... but the ref count should have been decreased
		ASSERT_EQ(2, module2->getReferenceCount());
	}

	// we left scope. the module loader should be destroyed
	ASSERT_TRUE(ModuleRegistry::getInstance()->findModules<ITrackingModule>().empty());

	// ... but the module itself should still be alive.
	ASSERT_EQ(1, TrackingModuleImpl::construction_count);
	ASSERT_EQ(0, TrackingModuleImpl::destruction_count);

	// ... and only one reference left
	ASSERT_EQ(1, module2->getReferenceCount());

	// now release the second module
	module2->release();
	module2 = NULL;

	// ... and check, if it's really gone
	ASSERT_EQ(1, TrackingModuleImpl::construction_count);
	ASSERT_EQ(1, TrackingModuleImpl::destruction_count);

	return;
}


/**
 * Test if modules are sorted correctly by their API version.
 */
TEST(ModuleApi, ApiVersionOrder) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITestModule>*> TestModList;

	// there shouldn't be any module in the registry
	TestModList list1 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list1.empty());

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE(ITestModule, TestMod17, &TestMod17::create, "TestMod", 0x01001700u, IModuleLoader::PriorityNormal);
		REGISTER_MODULE(ITestModule, TestMod42, &TestMod42::create, "TestMod", 0x01004200u, IModuleLoader::PriorityNormal);
		REGISTER_MODULE(ITestModule, TestMod23, &TestMod23::create, "TestMod", 0x01002300u, IModuleLoader::PriorityNormal);

		// now we should have exactly one module registered
		TestModList list2 = ModuleRegistry::getInstance()->findModules<ITestModule>();
		ASSERT_EQ(3u, list2.size());

		// get the moduiles
		ITestModule *testmod1 = list2[0]->create();
		ITestModule *testmod2 = list2[1]->create();
		ITestModule *testmod3 = list2[2]->create();

		// check if all modules were created
		ASSERT_TRUE(NULL != testmod1);
		ASSERT_TRUE(NULL != testmod2);
		ASSERT_TRUE(NULL != testmod3);

		// compare the result of the module
		EXPECT_EQ(42, testmod1->getTestValue());
		EXPECT_EQ(23, testmod2->getTestValue());
		EXPECT_EQ(17, testmod3->getTestValue());
	}

	// after we left the scope, the registry should be empty
	TestModList list3 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list3.empty());

	return;
}


/**
 * Test if modules are sorted correctly by their priority.
 * The API version should be ignored, because their API names are different.
 */
TEST(ModuleApi, PriorityOrder) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITestModule>*> TestModList;

	// there shouldn't be any module in the registry
	TestModList list1 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list1.empty());

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE(ITestModule, TestMod23, &TestMod23::create, "TestMod3", 0x01002300u, IModuleLoader::PriorityNormal);
		REGISTER_MODULE(ITestModule, TestMod42, &TestMod42::create, "TestMod2", 0x01004200u, IModuleLoader::PriorityLow);
		REGISTER_MODULE(ITestModule, TestMod17, &TestMod17::create, "TestMod1", 0x01001700u, IModuleLoader::PriorityHigh);

		// now we should have exactly one module registered
		TestModList list2 = ModuleRegistry::getInstance()->findModules<ITestModule>();
		ASSERT_EQ(3u, list2.size());

		// get the moduiles
		ITestModule *testmod1 = list2[0]->create();
		ITestModule *testmod2 = list2[1]->create();
		ITestModule *testmod3 = list2[2]->create();

		// check if all modules were created
		ASSERT_TRUE(NULL != testmod1);
		ASSERT_TRUE(NULL != testmod2);
		ASSERT_TRUE(NULL != testmod3);

		// compare the result of the module
		EXPECT_EQ(17, testmod1->getTestValue());
		EXPECT_EQ(23, testmod2->getTestValue());
		EXPECT_EQ(42, testmod3->getTestValue());
	}

	// after we left the scope, the registry should be empty
	TestModList list3 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list3.empty());

	return;
}


/**
 * Test if modules are sorted correctly, when using both api version and priority.
 */
TEST(ModuleApi, CombinedOrder) {
	// for shorter code...
	typedef std::vector<ModuleLoader<ITestModule>*> TestModList;

	// there shouldn't be any module in the registry
	TestModList list1 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list1.empty());

	// create a new scope
	{
		// register the module within this scope, so it should be gone when leaving the scope
		REGISTER_MODULE(ITestModule, TestMod23, &TestMod23::create, "TestMod", 0x01002300u, IModuleLoader::PriorityNormal);
		REGISTER_MODULE(ITestModule, TestMod42, &TestMod42::create, "TestMod", 0x01004200u, IModuleLoader::PriorityNormal);
		REGISTER_MODULE(ITestModule, TestMod17, &TestMod17::create, "TestMod", 0x01001700u, IModuleLoader::PriorityHigh);

		// now we should have exactly one module registered
		TestModList list2 = ModuleRegistry::getInstance()->findModules<ITestModule>();
		ASSERT_EQ(3u, list2.size());

		// get the moduiles
		ITestModule *testmod1 = list2[0]->create();
		ITestModule *testmod2 = list2[1]->create();
		ITestModule *testmod3 = list2[2]->create();

		// check if all modules were created
		ASSERT_TRUE(NULL != testmod1);
		ASSERT_TRUE(NULL != testmod2);
		ASSERT_TRUE(NULL != testmod3);

		// compare the result of the module
		EXPECT_EQ(17, testmod1->getTestValue());
		EXPECT_EQ(42, testmod2->getTestValue());
		EXPECT_EQ(23, testmod3->getTestValue());
	}

	// after we left the scope, the registry should be empty
	TestModList list3 = ModuleRegistry::getInstance()->findModules<ITestModule>();
	ASSERT_TRUE(list3.empty());

	return;
}
