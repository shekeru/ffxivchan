#pragma once
#include <../Detours/src/detours.h>
#include "offsets.h"
#include "objects.h"
#include "context.h"

// Hooking Macros
//#define ReplaceFN(type, NAME, ...) \
//	class NAME { public: \
//		inline static PVOID Original; \
//		static type Function(__VA_ARGS__); \
//		static PVOID GetLocation() { \
//			return Offsets::NAME.Resolve(Context::Current().game);}; \
//		static bool AttachHook() { \
//			Original = decltype(&Function)(GetLocation()); \
//			printf(__FUNCTION__ " replacing %p\n", Original); \
//		if (Original) return DetourAttach(&(PVOID&)Original, Function); };\
//		static bool ReleaseHook() { \
//			return DetourDetach(&(PVOID&)Original, Function);} }

#define get_original \
	static auto original = decltype(&Function)(Original)

#define display(str, ...) \
	printf("\n" __FUNCTION__ "(" str ")\n", __VA_ARGS__)
#define print(str, ...) \
	printf(" [+] " str "\n", __VA_ARGS__)

#define ReplaceFN(type, NAME, ...) \
	namespace NAME { \
		type Function(__VA_ARGS__); \
		extern decltype(&Function) Original; \
		inline bool ReleaseHook() { return DetourDetach(&(PVOID&)Original, Function); }; \
		inline PVOID GetLocation() { return Offsets::NAME.Resolve(Context::Current().game); }; \
		inline bool AttachHook() { \
			Original = decltype(&Function)(GetLocation()); \
			printf(__FUNCTION__ " replacing %p\n", Original); \
			if (Original) return DetourAttach(&(PVOID&)Original, Function); }; \
	}
#define $set_original(NAME) \
	decltype(&NAME::Function) NAME::Original;

namespace IconSwaps {
	ReplaceFN(char, IsIconReplaceable, UINT);
	ReplaceFN(UINT64, GetIcon, ActionSys*, UINT);
};

//ReplaceFN_2(void, On_Spawn_1, void*, __int64, UCHAR);
//ReplaceFN_2(char, Smithing_Related_1, __int64, ShopItem_A*);
//ReplaceFN_2(float, Get_Discovery_Base, float, UINT);
//ReplaceFN_2(float, Sum_Discovery_Buffs, void*);

//template <typename Method, Method FnPtr>
//class ReplaceFunction {
//	Method Original;
//	Method Replacement = FnPtr;
//	int x;
//public:
//	ReplaceFunction(Method& Function) {
//		Replacement = &Function;
//	}
//	ReplaceFunction FindPattern(const char* byte_mask, int start_idx) {
//		FindPattern<Method>(byte_mask, start_idx);
//		return this;
//	};
//	ReplaceFunction FollowPattern(const char* byte_mask, int start_idx, int asm_extra = 0) {
//		FollowPattern<Method>(byte_mask, start_idx, asm_extra);
//		return this;
//	};
//	template<typename T>
//	bool AttachHook() {
//		printf(__FUNCTION__ " replacing %p\n", Original);
//		if (Original) 
//			return DetourAttach(&(PVOID&)Original, Replacement);
//		return false;
//	}; 
//};
//
//void Stage_Load_1(void* _this, __int64 p2, UCHAR drop_runes) {
//
//};
//
//auto a = ReplaceFunction < , Stage_Load_1,
//	FollowPattern<decltype(&Stage_Load_1)>
//	("48 8b 89 ? ? ? ? 41 b0 01 48 8b d3 e8 ? ? ? ?", 14).AsFunction();
//
//ReplaceFunction<
//	decltype(Stage_Load_1), Stage_Load_1, FollowPattern
//>
//	(5);

//inline auto TEST = FnGeneric(On_Spawn_1::Function, Offsets::On_Spawn_1);