cmake_minimum_required(VERSION 3.18)

project(SuperBLT)

###############################################################################
## misc. definitions ##########################################################
###############################################################################

enable_language(ASM_MASM)

# Use CXX 20
set(CMAKE_CXX_STANDARD 20)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variable
# `sources`
file(GLOB_RECURSE	sources		src/*.cpp src/*.c src/*.h)

###############################################################################
## target definitions #########################################################
###############################################################################

set(mxml_sources mxml-attr.c mxml-entity.c mxml-file.c mxml-get.c mxml-index.c
	mxml-node.c mxml-search.c mxml-set.c mxml-private.c mxml-string.c)
list(TRANSFORM mxml_sources PREPEND lib/mxml/)
add_library(mxml STATIC ${mxml_sources})
target_include_directories(mxml PRIVATE lib/configs/mxml)
target_compile_options(mxml PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(mxml PUBLIC lib/mxml)

file(GLOB_RECURSE wren_sources
	# Public source file
	lib/wren/src/include/wren.h 
	# Core VM files
	lib/wren/src/vm/*.c
	lib/wren/src/vm/*.h
	# Optional components (Meta and Random)
	lib/wren/src/optional/*.c
	lib/wren/src/optional/*.h
)
add_library(wren STATIC ${wren_sources})
target_include_directories(wren PRIVATE lib/wren/src/optional lib/wren/src/vm)
target_compile_options(wren PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(wren PUBLIC lib/wren/src/include)

add_library(subhook STATIC lib/subhook/subhook.c)
target_compile_options(subhook PRIVATE -DSUBHOOK_EXPORT=;)
target_include_directories(subhook PUBLIC lib/subhook)

###############################################################################
## Wren source generation #####################################################
###############################################################################

find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(wren_src_files
	wren/native.wren
	wren/DB_001.wren
	wren/internal_001.wren
	wren/LuaInterface_001.wren
	wren/Environment_001.wren
	wren/Utils_001.wren
)
add_custom_command(
	OUTPUT wren_generated_src.c gen/wren_generated_src.h
	COMMAND ${Python3_EXECUTABLE} wren/generate.py ${CMAKE_CURRENT_BINARY_DIR} ${wren_src_files}
	MAIN_DEPENDENCY wren/generate.py
	DEPENDS ${wren_src_files}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	VERBATIM
)
list(APPEND sources ${CMAKE_CURRENT_BINARY_DIR}/wren_generated_src.c)

###############################################################################
## Base (non-platform-specific) part of SuperBLT ##############################
###############################################################################

add_library(SuperBLT STATIC ${sources})

target_compile_options(SuperBLT PRIVATE
	-DCURL_STATICLIB
	-DSUBHOOK_STATIC
)

# General optimisation breaks calls to certain lua functions, so replace it.
# We statically link to reduce dependencies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	if(${flag_var} MATCHES "/O2")
		string(REGEX REPLACE "/O2" "/Od /Oi /Ot /Oy /Ob2 /GF /Gy" ${flag_var} "${${flag_var}}")
	endif(${flag_var} MATCHES "/O2")
endforeach(flag_var)

# Link against libraries
target_link_libraries(SuperBLT ws2_32.lib)
target_link_libraries(SuperBLT version.lib)

# Allow both SuperBLT and the loaders access to the headers
target_include_directories(SuperBLT PUBLIC src)

# Make the generated wren header available
target_include_directories(SuperBLT PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)

###############################################################################
## dependencies ###############################################################
###############################################################################

target_link_libraries(SuperBLT mxml)
target_link_libraries(SuperBLT wren)
target_link_libraries(SuperBLT subhook)

#### zlib-ng ####

set(ZLIB_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(ZLIB_COMPAT ON CACHE BOOL "" FORCE)  # Ensures zlib-ng mimics zlib 1.2.x API
set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/sys/zlib EXCLUDE_FROM_ALL)

target_include_directories(zlib PUBLIC
    $<BUILD_INTERFACE:${zlib_SOURCE_DIR}>
    $<BUILD_INTERFACE:${zlib_BINARY_DIR}>
)

# Link to the expected CURL-compatible zlib target
set(ZLIB_LIBRARY zlib)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR})
target_link_libraries(SuperBLT zlib)

# Sanity check
get_target_property(_zlib_inc zlib INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "zlib-ng include dirs: ${_zlib_inc}")

#### CURL ####

# Required to build curl as static
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build as static, set by SBLT's buildscript")

# Use Windows SSPI for HTTPS
set(CURL_USE_SCHANNEL ON)

# Turn off all the crap we don't want
set(CURL_DISABLE_INSTALL ON)
set(HTTP_ONLY ON) # disable all protocols, but http(s) and ws(s)
set(ENABLE_UNIX_SOCKETS OFF) # disable unix-socket support [ http(s):// ]
set(CURL_DISABLE_NEGOTIATE_AUTH ON) # included in SSPI
set(CURL_DISABLE_KERBEROS_AUTH ON) # included in SSPI
set(CURL_DISABLE_NTLM ON) # included in SSPI
set(CURL_DISABLE_WEBSOCKETS ON) # disable websocket support [ ws(s):// ]

# link curl against our included static zlib, to enable it's libz features
set(CURL_ZLIB OFF CACHE BOOL "" FORCE)        # prevent it from trying to find system zlib
set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ON CACHE BOOL "" FORCE)  # disable external find
set(ZLIB_LIBRARY zlib CACHE STRING "" FORCE)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} CACHE PATH "" FORCE)

set(CURL_USE_LIBPSL OFF) # TODO: include https://github.com/rockdaboot/libpsl

add_subdirectory(lib/sys/curl EXCLUDE_FROM_ALL)
target_link_libraries(SuperBLT libcurl_static)

#### LuaJIT ####

add_subdirectory(src/luajit EXCLUDE_FROM_ALL)
target_link_libraries(SuperBLT lua51)

###############################################################################
## loader binary (this generates a seperate target for both Windows loaders) ##
###############################################################################

# Define the macro
macro(Add_Loader library_name)
	add_library(${library_name} SHARED ${ARGN})
	target_link_libraries(${library_name} LINK_PRIVATE SuperBLT)
endmacro()

Add_Loader("IPHLPAPI" loader/loader.asm loader/iphp.cpp loader/iphp.def src/Resource.rc)
Add_Loader("WSOCK32" loader/loader.asm loader/wsock.cpp loader/wsock.def src/Resource.rc)

set_property(SOURCE loader/loader.asm PROPERTY LANGUAGE ASM_MASM)

target_compile_options(IPHLPAPI PRIVATE -DBLT_USE_IPHLPAPI)
target_compile_options(WSOCK32 PRIVATE -DBLT_USE_WSOCK)

###############################################################################
## build external updater for dll                                            ##
###############################################################################
add_executable(SBLT_DLL_UPDATER updater/updater.cpp updater/compression.cpp updater/Resource.rc)
target_link_libraries(SBLT_DLL_UPDATER ws2_32.lib)
target_link_libraries(SBLT_DLL_UPDATER zlib)
target_link_libraries(SBLT_DLL_UPDATER libcurl_static)
target_include_directories(SBLT_DLL_UPDATER PRIVATE updater)
