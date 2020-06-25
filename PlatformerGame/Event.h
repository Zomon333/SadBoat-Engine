#pragma once
#include "headers.h"
using namespace std;

enum eventType
{
		ERROR, //ErrorNumber, ErrorName, EventName, EventParent

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
		LOAD
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
		virtual bool handle();
	};
