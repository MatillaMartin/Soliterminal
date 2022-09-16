project(SolitaireConsole)
cmake_minimum_required(VERSION 3.9.4)

set(Sources 
	main.cpp
	Card.cpp
	CardStack.cpp
	Console.cpp
	Game.cpp
	GameControl.cpp
	GameLayout.cpp
	Render.cpp
	UserInput.cpp
)

set(Headers
	Card.h
	CardStack.h
	Console.h
	Game.h
	GameAction.h
	GameControl.h
	GameLayout.h
	Render.h
	UserInput.h
)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})

# Add support for C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
