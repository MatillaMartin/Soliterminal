project(Soliterminal)
cmake_minimum_required(VERSION 3.9.4)

set(Sources 
	src/App.cpp
	src/AppControl.cpp
	src/AppRender.cpp
	src/Card.cpp
	src/CardStack.cpp
	src/FilesystemUtils.cpp
	src/Game.cpp
	src/GameControl.cpp
	src/GameFileIO.cpp
	src/GameRender.cpp
	src/main.cpp
	src/Menu.cpp
	src/Layout.cpp
	src/MenuControl.cpp
	src/MenuSelection.cpp
	src/MenuRender.cpp
	src/UserInput.cpp
)

set(Headers
	include/App.h
	include/AppControl.h
	include/AppRender.h
	include/Action.h
	include/Card.h
	include/CardStack.h
	include/Console.h
	include/FilesystemUtils.h
	include/Game.h
	include/GameControl.h
	include/GameFileIO.h
	include/GameRender.h
	include/Layout.h
	include/Menu.h
	include/MenuControl.h
	include/MenuSelection.h
	include/MenuRender.h
	include/Render.h
	include/UserInput.h
)

if(WIN32)
	list(APPEND Sources src/ConsoleWindows.cpp)
	list(APPEND Headers include/ConsoleWindows.h)
else()
	list(APPEND Sources src/ConsoleLinux.cpp)
	list(APPEND Headers include/ConsoleLinux.h)
endif()



add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_include_directories(${PROJECT_NAME} PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/"
)
set_source_files_properties(GameFileIO.cpp PROPERTIES COMPILE_DEFINITIONS PICOJSON_USE_INT64)

# Add support for C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
