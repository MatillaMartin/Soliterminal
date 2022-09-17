#include "GameLayout.h"

#include "Game.h"

#include <algorithm>

namespace panda
{
	void Graph::addNode(Graph::Node&& node) { m_nodes.push_back(node); }

	void Graph::addNode(int index, std::pair<int, int> layout) { m_nodes.emplace_back(index, std::move(layout)); }

	void Graph::addHorEdge(int left, int right)
	{
		auto leftIt = nodeIt(left);
		auto rightIt = nodeIt(right);
		if (leftIt == m_nodes.end() || rightIt == m_nodes.end())
			return;
		leftIt->right = rightIt->index;
		rightIt->left = leftIt->index;
	}

	void Graph::addVerEdge(int top, int bot)
	{
		auto topIt = nodeIt(top);
		auto botIt = nodeIt(bot);
		if (topIt == m_nodes.end() || botIt == m_nodes.end())
			return;
		topIt->down = botIt->index;
		botIt->up = topIt->index;
	}

	std::optional<Graph::Node> Graph::node(int index) const
	{
		auto it = std::find_if(m_nodes.cbegin(), m_nodes.cend(), [&index](const Node& node) -> bool { return node.index == index; });
		if (it == m_nodes.end())
			return {};
		return *it;
	}

	std::optional<Graph::Node> Graph::node(std::pair<int, int> layout) const
	{
		auto it = std::find_if(m_nodes.cbegin(), m_nodes.cend(), [&layout](const Node& node) -> bool { return node.layout == layout; });
		if (it == m_nodes.end())
			return {};
		return *it;
	}

	std::vector<Graph::Node>::iterator Graph::nodeIt(int index)
	{
		return std::find_if(m_nodes.begin(), m_nodes.end(), [&index](const Node& node) -> bool { return node.index == index; });
	}

	GameLayout::GameLayout()
	{
		// map game to the layout, where top row contains open, closed, end stacks, and bottom row contains central stacks
		// bottom row is one index down
		// layout is as follows:
		// 0:closed	| 1:open	| -			| 2:end0	| 3:end1	| 4:end2	| 5:end3	|
		// 6:cen0	| 7:cen1	| 8:cen2	| 9:cen3	| 10:cen4	| 11:cen5	| 12:cen6	|

		m_graph.addNode(0, {0, 0});
		m_graph.addNode(1, {1, 0});
		m_graph.addNode(2, {3, 0});
		m_graph.addNode(3, {4, 0});
		m_graph.addNode(4, {5, 0});
		m_graph.addNode(5, {6, 0});
		m_graph.addNode(6, {0, 1});
		m_graph.addNode(7, {1, 1});
		m_graph.addNode(8, {2, 1});
		m_graph.addNode(9, {3, 1});
		m_graph.addNode(10, {4, 1});
		m_graph.addNode(11, {5, 1});
		m_graph.addNode(12, {6, 1});

		m_graph.addHorEdge(0, 1);
		m_graph.addHorEdge(1, 2);
		m_graph.addHorEdge(2, 3);
		m_graph.addHorEdge(3, 4);
		m_graph.addHorEdge(4, 5);
		m_graph.addHorEdge(6, 7);
		m_graph.addHorEdge(7, 8);
		m_graph.addHorEdge(8, 9);
		m_graph.addHorEdge(9, 10);
		m_graph.addHorEdge(10, 11);
		m_graph.addHorEdge(11, 12);
		m_graph.addHorEdge(12, 13);

		m_graph.addVerEdge(0, 6);
		m_graph.addVerEdge(1, 7);
		m_graph.addVerEdge(2, 9);
		m_graph.addVerEdge(3, 10);
		m_graph.addVerEdge(4, 11);
		m_graph.addVerEdge(5, 12);
	}

	// Mapping between the layout and stacks index
	std::optional<int> GameLayout::layoutToIndex(int x, int y) const
	{
		if (auto node = m_graph.node({x, y}))
			return node->index;
		return {};
	}

	// Mapping between stacks index and layout
	std::optional<std::pair<int, int>> GameLayout::indexToLayout(int index) const
	{
		if (auto node = m_graph.node(index))
			return node->layout;
		return {};
	}

	int GameLayout::up(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->up)
			return index;
		return *node->up;
	}

	int GameLayout::down(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->down)
			return index;
		return *node->down;
	}

	int GameLayout::left(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->left)
			return index;
		return *node->left;
	}

	int GameLayout::right(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->right)
			return index;
		return *node->right;
	}
}
