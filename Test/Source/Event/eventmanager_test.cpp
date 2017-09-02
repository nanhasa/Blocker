#include "3rdParty/gtest/gtest.h"

#include "Event/eventmanager_test.h"
#include "Utility/utility.h"

//Hide functions from other files
namespace { 

	class EventManagerTest : public testing::Test {
	protected:
		TestClass1 m_testClass;
		const int m_invalidListenerId = -1;

		// Function called before every TEST_F call
		void SetUp() override
		{
			ASSERT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 0);
			m_testClass.registerListener();
			ASSERT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
			g_callbackCounter = 0;
		}

		// Function called after every TEST_F call
		void TearDown() override
		{
			DerivedEventManager::clearListeners();
			ASSERT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 0);
			DerivedEventManager::flushQueue();
			ASSERT_EQ(DerivedEventManager::getQueueLength(), 0);
		}

	};

	// Test removeListener function
	TEST_F(EventManagerTest, removeOnlyListener) {
		EXPECT_TRUE(m_testClass.unregisterListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 0);
	}
	TEST_F(EventManagerTest, removeSameListenerTwice) {
		EXPECT_TRUE(m_testClass.unregisterListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 0);
		EXPECT_FALSE(m_testClass.unregisterListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 0);
	}
	TEST_F(EventManagerTest, removeNonExistingEventListener) {
		TestClass2 testClass2;
		EXPECT_FALSE(testClass2.unregisterListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
	}
	TEST_F(EventManagerTest, removeInvalidEventListener) {
		EXPECT_FALSE(DerivedEventManager::removeListener(TestEvent::eventType, m_invalidListenerId));
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
	}
	TEST_F(EventManagerTest, removeListenerFromOneOfTwoEventTypes) {
		EXPECT_TRUE(m_testClass.registerListener(TestEvent2::eventType));
		EXPECT_TRUE(m_testClass.unregisterListener(TestEvent2::eventType));
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent2::eventType), 0);
	}

	//Test addListener function
	TEST_F(EventManagerTest, addDuplicateListeners) {
		EXPECT_FALSE(m_testClass.registerListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
	}
	TEST_F(EventManagerTest, addDerivedClassListener) {
		SubTestClass1 sub;
		EXPECT_TRUE(sub.registerListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 2);
	}
	TEST_F(EventManagerTest, addSiblingDerivedClassListeners) {
		DerivedEventManager::clearListeners();
		SubTestClass1 sub1;
		SubTestClass2 sub2;
		EXPECT_TRUE(sub1.registerListener());
		EXPECT_TRUE(sub2.registerListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 2);
	}
	TEST_F(EventManagerTest, addTwoInstancesOfSameListenerClass) {
		TestClass1 anotherInstance;
		EXPECT_TRUE(anotherInstance.registerListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 2);
	}
	TEST_F(EventManagerTest, addTwoDifferentClassesSameEvent) {
		TestClass2 anotherClass;
		EXPECT_TRUE(anotherClass.registerListener());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 2);
	}
	TEST_F(EventManagerTest, addTwoEvents) {
		TestClass2 anotherClass;
		EXPECT_TRUE(anotherClass.registerListener(TestEvent2::eventType));
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), 1);
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent2::eventType), 1);
	}

	// Test triggerEvent
	TEST_F(EventManagerTest, triggerNonExistingEvent) {
		DerivedEventManager::clearListeners();
		EventManager::triggerEvent(std::make_shared<TestEvent>());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), g_callbackCounter);
	}
	TEST_F(EventManagerTest, triggerValidEventWithOneListener) {
		EventManager::triggerEvent(std::make_shared<TestEvent>());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), g_callbackCounter);
	}
	TEST_F(EventManagerTest, triggerValidEventWithTwoListeners) {
		TestClass2 anotherClass;
		anotherClass.registerListener();
		EventManager::triggerEvent(std::make_shared<TestEvent>());
		EXPECT_EQ(DerivedEventManager::getEventListenerCount(TestEvent::eventType), g_callbackCounter);
	}

	// Test queueEvent
#ifdef NDEBUG
	TEST_F(EventManagerTest, queueInvalidEvent) { // This test breaks precondition and therefore cannot be run in debug mode
		EXPECT_FALSE(DerivedEventManager::queueEvent(nullptr));
		EXPECT_EQ(DerivedEventManager::getQueueLength(), 0);
	}
#endif // RELEASE

	TEST_F(EventManagerTest, queueValidEvent) {
		EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent>()));
		EXPECT_EQ(DerivedEventManager::getQueueLength(), 1);
	}
	TEST_F(EventManagerTest, queueTwoDifferentEvents) {
		EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent>()));
		EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent2>()));
		EXPECT_EQ(DerivedEventManager::getQueueLength(), 2);
	}

	// Test onUpdate
#ifdef NDEBUG
	TEST_F(EventManagerTest, onUpdateInvalidProcessTime) { // This test breaks precondition and therefore cannot be run in debug mode
		EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent>()));
		unsigned int queueBefore = DerivedEventManager::getQueueLength();
		EventManager::onUpdate(-1.0f);
		EXPECT_EQ(DerivedEventManager::getQueueLength(), queueBefore);
	}
#endif // RELEASE

	TEST_F(EventManagerTest, onUpdateProcessTime) {
		// Populate queue
		for (unsigned int i = 0; i < 1000; ++i) {
			EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent>()));
			EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent2>()));
		}
		const float startTime = utility::getTimestamp();
		float processTime = 50.0f;
		DerivedEventManager::onUpdate(processTime);
		float delta = utility::getTimestamp() - startTime;
		EXPECT_GT(DerivedEventManager::getQueueLength(), static_cast<unsigned int>(0)); // Make sure the processing ended to process time and not because queue was empty
		EXPECT_NEAR(processTime, delta, 3.0f); // Actual processing time varies from given time the amount it takes to process one event
	}

	TEST_F(EventManagerTest, onUpdateUntilQueueEmpty) {
		// Populate queue
		for (unsigned int i = 0; i < 100; ++i) {
			EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent>()));
			EXPECT_TRUE(DerivedEventManager::queueEvent(std::make_shared<TestEvent2>()));
		}
		const float processTime = 1000.0f;
		DerivedEventManager::onUpdate(processTime);
		EXPECT_EQ(DerivedEventManager::getQueueLength(), static_cast<unsigned int>(0));
	}
}