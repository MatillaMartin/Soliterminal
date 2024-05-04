#include "Layout.h"

#include "Game.h"

#include <algorithm>
#include <assert.h>

namespace panda
{
	void Graph::addNode(Graph::Node&& node) { m_nodes.push_back(node); }

	void Graph::addNode(int index, std::pair<int, int> layout) { m_nodes.emplace_back(index, std::move(layout)); }

	void Graph::addHorEdge(int left, int right) { addHorChain({left, right}); }

	void Graph::addVerEdge(int top, int bot) { addVerChain({top, bot}); }

	void Graph::addVerChain(const std::vector<int>& chain)
	{
		applyChain(chain, [](Node& first, Node& second) {
			// update relative indices
			first.down = second.index;
			second.up = first.index;
		});
	}

	void Graph::addHorChain(const std::vector<int>& chain)
	{
		applyChain(chain, [](Node& first, Node& second) {
			// update relative indices
			first.right = second.index;
			second.left = first.index;
		});
	}

	void Graph::addUpEdge(int from, int to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.up = second.index;
		});
	}

	void Graph::addDownEdge(int from, int to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.down = second.index;
		});
	}

	void Graph::addLeftEdge(int from, int to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.left = second.index;
		});
	}

	void Graph::addRightEdge(int from, int to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.right = second.index;
		});
	}

	void Graph::applyChain(const std::vector<int>& chain, std::function<void(Node&, Node&)> relation)
	{
		if (chain.size() < 2)
			return;

		// init first to first index of chain
		std::vector<Node>::iterator firstIt = nodeIt(*chain.begin());
		for (auto second = std::next(chain.begin()); second != chain.end(); second++)
		{
			// second to next index in chain
			auto secondIt = nodeIt(*second);

			// Nodes could not be found
			assert(firstIt != m_nodes.end() && secondIt != m_nodes.end());
			if (firstIt == m_nodes.end() || secondIt == m_nodes.end())
				continue;

			// update relative indices
			relation(*firstIt, *secondIt);

			// optimize first to take second's iterator, no need to search again
			firstIt = secondIt;
		}
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

	Layout::Layout(Graph graph)
		: m_graph(std::move(graph))
	{
	}

	// Mapping between the layout and stacks index
	std::optional<int> Layout::layoutToIndex(int x, int y) const
	{
		if (auto node = m_graph.node({x, y}))
			return node->index;
		return {};
	}

	// Mapping between stacks index and layout
	std::optional<std::pair<int, int>> Layout::indexToLayout(int index) const
	{
		if (auto node = m_graph.node(index))
			return node->layout;
		return {};
	}

	int Layout::up(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->up)
			return index;
		return *node->up;
	}

	int Layout::down(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->down)
			return index;
		return *node->down;
	}

	int Layout::left(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->left)
			return index;
		return *node->left;
	}

	int Layout::right(int index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->right)
			return index;
		return *node->right;
	}
}
