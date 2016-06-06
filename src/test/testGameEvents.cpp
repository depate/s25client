// Copyright (c) 2005 - 2015 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.

#include "defines.h" // IWYU pragma: keep
#include "GameObject.h"
#include "EventManager.h"
#include "GameEvent.h"
#include "RTTR_AssertError.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(GameEventsTestSuite)

BOOST_AUTO_TEST_CASE(AdvanceGFs)
{
    EventManager evMgr(42);
    BOOST_REQUIRE_EQUAL(evMgr.GetCurrentGF(), 42u);
    for(unsigned gf = 43; gf < 50; gf++)
    {
        evMgr.ExecuteNextGF();
        BOOST_REQUIRE_EQUAL(evMgr.GetCurrentGF(), gf);
    }
}

class TestEventHandler: public GameObject
{
public:
    std::vector<unsigned> handledEventIds;

    void HandleEvent(const unsigned evId) override
    {
        handledEventIds.push_back(evId);
    }

    void Destroy() override{}
    void Serialize(SerializedGameData& sgd) const override{}
    GO_Type GetGOT() const override{ return GOT_UNKNOWN; }
};

BOOST_AUTO_TEST_CASE(AddAndExecuteEvent)
{
    const unsigned startGF = 1;
    EventManager evMgr(startGF);
    TestEventHandler obj;
    // Check that adding an event works and data is correctly set
    GameEvent* ev = evMgr.AddEvent(&obj, 5, 42);
    BOOST_REQUIRE_EQUAL(ev->obj, &obj);
    BOOST_REQUIRE_EQUAL(ev->startGF, startGF);
    BOOST_REQUIRE_EQUAL(ev->length, 5u);
    BOOST_REQUIRE_EQUAL(ev->id, 42u);
    BOOST_REQUIRE_EQUAL(ev->GetTargetGF(), startGF + ev->length);
    BOOST_CHECK(evMgr.IsEventActive(&obj, 42));
    BOOST_CHECK(!evMgr.IsEventActive(&obj, 43));
    BOOST_CHECK(evMgr.ObjectHasEvents(&obj));
    // Check that event is not executed before it is due
    for(unsigned i = startGF + 1; i < ev->GetTargetGF(); i++)
    {
        evMgr.ExecuteNextGF();
        BOOST_REQUIRE_EQUAL(evMgr.GetCurrentGF(), i); // Rechecked for clarity
        BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 0u);
    }
    BOOST_REQUIRE_EQUAL(evMgr.GetCurrentGF() + 1u, ev->GetTargetGF());
    // Now the event should be executed
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 1u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.front(), 42u);
    // And nothing should be left
    BOOST_CHECK(!evMgr.IsEventActive(&obj, 42));
    BOOST_CHECK(!evMgr.ObjectHasEvents(&obj));
}

BOOST_AUTO_TEST_CASE(AddSuspendedEvent)
{
    const unsigned startGF = 100;
    EventManager evMgr(startGF);
    TestEventHandler obj;
    // Add an event with length 50, where 48 GFs have already elapsed (2 left)
    GameEvent* ev = evMgr.AddEvent(&obj, 50, 42, 48);
    BOOST_REQUIRE_EQUAL(ev->obj, &obj);
    BOOST_REQUIRE_EQUAL(ev->startGF, startGF - 48u); // "Started" 48 GFs ago
    BOOST_REQUIRE_EQUAL(ev->length, 50u);
    BOOST_REQUIRE_EQUAL(ev->id, 42u);
    BOOST_REQUIRE_EQUAL(ev->GetTargetGF(), startGF + 2);
    // Check that event is not executed before it is due
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 0u);
    // Now the event should be executed
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 1u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.front(), 42u);
}

BOOST_AUTO_TEST_CASE(MultipleEvents)
{
    EventManager evMgr(0);
    TestEventHandler obj;
    evMgr.AddEvent(&obj, 5, 42);
    evMgr.AddEvent(&obj, 5, 43);
    evMgr.AddEvent(&obj, 6, 44);
    // Execute first 2 events
    for(unsigned i = 1; i <= 5; i++)
        evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 2u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds[0], 42u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds[1], 43u);
    // And 3rd
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 3u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds[2], 44u);
}

class TestLogKill: public TestEventHandler
{
public:
    EventManager& em;
    TestLogKill(EventManager& em): em(em){}

    static unsigned killNum;
    ~TestLogKill()
    {
        killNum++;
    }
    void HandleEvent(const unsigned evId) override
    {
        BOOST_REQUIRE(!em.ObjectIsInKillList(this));
        // Kill this on event
        em.AddToKillList(this);
        BOOST_REQUIRE(em.ObjectIsInKillList(this));
    }
};

unsigned TestLogKill::killNum = 0;

BOOST_AUTO_TEST_CASE(KillList)
{
    EventManager evMgr(0);
    TestLogKill* obj = new TestLogKill(evMgr);
    evMgr.AddEvent(obj, 2);
    // Nothing should happened yet
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(TestLogKill::killNum, 0u);
    // And not it should be killed
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(TestLogKill::killNum, 1u);
    BOOST_REQUIRE(!evMgr.ObjectIsInKillList(obj));
}

class TestRemoveEvent: public TestEventHandler
{
public:
    EventManager& em;
    GameEvent* ev2Remove;
    TestRemoveEvent(EventManager& em): em(em){}

    void HandleEvent(const unsigned evId) override
    {
        TestEventHandler::HandleEvent(evId);
        if(evId == 42 && ev2Remove)
        {
            em.RemoveEvent(ev2Remove);
            // Should be set to NULL
            BOOST_REQUIRE(!ev2Remove);
        }
    }
};

BOOST_AUTO_TEST_CASE(RemoveEvent)
{
    EventManager evMgr(0);
    TestRemoveEvent obj(evMgr);
    evMgr.AddEvent(&obj, 1, 42);
    obj.ev2Remove = evMgr.AddEvent(&obj, 2, 43);
    // Execute time for both events. Only first should have fired
    evMgr.ExecuteNextGF();
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 1u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.front(), 42u);

    // Try the same but with an event in the same GF
    evMgr.AddEvent(&obj, 1, 42);
    obj.ev2Remove = evMgr.AddEvent(&obj, 1, 43);
    evMgr.ExecuteNextGF();
    BOOST_REQUIRE_EQUAL(obj.handledEventIds.size(), 2u);
    BOOST_REQUIRE_EQUAL(obj.handledEventIds[1], 42u);
    BOOST_CHECK(!evMgr.ObjectHasEvents(&obj));
}

BOOST_AUTO_TEST_CASE(InvalidEvent)
{
#if RTTR_ENABLE_ASSERTS
    RTTR_AssertEnableBreak = false;
    EventManager evMgr(100);
    TestEventHandler obj;
    // Need object
    BOOST_CHECK_THROW(evMgr.AddEvent(NULL, 1), RTTR_AssertError);
    // Length must be > 0
    BOOST_CHECK_THROW(evMgr.AddEvent(&obj, 0), RTTR_AssertError);
    // ... even for continued events
    BOOST_CHECK_THROW(evMgr.AddEvent(NULL, 50, 0, 50), RTTR_AssertError);
    // conitinued event cannot start before the game
    BOOST_CHECK_THROW(evMgr.AddEvent(NULL, 200, 0, 150), RTTR_AssertError);
    RTTR_AssertEnableBreak = true;
#endif
}

BOOST_AUTO_TEST_SUITE_END()