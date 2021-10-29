cmake_minimum_required(VERSION 3.6)

set(TARGET_NAME gw2-sct)

# Project Name
PROJECT(${TARGET_NAME})

link_directories(${DXSDK_DIR}\\Lib\\x64)
link_directories(${OPENSSL_LIBRARY_DIR})

#########################################################
# Include Files
#########################################################

FILE(GLOB HEADERS include/*.h include/*.hpp)
FILE(GLOB SOURCES src/*.cpp src/*.c)

FILE(GLOB IMGUI_SOURCES submodules/imgui/*.cpp submodules/imgui/misc/cpp/*.cpp submodules/imgui/*.c)
FILE(GLOB SIMPLEINI_SOURCES submodules/simpleini/*.cpp submodules/simpleini/*.c)

list(APPEND SOURCES ${IMGUI_SOURCES} ${CURLITE_SOURCES} ${SIMPLEINI_SOURCES})

add_library(${TARGET_NAME} SHARED ${SOURCES} ${HEADERS})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
SET_SOURCE_FILES_PROPERTIES( ${SOURCES} PROPERTIES LANGUAGE CXX ) 

target_include_directories(${TARGET_NAME} PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/misc/cpp>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/httplib>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/json/single_include/nlohmann>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/simpleini>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/stb>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/submodules/object_threadsafe>"
)

include_directories(${DXSDK_DIR}\\Include)
include_directories(${OPENSSL_INCLUDE_DIR})

add_definitions("-DNOMINMAX")

if(MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC "/Zc:preprocessor")
    target_compile_options(${TARGET_NAME} PUBLIC "/MP")
endif()

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND if exist \"${GW2_PATH}${GW2_SUBPATH}\\d3d9_arcdps_sct.dll\" del \"${GW2_PATH}${GW2_SUBPATH}\\d3d9_arcdps_sct.dll\")
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND xcopy \"$(TargetPath)\" \"${GW2_PATH}${GW2_SUBPATH}\\*\" /d /y)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ren \"${GW2_PATH}${GW2_SUBPATH}\\$(TargetFileName)\" \"d3d9_arcdps_sct.dll\")

set( DEBUG_COMMAND "${GW2_PATH}\\Gw2-64.exe" )
set( DEBUG_COMMAND_ARGUMENTS "-maploadinfo" )
set( DEBUG_ENVIRONMENT "${GW2_PATH}" )
configure_file( template.vcxproj.user.in ${CMAKE_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )
