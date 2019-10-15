cmake_minimum_required(VERSION 3.12.4)
get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)
	option(unittests "unittests" OFF)
	get_filename_component(_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
	set_property(GLOBAL PROPERTY GLOBAL_FETCHDEPS_BASE ${_PARENT_DIR}/al2o3 )
	include(FetchContent)
	FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
	FetchContent_GetProperties(al2o3)
	if(NOT al2o3_POPULATED)
		FetchContent_Populate(al2o3)
		add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
	endif()
	INIT_AL2O3(${CMAKE_CURRENT_SOURCE_DIR})
endif ()

SET_MIN_VERSIONS()
set(LibName al2o3_thread)
project(${LibName})

file(GLOB Headers CONFIGURE_DEPENDS include/al2o3_thread/*.h include/al2o3_thread/*.hpp)
file(GLOB Src CONFIGURE_DEPENDS src/*.c src/*.cpp)
if(WIN32)
	file(GLOB_RECURSE WindowsSrc CONFIGURE_DEPENDS src/windows/*.c src/windows/*.cpp)
	list(APPEND Headers include/al2o3_thread/impl/atomic_msvc.h)
endif()
if(APPLE OR UNIX)
	file(GLOB_RECURSE PosixSrc CONFIGURE_DEPENDS src/posix/*.c src/posix/*.cpp)
	list(APPEND Headers include/al2o3_thread/impl/atomic_gcc_x64.h)
endif()

list(APPEND Src ${Headers} ${WindowsSrc} ${PosixSrc} )

set(Deps
		al2o3_platform
		al2o3_memory
		)

ADD_LIB2(${LibName} "${Src}" "${Deps}")
if (UNIX)
	target_link_libraries(${LibName} PRIVATE "pthread")
endif ()
file( GLOB_RECURSE Tests CONFIGURE_DEPENDS tests/*.cpp )

set( TestDeps
		al2o3_catch2
		utils_simple_logmanager)
ADD_LIB2_TESTS(${LibName} "${Tests}" "${TestDeps}")