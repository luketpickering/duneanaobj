add_subdirectory(Proxy)
# add_subdirectory(Flat)

if(NOT STANDALONE_BUILD)
  # for classes_def.xml!!
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  set( PACKAGE duneanaobj_StandardRecord )
  FILE( GLOB src_files *.cxx )

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
    StandardRecord.cxx
    SRLorentzVector.cxx
    SRTrack.cxx
    SRWeightGlobal.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/StandardRecordDict.cxx)

  SET(SR_HEADER_FILES
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

  add_library(duneanaobj_StandardRecord SHARED ${SR_IMPL_FILES})
  target_link_libraries(duneanaobj_StandardRecord PUBLIC ROOT::ROOT)
  target_include_directories(duneanaobj_StandardRecord PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
  set_target_properties(duneanaobj_StandardRecord PROPERTIES PUBLIC_HEADER "${SR_HEADER_FILES}")

  install(TARGETS duneanaobj_StandardRecord 
    DESTINATION lib 
    PUBLIC_HEADER DESTINATION include/duneanaobj/StandardRecord)
  install(FILES 
      ${CMAKE_CURRENT_BINARY_DIR}/libStandardRecordDict_rdict.pcm 
      ${CMAKE_CURRENT_BINARY_DIR}/libStandardRecordDict.rootmap 
    DESTINATION lib)
endif()
