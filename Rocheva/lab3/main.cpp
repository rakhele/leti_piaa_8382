#include <iostream>
#include <vector>
#include <algorithm>

struct Vertex {
    std::vector<std::pair<char, std::pair<int, int>>> neighbors; // вершины, в которые можно попасть из данной:
                                                                 // <имя вершины, <C, поток>>
    char name;
    bool visited;
    char parent;
};

// возвращает вершину сети по заданному имени
Vertex *returnVertex(char name, std::vector<Vertex> &net){
    for (auto & vertex : net)
        if (vertex.name == name)
            return &vertex;
    return nullptr;
}

// возвращает следующую вершину, которую нужно рассматривать в поиске в глубину
char returnNextVertex(Vertex &vertex, std::vector<Vertex> &net){
    char nextVertex = '\0';
    for (auto & vert : vertex.neighbors){
        // если вершина еще не посещена и емкость ребра больше нуля
        if(!(returnVertex(vert.first, net)->visited) && vert.second.first > 0)
            nextVertex = vert.first;
    }
    return nextVertex;
}

// ищем путь поиском в глубину
std::vector<char> findPath(std::vector<Vertex> net, char startVertex, char finishVertex){
    char currentVertex = startVertex;
    std::vector<char> path;
    path.push_back(startVertex);

    char nextVertex = returnNextVertex(*returnVertex(currentVertex, net), net);
    if (nextVertex == '\0')
        return path; // собственно возвратится стартовая вершина, т.к. сеть состоит только из нее

    // итеративный поиск в глубину. Закончится, когда дойдем до конца или если текущая вершина равна стартовой
                                                // и при этом переход в следующую вершину не может быть осуществлен
    while(currentVertex != finishVertex && !(currentVertex == startVertex && nextVertex == '\0')){

        returnVertex(currentVertex, net)->visited = true;
        if(nextVertex != '\0') {
            returnVertex(nextVertex, net)->parent = currentVertex;
            currentVertex = nextVertex;
            path.push_back(currentVertex);
        }
        else { // нет свободной вершины, в которую можно перейти
            path.pop_back();
            currentVertex = returnVertex(currentVertex, net)->parent;
        }

        if ((returnVertex(currentVertex, net)->neighbors).empty()) // у новой текущей вершины нет соседей
            continue;

        nextVertex = returnNextVertex(*returnVertex(currentVertex, net), net);
    }

    return path;
}

// ищем ребро с минимальной пропускной способностью в пути и возвращаем это значение
int minPathCapacity(std::vector<char> path, std::vector<Vertex> &residualNet){
    int minCapacity = -1;
    while(path.size() > 1){

        // рассматриваем соседей вершины, которая сейчас находится в начале пути
        for (auto & vertex : (returnVertex(path[0], residualNet)->neighbors))
            // если нашли соседа, который является второй вершиной в пути,
            // и при этом емкость через ребро к этой вершине меньше известной минимальной, то меняем мин. емкость
            if (vertex.first == path[1] && (minCapacity == -1 || vertex.second.first < minCapacity)) {
                std::cout << "capacity (" << path[0] << ", " << path[1] << ") = " << vertex.second.first << std::endl;
                std::cout << "now it is minimum capacity." << std::endl;
                minCapacity = vertex.second.first;
            }
        path.erase(path.begin()); // удаляем текущую верхнюю вершину пути
    }
    return minCapacity;
}

// модифицируем остаточную сеть,
void modifyNet(std::vector<char> &path, std::vector<Vertex>& residualNet){
    std::cout << "Update residual net using path [";
    for (auto & vert : path)
        std::cout << vert;
    std::cout << "]" << std::endl;
    int minCapacity = minPathCapacity(path, residualNet);
    std::cout << "Minimum capacity for path = " << minCapacity << std::endl;

    while(path.size() > 1){

        for (auto & vertex1 : returnVertex(path[0], residualNet)->neighbors){

            if (vertex1.first == path[1]) { // если сосед является следующей вершиной пути
                vertex1.second.second += minCapacity; // добавляем поток соседу
                vertex1.second.first -= vertex1.second.second; // и уменьшаем его пропускную способность

                // пробегаем по соседам соседа в поисках обратного ребра
                for (auto & vertex2 : returnVertex(path[1], residualNet)->neighbors){
                    if (vertex2.first == path[0]){ // если нашли ребро к родительской вершине
                        vertex2.second.second -= minCapacity; // то уменьшаем поток между ними
                        vertex2.second.first -= vertex2.second.second; // и емкость
                    }
                }
            }
        }
        path.erase(path.begin());
    }
}

// для соседей вершины
bool cmpVert(std::pair<char, std::pair<int, int>> const &a, std::pair<char, std::pair<int, int>> const &b) {
    return a.first < b.first;
}

// для графа
bool cmpNet(Vertex const &a, Vertex const &b){
    return a.name < b.name;
}

void findMaxFlow(std::vector<Vertex> & residualNet, char sourceVertex, char sinkVertex){

    std::vector<char> path = findPath(residualNet, sourceVertex, sinkVertex);

    while (path.size() != 1) {
        modifyNet(path, residualNet);
        path = findPath(residualNet, sourceVertex, sinkVertex);
    }

    int maxFlow = 0;

    std::vector<int> resultFlows;
    // макимальный поток = сумма потоков соседей истока
    std::cout << "max flow = 0";
    for (auto & vertex : returnVertex(sourceVertex, residualNet)->neighbors){
        std::cout << " + max flow (" << vertex.first << ")";
        resultFlows.push_back(vertex.second.second);
        maxFlow += vertex.second.second;
    }
    std::cout << " = 0";
    for (auto & flow : resultFlows)
        std::cout << " + " << flow;
    std::cout << " = " << maxFlow << std::endl;

    std::cout << "RESULT" << std::endl;
    std::cout << maxFlow << std::endl;
    std::sort(residualNet.begin(), residualNet.end(), cmpNet);
    for(auto & vertex : residualNet) {
        std::sort(vertex.neighbors.begin(), vertex.neighbors.end(), cmpVert);
        for(auto neighbor : vertex.neighbors)
            std::cout << vertex.name << " " << neighbor.first << " " << std::max(0, neighbor.second.second) << std::endl;
    }
}

int main(){
    std::vector<Vertex> residualNet; // остаточная сеть
    int n;
    char sourceVertex, sinkVertex; // исток и сток
    std::cin >> n;
    std::cin >> sourceVertex;
    std::cin >> sinkVertex;
    char from, to;
    int capacity;
    for(int i = 0; i < n; i++) {
        std::cin >> from >> to >> capacity;
        Vertex *vertex = returnVertex(from, residualNet);
        if (vertex == nullptr){
            vertex = new Vertex;
            vertex->name = from;
            vertex->visited = false;
            vertex->neighbors.push_back({to, {capacity, 0}});
            residualNet.push_back(*vertex);
        } else {
            vertex->neighbors.push_back({to, {capacity, 0}}); // 0 - обнуляем все потоки сначала
        }
        if (!returnVertex(to, residualNet)){
            auto vertex1 = new Vertex;
            vertex1->name = to;
            vertex1->visited = false;
            residualNet.push_back(*vertex1);
        }
    }

    std::cout << "Graph:" << std::endl;
    for (auto & edge : residualNet){
        std::cout << edge.name << ":";
        for (auto & ver : edge.neighbors)
            std::cout << "\t" << ver.first << " " << ver.second.first << " " << ver.second.second << std::endl;
        std::cout << std::endl;
    }

    findMaxFlow(residualNet, sourceVertex, sinkVertex);

    return 0;
}
