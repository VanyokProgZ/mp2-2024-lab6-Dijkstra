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

#ifdef _WIN32
#include <windows.h>
PROCESS_INFORMATION pi;
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
pid_t child_pid;
#endif


//Это для сервера (для визуализации) в дочернем процессе
void cleanup() {
#ifdef _WIN32
    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hProcess);
#else
    if (child_pid > 0) kill(child_pid, SIGTERM);
#endif
}

void startChildProcess(const std::string& command) {
#ifdef _WIN32
    STARTUPINFO si = { sizeof(si) };
    char cmd[256];
    strncpy(cmd, command.c_str(), sizeof(cmd) - 1);
    cmd[255] = '\0';

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
    }
#else
    child_pid = fork();
    if (child_pid == 0) {
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
        exit(EXIT_FAILURE);
    }
    else if (child_pid < 0) {
        std::cerr << "Fork failed" << std::endl;
    }
#endif
}

template <class T>
void random_shuffle(vector<T>& v) {
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t n = v.size();

    for (size_t i = n - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = dist(gen);
        std::swap(v[i], v[j]);
    }
}

template <typename T>
bool check(const vector<T>& v, size_t el) {
    for (size_t i = 0; i < v.size(); i++) {
        if (el == v[i].first) return true;
    }
    return false;
}

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
            if (v >= d.size()) {
                throw std::logic_error("netu takoy vershini");
            }
            return !std::isinf(d[v]);
        }
    };
    size_t vert_cnt(size_t th, vector<bool>& was, size_t c) {
        c++;
        was[th] = 1;
        for (size_t i = 0; i < g[th].size(); i++) {
            if (!was[g[th][i].first]) c += vert_cnt(g[th][i].first,was,0);
        }
        return c;
    }
    
public:
    vector<vector<std::pair<size_t, l>>> g;
    vector<size_t> good_vertex;

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


    void check_graph(vector<size_t>& vec) {

        size_t all_v = g.size();
        vector<bool> was(g.size(),0);
        for (size_t i = 0; i < g.size(); i++) {
            size_t r = vert_cnt(i,was,0);
            if (r == all_v) {
                vec.push_back(i);
            }
            was = vector<bool>(g.size(),0);
        }
    }

    void random_gen_graph(size_t vert, double procent) {
        good_vertex = vector<size_t>();
        //граф ориентированный и может содержать петли и может содержать несколько ребер между двумя вершинами, поэтому нет зависимости на кол-во ребер
        g = vector<vector<std::pair<size_t, l>>>(vert);
        if (vert == 0)return;
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t a, b;
        auto distrib = std::uniform_int_distribution<size_t>(0, vert-1);
        l len;
        //вообще в случае мультиграфа мы вполне можем иметь значение procent > 1...
        if (procent > 1) {
            throw std::logic_error("bad procent");
        }
        
        if constexpr (std::is_integral_v<l>) {
            auto distrib_l = std::uniform_int_distribution<l>(0, 100);
            vector<size_t> pre_vert;
            for (size_t i = 0; i < vert; i++) {
                pre_vert.push_back(i);
            }
            random_shuffle(pre_vert);
            //std::cout << "Garanted good for dijkstra vertex: " << pre_vert[0]<<'\n';
            size_t sz = 0;
            for (size_t i = 0; i < pre_vert.size()-1; i++) {
                len = distrib_l(gen);
                g[pre_vert[i]].push_back(std::pair(size_t, l) { pre_vert[i + 1], len });
                sz++;
            }

            vector<std::pair<size_t, size_t>> free;
            for (size_t i = 0; i < vert; i++) for (size_t j = 0; j < vert; j++) {
                if (check(g[i], j)) {
                    free.push_back(std::pair<size_t, size_t>{i, j});
                }
            }
            random_shuffle(free);
            size_t ind = 0;
            while ((double)sz / (double)vert*(vert-1.0) < procent-0.0000000001) {
                sz++;
                len = distrib_l(gen);
                g[free[ind].first].push_back(std::pair<size_t, l>{free[ind].second,len });
                ind++;
            }

        }
        else if constexpr (std::is_floating_point_v<l>) {
            auto distrib_l = std::uniform_real_distribution<l>(0.0, 100.0);
            vector<size_t> pre_vert;
            for (size_t i = 0; i < vert; i++) {
                pre_vert.push_back(i);
            }
            random_shuffle(pre_vert);
            //std::cout << "Garanted good for dijkstra vertex: " << pre_vert[0]<<'\n';
            size_t sz = 0;
            for (size_t i = 0; i < pre_vert.size() - 1; i++) {
                len = distrib_l(gen);
                g[pre_vert[i]].push_back(std::pair<size_t, l> { pre_vert[i + 1], len });
                sz++;
            }

            vector<std::pair<size_t, size_t>> free;
            for (size_t i = 0; i < vert; i++) for (size_t j = 0; j < vert; j++) {
                if (!check(g[i], j) && i!=j) {
                    free.push_back(std::pair<size_t, size_t>{i, j});
                }
            }

            random_shuffle(free);
            size_t ind = 0;
            while ((double)sz /( (double)vert * (vert - 1.0) )< procent - 0.0000000001) {
                sz++;
                len = distrib_l(gen);
                g[free[ind].first].push_back(std::pair<size_t, l>{free[ind].second, len });
                ind++;
            }
        }
        else {
            throw std::logic_error("unsupported type for random")
        }
        check_graph(good_vertex);
        return;
    }

    void do_dijkstra(size_t start_vert) {
        
        bool found = 0;
        for (auto el : good_vertex) {
            if (el == start_vert) { found = 1; break; }
        }
        if (!found) {
            std::cout << "There are ways from this vertex not to all, try another started vertex or graph\n";
            return;
        }
        if (start_vert >= g.size()) {
            throw std::out_of_range("out of range");
        }
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
        g.good_vertex = vector<size_t>();
        size_t sz_vert, sz_l;
        std::cout << "type count of vertex: ";
        is >> sz_vert;
        std::cout << "type count of edges: ";
        is >> sz_l;
        l tmp;
        g.g = vector<vector<std::pair<size_t, l>>>(sz_vert);
        size_t a, b;
        for (size_t i = 0; i < sz_l; i++) {
            std::cout << "type edge " << i << " in format 'a b c', where a and b - number of vertex (0...n-1), c - weight\n";
            is >> a >> b >> tmp;
            if (a >= g.g.size() || b >= g.g.size()) {
                throw std::out_of_range("big vertex number!");
            }
            if (tmp < 0) throw std::logic_error("Elements must be greater 0");
            g.g.at(a).push_back(std::pair<size_t, l>{ b,tmp });
        }
        g.check_graph(g.good_vertex);
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
            std::string startServer = "python -m http.server 8000";
        #else
            std::string startServer = "python -m http.server 8000";
        #endif
        
        startChildProcess(startServer.c_str());
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
