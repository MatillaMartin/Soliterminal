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
			Node(size_t index,
				 std::pair<size_t, size_t> layout,
				 std::optional<size_t> up = std::nullopt,
				 std::optional<size_t> down = std::nullopt,
				 std::optional<size_t> left = std::nullopt,
				 std::optional<size_t> right = std::nullopt)
				: index(index)
				, layout(layout)
				, up(up)
				, down(down)
				, left(left)
				, right(right)
			{
			}

			size_t index;
			std::pair<size_t, size_t> layout;
			std::optional<size_t> up;
			std::optional<size_t> down;
			std::optional<size_t> left;
			std::optional<size_t> right;
		};

		// Adds a node to the graph
		void addNode(Node&& node);
		// Adds a basic node to the graph, without orientations
		void addNode(size_t index, std::pair<size_t, size_t> layout);
		// Adds a bidirectional horizontal relation between two nodes
		void addHorEdge(size_t left, size_t right);
		// Adds a bidirectional vertical relation between multiple nodes
		void addHorChain(const std::vector<size_t>& chain);
		// Adds a bidirectional vertical relation between two nodes
		void addVerEdge(size_t top, size_t bot);
		// Adds a bidirectional vertical relation between multiple nodes
		void addVerChain(const std::vector<size_t>& chain);

		// Adds a directional relation
		void addUpEdge(size_t from, size_t to);
		// Adds a directional relation
		void addDownEdge(size_t from, size_t to);
		// Adds a directional relation
		void addLeftEdge(size_t from, size_t to);
		// Adds a directional relation
		void addRightEdge(size_t from, size_t to);

		// Searchs for a node with its index
		std::optional<Node> node(size_t index) const;
		// Searchs for a node with its layout
		std::optional<Node> node(std::pair<size_t, size_t> layout) const;

	private:
		void applyChain(const std::vector<size_t>& chain, std::function<void(Node&, Node&)> relation);
		std::vector<Node>::iterator Graph::nodeIt(size_t index);
		std::vector<Node> m_nodes;
	};

	class Layout
	{
	public:
		Layout(Graph graph);

		// Mapping between the layout and stacks index
		std::optional<size_t> layoutToIndex(int x, int y) const;

		// Mapping between stacks index and layout
		std::optional<std::pair<int, int>> indexToLayout(size_t index) const;

		// Returns the index of the element up of index
		size_t up(size_t index) const;

		// Returns the index of the element down of index
		size_t down(size_t index) const;

		// Returns the index of the element left of index
		size_t left(size_t index) const;

		// Returns the index of the element right of index
		size_t right(size_t index) const;

	private:
		Graph m_graph;
	};
}