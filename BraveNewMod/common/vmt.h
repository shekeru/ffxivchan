#pragma once

#define VMethod(ix, type, Name, ...) \
	class Name { public: \
		static const USHORT Index = ix; \
		static type Function(__VA_ARGS__); }
#define get_virtual(VMT_PTR) \
	static auto original = decltype(&Function)(VMT_PTR->original[Index])

namespace vMethods {
	namespace DXGI_Factory {
		VMethod(10, HRESULT, CreateSwapChain, IDXGIFactory*, ID3D11Device*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
	};
	namespace SwapChain {
		VMethod(8, HRESULT, Present, IDXGISwapChain*, UINT, UINT);
	};
};

class LockedRegion {
	// https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualprotect
public:
	LockedRegion(void* base, size_t length, uint32_t flags) {
		if (!VirtualProtect(_base = base, _length = length, flags, (PDWORD)&_old))
			throw std::runtime_error("Failed to protect region.");
	};
	~LockedRegion() {
		VirtualProtect(_base, _length, _old, (PDWORD)&_old);
	};
	static bool can_execute(uintptr_t memory) {
		if (!memory || memory > 0x7fffffffffff)
			return false;

		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(LPCVOID(memory), &info, sizeof(MEMORY_BASIC_INFORMATION));
		return info.AllocationProtect & PAGE_EXECUTE_WRITECOPY;
	};
private:
	void* _base; size_t _length; uint32_t _old;
};

class VMT
{
public:
	uintptr_t* replacement;
	uintptr_t* original = nullptr;
	uintptr_t** object = nullptr;
	size_t method_count = 0;
	VMT(void* instance)
	{
		original = *(object = reinterpret_cast<uintptr_t**>(instance));

		while (LockedRegion::can_execute(original[method_count]))
			method_count++;

		replacement = new uintptr_t[method_count + 1]();
		memcpy(&replacement[1], original, sizeof(uintptr_t) * method_count);
		printf("vm Total: %p => %d\n", object, method_count);
	}
	template <class VFunc>
	void ReplaceMethod()
	{
		replacement[VFunc::Index + 1] = reinterpret_cast<uintptr_t>(VFunc::Function);
	}
	void ApplyVMT()
	{
		auto guard = LockedRegion{ object, sizeof(uintptr_t), PAGE_READWRITE };
		*replacement = original[-1];
		*object = &replacement[1];
		printf("\t VMT Applied!\n");
	}
	void ReleaseVMT()
	{
		auto guard = LockedRegion{ object, sizeof(uintptr_t), PAGE_READWRITE };
		if (object && *object && original)
			*this->object = original;
		printf("\t VMT Released!\n");
	}
	~VMT() {
		ReleaseVMT();
		delete[] replacement;
	}
};