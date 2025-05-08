#include "write_html.h"

void write_js_script(const std::string& name) {

	std::ofstream FILE_(name);
	if (!FILE_.is_open()) {
		throw std::logic_error("cant open file");
	}
	FILE_ << "<!DOCTYPE html>\n";
	FILE_ << "<html>\n";
	FILE_ << "<head>\n";
	FILE_ << "  <title>Graph</title>\n";
	FILE_ << "  <style type=\"text/css\">\n";
	FILE_ << "    #mynetwork {\n";
	FILE_ << "      width: 800px;\n";
	FILE_ << "      height: 600px;\n";
	FILE_ << "      border: 1px solid lightgray;\n";
	FILE_ << "    }\n";
	FILE_ << "  </style>\n";
	FILE_ << "  <script type=\"text/javascript\" src=\"https://unpkg.com/vis-network/standalone/umd/vis-network.min.js\"></script>\n";
	FILE_ << "</head>\n";
	FILE_ << "<body>\n";
	FILE_ << "\n";
	FILE_ << "  <h1>Граф можно потрогать, подвигать, помасштабировать...). Вершины делают увиииииии...</h1>\n";
	FILE_ << "  <h2>Пожалуйста, выключите сервер когда визуализация станет не нужна</h2>\n";
	FILE_ << "  <p>Если вдруг в квадратике ничего нет, то возможно надо подождать - данные подгружаются из json</p>\n";
	FILE_ << "  <div id=\"mynetwork\"></div>\n";
	FILE_ << "\n";
	FILE_ << "  <script type=\"text/javascript\">\n";
	FILE_ << "    const container = document.getElementById('mynetwork');\n";
	FILE_ << "\n";
	FILE_ << "    const options = {\n";
	FILE_ << "        interaction: {\n";
	FILE_ << "            dragNodes: true,\n";
	FILE_ << "            zoomView: true,\n";
	FILE_ << "            dragView: true\n";
	FILE_ << "        },\n";
	FILE_ << "        physics: {\n";
	FILE_ << "            enabled: true,\n";
	FILE_ << "            stabilization: {\n";
	FILE_ << "                iterations: 1000,\n";
	FILE_ << "                fit: true\n";
	FILE_ << "            }\n";
	FILE_ << "        },\n";
	FILE_ << "        edges: {\n";
	FILE_ << "            arrows: {\n";
	FILE_ << "                to: { enabled: true }\n";
	FILE_ << "            }\n";
	FILE_ << "        }\n";
	FILE_ << "    };\n";
	FILE_ << "\n";
	FILE_ << "    fetch('graph_data.json')\n";
	FILE_ << "      .then(response => response.json())\n";
	FILE_ << "      .then(data => {\n";
	FILE_ << "        const nodes = new vis.DataSet(data.nodes);\n";
	FILE_ << "        const edges = new vis.DataSet(data.edges);\n";
	FILE_ << "\n";
	FILE_ << "        const dataVis = {\n";
	FILE_ << "          nodes: nodes,\n";
	FILE_ << "          edges: edges\n";
	FILE_ << "        };\n";
	FILE_ << "\n";
	FILE_ << "        const network = new vis.Network(container, dataVis, options);\n";
	FILE_ << "      })\n";
	FILE_ << "      .catch(error => console.error('error load:', error));\n";
	FILE_ << "  </script>\n";
	FILE_ << "\n";
	FILE_ << "</body>\n";
	FILE_ << "</html\n";

}