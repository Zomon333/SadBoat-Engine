#pragma once
#include "headers.h"
/*
Dagan Poulin, SadBoat Entertainment, 4/12/2021
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
using namespace std;
namespace PPS {
	enum eventType
	{
		_ERROR, //ErrorNumber, ErrorName, EventName, EventParent

		COLLISION, //CollisionA, CollisionB

		PHYSICS, //Location, Velocity, Direction
		PROJECTILEPHYSICS,

		MAPCHANGE, //NewMap
		CUTSCENE, // 
		DIALOGUE, //SpeakerObject, Text, Instant, Duration

		ACTIONTRIGGER, //ObjectTrigger
		COMBAT, //UnitA, UnitB
		AI, //Location, PlayerLocation

		KEYPRESS,
		CLICK,

		AUDIO, //Sound, Duration
		RENDER, //

		PAUSE,
		EXIT,
		SAVE,
		LOAD,
		JOIN
	};

	struct EventParameters
	{
		EventParameters();

		//ERROR
		int ERROR_ErrorNumber;
		string ERROR_EventName;
		string ERROR_EventParent;

		
		//COLLISION
		//Object* COLLISION_ObjectA;
		//Object* COLLISION_ObjectB;

		//PHYSICS
		//Point* PHYSICS_Location;
		//float PHYSICS_Velocity;
		//float PHYSICS_Direction;

		//MAPCHANGE
		//Map* MAPCHANGE_NewMap;

		//CUTSCENE

		//DIALOGUE
		//Object* DIALOGUE_SpeakerObject;
		string DIALOGUE_text;
		bool DIALOGUE_instant;
		float DIALOGUE_duration;

		//ACTIONTRIGGER
		//Object* ACTION_ActionTrigger;

		//COMBAT
		//Object* COMBAT_UnitA;
		//Object* COMBAT_UnitB;

		//AI
		//Point* AI_Location;
		//Point* AI_PlayerLocation;

		//KEYPRESS / MOUSE
		int KEYPRESS_key;
		bool KEYPRESS_keyPressed;
		//Point KEYPRESS_MouseLocation;

		//SOUND
		//RENDER
		//PAUSE
		bool PAUSE_newState;
		//EXIT
		string EXIT_reason;
		//SAVE
		//LOAD
	};

	class Event
	{
	private:
		eventType type;
		EventParameters params;
		static string ErrorTypes[332];
	public:
		Event(eventType type, EventParameters params);
		eventType getType();
		EventParameters getParams();
		virtual void handle();
	};

}