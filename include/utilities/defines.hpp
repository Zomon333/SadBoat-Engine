/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef ENGINE_DEFINES_H
#define ENGINE_DEFINES_H

using namespace std;

//  Engine specific defines
//----------------------------------

//Shorthand lambda function body
#define F(a...) [](a...)

//Shorthand lambda function for internal captures
#define lF(a...) [this](a...)

//Microseconds-- engine unit time.
#define uTime std::chrono::milliseconds

//100 HZ -- engine unit frequency.
#define uFreq uTime(10)

//2 kHZ -- maximum engine frequency
#define uFreqMax std::chrono::microseconds(500)

//pow(2, -47)-- engine unit tolerance. Arbitrarily small.
#define uTol pow(2,-32)

//Steady clock for timekeeping.
#define EngineClock std::chrono::steady_clock

//Shorthand for the current time on the clock
#define Now EngineClock::now()

//An instant in time-- a microsecond by the engine's clock.
#define Instant EngineClock::time_point

#endif