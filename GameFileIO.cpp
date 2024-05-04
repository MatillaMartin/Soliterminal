
#include "GameFileIO.h"

#include "nlohmann/json.hpp"

#include <fstream>

using json = nlohmann::json;

namespace panda
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Card, number, state, suit)
	void to_json(json& j, const CardStack& stack) { j = json{{"cards", stack.cards()}}; }
	void to_json(json& j, const Game& game) { j = json{{"stacks", game.stacks()}}; }

	namespace GameFileIO
	{
		bool saveGame(const Game& game)
		{
			json gameJson = game;

			// open file to write
			auto pathDir = FilesystemUtils::appDataPath() / "Soliterminal";
			std::filesystem::create_directory(pathDir);
			auto fullPath = pathDir.append("saveFile.json");
			std::ofstream file;
			file.open(fullPath);
			if (!file.is_open())
				return false;

			file << gameJson;
			return true;
		}

		std::optional<Game> loadGame()
		{
			// save to file in appdata
			auto pathDir = FilesystemUtils::appDataPath() / "Soliterminal";
			auto fullPath = pathDir.append("saveFile.json");
			// open file to read
			std::ifstream file;
			file.open(fullPath);
			if (!file.is_open())
				return std::nullopt;

			try
			{
				json gameJson = json::parse(file);


				std::vector<Card> cards;

				json& stacks = gameJson.at("stacks");
				stacks.at(0).at("cards").get_to(cards);
				CardStack closedStack{std::move(cards)};

				stacks.at(1).at("cards").get_to(cards);
				CardStack openStack{std::move(cards)};

				std::array<CardStack, 4> endStack;
				for (int i = 0; i < endStack.size(); ++i)
				{
					stacks.at(2 + i).at("cards").get_to(cards);
					endStack[i] = CardStack{std::move(cards)};
				}

				std::array<CardStack, 7> centralStack;
				for (int i = 0; i < centralStack.size(); ++i)
				{
					stacks.at(2 + endStack.size() + i).at("cards").get_to(cards);
					centralStack[i] = CardStack{std::move(cards)};
				}

				Game game(Game::Stacks{std::move(endStack), std::move(centralStack), std::move(closedStack), std::move(openStack)});
				return game;
			}
			catch (std::exception&)
			{
			}
			return std::nullopt;
		}
		bool hasSavedGame() { return false; }
	}
}