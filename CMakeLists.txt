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

project(duneanaobj LANGUAGES CXX VERSION 02.02.00)
set(${PROJECT_NAME}_CMAKE_PROJECT_VERSION_STRING ${PROJECT_VERSION})

message(STATUS "\n\n  ==========================   ${PROJECT_NAME}   ==========================")

option(STANDALONE_BUILD "If <ON> do not rely on cetbuildtools/UPS" OFF)

if(NOT STANDALONE_BUILD)
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
else()
  LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)
  include(CPM)

  CPMFindPackage(
    NAME SRProxy
    GIT_TAG feature/Clingification
    GITHUB_REPOSITORY luketpickering/SRProxy
  )
  if(NOT COMMAND GenSRProxy)
    include(${SRProxy_SOURCE_DIR}/cmake/Modules/GenSRProxy.cmake)
  endif()

endif()

add_subdirectory(duneanaobj)

if(NOT STANDALONE_BUILD)
  add_subdirectory(ups)
  # packaging utility
  include(UseCPack)
else()

  add_library(duneanaobj INTERFACE)
  target_link_libraries(duneanaobj INTERFACE duneanaobj_StandardRecordProxy duneanaobj_StandardRecordFlat)
  set_target_properties(duneanaobj PROPERTIES EXPORT_NAME duneanaobj::All)
  install(TARGETS duneanaobj EXPORT duneanaobjTargets)

  add_library(duneanaobj::All ALIAS duneanaobj)

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
      "${PROJECT_BINARY_DIR}/duneanaobjConfigVersion.cmake"
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY AnyNewerVersion
  )
  configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/duneanaobjConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/duneanaobjConfig.cmake"
    INSTALL_DESTINATION this/is/ignored
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

  install(EXPORT duneanaobjTargets
          DESTINATION lib/cmake/duneanaobj)
  install(FILES "${PROJECT_BINARY_DIR}/duneanaobjConfigVersion.cmake"
                "${PROJECT_BINARY_DIR}/duneanaobjConfig.cmake"
          DESTINATION lib/cmake/duneanaobj)

  configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/setup.duneanaobj.sh.in ${PROJECT_BINARY_DIR}/setup.duneanaobj.sh @ONLY)
  install(PROGRAMS ${PROJECT_BINARY_DIR}/setup.duneanaobj.sh DESTINATION bin)
endif()
