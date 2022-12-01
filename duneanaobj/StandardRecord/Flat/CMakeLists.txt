if(NOT STANDALONE_BUILD)
  FILE(GLOB SR_DEPENDENCIES duneanaobj/StandardRecord/*.h)

    add_custom_command(# Rebuild if anything in StandardRecord/ changes
                   DEPENDS ${SR_DEPENDENCIES}
                   OUTPUT FlatRecord.cxx FlatRecord.h FwdDeclare.h
                   COMMAND gen_srproxy --flat -i duneanaobj/StandardRecord/StandardRecord.h -o FlatRecord --target caf::StandardRecord --include-path ${PROJECT_SOURCE_DIR}:$ENV{ROOT_INC} --output-path duneanaobj/StandardRecord/Flat/ --prolog ${CMAKE_CURRENT_SOURCE_DIR}/Prolog.h --extra-cflags ' -D_Float16=short -fsized-deallocation'
  )

  include_directories($ENV{SRPROXY_INC})

  if(DEFINED CETMODULES_CURRENT_PROJECT_NAME)
      cet_make_library(LIBRARY_NAME duneanaobj_StandardRecordFlat
                       SOURCE       FlatRecord.cxx
                       LIBRARIES    ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer
                       )
      if (DEFINED ENV{MRB_BUILDDIR} AND NOT "$ENV{MRB_BUILDDIR}" STREQUAL "")
          set(builddir $ENV{MRB_BUILDDIR}/duneanaobj)
      else()
          set(builddir ${CMAKE_BINARY_DIR})
      endif()
      install_headers(EXTRAS ${builddir}/duneanaobj/StandardRecord/Flat/FlatRecord.h ${builddir}/duneanaobj/StandardRecord/Flat/FwdDeclare.h)
  else()
      add_library(duneanaobj_StandardRecordFlat
                  FlatRecord.cxx)
      target_link_libraries(duneanaobj_StandardRecordFlat ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer)

      install(FILES ${CMAKE_BINARY_DIR}/duneanaobj/StandardRecord/Flat/FlatRecord.h ${CMAKE_BINARY_DIR}/duneanaobj/StandardRecord/Flat/FwdDeclare.h
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/duneanaobj)

  endif()
else()
  GenSRProxy(
    FLAT
    HEADER duneanaobj/StandardRecord/StandardRecord.h
    OUTPUT_NAME SRFlat
    TARGETNAME caf::StandardRecord
    INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../ ${ROOT_INCLUDE_DIRS}
    DEPENDENCIES ${SR_HEADER_FILES}
    VERBOSE)

  add_library(duneanaobj_StandardRecordFlat SHARED SRFlat.cxx)
  target_link_libraries(duneanaobj_StandardRecordFlat PUBLIC SRProxy::FlatTypes)

  target_include_directories(duneanaobj_StandardRecordFlat PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> 
    $<INSTALL_INTERFACE:include>)

  install(TARGETS duneanaobj_StandardRecordFlat DESTINATION lib)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SRFlat.h 
                ${CMAKE_CURRENT_BINARY_DIR}/FwdDeclare.h 
          DESTINATION include/duneanaobj/StandardRecord/Flat)

endif()
