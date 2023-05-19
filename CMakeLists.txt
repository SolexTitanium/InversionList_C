cmake_minimum_required(VERSION 3.0)

project(InversionList LANGUAGES C VERSION 0.0.1)

# Voir https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
# Voir https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_RPATH_USE_LINK_PATH.html
# Voir https://en.wikipedia.org/wiki/Rpath
# Voir https://en.wikipedia.org/wiki/Environment_variable#$LD_LIBRARY_PATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# See https://cmake.org/cmake/help/latest/policy/CMP0110.html
if(POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif()

add_library(inversion-list SHARED src/c/inversion-list.c include/inversion-list.h src/c/inversion-list.inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

install(
  TARGETS inversion-list
  LIBRARY DESTINATION lib COMPONENT "Libraries"
  ARCHIVE DESTINATION lib COMPONENT "Libraries"
  RUNTIME DESTINATION bin COMPONENT "Libraries"
)

install(
  FILES inversion-list.h
  DESTINATION include COMPONENT "Headers"
)

# Ajout d'un fichier de configuration de type pkgconfig
# Voir https://fr.wikipedia.org/wiki/Pkg-config et
# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/inversion-list.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/inversion-list.pc
  @ONLY
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/inversion-list.pc
  DESTINATION share/pkgconfig
  COMPONENT "PkgConfig"
)

# Ajout d'un fichier de configuration de type cmake
# Voir https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/InversionListConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/InversionListConfig.cmake
  INSTALL_DESTINATION cmake
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/InversionListConfig.cmake
  DESTINATION cmake COMPONENT "cmake"
)

find_library(EFENCE efence)
if(EFENCE)
  target_link_libraries(inversion-list ${EFENCE})
endif()

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/c/test-*.c")

enable_testing()

find_program(VALGRIND valgrind)

foreach(FILENAME ${FILES})
  get_filename_component(SRC ${FILENAME} NAME)
  get_filename_component(TEST ${FILENAME} NAME_WE)
  add_executable(${TEST} ${FILENAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/inversion-list.h ${CMAKE_CURRENT_SOURCE_DIR}/src/c/inversion-list.inc)
  add_dependencies(${TEST} inversion-list)
  target_link_libraries(${TEST} inversion-list)
  if(VALGRIND)
    add_test(
      "${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST}
    )
  endif()
  add_test(${TEST} ./${TEST})
endforeach()

set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

# Make docs command

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html
    COMMAND sphinx-build -b html ${CMAKE_CURRENT_SOURCE_DIR}/docs ${CMAKE_CURRENT_BINARY_DIR}/html
)

add_custom_target(
    docs
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/html
)
#make html-coverage command

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/coverage-html
        COMMAND lcov --capture --directory ${CMAKE_CURRENT_BINARY_DIR} --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory ${CMAKE_CURRENT_BINARY_DIR}/coverage-html
)
add_custom_target(
        html-coverage
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/coverage-html
)