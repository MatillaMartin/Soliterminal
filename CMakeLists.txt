project(SolitaireConsole)
cmake_minimum_required(VERSION 3.9.4)

set(Sources 
	main.cpp
	Card.cpp
	CardStack.cpp
	Game.cpp
	Render.cpp
)

set(Headers
	Card.h
	CardStack.h
	Game.h
	Render.h
)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})

# Add support for C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
