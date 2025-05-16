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
	g.random_gen_graph(4, 1);
	size_t r = 0;
	for (auto el : g.g) {
		r += el.size();
	}
	ASSERT_EQ(12, r);
}
TEST(graph, input_can) {
	std::istringstream inp;
	inp.str("4 5 0 1 2.5 1 2 4 0 2 6.7 0 3 9.5 3 2 9.3");
	Graph<int, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	ASSERT_NO_THROW(inp >> g;);
}
TEST(graph, input) {
	std::istringstream inp;
	inp.str("4 5 0 1 2.5 1 2 4 0 2 6.7 0 3 9.5 3 2 9.3");
	Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	inp >> g;
	vector<vector<std::pair<size_t, double>>> v;
	v.push_back({ {1,2.5}, {2,6.7}, {3,9.5} });
	v.push_back({ {2,4} });
	v.push_back({ });
	v.push_back({ { 2, 9.3 } });
	ASSERT_EQ(g.g, v);
}
TEST(graph, print) {
	std::ostringstream pr;
	std::string o= "4 5\n0 1 2.5\n0 2 6.7\n0 3 9.5\n1 2 4\n3 2 9.3\n";
	Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g;
	vector<vector<std::pair<size_t, double>>> v;
	v.push_back({ {1,2.5}, {2,6.7}, {3,9.5} });
	v.push_back({ {2,4} });
	v.push_back({ });
	v.push_back({ { 2, 9.3 } });
	g = v;
	pr << g;
	ASSERT_EQ(pr.str(), o);
}


TEST(gijkstra, has_path) {
	vector<vector<std::pair<size_t, double>>> v;
	v.push_back({ {1,2.5}, {2,6.7}, {3,9.5} });
	v.push_back({ {2,4} });
	v.push_back({ });
	v.push_back({ { 2, 9.3 } });
	
	Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g(v);
	
	g.do_dijkstra(0);
	ASSERT_EQ(true, g.has_path(2));
}
TEST(gijkstra, distance) {
	vector<vector<std::pair<size_t, double>>> v;
	v.push_back({ {1,2.5}, {2,6.7}, {3,9.5} });
	v.push_back({ {2,4} });
	v.push_back({ });
	v.push_back({ { 2, 9.3 } });
	Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g(v);
	g.do_dijkstra(0);
	ASSERT_EQ(6.5, g.get_distance(2));
}
TEST(gijkstra, path) {
	vector<vector<std::pair<size_t, double>>> v;
	v.push_back({ {1,2.5}, {2,6.7}, {3,1.1} });
	v.push_back({ {2,4} });
	v.push_back({ });
	v.push_back({ { 2, 3 } });
	Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g(v);
	g.do_dijkstra(0);
	vector<size_t> f = { 0,3,2 };
	auto e = g.get_path(2);
	ASSERT_EQ(f, e);
}