if(NOT STANDALONE_BUILD)
  FILE(GLOB SR_DEPENDENCIES duneanaobj/StandardRecord/*.h)

add_custom_command(# Rebuild if anything in StandardRecord/ changes
                   DEPENDS ${SR_DEPENDENCIES}
                   OUTPUT SRProxy.cxx SRProxy.h FwdDeclare.h
                   COMMAND gen_srproxy -i duneanaobj/StandardRecord/StandardRecord.h -o SRProxy --target caf::StandardRecord --include-path ${PROJECT_SOURCE_DIR}:$ENV{ROOT_INC} --output-path duneanaobj/StandardRecord/Proxy/ --prolog ${CMAKE_CURRENT_SOURCE_DIR}/Prolog.h --epilog-fwd ${CMAKE_CURRENT_SOURCE_DIR}/EpilogFwd.h --extra-cflags ' -D_Float16=short -fsized-deallocation'
  )

  include_directories($ENV{SRPROXY_INC})

  # This is a very picky error buried inside template instantiations
  #add_definitions(-Wno-int-in-bool-context)

  if(DEFINED CETMODULES_CURRENT_PROJECT_NAME)
      message(STATUS "CETMODULES_CURRENT_PROJECT_NAME = '${CETMODULES_CURRENT_PROJECT_NAME}'")
      cet_make_library(LIBRARY_NAME duneanaobj_StandardRecordProxy
              SOURCE       SRProxy.cxx Instantiations.cxx
              LIBRARIES    ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer)

      if (DEFINED ENV{MRB_BUILDDIR} AND NOT "$ENV{MRB_BUILDDIR}" STREQUAL "")
          message(STATUS "MRB_BUILDDIR = $ENV{MRB_BUILDDIR}")
          set(builddir $ENV{MRB_BUILDDIR}/duneanaobj)
      else()
          set(builddir ${CMAKE_BINARY_DIR})
      endif()
      install_headers(EXTRAS ${builddir}/duneanaobj/StandardRecord/Proxy/SRProxy.h ${builddir}/duneanaobj/StandardRecord/Proxy/FwdDeclare.h)
  else()
      add_library(duneanaobj_StandardRecordProxy
                  SRProxy.cxx Instantiations.cxx)
      target_link_libraries(duneanaobj_StandardRecordProxy ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer)

      install(FILES ${CMAKE_BINARY_DIR}/duneanaobj/StandardRecord/Proxy/SRProxy.h ${CMAKE_BINARY_DIR}/duneanaobj/StandardRecord/Proxy/FwdDeclare.h
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/duneanaobj)
  endif()


else()

  GenSRProxy(
    HEADER duneanaobj/StandardRecord/StandardRecord.h
    OUTPUT_NAME SRProxy
    TARGETNAME caf::StandardRecord
    EPILOG_FWD ${CMAKE_CURRENT_SOURCE_DIR}/EpilogFwd.h
    INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../ ${ROOT_INCLUDE_DIRS}
    DEPENDENCIES ${SR_HEADER_FILES}
    VERBOSE)

  add_library(duneanaobj_StandardRecordProxy SHARED SRProxy.cxx)
  target_link_libraries(duneanaobj_StandardRecordProxy 
    PUBLIC duneanaobj::StandardRecord SRProxy::BasicTypes)

  set_target_properties(duneanaobj_StandardRecordProxy PROPERTIES 
    EXPORT_NAME duneanaobj::StandardRecordProxy)
  add_library(duneanaobj::StandardRecordProxy ALIAS duneanaobj_StandardRecordProxy)

  install(TARGETS duneanaobj_StandardRecordProxy EXPORT duneanaobjTargets DESTINATION lib)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SRProxy.h 
                ${CMAKE_CURRENT_BINARY_DIR}/FwdDeclare.h 
          DESTINATION include/duneanaobj/StandardRecord/Proxy)
endif()
