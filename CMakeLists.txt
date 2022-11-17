# ======================================================================
#  larsoft main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/larsoft/ups/setup_for_development <-d|-p>
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


cmake_minimum_required (VERSION 3.12...3.18 FATAL_ERROR)

project(duneanaobj LANGUAGES CXX)
set(${PROJECT_NAME}_CMAKE_PROJECT_VERSION_STRING 03.01.00)

message(STATUS "\n\n  ==========================   ${PROJECT_NAME}   ==========================")


option(STANDALONE_BUILD "If <ON> do not rely on cetbuildtools/UPS" ON)

if(NOT STANDALONE_BUILD)

  if(DEFINED ENV{UPS_DIR})
      # cetbuildtools contains our cmake modules
      find_package(cetbuildtools REQUIRED)

      include(CetCMakeEnv)
      cet_cmake_env()

      set_install_root()

      # The specification of the C++ standard is required because CET macros
      # set it to C++98 by default, and then override it depending on the
      # compiler version, as determined by the compiler qualifier.
      # Removing that qualifier, we need to supply that information manually,
      # and maintain it too.
      cet_set_compiler_flags(DIAGS CAUTIOUS
        WERROR
        NO_UNDEFINED
        ALLOW_DEPRECATIONS
        EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs -Wno-undefined-var-template -I $ENV{CLANG_FQ_DIR}/include/c++/v1
      )

      cet_report_compiler_flags()

      # these are minimum required versions, not the actual product versions
      find_ups_root(v6_00_00)
      find_ups_product( cetbuildtools )

      include(BuildDictionary)

      # packaging utility
      include(UseCPack)


  else()
      find_package(ROOT)
      include_directories(.)
  endif()
else()
  LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)
  include(CPM)

  CPMAddPackage(
    NAME CMakeModules
    GIT_TAG v0.2.2
    GITHUB_REPOSITORY NuHepMC/CMakeModules
    DOWNLOAD_ONLY
  )
  include(${CMakeModules_SOURCE_DIR}/NuHepMCModules.cmake)

  CPMAddPackage(
    NAME SRProxy
    GIT_TAG master
    GITHUB_REPOSITORY luketpickering/SRProxy
  )
  include(${SRProxy_SOURCE_DIR}/cmake/Modules/GenSRProxy.cmake)

  include(ROOT)
endif()

add_subdirectory(duneanaobj)

add_library(duneanaobj INTERFACE)
target_link_libraries(duneanaobj INTERFACE duneanaobj_StandardRecordProxy duneanaobj_StandardRecord)

if(NOT STANDALONE_BUILD)
  add_subdirectory(ups)
  # packaging utility
  include(UseCPack)
endif()
