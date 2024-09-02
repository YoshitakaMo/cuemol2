cmake_minimum_required(VERSION 3.12)
project(cuetty CXX C)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)  # ref. https://texus.me/2015/09/06/cmake-and-gcov/
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cli/cmake)

##########
# libcuemol2
find_package(LIBCUEMOL2 REQUIRED)
message(STATUS "LIBCUEMOL2: ${LIBCUEMOL2_FOUND}")
message(STATUS "LIBCUEMOL2_LIBDIR: ${LIBCUEMOL2_LIBDIR}")
message(STATUS "LIBCUEMOL2_INCLUDE_DIR: ${LIBCUEMOL2_INCLUDE_DIR}")
add_definitions(-DHAVE_CONFIG_H=1)

##########
# Boost
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.50.0 REQUIRED
  COMPONENTS system thread filesystem chrono timer)

##########
# Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python3_Development_FOUND=${Python3_Development_FOUND}")
message(STATUS "Python3_EXECUTABLE=${Python3_EXECUTABLE}")
message(STATUS "Python: version=${Python3_VERSION}")
message(STATUS "Python3_INCLUDE_DIRS=${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARIES=${Python3_LIBRARIES}")

#####################

add_definitions(-DHAVE_PYTHON=1 -DBUILD_PYMODULE=1)
if (WIN32)
  add_definitions(-DPYBR_EXPORTS=1)
endif()

# Dependent libs
SET(PYMOD_LINK_LIBRARIES
  cuemol2
  Boost::filesystem
)

# Normal C++ source files
SET(PYMODULE_SRCS
    wrapper.cpp
    convert.cpp
    methodobj.cpp
    initcuemol.cpp
  )

# set(DATA_DIR ${CMAKE_SOURCE_DIR}/../data/)

# set(DATA_FILES
# ${DATA_DIR}/prot_top.xml
# ${DATA_DIR}/prot_props.xml
# ${DATA_DIR}/nucl_top.xml
# ${DATA_DIR}/nucl_props.xml
# ${DATA_DIR}/sugar_top.xml
# ${DATA_DIR}/mono_top.xml
# ${DATA_DIR}/default_params.xml
# ${DATA_DIR}/symop.dat
# ${DATA_DIR}/default_style.xml
# )

# set(SYSCONFIG_FILE ${DATA_DIR}/sysconfig.xml)

#####################
# pymodule target

Python3_add_library(_cuemol_internal MODULE ${PYMODULE_SRCS})

target_include_directories(
  _cuemol_internal PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src
  ${Boost_INCLUDE_DIRS}
  ${LIBCUEMOL2_INCLUDE_DIR}
)

target_link_directories(
  _cuemol_internal PRIVATE
  ${LIBCUEMOL2_LIBDIR}
)
file(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/data/sysconfig.xml" DEFAULT_SYSCONFIG_PATH)
STRING(REPLACE "\\" "\\\\" DEFAULT_SYSCONFIG_PATH ${DEFAULT_SYSCONFIG_PATH})
message("DEFAULT_SYSCONFIG_PATH: ${DEFAULT_SYSCONFIG_PATH}")
target_compile_definitions(_cuemol_internal PUBLIC DEFAULT_CONFIG=\"${DEFAULT_SYSCONFIG_PATH}\")
target_compile_definitions(_cuemol_internal PUBLIC LINK_SHARED)

# if (BUILD_PYTHON_BINDINGS)
#   list(APPEND PYMOD_LINK_LIBRARIES pybr)
#   target_compile_definitions(_cuemol_internal PUBLIC HAVE_PYTHON=1)
# endif ()

target_link_libraries(_cuemol_internal PRIVATE
  ${PYMOD_LINK_LIBRARIES}
)

if (WIN32)
  set_target_properties(_cuemol_internal PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else ()
  set_target_properties(_cuemol_internal PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

# install(TARGETS _cuemol_internal
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )
# install(FILES ${DATA_FILES} DESTINATION data/data)
# install(FILES ${SYSCONFIG_FILE} DESTINATION data)

# For VS Debug
set_target_properties(_cuemol_internal PROPERTIES
  VS_DEBUGGER_ENVIRONMENT "PATH=${BOOST_LIBRARYDIR};${CMAKE_PREFIX_PATH}/bin;%PATH%")

# set_target_properties(_cuemol_internal PROPERTIES
#   INSTALL_RPATH "@executable_path/../lib")
