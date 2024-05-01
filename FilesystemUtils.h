#pragma once

#include <filesystem>

#ifdef WIN32
#	include "shlobj.h"
#endif

namespace panda
{
	namespace FilesystemUtils
	{
		std::filesystem::path appDataPath()
		{
#ifdef WIN32
			std::filesystem::path path;
			PWSTR path_tmp;
			auto folder = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);

			/* Error check */
			if (folder != S_OK)
			{
				CoTaskMemFree(path_tmp);
				return "";
			}
			path = path_tmp;
			CoTaskMemFree(path_tmp);
			return path;
#else
			return "";
#endif
		}
	}
}