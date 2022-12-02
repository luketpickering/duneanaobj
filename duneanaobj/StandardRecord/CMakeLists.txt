execute_process(COMMAND git rev-parse --short=8 HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} 
  OUTPUT_VARIABLE DUNEANAOBJ_GITSHORTHASH8
  RESULT_VARIABLE GIT_CMD_SUCCESS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT GIT_CMD_SUCCESS EQUAL 0)
  message(FATAL_ERROR "Failed to run git rev-parse --short=8 HEAD in ${CMAKE_SOURCE_DIR}")
endif()

configure_file(StandardRecord.cxx.in ${CMAKE_CURRENT_BINARY_DIR}/StandardRecord.cxx @ONLY)

if(NOT STANDALONE_BUILD)
  # for classes_def.xml!!
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  set( PACKAGE duneanaobj_StandardRecord )
  FILE( GLOB src_files *.cxx ${CMAKE_CURRENT_BINARY_DIR}/StandardRecord.cxx )

  # For this directory only, pedantic option to catch uninitialized SR fields
  # add_compile_options(-Weffc++)

  if(DEFINED CETMODULES_CURRENT_PROJECT_NAME)
      cet_make_library( LIBRARY_NAME duneanaobj_StandardRecord
                        SOURCE       ${src_files}
                        LIBRARIES    ${ROOT_BASIC_LIB_LIST}
                      )

      build_dictionary( duneanaobj_StandardRecord
                        DICTIONARY_LIBRARIES duneanaobj_StandardRecord
                      )

      install_headers()
      install_source()
  else()
      add_library(duneanaobj_StandardRecord
                  ${src_files})
      target_link_libraries(duneanaobj_StandardRecord
                       ${ROOT_LIBRARIES})

      # n.b.: missing the dictionary and installation!
  endif()

else()

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)

  ROOT_GENERATE_DICTIONARY(StandardRecordDict StandardRecord.h
    LINKDEF classes_def.xml)

  SET(SR_IMPL_FILES
    SRGlobal.cxx
    SRSystParamHeader.cxx
    SRVector3D.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/StandardRecord.cxx
    SRLorentzVector.cxx
    SRTrack.cxx
    SRWeightGlobal.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/StandardRecordDict.cxx)

  SET(SR_HEADER_FILES_REL
    SRGAr.h
    SRGlobal.h
    SRLorentzVector.h
    SRNDBranch.h
    SRNDLAr.h
    SRNDTrackAssn.h
    SRParticleTruth.h
    SRShower.h
    SRSystParamHeader.h
    SRTMS.h
    SRTrack.h
    SRVector3D.h
    SRWeightGlobal.h
    StandardRecord.h)

  set(SR_HEADER_FILES)
  foreach(HDR ${SR_HEADER_FILES_REL})
    list(APPEND SR_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HDR})
  endforeach()

  add_library(duneanaobj_StandardRecord SHARED ${SR_IMPL_FILES})
  target_link_libraries(duneanaobj_StandardRecord PUBLIC ROOT::Physics ROOT::Core)
  target_include_directories(duneanaobj_StandardRecord PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
  set_target_properties(duneanaobj_StandardRecord PROPERTIES 
    PUBLIC_HEADER "${SR_HEADER_FILES_REL}" 
    EXPORT_NAME duneanaobj::StandardRecord)

  add_library(duneanaobj::StandardRecord ALIAS duneanaobj_StandardRecord)

  install(TARGETS duneanaobj_StandardRecord
    EXPORT duneanaobjTargets
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include/duneanaobj/StandardRecord)
  install(FILES 
      ${CMAKE_CURRENT_BINARY_DIR}/StandardRecordDict.rootmap 
      ${CMAKE_CURRENT_BINARY_DIR}/StandardRecordDict_rdict.pcm
    DESTINATION lib)
endif()

add_subdirectory(Flat)
add_subdirectory(Proxy)
