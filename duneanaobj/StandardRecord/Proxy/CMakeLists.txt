if(NOT STANDALONE_BUILD)
  FILE(GLOB SR_DEPENDENCIES duneanaobj/StandardRecord/*.h)

  add_custom_command(# Rebuild if anything in StandardRecord/ changes
                     DEPENDS ${SR_DEPENDENCIES}
                     OUTPUT SRProxy.cxx SRProxy.h FwdDeclare.h
                     COMMAND gen_srproxy -i duneanaobj/StandardRecord/StandardRecord.h 
                                         -o SRProxy --target caf::StandardRecord 
                                         --include-path $ENV{DUNEANAOBJ_DIR}:$ENV{ROOT_INC} 
                                         --output-path duneanaobj/StandardRecord/Proxy/ 
                                         --epilog-fwd ${CMAKE_CURRENT_SOURCE_DIR}/EpilogFwd.h 
                                         --extra-cflags ' -D_Float16=short'
    )

  include_directories($ENV{SRPROXY_INC})

  # This is a very picky error buried inside template instantiations
  #add_definitions(-Wno-int-in-bool-context)

  cet_make_library(LIBRARY_NAME duneanaobj_StandardRecordProxy
                   SOURCE       SRProxy.cxx Instantiations.cxx
                   LIBRARIES    ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer)

  install_headers(EXTRAS $ENV{MRB_BUILDDIR}/duneanaobj/duneanaobj/StandardRecord/Proxy/SRProxy.h 
    $ENV{MRB_BUILDDIR}/duneanaobj/duneanaobj/StandardRecord/Proxy/FwdDeclare.h)

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
  target_link_libraries(duneanaobj_StandardRecordProxy PUBLIC SRProxy_BasicTypes)

  target_include_directories(duneanaobj_StandardRecordProxy PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> 
    $<INSTALL_INTERFACE:include>)

  install(TARGETS duneanaobj_StandardRecordProxy DESTINATION lib)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SRProxy.h 
                ${CMAKE_CURRENT_BINARY_DIR}/FwdDeclare.h 
          DESTINATION include/duneanaobj/StandardRecord/Proxy)
endif()