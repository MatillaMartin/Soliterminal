#pragma once

#include <array>
#include <functional>
#include <optional>

namespace panda
{
	class Game;
	class CardStack;

	class Graph
	{
	public:
		struct Node
		{
			Node(int index,
				 std::pair<int, int> layout,
				 std::optional<int> up = {},
				 std::optional<int> down = {},
				 std::optional<int> left = {},
				 std::optional<int> right = {})
				: index(index)
				, layout(layout)
				, up(up)
				, down(down)
				, left(left)
				, right(right)
			{
			}

			int index = -1;
			std::pair<int, int> layout = {-1,-1};
			std::optional<int> up;
			std::optional<int> down;
			std::optional<int> left;
			std::optional<int> right;
		};

		// Adds a node to the graph
		void addNode(Node&& node);
		// Adds a basic node to the graph, without orientations
		void addNode(int index, std::pair<int, int> layout);
		// Adds a horizontal relation between two nodes
		void addHorEdge(int left, int right);
		// Adds a vertical relation between multiple nodes
		void addHorChain(const std::vector<int>& chain);
		// Adds a vertical relation between two nodes
		void addVerEdge(int top, int bot);
		// Adds a vertical relation between multiple nodes
		void addVerChain(const std::vector<int>& chain);
		// Searchs for a node with its index
		std::optional<Node> node(int index) const;
		// Searchs for a node with its layout
		std::optional<Node> node(std::pair<int, int> layout) const;

	private:
		void applyChain(const std::vector<int>& chain, std::function<void(Node&, Node&)> relation);
		std::vector<Node>::iterator Graph::nodeIt(int index);
		std::vector<Node> m_nodes;
	};

	class GameLayout
	{
	public:
		GameLayout();

		// Mapping between the layout and stacks index
		std::optional<int> layoutToIndex(int x, int y) const;

		// Mapping between stacks index and layout
		std::optional<std::pair<int, int>> indexToLayout(int index) const;

		// Returns the index of the element up of index
		int up(int index) const;

		// Returns the index of the element down of index
		int down(int index) const;

		// Returns the index of the element left of index
		int left(int index) const;

		// Returns the index of the element right of index
		int right(int index) const;

	private:
		// Offsets the index by the given amount
		int offset(int index, int dx, int dy) const;
		Graph m_graph;
	};
}