#pragma once
// Pointer Class
class IntPtr {
public:
	uintptr_t value;
	IntPtr(uintptr_t value) {
		this->value = value;
	};

	IntPtr operator[](uintptr_t next) const {
		return IntPtr(*(uintptr_t*)(value + next));
	};

	template<typename obj>
	obj Cast() {
		return (obj)value;
	};
};
// Memory Scanning
class MemorySystem {
public:
	MODULEINFO baseModule;
	MemorySystem(LPCWSTR exe_name = L"ffxiv_dx11.exe");
	UCHAR* FindPattern(const char* signature, int start = 0);

	//	IntPtr ResolvePattern(const char* signature, int start, int extra = 0) {
		//	auto offset = FindPattern<int>(signature, start); return
			//	IntPtr(*offset + uintptr_t(offset) + 4 + extra);
		//};

		//void DetourAll();
	void StackTrace();
private:
	static std::vector<short> SignatureArray(char* ptr);
};

template<typename DefaultType = void>
class FindPattern {
public:
	const char* mask;
	int start;

	FindPattern(const char* byte_mask, int start_idx) {
		mask = byte_mask, start = start_idx;
	};

	template<typename Type = DefaultType>
	Type* Resolve(MemorySystem& game) {
		return (Type*)game.FindPattern(mask, start);
	};
};

template<class Pattern, typename Method>
class ExternalFn {
public:
	Method* original;
	Method* replacement;
	Pattern* pattern;

	ExternalFn(Pattern* pattern, Method* function) {
		this->pattern = pattern;
		this->replacement = function;
	}
};

template<typename DefaultType = void>
class FollowPattern : FindPattern<DefaultType> {
public:
	int extra;

	FollowPattern(
		const char* byte_mask, int start_idx, int asm_extra = 0
	) : FindPattern<DefaultType>(byte_mask, start_idx) {
		extra = asm_extra;
	};

	template<typename Type = DefaultType>
	Type* Resolve(MemorySystem& game) {
		INT32* location = FindPattern<DefaultType>::Resolve<INT32>(game);
		// Return RIP -> Absolute Memory
		return (Type*)(*location
			// Using start of displacement
			+ size_t(location)
			// Adjusted to next instruction
			+ 4 + extra);
	};

	ExternalFn<FollowPattern, DefaultType> ToFunction(DefaultType* FnPtr) {
		return ExternalFn(this, FnPtr);
	};

};

template <typename Type>
class ReferencePattern : FollowPattern<Type> {
public:

	ReferencePattern(
		const char* byte_mask, int start_idx, int asm_extra = 0
	) : FollowPattern<Type>(byte_mask, start_idx, asm_extra) {};

	Type& Resolve(MemorySystem& game) {
		return *FollowPattern<Type>::Resolve(game);
	};
};