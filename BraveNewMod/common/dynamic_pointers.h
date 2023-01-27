#pragma once

namespace Dynamic {
	class Pointer {
	public:
		Pointer(FARPROC ptr) : _ptr(ptr) {};
		template <typename T, typename = std::
			enable_if_t<std::is_function_v<T>>>
		operator T* () const {
			return reinterpret_cast<T*>(_ptr);
		};
	private:
		FARPROC _ptr;
	};

	class Library {
	public:
		Library(LPCWSTR file_name) {
			_module = LoadLibrary(file_name);
		};
		Pointer operator[](LPCSTR proc_name) const {
			return Pointer(GetProcAddress(_module, proc_name));
		}; HMODULE _module;
	};
};