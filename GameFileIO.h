#pragma once

#include "FilesystemUtils.h"
#include "Game.h"

namespace panda
{
	namespace GameFileIO
	{
		/// Saves the game to a local file in AppData
		bool saveGame(const Game& game);

		/// Loads the game from the local AppData file
		std::optional<Game> loadGame();
	}
}