cmake_minimum_required(VERSION 3.18)

project(SuperBLT)

# Allow us to attach the winmm.lib dependency to OpenAL on Windows
cmake_policy(SET CMP0079 NEW)

enable_language(ASM_MASM)

###############################################################################
## misc. definitions ##########################################################
###############################################################################

# Use CXX 17
set(CMAKE_CXX_STANDARD 17)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources` and `data`
file(GLOB_RECURSE	sources		src/*.cpp src/*.c src/*.h)

set(				win32_all_game_path			platforms/w32/game)
set(				win32_game_path				${win32_all_game_path}/raid)
file(GLOB_RECURSE	all_game_win32_sources	${win32_all_game_path}/*.cpp ${win32_all_game_path}/*.h)
file(GLOB_RECURSE	game_win32_sources		${win32_game_path}/*.cpp ${win32_game_path}/*.h)
# Only Payday 2 has a linux build at the moment so we don't really need game specific stuff for it. 

file(GLOB_RECURSE	win32_sources		platforms/w32/*.cpp platforms/w32/*.h)
set(				win32_loader_path	platforms/w32/loader)

file(GLOB_RECURSE	win32_loaders		${win32_loader_path}/*.cpp)
list(REMOVE_ITEM	win32_sources		${win32_loaders})
list(REMOVE_ITEM	win32_sources		${all_game_win32_sources})
list(APPEND			win32_sources		${game_win32_sources})

# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

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
	lib/wren/src/include/wren.h # Public source file
		lib/wren/src/vm/*.c lib/wren/src/vm/*.h # Core VM files
		lib/wren/src/optional/*.c lib/wren/src/optional/*.h # Optional components (Meta and Random)
)
add_library(wren STATIC ${wren_sources})
target_include_directories(wren PRIVATE lib/wren/src/optional lib/wren/src/vm)
if(MSVC)
	target_compile_options(wren PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(wren PRIVATE -std=c99 -Wall -fPIC)
endif()
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

add_library(SuperBLT STATIC ${sources} ${win32_sources} platforms/w32/platform_xml.asm)
set_property(SOURCE platforms/w32/platform_xml.asm PROPERTY LANGUAGE ASM_MASM)

target_compile_options(SuperBLT PRIVATE
	-DAL_LIBTYPE_STATIC
	-DCURL_STATICLIB
	-DSUBHOOK_STATIC
)

# General optimisation breaks calls to certain lua functions, so replace it.
if(MSVC)
	# We statically link to reduce dependencies
	foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/O2")
			string(REGEX REPLACE "/O2" "/Od /Oi /Ot /Oy /Ob2 /GF /Gy" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/O2")
	endforeach(flag_var)
endif(MSVC)

# Link against libraries
target_link_libraries(SuperBLT ws2_32.lib)
target_link_libraries(SuperBLT crypt32.lib)

# Allow both SuperBLT and the loaders access to the headers
target_include_directories(SuperBLT PUBLIC src)

# Make the generated wren header available
target_include_directories(SuperBLT PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)

# Give access to game specific platforms
target_include_directories(SuperBLT PUBLIC ${win32_game_path})

###############################################################################
## dependencies ###############################################################
###############################################################################

target_link_libraries(SuperBLT mxml)
target_link_libraries(SuperBLT wren)
target_link_libraries(SuperBLT subhook)

# Disable all the install targets, otherwise it causes errors when we add dependencies to curl
function(install)
endfunction()

# Note that libressl must be downloaded seperately, since it requires some pre-configuration that
# is inconvenient on Windows. Here's the download URL:
# https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.7.3.tar.gz
set(LSSL_SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/sys/libressl-3.7.3)
set(LSSL_TARBALL ${LSSL_SRCDIR}.tar.gz)
if(NOT EXISTS ${LSSL_TARBALL})
	# TODO move this to my (ZNix's) server, in case the paths ever change
	message("Downloading LibreSSL")
	file(DOWNLOAD "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.7.3.tar.gz" ${LSSL_TARBALL}
		#TIMEOUT 60  # seconds
		EXPECTED_HASH SHA256=7948c856a90c825bd7268b6f85674a8dcd254bae42e221781b24e3f8dc335db3
		TLS_VERIFY ON
	)
endif()
if(NOT EXISTS ${LSSL_SRCDIR})
	message("Extracting LibreSSL")
	file(ARCHIVE_EXTRACT INPUT ${LSSL_TARBALL} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/sys/ PATTERNS libressl-3.7.3)
endif()

######
###### Dependency stuff
######

# Required to build LibreSSL and curl as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build as static, set by SBLT's buildscript")

# # zlib requires an old version of cmake before target_include_directories was added
add_subdirectory(lib/sys/zlib EXCLUDE_FROM_ALL)
target_include_directories(zlibstatic PUBLIC ${zlib_BINARY_DIR} ${zlib_SOURCE_DIR})
target_link_libraries(SuperBLT zlibstatic)

# Unfortunately cmake doesn't have namespacing, so we have to set stuff here
set(LIBTYPE STATIC)
add_subdirectory(lib/sys/openalsoft EXCLUDE_FROM_ALL)
target_link_libraries(OpenAL PRIVATE winmm.lib) # Not set up by default?
target_link_libraries(SuperBLT OpenAL)
# target_include_directories(SuperBLT PRIVATE ${OpenAL_SOURCE_DIR}/include) # No longer needed

add_subdirectory(lib/sys/libressl-3.7.3 EXCLUDE_FROM_ALL)
target_include_directories(SuperBLT PRIVATE ${LSSL_SRCDIR}/include) # Required for latest version of LibreSSL
target_link_libraries(SuperBLT crypto)

#### CURL ####
# Turn off all the crap we don't want
option(HTTP_ONLY "" ON)
option(CURL_DISABLE_CRYPTO_AUTH "" ON)

# Use WinSSL rather than our static-linked copy of LibreSSL so we can use the
# default Windows certificate store without any hassle
option(CMAKE_USE_WINSSL "" ON)

# TODO enable zlib

add_subdirectory(lib/sys/curl EXCLUDE_FROM_ALL)
target_link_libraries(libcurl crypto ssl)
target_link_libraries(SuperBLT libcurl)

###############################################################################
## loader binary (this generates a seperate target for both Windows loaders) ##
###############################################################################

# Define the macro
macro(Add_Loader library_name)
	add_library(${library_name} SHARED ${ARGN})
	target_link_libraries(${library_name} LINK_PRIVATE SuperBLT)
endmacro()

Add_Loader("IPHLPAPI" ${win32_loader_path}/loader.asm ${win32_loader_path}/iphp.cpp ${win32_loader_path}/iphp.def)
Add_Loader("WSOCK32" ${win32_loader_path}/loader.asm ${win32_loader_path}/wsock.cpp ${win32_loader_path}/wsock.def)

set_property(SOURCE ${win32_loader_path}/loader.asm PROPERTY LANGUAGE ASM_MASM)
set_property(SOURCE ${win32_loader_path}/loader.asm PROPERTY LANGUAGE ASM_MASM)

target_compile_options(IPHLPAPI PRIVATE -DBLT_USE_IPHLPAPI)
target_compile_options(WSOCK32 PRIVATE -DBLT_USE_WSOCK)

