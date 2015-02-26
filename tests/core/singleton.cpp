/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <gamekeeper/core/singleton.h>

#include <thread>

#include <gtest/gtest.h>

using namespace gamekeeper::core;

class SingletonTest : public testing::Test{};

class TestSingleton{};

TEST_F(SingletonTest, integrity)
{
	ASSERT_EQ(&Singleton::get<TestSingleton>(), &Singleton::get<TestSingleton>());
}

struct SingletonCyclic1{SingletonCyclic1();};
struct SingletonCyclic2{SingletonCyclic2();};

SingletonCyclic1::SingletonCyclic1()
{
	Singleton::get<SingletonCyclic2>();
};

SingletonCyclic2::SingletonCyclic2()
{
	Singleton::get<SingletonCyclic1>();
};


TEST_F(SingletonTest, cyclicDetection)
{
	ASSERT_THROW(SingletonCyclic1(), Singleton::CyclicDependency);
	ASSERT_THROW(SingletonCyclic2(), Singleton::CyclicDependency);
}

TEST_F(SingletonTest, errorShouldKeepUninitialized)
{
	ASSERT_THROW(SingletonCyclic1(), Singleton::CyclicDependency);
	ASSERT_THROW(SingletonCyclic1(), Singleton::CyclicDependency);
}

TEST_F(SingletonTest, cyclicDetectionRuntimeSafety)
{
	// run first inside another thread
	std::thread([](){
		ASSERT_THROW(SingletonCyclic1(), Singleton::CyclicDependency);
	}).join();
	ASSERT_THROW(SingletonCyclic1(), Singleton::CyclicDependency);
}

struct DelayedSingleton{DelayedSingleton(){std::this_thread::sleep_for(0.01s);}};

TEST_F(SingletonTest, cyclicDetectionFalseAlarm)
{
	std::thread([](){
		Singleton::get<DelayedSingleton>();
	}).detach();
	std::this_thread::sleep_for(0.001s);
	ASSERT_NO_THROW(Singleton::get<DelayedSingleton>());
}
