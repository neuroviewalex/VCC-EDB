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

		// Relative MPI plugin/ROM paths are always rooted at VCC.exe.
		// Do not probe the process working directory first.
		return ::VCC::Util::QualifyModPath(path);
	}
}
