set(PROJECT_NAME big)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "include/big_cache.hpp"
    "include/big_cache_read.hpp"
    "include/big_consts.hpp"
    "include/big_core_base.hpp"
    "include/big_core_read.hpp"
    "include/big_core_read_info.hpp"
    "include/big_core_write.hpp"
    "include/half.hpp"
    "include/pugiconfig.hpp"
    "include/pugixml.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/big_cache.cpp"
    "src/big_cache_read.cpp"
    "src/big_core_base.cpp"
    "src/big_core_read.cpp"
    "src/big_core_read_info.cpp"
    "src/big_core_write.cpp"
    "src/pugixml.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE big)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            "_LIB;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "_LIB;"
            "UNICODE;"
            "_UNICODE"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "WIN32;"
            "_DEBUG;"
            "_LIB;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "WIN32;"
            "NDEBUG;"
            "_LIB;"
            "UNICODE;"
            "_UNICODE"
        ">"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /W3;
                /Y-
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy;
                /W3;
                /Y-
            >
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /W3;
                /Y-
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy;
                /W3;
                /Y-
            >
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG:FULL;
                /SUBSYSTEM:WINDOWS
            >
            $<$<CONFIG:Release>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF
            >
            $<$<CONFIG:test>:
                /DEBUG:FULL
            >
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /SUBSYSTEM:WINDOWS
            >
            $<$<CONFIG:Release>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF
            >
            $<$<CONFIG:test>:
                /DEBUG:FULL
            >
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMAND   $<CONFIG:Debug> copy /Y "${OUTPUT_DIRECTORY}${TARGET_NAME}.lib" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\lib\\${TARGET_NAME}d.lib
        COMMAND   $<CONFIG:Release> copy /Y "${OUTPUT_DIRECTORY}${TARGET_NAME}.lib" "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\lib\\${TARGET_NAME}.lib
    )
endif()


