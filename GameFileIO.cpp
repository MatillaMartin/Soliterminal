
#include "GameFileIO.h"

#include "picojson/picojson.h"

#include <fstream>

namespace panda
{
	bool saveGame(const Game& game)
	{
		// save to file in appdata
		auto path = FilesystemUtils::appDataPath();

		picojson::value gameJson;

		picojson::array stackJson;

		const auto& stacks = game.stacks();
		for (const auto& stack : stacks)
		{
			picojson::array cardsJson;
			const auto& cards = stack.cards();
			for (const auto& card : cards)
			{
				picojson::object cardJson;
				cardJson["number"].set(static_cast<int64_t>(card.number));
				cardJson["state"].set(static_cast<int64_t>(card.state));
				cardJson["suit"].set(static_cast<int64_t>(card.suit));
				cardsJson.emplace_back(std::move(cardJson));
			}
			stackJson.emplace_back(cardsJson);
		}
		gameJson.set(stackJson);

		// open file to write
		std::ofstream file;
		file.open(path);
		if (!file.is_open())
			return false;

		file << gameJson;
		return true;
	}

	std::optional<Game> loadGame()
	{
		// save to file in appdata
		auto path = FilesystemUtils::appDataPath();

		picojson::value gameJson;

		// open file to read
		std::ifstream file;
		file.open(path);
		if (!file.is_open())
			return std::nullopt;

		file >> gameJson;

		std::string err = picojson::get_last_error();
		if (!err.empty())
			return std::nullopt;

		return std::nullopt;
	}
}