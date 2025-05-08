#pragma once
#include "vector.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
#include <cstdlib>
#include <type_traits>
#include <tuple>
#include "write_html.h"

#ifdef _WIN32
#define OS_NAME_NT
#endif

template<typename l, typename Queue>
class Graph {

    class Dijkstra {
        Graph* G;
        const l INF_= std::numeric_limits<l>::infinity();

        public:
            size_t start_vertex;
            vector<l> d;
            vector<bool> u;
            vector<vector<size_t>> paths;

            Dijkstra(size_t ver, Graph* g) :start_vertex(ver), G(g), d(g->table_size(), INF_), u(g->table_size()),paths(g->table_size()) {
                d[start_vertex] = 0;
                paths[start_vertex].push_back(start_vertex);
                Queue q;
                q.push(std::pair<size_t, size_t>{ 0, start_vertex });
                while (!q.empty()) {
                    size_t v = q.top().second, cur_d = q.top().first;
                    q.extract();
                    if (cur_d > d[v])  continue;

                    for (size_t j = 0; j < G->g[v].size(); ++j) {
                        size_t to = G->g[v][j].first;
                        l len = G->g[v][j].second;
                        if (d[v] + len < d[to]) {
                            d[to] = d[v] + len;
                            paths[to] = paths[v];
                            paths[to].push_back(to);
                            q.push(std::pair<size_t,size_t>{ d[to], to });
                        }
                    }
                }
            }
    public:
        bool is_connected() {   //то что метод в классе дейкстры гарантирует то, что проверка будет осуществлятся на выполненном алгосе
            for (auto el : d) if (std::isinf(el)) return false;
            return true;
        }
        bool has_path(size_t v) {
            return !std::isinf(d[v]);
        }
    };
    
public:
    vector<vector<std::pair<size_t, l>>> g;

    Graph::Dijkstra* dijkstra;

    Graph() :g(),dijkstra(nullptr) {};
    ~Graph() {
        if (dijkstra) delete dijkstra;
    }
    bool has_path(size_t v) {
        if (!dijkstra) throw std::logic_error("do dijkstra first");
        return dijkstra->has_path(v);
    }
	Graph(const vector<vector<std::pair<size_t, l>>>& g_): g(g_), dijkstra(nullptr){};
	size_t table_size()const { return g.size(); }
    size_t row_size(size_t j) const { return g.at(j).size(); }

    void random_gen_graph(size_t vert, size_t edges) {
        //граф ориентированный и может содержать петли и может содержать несколько ребер между двумя вершинами, поэтому нет зависимости на кол-во ребер
        g.clear();
        g.resize(vert);
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t a, b;
        auto distrib = std::uniform_int_distribution<size_t>(0, vert-1);
        l len;
        if constexpr (std::is_integral_v<l>) {
            auto distrib_l = std::uniform_int_distribution<l>(0, 1000000);
            for (size_t i = 0; i < edges; i++) {
                a = distrib(gen);
                b = distrib(gen);
                len = distrib_l(gen);
                g[a].push_back(std::pair<size_t, l>{b, len});
            }
        }
        else if constexpr (std::is_floating_point_v<l>) {
            auto distrib_l = std::uniform_real_distribution<l>(0.0, 1000000.0);
            for (size_t i = 0; i < edges; i++) {
                a = distrib(gen);
                b = distrib(gen);
                len = distrib_l(gen);
                g[a].push_back(std::pair<size_t, l>{b, len});
            }
        }
        else {
            throw std::logic_error("unsupported type for random")
        }

        return;
    }

    void do_dijkstra(size_t start_vert) {
        dijkstra = new Dijkstra(start_vert, this);
    }
    l get_distance(size_t vert) {
        if (!dijkstra) throw std::logic_error("you didn't run dijkstra!");
        return dijkstra->d[vert];
    }
    auto get_path(size_t vert) {
        if (!dijkstra) throw std::logic_error("you didn't run dijkstra!");
        return dijkstra->paths[vert];
    }

    friend std::ostream& operator<<(std::ostream& os, Graph& g) {
        size_t sz = g.table_size();
        size_t sz_l = 0;
        for (size_t i = 0; i < g.table_size(); i++) {
            sz_l += g.g[i].size();
        }
        os << sz<<' '<<sz_l << '\n';
        for (size_t i = 0; i < sz; i++) for (size_t j = 0; j < g.row_size(i); j++)
                os << i << ' ' << g.g[i][j].first << ' ' << g.g[i][j].second << '\n';
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Graph& g) {
        size_t sz_vert, sz_l;
        std::cout << "type count of vertex: ";
        is >> sz_vert;
        std::cout << "type count of edges: ";
        is >> sz_l;
        l tmp;
        g.g.resize(sz_vert);
        size_t a, b;
        for (size_t i = 0; i < sz_l; i++) {
            std::cout << "type edge " << i << " in format 'a b c', where a and b - number of vertex (0...n-1), c - weight\n";
            is >> a >> b >> tmp;
            if (tmp < 0) throw std::logic_error("Elements must be greater 0");
            g.g.at(a).push_back(std::pair<size_t, l>{ b,tmp });
        }
        return is;
    }
    void visualisation() {
        write_js_script("graph.html");
        std::ofstream GRAPH("graph_data.json");
        if (!GRAPH.is_open()) {
            throw std::logic_error("cant open file to write");
        }
        GRAPH << "{\n";
        GRAPH << "  \"nodes\": [\n";
        for (size_t i = 0; i < g.size(); ++i) {
            GRAPH << "    { \"id\": " << i << ", \"label\": \"" << i << "\" }";
            if (i < g.size() - 1) {
                GRAPH << ",";
            }
            GRAPH << "\n";
        }
        GRAPH << "  ],\n";
        GRAPH << "  \"edges\": [\n";
        size_t sz_l = 0;
        for (size_t i = 0; i < g.size(); i++) {
            sz_l += g[i].size();
        }
        size_t cnt = 0;
        for (size_t i = 0; i < g.size(); i++) {
            for (auto ell : g[i]) {
                GRAPH << "    { \"from\": " << i << ", \"to\": " << ell.first << ", \"label\": \"" << std::fixed << std::setprecision(2) << ell.second << "\", \"weight\": " << std::fixed << std::setprecision(2) << ell.second << " }";
                cnt++;
                if (cnt < sz_l) {
                    GRAPH << ",";
                }
                GRAPH << "\n";
            }
        }
        GRAPH << "  ]\n";
        GRAPH << "}\n";
        GRAPH.close();
        std::cout << "Please turn off the server after you have played enough.\n";
        #ifdef OS_NAME_NT
            std::string startServer = "start python -m http.server 8000";
        #else
            std::string startServer = "python -m http.server 8000 &";
        #endif
        
        int res = system(startServer.c_str());
        if (res) {
            throw std::logic_error("cant start server");
        }
        std::string brows;

        #ifdef OS_NAME_NT
            brows = "start http://localhost:8000/graph.html";
        #else
            brows = "xdg-open http://localhost:8000/graph.html";
        #endif
        int bres = system(brows.c_str());
        if (bres) {
            throw std::logic_error("cant start browser");
        }

        return;
    }
};