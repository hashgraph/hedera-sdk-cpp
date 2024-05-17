set(HAPI_LIBRARY_HASH "13f0309b97e9bfc73f2dfd2c10c7eb26b1a72ea11722d4863f6408b34edb0185" CACHE STRING "Use the configured hash to verify the Hedera API protobuf library release")
set(HAPI_LIBRARY_URL "https://github.com/hashgraph/hedera-protobufs-cpp/releases/download/v0.50.0/hapi-library-9638abc0.tar.gz" CACHE STRING "Use the configured URL to download the Hedera API protobuf library package")

set(HAPI_LOCAL_LIBRARY_PATH "" CACHE STRING "Overrides the configured HAPI_LIBRARY_URL setting and instead uses the local path to retrieve the artifacts")

if (HAPI_LOCAL_LIBRARY_PATH STREQUAL "")
    # Fetch the HAPI Library
    FetchContent_Declare(
            hapi
            URL ${HAPI_LIBRARY_URL}
            URL_HASH SHA256=${HAPI_LIBRARY_HASH}
    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(hapi)
else ()
    set(hapi_SOURCE_DIR ${HAPI_LOCAL_LIBRARY_PATH})
endif ()

set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

if (NOT EXISTS ${HAPI_ROOT_DIR})
    set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

    if (NOT EXISTS ${HAPI_ROOT_DIR})
        message(FATAL_ERROR "Failed to the HAPI_ROOT_DIR at `${HAPI_ROOT_DIR}`")
    endif ()
endif ()

set(HAPI_INCLUDE_DIR ${HAPI_ROOT_DIR}/include)
set(HAPI_LIB_DIR ${HAPI_ROOT_DIR}/lib)

function(create_req_link_target prefix name nixlib winlib)
    string(TOUPPER "${prefix}" prefixUpper)
    string(TOUPPER "${name}" nameUpper)
    if (prefix STREQUAL "")
        set(variableName "${nameUpper}_LINK_TARGET")
    else ()
        set(variableName "${prefixUpper}_${nameUpper}_LINK_TARGET")
    endif ()

    set(linkTarget "${HAPI_LIB_DIR}/${nixlib}")
    if (NOT EXISTS ${linkTarget})
        set(linkTarget "${HAPI_LIB_DIR}/${winlib}")

        if (NOT EXISTS ${linkTarget})
            message(FATAL_ERROR "Failed to locate the ${variableName} at `${HAPI_LIB_DIR}/(${nixlib}|${winlib})`")
        endif ()
    endif ()

    set(${variableName} ${linkTarget} PARENT_SCOPE)
endfunction()

function(create_opt_link_target prefix name nixlib winlib)
    string(TOUPPER "${prefix}" prefixUpper)
    string(TOUPPER "${name}" nameUpper)
    if (prefix STREQUAL "")
        set(variableName "${nameUpper}_LINK_TARGET")
    else ()
        set(variableName "${prefixUpper}_${nameUpper}_LINK_TARGET")
    endif ()

    set(linkTarget "${HAPI_LIB_DIR}/${nixlib}")
    if (NOT EXISTS ${linkTarget})
        set(linkTarget "${HAPI_LIB_DIR}/${winlib}")

        if (NOT EXISTS ${linkTarget})
            set(linkTarget "")
            message(WARNING "Failed to locate the ${variableName} at `${HAPI_LIB_DIR}/(${nixlib}|${winlib})`")
        endif ()
    endif ()

    set(${variableName} ${linkTarget} PARENT_SCOPE)
endfunction()

function(create_external_package prefix)
    math(EXPR lastIndex "${ARGC}-1")
    string(TOUPPER "${prefix}" prefixUpper)

    # Create all the imported library targets first
    foreach (index RANGE 1 ${lastIndex})
        string(TOUPPER "${ARGV${index}}" nameUpper)
        if (prefix STREQUAL "")
            set(LIBNAME${index} "${ARGV${index}}")
            set(LINKTARGET${index} "${nameUpper}_LINK_TARGET")
        else ()
            set(LIBNAME${index} "${prefix}::${ARGV${index}}")
            set(LINKTARGET${index} "${prefixUpper}_${nameUpper}_LINK_TARGET")
        endif ()

        #        message("LIBNAME${index}=${LIBNAME${index}}")
        #        message("LINKTARGET${index}=${LINKTARGET${index}}")
        if (NOT ${${LINKTARGET${index}}} STREQUAL "")
            add_library(${LIBNAME${index}} STATIC IMPORTED)
        endif ()
    endforeach ()

    # Now import the link targets
    foreach (index RANGE 1 ${lastIndex})
        if (NOT ${${LINKTARGET${index}}} STREQUAL "")
            set_target_properties(${LIBNAME${index}} PROPERTIES IMPORTED_LOCATION ${${LINKTARGET${index}}})
        endif ()
    endforeach ()
endfunction()


create_req_link_target(hapi proto libhapi.a hapi.lib)
create_external_package(hapi proto)
