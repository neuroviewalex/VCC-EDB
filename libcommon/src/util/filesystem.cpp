#include <vcc/util/fileutil.h>
#include <vcc/util/filesystem.h>
#include <filesystem>

//TODO filesystem.cpp is depreciated
//TODO move this it is used only in mpi/multipak_cartridge.cpp:

namespace VCC::Util
{
	std::string find_pak_module_path(std::string path)
	{
		if (path.empty())
		{
			return path;
		}

		auto file_handle(CreateFile(path.c_str(), 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
		if (file_handle == INVALID_HANDLE_VALUE)
		{
			const auto application_path = ::VCC::Util::GetDirectoryPart(::VCC::Util::ModulePath(NULL));
			// Compose the fallback path using filesystem semantics so the
			// directory separator is inserted when needed.
			const auto alternate_path = (std::filesystem::path(application_path) / std::filesystem::path(path)).string();
			file_handle = CreateFile(alternate_path.c_str(), 0, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (file_handle == INVALID_HANDLE_VALUE)
			{
				path = alternate_path;
			}
		}

		if (file_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file_handle);
		}

		return path;
	}
}
