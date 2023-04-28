# CMakeList.txt: Main 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(CMAKE_CXX_STANDARD 20)
# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE HEADER_FILE "*.h")
file(GLOB_RECURSE SRC_FILES "*.cpp" "*.c" "*.ixx")
#aux_source_directory(. SRC_FILES)

#set(VS_JUST_MY_CODE_DEBUGGING ON)
#add_compile_options( /Zc:__cplusplus /ZI )
add_compile_options( /JMC )
add_executable (Main ${SRC_FILES} ${HEADER_FILE} "Source/Framework/Input.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Source")

set_property(TARGET Main PROPERTY CXX_STANDARD 20)

target_include_directories(Main PRIVATE Source/Libraries)
target_precompile_headers(Main 
PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Libraries/OpenGL.h>"
PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:Libraries/Libs.h>")

get_target_property(var Main PRECOMPILE_HEADERS)
message("var: ${var}")

target_link_libraries(Main PRIVATE glfw3 assimp)

#link_libraries("Debug/")
#Assimp
#link_directories("${ASSIMP_LIBRARY_OUTPUT_DIRECTORY}/Debug")
#set_target_properties(Main PROPERTIES
#        VS_DEBUGGER_ENVIRONMENT "PATH=${ASSIMP_LIBRARY_OUTPUT_DIRECTORY}/$<IF:$<CONFIG:Debug>,Debug,Release>")
#message("${ASSIMP_LIBRARY_OUTPUT_DIRECTORY}/Debug")
#if not found assimp then copy

if(
	NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/assimp*.dll" OR 
	NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/assimp*.ilk" OR
	${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/assimp*.pdb"
)
	message("Assimp library files are not up-to-date, copying ${ASSIMP_LIBRARY_OUTPUT_DIRECTORY} to ${CMAKE_CURRENT_BINARY_DIR}")
	# 执行复制命令
	add_custom_command(TARGET Main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${ASSIMP_LIBRARY_OUTPUT_DIRECTORY}"
      "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND_EXPAND_LISTS)
else()
	message("EXISTS EXE")
endif()
  

set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"MSVCRT.lib")

message(OUTPUT_NAME)


#


