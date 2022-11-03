$vcpkg_folder = "$pwd\vcpkg"
$vcpkg_exe = "$vcpkg_folder\vcpkg.exe"

$git_clone_cmd = "git clone https://github.com/microsoft/vcpkg $vcpkg_folder"
$vcpkg_install = ("$vcpkg_exe install" +
				" --vcpkg-root $vcpkg_folder" +
				" --x-manifest-root=`"$vcpkg_folder`"" +
				" --x-install-root=`"$vcpkg_folder\installed`"" +
				" --clean-after-build")
$cmake_configure = "cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$vcpkg_folder/scripts/buildsystems/vcpkg.cmake .."
$cmake_build = "cmake --build . --config Release"

Invoke-Expression "$git_clone_cmd"
Copy-Item "$pwd\vcpkg.json" "$vcpkg_folder"
Invoke-Expression "cmd.exe /c $vcpkg_folder\bootstrap-vcpkg.bat"
Invoke-Expression "$vcpkg_install"
Invoke-Expression "$cmake_configure"
Invoke-Expression "$cmake_build"