
if(NOT WIN32)
    if(NOT DEFINED RESOLV_LIBRARY)
        find_library(RESOLV_LIBRARY resolv)
    endif()

    add_library(sys::resolv STATIC IMPORTED)
    set_target_properties(sys::resolv PROPERTIES IMPORTED_LOCATION ${RESOLV_LIBRARY})
endif()

if(APPLE)
    if(NOT DEFINED OSX_CORE_FOUNDATION)
        find_library(OSX_CORE_FOUNDATION CoreFoundation)
    endif()

    if(NOT DEFINED OSX_CF_NETWORK)
        find_library(OSX_CF_NETWORK CFNetwork)
    endif()
endif()
