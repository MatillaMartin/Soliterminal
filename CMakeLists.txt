project(Soliterminal)
cmake_minimum_required(VERSION 3.9.4)

set(Sources 
	main.cpp
	Card.cpp
	CardStack.cpp
	Game.cpp
	GameControl.cpp
	GameFileIO.cpp
	GameLayout.cpp
	Render.cpp
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
	GameLayout.h
	Render.h
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

# Add support for C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
