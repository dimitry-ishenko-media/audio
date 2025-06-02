# lib

find_package(ALSA REQUIRED)

set(HEADERS
    audio++/error.hpp
    audio++/mixer.hpp
)
set(OVERALL_HEADER audio++.hpp)

set(SOURCES
    error.cpp
    internal.cpp
    mixer.cpp
)

####################
include(GNUInstallDirs)

set(name ${PROJECT_NAME})

add_library(${name}_objects OBJECT ${HEADERS} ${SOURCES})
set_target_properties(${name}_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(${name}_static STATIC $<TARGET_OBJECTS:${name}_objects>)
set_target_properties(${name}_static PROPERTIES
    OUTPUT_NAME ${name}
)

add_library(${name}_shared SHARED $<TARGET_OBJECTS:${name}_objects>)
set_target_properties(${name}_shared PROPERTIES
    OUTPUT_NAME ${name}
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_VERSION_MAJOR}
)

foreach(target ${name}_objects ${name}_static ${name}_shared)
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    target_link_libraries(${target} PUBLIC ALSA::ALSA)
endforeach()

add_library(${name}::${name}_static ALIAS ${name}_static)
add_library(${name}::${name}_shared ALIAS ${name}_shared)
add_library(${name}::${name} ALIAS ${name}_shared)

####################
install(TARGETS ${name}_shared ${name}_static
    EXPORT ${name}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${name})
install(FILES ${OVERALL_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${name}Targets
    FILE ${name}Targets.cmake
    NAMESPACE ${name}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${name}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${name}ConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${name}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${name}
)
