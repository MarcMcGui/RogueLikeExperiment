set(CMAKE_CXX_STANDARD 11)
project(RogueGame)

add_executable(RogueGame src/Main.cpp src/MapGenerator.cpp src/Room.cpp src/Coords.cpp src/ProjectSettings.cpp src/Map.cpp src/GameManager.cpp src/Items/Item.cpp src/Enemies/Enemy.cpp src/Player.cpp src/Pathfinding/PathPointNode.cpp)