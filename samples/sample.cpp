#include <iostream>
#include "heap.h"
#include "graph.h"
#include <algorithm>

#ifdef _WIN32
#define OS_NAME_NT
#endif

#ifdef OS_NAME_NT
const std::string CLS = "cls";
#else
const std::string CLS = "clear";
#endif

int main() {
	try {
		Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t,size_t>>>> g;
		size_t start_vertex = 0;
		bool Ready = 0;
		while (1) {
			std::cout << "-------------------\n0 - input graph\n1 - print (console) graph\n2 - do dijkstra calculate\n3 - get length\n4 - get min path\n5 - generate graph\n6 - js visualisation\ne - exit\nYour choice: ";
			std::string q;
			std::cin >> q;
			
			if (q == "0") {
				std::cin >> g;
			}
			else if (q == "1") {
				std::cout << g;
			}
			else if (q == "2") {
				size_t vert;
				std::cout << "type the num of start vertex (0...n-1): ";
				std::cin >> vert;
				start_vertex = vert;
				g.do_dijkstra(vert);
				Ready = 1;
			}
			else if (q == "3") {
				if (!Ready) {
					std::cout << "DO DIJKSTRA!\n";
					continue;
				}
				size_t vert;
				std::cout << "type the num of end vertex (0...n-1): ";
				std::cin >> vert;
				if (!g.has_path(vert)) {
					std::cout << "No path from " << start_vertex << " to " << vert << '\n';
				}
				else {
					std::cout <<"length: "<< g.get_distance(vert) << '\n';
				}
			}
			else if (q == "4") {
				if (!Ready) {
					std::cout << "DO DIJKSTRA!\n";
					continue;
				}
				size_t vert;
				std::cout << "type the num of end vertex (0...n-1): ";
				std::cin >> vert;
				if (!g.has_path(vert)) {
					std::cout << "No path from " << start_vertex << " to " << vert << '\n';
				}
				else {
					std::cout << "path: " << g.get_path(vert) << '\n';
				}
			}
			else if (q == "5") {
				size_t vert, edg;
				std::cout << "Input count of vertex: ";
				std::cin >> vert;
				std::cout << "Input count of edges: ";
				std::cin >> edg;
				g.random_gen_graph(vert, edg);
			}
			else if (q == "6") {
				g.visualisation();
			}
			else if (q == "e") {
				std::cout << "Goodbye!\n";
				exit(0);
			}
			else {
				std::cout << "Incorrect command!\n";
			}
			std::cout << "Type any to continue\n";
			std::cin >> q;
			system(CLS.c_str());
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
}