project(Soliterminal)
cmake_minimum_required(VERSION 3.9.4)

set(Sources 
	main.cpp
	Card.cpp
	CardStack.cpp
	FilesystemUtils.cpp
	Game.cpp
	GameControl.cpp
	GameFileIO.cpp
	Layout.cpp
	GameRender.cpp
	UserInput.cpp
)

set(Headers
	Card.h
	CardStack.h
	Console.h
	FilesystemUtils.h
	Game.h
	GameAction.h
	GameControl.h
	GameFileIO.h
	Layout.h
	GameRender.h
	UserInput.h
)

if(WIN32)
	list(APPEND Sources ConsoleWindows.cpp)
	list(APPEND Headers ConsoleWindows.h)
else()
	list(APPEND Sources ConsoleLinux.cpp)
	list(APPEND Headers ConsoleLinux.h)
endif()



add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/")
set_source_files_properties(GameFileIO.cpp PROPERTIES COMPILE_DEFINITIONS PICOJSON_USE_INT64)

# Add support for C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
