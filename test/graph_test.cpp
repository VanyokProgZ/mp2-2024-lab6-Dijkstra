#include <gtest.h>
#include "graph.h"
#include "heap.h"

TEST(graph, null_ptr) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	ASSERT_EQ(g.dijkstra, nullptr);
}
TEST(graph, null_sz) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	ASSERT_EQ(g.g.size(),0);
}
TEST(graph, copy_cst) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g({ {{1,4}}, {{0,5}} });
	ASSERT_EQ(g.g[0][0].first,1 );
	ASSERT_EQ(g.g[0][0].second, 4);
}
TEST(graph, table_size) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g({ {1}, {0} });
	ASSERT_EQ(g.table_size(),2);
}
TEST(graph, row_size) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g({ {{1,4}, {1,6}}, {{0,5}} });
	ASSERT_EQ(g.row_size(0), 2);
}

TEST(graph, rand_gen) {
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	g.random_gen_graph(4, 10);
	size_t r = 0;
	for (auto el : g.g) {
		r += el.size();
	}
	ASSERT_EQ(10, r);
}

