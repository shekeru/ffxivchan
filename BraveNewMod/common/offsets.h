#pragma once
#include "Actor.h"
#include "JobGauge.h"
#include "context.h"
#include "memory.h"

namespace Offsets {
	inline auto security_check_cookie = FollowPattern
		("e8 ? ? ? ? 4c 8d 9c 24 80 03 00", 1);

	inline auto LocalActorPtr = ReferencePattern<Actor*>
		("89 51 68 48 8b 05 ? ? ? ?", 6);

	inline auto GameObjectMgr = FollowPattern<GameObjectManager>
		("48 8D 35 ? ? ? ? 81 FA", 3);

	inline auto TargetSys = FollowPattern<TargetSystem>
		("48 8D 0D ? ? ? ? E8 ? ? ? ? 48 3B C6 0F 95 C0", 3);

	inline auto JobGaugePtr = FollowPattern<JobGaugeManager>
		("48 8B 3D ? ? ? ? 33 ED", 3);
	
	// Get Icon Found Below IsIconReplaceable
	inline auto IsIconReplaceable = FollowPattern
		("E8 ? ? ? ? 84 C0 74 4C 8B D3", 1);
	// Alt: via massive case statement
	// 8D 42 EB 3D FC 00
	inline auto GetIcon = FollowPattern
		("E8 ? ? ? ? 8B F8 3B DF", 1);

	inline auto GetRecastTime = FollowPattern
		("E8 ? ? ? ? 83 7F 4C 01 44 0F 28 C8", 1);

	inline auto GetRecastIndex = FollowPattern
		("48 8b f9 e8 ? ? ? ? 8b d0 48 8b cf e8", 4);

	inline auto GetRecastStruct = FollowPattern
		("48 8b cf e8 ? ? ? ? 48 8b d8 48 85 c0 75 04 32 c0", 4);

	inline auto AllowRequestsGCD = FollowPattern
		("44 8b c3 41 8b d5 48 8b cf e8 ? ? ? ? 84 c0 74 37", 10);

	inline auto RequestAction = FollowPattern
		("E8 ? ? ? ? EB 64 B1 01", 1);

};

namespace Globals {
	inline Actor*& LocalActor = Offsets::LocalActorPtr.Resolve(Context::game);
	inline GameObjectManager* GameObjectMgr = Offsets::GameObjectMgr.Resolve(Context::game);
	inline TargetSystem* TargetSys = Offsets::TargetSys.Resolve(Context::game);
};