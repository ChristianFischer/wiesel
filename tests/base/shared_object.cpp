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

#include <wiesel/util/shared_object.h>
#include <stdio.h>

using namespace wiesel;




enum TestObjectState {
	State_Uninitialized,
	State_Constructed,
	State_Destroyed,
};



/**
 * @brief A testobject, which receives a pointer to an external variable to store the object's state.
 * The object will change the state to 'constructed' when the constructor was called and 'destroyed'
 * when the object was destroyed.
 */
class TestObject : public virtual SharedObject
{
private:
	TestObject();
	
public:
	TestObject(TestObjectState *pState) {
		this->pState = pState;
		
		// set the state to "Constructed"
		*(this->pState) = State_Constructed;
		
		return;
	}
	
    virtual ~TestObject() {
		*(this->pState) = State_Destroyed;
	}
	
private:
	TestObjectState*	pState;
};





/**
 * Test the lifetime of a temporary object,
 * which will be deleted immediately.
 */
TEST(SharedObject, ObjectLifetime_Temporary) {
	TestObjectState state = State_Uninitialized;
	TestObject *object = new TestObject(&state);
	
	// the object should be in state 'constructed'
	EXPECT_EQ(State_Constructed, state);
	
	// the reference counter should be at zero after construction
	EXPECT_EQ(0, object->getReferenceCount());
	
	// now delete the object
	delete object;
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object, which was kept and released.
 */
TEST(SharedObject, ObjectLifetime_Keep_Release) {
	TestObjectState state = State_Uninitialized;
	TestObject *object = new TestObject(&state);
	
	// the object should be in state 'constructed'
	EXPECT_EQ(State_Constructed, state);
	
	// the reference counter should be at zero after construction
	EXPECT_EQ(0, object->getReferenceCount());
	
	// keep the object
	keep(object);
	
	// the object's state should still be 'constructed'
	EXPECT_EQ(State_Constructed, state);
	
	// the reference counter now should be increased
	EXPECT_EQ(1, object->getReferenceCount());
	
	// now release the object
	release(object);
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
	
	// the pointer itself should be untouched
	EXPECT_TRUE(object != NULL);
}


/**
 * Test the lifetime of an object, which was kept and released.
 */
TEST(SharedObject, ObjectLifetime_Keep_SafeRelease) {
	TestObjectState state = State_Uninitialized;
	TestObject *object = new TestObject(&state);
	
	// the object should be in state 'constructed'
	EXPECT_EQ(State_Constructed, state);
	
	// the reference counter should be at zero after construction
	EXPECT_EQ(0, object->getReferenceCount());
	
	// keep the object
	keep(object);
	
	// the object's state should still be 'constructed'
	EXPECT_EQ(State_Constructed, state);
	
	// the reference counter now should be increased
	EXPECT_EQ(1, object->getReferenceCount());
	
	// now release the object
	safe_release(object);
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
	
	// the pointer itself should be untouched
	EXPECT_TRUE(object == NULL);
}


/**
 * Test the lifetime of an object, which was kept and released multiple times.
 */
TEST(SharedObject, ObjectLifetime_KeepMultipleTimes) {
	TestObjectState state = State_Uninitialized;
	TestObject *object = new TestObject(&state);
	
	// the object should be in state 'constructed'
	EXPECT_EQ(state, State_Constructed);
	
	// the reference counter should be at zero after construction
	EXPECT_EQ(0, object->getReferenceCount());
	
	for(int i=1; i<=5; i++) {
		// keep the object
		keep(object);
		
		// the object's state should still be 'constructed'
		EXPECT_EQ(state, State_Constructed);
		
		// the reference counter now should be increased
		EXPECT_EQ(i, object->getReferenceCount());
	}
	
	// after keeping five times, the reference counter should be 5
	EXPECT_EQ(5, object->getReferenceCount());
	
	for(int i=4; i>=1; i--) {
		// release the object
		release(object);
		
		// the pointer should be untouched
		EXPECT_TRUE(object != NULL);
			
		// the object's state should still be 'constructed'
		EXPECT_EQ(state, State_Constructed);
		
		// the reference counter now should be decreased
		EXPECT_EQ(i, object->getReferenceCount());
	}
	
	// after releasing four times, the reference counter should be one
	EXPECT_EQ(1, object->getReferenceCount());
	
	// do the 'final' release
	release(object);
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(state, State_Destroyed);
	
	// the pointer itself should be untouched
	EXPECT_TRUE(object != NULL);
}


/**
 * Test the lifetime of a temporary object,
 * which lives within a scope and will be destroyed automatically.
 */
TEST(SharedObject, ObjectLifetime_Scoped) {
	TestObjectState state = State_Uninitialized;
	
	// scope begin
	{
		TestObject object(&state);
		
		// the object should be in state 'constructed'
		EXPECT_EQ(State_Constructed, state);
		
		// the reference counter should be at zero after construction
		EXPECT_EQ(0, object.getReferenceCount());
	}
	
	// the object's state should be 'destroyed' after the scope has ended
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored by using a smartpointer.
 */
TEST(SharedObject, Ref) {
	TestObjectState state = State_Uninitialized;
	
	// scope begin
	{
		ref<TestObject> object(new TestObject(&state));
		
		// the object should be in state 'constructed'
		EXPECT_EQ(State_Constructed, state);
		
		// the reference counter should be at one after construction
		// because the object will be kept by the ref
		EXPECT_EQ(1, object->getReferenceCount());
		
		// when we leave the scope, the ref object should be destroyed
		// and thereby decrement the reference counter and destroy the testobject, too
	}
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored by using a smartpointer.
 */
TEST(SharedObject, Ref2) {
	TestObjectState state = State_Uninitialized;
	
	// scope begin
	{
		ref<TestObject> object(new TestObject(&state));
		
		// the object should be in state 'constructed'
		EXPECT_EQ(State_Constructed, state);
		
		// the reference counter should be at one after construction
		// because the object will be kept by the ref
		EXPECT_EQ(1, object->getReferenceCount());
		
		// begin another scope
		{
			ref<TestObject> object2(object);
			
			// since the object is stored by another smartpointer,
			// the reference counter should have been increased
			EXPECT_EQ(2, object->getReferenceCount());
		}
		
		// but after leaving the scopem the object should still be alive,
		// with an decreased reference counter
		EXPECT_EQ(State_Constructed, state);
		EXPECT_EQ(1, object->getReferenceCount());
		
		// when we leave the scope, the ref object should be destroyed
		// and thereby decrement the reference counter and destroy the testobject, too
	}
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored by using a smartpointer.
 */
TEST(SharedObject, RefAssign) {
	TestObjectState state = State_Uninitialized;
	
	// create a new object and check if correctly initialized
	TestObject *object = new TestObject(&state);
	EXPECT_EQ(State_Constructed, state);
	EXPECT_EQ(0, object->getReferenceCount());

	// create an empty pointer
	ref<TestObject> pObject;
	
	// the pointer should be empty
	EXPECT_TRUE((*pObject) == NULL);
	
	// now assign the object to our pointer
	pObject = object;
	
	// the object's reference counter should be incremented
	EXPECT_EQ(State_Constructed, state);
	EXPECT_EQ(1, object->getReferenceCount());
	
	// now clear the pointer
	pObject = NULL;
	
	// this should have been destroyed our object
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored by using a smartpointer.
 */
TEST(SharedObject, RefWithExternalReference) {
	TestObjectState state = State_Uninitialized;
	TestObject *object2 = NULL;
	
	// scope begin
	{
		ref<TestObject> object(new TestObject(&state));
		
		// the object should be in state 'constructed'
		EXPECT_EQ(State_Constructed, state);
		
		// the reference counter should be at one after construction
		// because the object will be kept by the ref
		EXPECT_EQ(1, object->getReferenceCount());
		
		// store the reference in an external variable
		object2 = object;
		
		// keep the 2nd reference
		keep(object2);
		
		// the reference counter should have been increased
		EXPECT_EQ(2, object->getReferenceCount());
	}
	
	// after leaving the scope, the smartpointer was destroyed
	// and the reference counter of our object should be one
	EXPECT_EQ(State_Constructed, state);
	EXPECT_EQ(1, object2->getReferenceCount());
	
	// now release our extrnal reference
	safe_release(object2);
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored in a smartpointer list.
 */
TEST(SharedObject, RefList) {
	TestObjectState state = State_Uninitialized;
	TestObject *object = new TestObject(&state);
	
	// the object should be in state 'constructed' with no reference
	EXPECT_EQ(State_Constructed, state);
	EXPECT_EQ(0, object->getReferenceCount());
	
	// scope begin
	{
		ref<TestObject>::list references;
		
		// put the object into our list
		references.push_back(object);
		
		// the object should be in state 'constructed'
		EXPECT_EQ(State_Constructed, state);
		
		// the reference counter should be at zero after construction
		// because the object will be kept by the ref
		EXPECT_EQ(1, object->getReferenceCount());
		
		// when we leave the scope, the ref-list should be destroyed
		// and thereby decrement the reference counter and destroy the testobject, too
	}
	
	// the object's state should be 'destroyed'
	EXPECT_EQ(State_Destroyed, state);
}


/**
 * Test the lifetime of an object,
 * which is stored in a smartpointer vector.
 */
TEST(SharedObject, RefVector) {
	const unsigned int NUM_OBJECTS = 10;
	TestObjectState states[NUM_OBJECTS];
	TestObject* objects[NUM_OBJECTS];

	// initialize some test objects
	for(unsigned int create_index=0; create_index<NUM_OBJECTS; create_index++) {
		states[create_index]  = State_Uninitialized;
		objects[create_index] = new TestObject(&states[create_index]);

		// each object should be in state 'constructed' with no references
		EXPECT_EQ(State_Constructed, states[create_index]) << "create_index=" << create_index;
		EXPECT_EQ(0, objects[create_index]->getReferenceCount()) << "create_index=" << create_index;
	}

	// scope begin
	{
		ref<TestObject>::vector references;

		for(unsigned int add_index=0; add_index<NUM_OBJECTS; add_index++) {
			// put the according object into our list
			references.push_back(objects[add_index]);

			// the object should be in state 'constructed'
			EXPECT_EQ(State_Constructed, states[add_index]) << "add_index=" << add_index;

			// the reference counter should be at one
			// because the object will be kept by the ref
			EXPECT_EQ(1, objects[add_index]->getReferenceCount()) << "add_index=" << add_index;

			// check each element in the vector
			for(unsigned int check_index=0; check_index<=add_index; check_index++) {
				// the referenced object should be the same as the one in our array
				EXPECT_EQ(objects[check_index], *(references[check_index]))
						<< "add_index=" << add_index << ", check_index=" << check_index
				;

				// the object's state should be 'constructed'
				EXPECT_EQ(State_Constructed, states[check_index])
						<< "add_index=" << add_index << ", check_index=" << check_index
				;

				// all objects should have only one reference
				EXPECT_EQ(1, objects[check_index]->getReferenceCount())
						<< "add_index=" << add_index << ", check_index=" << check_index
				;
			}
		}

		// all objects should have been added
		EXPECT_EQ(NUM_OBJECTS, references.size());

		// we start to delete some of the objects
		for(unsigned int delete_index=0; delete_index<NUM_OBJECTS/2; delete_index++) {
			// erase the first remaining object
			references.erase(references.begin());

			// check if the object was deleted
			EXPECT_EQ(State_Destroyed, states[delete_index]) << "delete_index=" << delete_index;

			// all other objects should still be alive
			for(unsigned int check_index=delete_index+1; check_index<NUM_OBJECTS; check_index++) {
				EXPECT_EQ(State_Constructed, states[check_index])
						<< "delete_index=" << delete_index << ", check_index=" << check_index
				;
			}
		}
	}

	// check if all objects were deleted
	for(unsigned int check_index=0; check_index<NUM_OBJECTS; check_index++) {
		EXPECT_EQ(State_Destroyed, states[check_index]) << "check_index=" << check_index;
	}
}


/**
 * Test the autorelease function.
 */
TEST(SharedObject, Autorelease) {
	TestObjectState state1 = State_Uninitialized;
	TestObjectState state2 = State_Uninitialized;
	
	// scope begin
	{
		// create an object in a scoped pointer
		ref<TestObject> object1(new TestObject(&state1));
		
		// you know it..
		EXPECT_EQ(State_Constructed, state1);
		EXPECT_EQ(1, object1->getReferenceCount());
		
		// now mark the object to be autoreleased
		autorelease(object1);
	}
	
	// the object should be still alive, because of the autorelease pointer
	EXPECT_EQ(State_Constructed, state1);
	
	{
		// now create a second object
		ref<TestObject> object2(new TestObject(&state2));
		
		// ...
		EXPECT_EQ(State_Constructed, state2);
		EXPECT_EQ(1, object2->getReferenceCount());
		
		// our first object should still be alive
		EXPECT_EQ(State_Constructed, state1);
		
		// mark the 2nd object to be autoreleased
		autorelease(object2);
		
		// this should have been killed our first object
		EXPECT_EQ(State_Destroyed, state1);
	}
	
	// since the 2nd object was marked to be autoreleased too,
	// it should be alive after the scope was closed
	EXPECT_EQ(State_Constructed, state2);
	
	// but now we "clear" the autorelease pointer
	autorelease(NULL);
	
	// and both objects are gone
	EXPECT_EQ(State_Destroyed, state1);
	EXPECT_EQ(State_Destroyed, state2);
}
