#include "Layout.h"

#include "Game.h"

#include <algorithm>
#include <assert.h>

namespace panda
{
	void Graph::addNode(Graph::Node&& node)
	{
		m_nodes.push_back(node);
	}

	void Graph::addNode(size_t index, std::pair<size_t, size_t> layout)
	{
		m_nodes.emplace_back(index, std::move(layout));
	}

	void Graph::addHorEdge(size_t left, size_t right)
	{
		addHorChain({left, right});
	}

	void Graph::addVerEdge(size_t top, size_t bot)
	{
		addVerChain({top, bot});
	}

	void Graph::addVerChain(const std::vector<size_t>& chain)
	{
		applyChain(chain, [](Node& first, Node& second) {
			// update relative indices
			first.down = second.index;
			second.up = first.index;
		});
	}

	void Graph::addHorChain(const std::vector<size_t>& chain)
	{
		applyChain(chain, [](Node& first, Node& second) {
			// update relative indices
			first.right = second.index;
			second.left = first.index;
		});
	}

	void Graph::addUpEdge(size_t from, size_t to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.up = second.index;
		});
	}

	void Graph::addDownEdge(size_t from, size_t to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.down = second.index;
		});
	}

	void Graph::addLeftEdge(size_t from, size_t to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.left = second.index;
		});
	}

	void Graph::addRightEdge(size_t from, size_t to)
	{
		applyChain({from, to}, [](Node& first, Node& second) {
			// update relative indices
			first.right = second.index;
		});
	}

	void Graph::applyChain(const std::vector<size_t>& chain, std::function<void(Node&, Node&)> relation)
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

	std::optional<Graph::Node> Graph::node(size_t index) const
	{
		auto it = std::find_if(m_nodes.cbegin(), m_nodes.cend(), [&index](const Node& node) -> bool { return node.index == index; });
		if (it == m_nodes.end())
			return {};
		return *it;
	}

	std::optional<Graph::Node> Graph::node(std::pair<size_t, size_t> layout) const
	{
		auto it = std::find_if(m_nodes.cbegin(), m_nodes.cend(), [&layout](const Node& node) -> bool { return node.layout == layout; });
		if (it == m_nodes.end())
			return {};
		return *it;
	}

	std::vector<Graph::Node>::iterator Graph::nodeIt(size_t index)
	{
		return std::find_if(m_nodes.begin(), m_nodes.end(), [&index](const Node& node) -> bool { return node.index == index; });
	}

	Layout::Layout(Graph graph)
		: m_graph(std::move(graph))
	{
	}

	// Mapping between the layout and stacks index
	std::optional<size_t> Layout::layoutToIndex(int x, int y) const
	{
		if (auto node = m_graph.node({x, y}))
			return node->index;
		return {};
	}

	// Mapping between stacks index and layout
	std::optional<std::pair<int, int>> Layout::indexToLayout(size_t index) const
	{
		if (auto node = m_graph.node(index))
			return std::pair<int,int>({static_cast<int>(node->layout.first), static_cast<int>(node->layout.second)});
		return {};
	}

	size_t Layout::up(size_t index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->up)
			return index;
		return *node->up;
	}

	size_t Layout::down(size_t index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->down)
			return index;
		return *node->down;
	}

	size_t Layout::left(size_t index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->left)
			return index;
		return *node->left;
	}

	size_t Layout::right(size_t index) const
	{
		auto node = m_graph.node(index);
		if (!node)
			return index;
		if (!node->right)
			return index;
		return *node->right;
	}
}
