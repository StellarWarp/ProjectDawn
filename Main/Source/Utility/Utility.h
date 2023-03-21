#pragma once

#include "Libraries/Libs.h"

namespace Utility
{

	// 以下不可删除
#pragma warning(push)
#pragma warning(disable: 28251)
	extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char* str, int cbmb, wchar_t* widestr, int cchwide);
	extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t* widestr, int cchwide, char* str, int cbmb, const char* defchar, int* used_default);
#pragma warning(pop)

	inline std::wstring UTF8ToWString(std::string_view utf8str)
	{
		if (utf8str.empty()) return std::wstring();
		int cbMultiByte = static_cast<int>(utf8str.size());
		int req = MultiByteToWideChar(65001, 0, utf8str.data(), cbMultiByte, nullptr, 0);
		std::wstring res(req, 0);
		MultiByteToWideChar(65001, 0, utf8str.data(), cbMultiByte, &res[0], req);
		return res;
	}

	inline std::string WStringToUTF8(std::wstring_view wstr)
	{
		if (wstr.empty()) return std::string();
		int cbMultiByte = static_cast<int>(wstr.size());
		int req = WideCharToMultiByte(65001, 0, wstr.data(), cbMultiByte, nullptr, 0, nullptr, nullptr);
		std::string res(req, 0);
		WideCharToMultiByte(65001, 0, wstr.data(), cbMultiByte, &res[0], req, nullptr, nullptr);
		return res;
	}

	inline std::filesystem::path ProjectPath() {
		namespace fs = std::filesystem;
		auto srcPath = fs::path{ __FILE__ };

		while (srcPath.filename() != L"Main") {
			srcPath = srcPath.parent_path();
		}
		return srcPath;
	}

	inline std::filesystem::path RecursiveFind(std::wstring_view dir, std::wstring_view file_name)
	{
		namespace fs = std::filesystem;
		for (const auto& entry : fs::recursive_directory_iterator(dir)) {
			if (entry.is_regular_file() && entry.path().filename().stem() == file_name) {
				return entry.path();
			}
		}
		return {};
	}

	inline std::filesystem::path RecursiveFind(std::filesystem::path dir, std::wstring_view file_name)
	{
		namespace fs = std::filesystem;
		for (const auto& entry : fs::recursive_directory_iterator(dir)) {
			if (entry.is_regular_file() && entry.path().filename().stem() == file_name) {
				return entry.path();
			}
		}
		return {};
	}

	namespace {
		template<int N>
		struct flag {
			friend constexpr int adl_flag(flag<N>);
		};
		template<int N>
		struct writer {
			friend constexpr int adl_flag(flag<N>) {
				return N;
			}

			static constexpr int value = N;
		};
		template<int N, int = adl_flag(flag<N> {}) >
		int constexpr reader(int, flag<N>) {
			return N;
		}

		template<int N>
		int constexpr reader(float, flag<N>, int R = reader(0, flag<N - 1> {})) {
			return R;
		}

		int constexpr reader(float, flag<0>) {
			return 0;
		}
	}

	template<int N = 1>
	int constexpr next(int R = writer < reader(0, flag<32> {}) + N > ::value) {
		return R;
	}
}