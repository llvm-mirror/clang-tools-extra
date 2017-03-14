option(BUILD_CLANG_TIDY_VS_PLUGIN "Build clang-tidy VS plugin" OFF)
if (BUILD_CLANG_TIDY_VS_PLUGIN)
  add_custom_target(clang_tidy_exe_for_vsix
      ${CMAKE_COMMAND} -E copy_if_different
      "${LLVM_TOOLS_BINARY_DIR}/clang-tidy.exe"
      "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy/clang-tidy.exe"
      DEPENDS clang-tidy)

  add_custom_target(clang_tidy_license
      ${CMAKE_COMMAND} -E copy_if_different
      "${CLANG_SOURCE_DIR}/LICENSE.TXT"
      "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy/license.txt")

  # Build number added to Clang version to ensure that new VSIX can be upgraded
  string(TIMESTAMP CLANG_TIDY_VSIX_BUILD %y%m%d%H%M UTC)

  if (NOT CLANG_TIDY_VS_VERSION)
    set(CLANG_TIDY_VS_VERSION "${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}.${CLANG_TIDY_VSIX_BUILD}")
  endif()

  configure_file("source.extension.vsixmanifest.in"
      "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy/source.extension.vsixmanifest")

  find_program(NUGET_EXE nuget PATHS ${NUGET_EXE_DIR})
  if (NOT NUGET_EXE)
    message(FATAL_ERROR "Could not find nuget.exe. Download from https://www.nuget.org/nuget.exe"
                        " and add parent directory to PATH or pass it via NUGET_EXE_DIR var.")
  endif()

  add_custom_target(clang_tidy_vsix ALL
      COMMAND ${NUGET_EXE} restore "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy.sln"
      COMMAND devenv "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy.sln" /Build Release
      DEPENDS clang_tidy_exe_for_vsix "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy/source.extension.vsixmanifest"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_SOURCE_DIR}/ClangTidy/bin/Release/ClangTidy.vsix"
      "${LLVM_TOOLS_BINARY_DIR}/ClangTidy.vsix"
      DEPENDS clang_tidy_exe_for_vsix clang_tidy_license)
endif()
