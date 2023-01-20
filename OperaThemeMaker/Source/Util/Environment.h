#pragma once

#include <windows.h>
#include <shlobj.h>

#include <filesystem>

namespace OperaThemeMaker
{
	namespace Util
	{

        /*
         * Modified Code from Stack Overflow Question: https://stackoverflow.com/questions/5920853/how-to-open-a-folder-in-appdata-with-c
         * Author Profile: https://stackoverflow.com/users/4809297/phil-loctaux
         */

        inline std::filesystem::path GetAppDataRoaming()
        {
	        PWSTR path_tmp;

            /* Attempt to get user's AppData folder
             *
             * Microsoft Docs:
             * https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
             * https://learn.microsoft.com/en-us/windows/win32/shell/knownfolderid
             */
            const auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);

            /* Error check */
            if (get_folder_path_ret != S_OK)
            {
                CoTaskMemFree(path_tmp);
            }

            /* Convert the Windows path type to a C++ path */
            std::filesystem::path path = path_tmp;

            /* Free memory :) */
            CoTaskMemFree(path_tmp);

            return path;
        }

	}
}