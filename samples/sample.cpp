#include <iostream>
#include "heap.h"
#include "graph.h"
#include <algorithm>

#include "intrin.h"
#pragma intrinsic(__rdtsc)

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
		unsigned __int64 a, b;

		std::string q;
		Graph<double, heap<std::pair<size_t, size_t>, std::less<std::pair<size_t,size_t>>>> g1;
		Graph<double, BinomialHeap<std::pair<size_t, size_t>, std::less<std::pair<size_t, size_t>>>> g2;
		size_t start_vertex = 0;
		bool Ready[2] = {0,0};
		bool has_data[2] = { 0,0 };
		int current_heap = 0;
		std::string heaps_info[2] = { "Work with binary heap alg\n-----------------------------","Work with binominal heap alg\n-----------------------------" };
		while (1) {
			std::cout << heaps_info[current_heap] << '\n';
			std::cout << "-------------------\n0 - input graph\n1 - print (console) graph\n2 - do dijkstra calculate\n3 - get length\n4 - get min path\n5 - generate graph\n6 - js visualisation\ne - exit\nb - work with another heap\nYour choice: ";
			std::cin >> q;
			
			if (q == "b") {
				current_heap = !current_heap;
			}
			else if (q == "0") {
				
				//a = __rdtsc();
				if (current_heap) {
					std::cin >> g2;
					if (g2.good_vertex.size()) {
						std::cout << "good started points for dijk: ";
						for (auto el : g2.good_vertex) {
							std::cout << el << ' ';
						}
						std::cout << '\n';
					}
				}
				else { std::cin >> g1; 
				if (g1.good_vertex.size()) {
					std::cout << "good started points for dijk: ";
					for (auto el : g1.good_vertex) {
						std::cout << el << ' ';
					}
					std::cout << '\n';
				}
				}
				//b = __rdtsc();
				has_data[current_heap] = 1;
				
			}
			else if (q == "1") {
				//a = __rdtsc();
				if (current_heap)
					std::cout << g2;
				else std::cout << g1;
				//b = __rdtsc();
			}
			else if (q == "2") {
				if (!has_data[current_heap]) {
					std::cout << "INPUT GRAPH FIRST!\n";
					continue;
				}
				size_t vert;
				std::cout << "type the num of start vertex (0...n-1): ";
				std::cin >> vert;
				start_vertex = vert;
				a = __rdtsc();
				if(current_heap)
				g2.do_dijkstra(vert);
				else {
					g1.do_dijkstra(vert);
				}
				b = __rdtsc();
				Ready[current_heap] = 1;
				std::cout << "tick count of operation: " << b - a << std::endl;
			}
			else if (q == "3") {
				
				if (!Ready[current_heap]) {
					std::cout << "DO DIJKSTRA!\n";
					continue;
				}
				size_t vert;
				std::cout << "type the num of end vertex (0...n-1): ";
				std::cin >> vert;
				//a = __rdtsc();
				if (current_heap) {
					if (!g2.has_path(vert)) {
						std::cout << "No path from " << start_vertex << " to " << vert << '\n';
					}
					else {
						std::cout << "length: " << g2.get_distance(vert) << '\n';
					}
				}
				else {
					if (!g1.has_path(vert)) {
						std::cout << "No path from " << start_vertex << " to " << vert << '\n';
					}
					else {
						std::cout << "length: " << g1.get_distance(vert) << '\n';
					}
				}
				//b = __rdtsc();
			}
			else if (q == "4") {
				if (!Ready[current_heap]) {
					std::cout << "DO DIJKSTRA!\n";
					continue;
				}
				size_t vert;
				std::cout << "type the num of end vertex (0...n-1): ";
				std::cin >> vert;
				//a = __rdtsc();
				if (current_heap) {
					if (!g2.has_path(vert)) {
						std::cout << "No path from " << start_vertex << " to " << vert << '\n';
					}
					else {
						std::cout << "path: " << g2.get_path(vert) << '\n';
					}
				}
				else {
					if (!g1.has_path(vert)) {
						std::cout << "No path from " << start_vertex << " to " << vert << '\n';
					}
					else {
						std::cout << "path: " << g1.get_path(vert) << '\n';
					}
				}
				//b = __rdtsc();
			}
			else if (q == "5") {
				size_t vert;
				double edg;
				std::cout << "Input count of vertex: ";
				std::cin >> vert;
				std::cout << "Input procent of edges: ";
				std::cin >> edg;
				//a = __rdtsc();
				if (current_heap) {
					g2.random_gen_graph(vert, edg);
					if (g2.good_vertex.size()) {
						std::cout << "good started points for dijk: ";
						for (auto el : g2.good_vertex) {
							std::cout << el << ' ';
						}
						std::cout << '\n';
					}
				}
				else {
					g1.random_gen_graph(vert, edg);
					if (g1.good_vertex.size()) {
						std::cout << "good started points for dijk: ";
						for (auto el : g1.good_vertex) {
							std::cout << el << ' ';
						}
						std::cout << '\n';
					}
				}
				//b = __rdtsc();
				has_data[current_heap] = 1;
			}
			else if (q == "6") {
				cleanup();
				if (current_heap)
					g2.visualisation();
				else g1.visualisation();
			}
			else if (q == "e") {
				cleanup();
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
		cleanup();
		std::cout << e.what() << '\n';
	}
}