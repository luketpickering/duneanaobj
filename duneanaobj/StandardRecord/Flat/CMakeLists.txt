if(NOT STANDALONE_BUILD)
  FILE(GLOB SR_DEPENDENCIES duneanaobj/StandardRecord/*.h)

  add_custom_command(# Rebuild if anything in StandardRecord/ changes
                     DEPENDS ${SR_DEPENDENCIES}
                     OUTPUT FlatRecord.cxx FlatRecord.h FwdDeclare.h
                     COMMAND gen_srproxy --flat -i duneanaobj/StandardRecord/StandardRecord.h -o FlatRecord --target caf::StandardRecord --include-path $ENV{DUNEANAOBJ_DIR}:$ENV{ROOT_INC} --output-path duneanaobj/StandardRecord/Flat/ --extra-cflags ' -D_Float16=short'
    )

  include_directories($ENV{SRPROXY_INC})

  cet_make_library(LIBRARY_NAME duneanaobj_StandardRecordFlat
                   SOURCE       FlatRecord.cxx
                   LIBRARIES    ${ROOT_BASIC_LIB_LIST} ROOT::TreePlayer
                   )

  install_headers(EXTRAS $ENV{MRB_BUILDDIR}/duneanaobj/duneanaobj/StandardRecord/Flat/FlatRecord.h $ENV{MRB_BUILDDIR}/duneanaobj/duneanaobj/StandardRecord/Flat/FwdDeclare.h)
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
  target_link_libraries(duneanaobj_StandardRecordFlat 
    PUBLIC duneanaobj::StandardRecord SRProxy::FlatTypes)

  set_target_properties(duneanaobj_StandardRecordFlat PROPERTIES EXPORT_NAME duneanaobj::StandardRecordFlat)
  add_library(duneanaobj::StandardRecordFlat ALIAS duneanaobj_StandardRecordFlat)

  install(TARGETS duneanaobj_StandardRecordFlat EXPORT duneanaobjTargets DESTINATION lib)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SRFlat.h 
                ${CMAKE_CURRENT_BINARY_DIR}/FwdDeclare.h 
          DESTINATION include/duneanaobj/StandardRecord/Flat)

endif()