#include <iostream>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

// вершина графа
typedef struct Vertex{
    map <char, double> edges; // название соседней вершины и путь до нее
    vector <char> seen; // просмотренные вершины
    char name;
} Vertex;

// возвращает указатель на вершину в графе с именем name
Vertex* returnVertex(vector <Vertex*> &graph, char name){
    if (graph.empty())
        return nullptr;
    for (auto & vert : graph){
        if (vert->name == name)
            return vert;
    }
    return nullptr;
}

// проверка на содержание вершины с именем name в каком-либо векторе
bool inVect(const vector <char>& vect, char name){
    if (vect.empty())
        return false;
    for (auto & v : vect){
        if (v == name)
            return true;
    }
    return false;
}

// все ли ребра просмотрены
bool allSeen(Vertex &vert){
    if ((vert.seen).empty())
        return false;
    for (auto &edge : vert.edges){
        if (!inVect(vert.seen, edge.first))
            return false;
    }
    return true;
}

char findMin(Vertex &vertex, const vector <char>& stop){

    if (vertex.edges.empty() || allSeen(vertex)){ // в вершине больше соседних вершин, которые можно просмотреть
        cout << vertex.name << " has no free edges" << endl;
        return vertex.name;
    }

    double min = -1;
    char tmp = vertex.name; // вернет свое имя, если не найдется min

    for (auto & vert : vertex.edges){
        if (inVect(vertex.seen, vert.first)){
            // не рассматриваем вершину, которая уже просмотрена
            continue;
        }
        if (inVect(stop, vert.first)){
            // не рассматриваем вершину, которая лежит в stop
            continue;
        }
        if (min == -1 || vert.second < min){
            min = vert.second;
            tmp = vert.first;
        }
    }
    cout << "min: " << tmp << endl;
    return tmp;
}

void findPath(vector <Vertex*>& graph, char start, char finish, vector <char> &path){

    path.push_back(start);
    vector <char> stop; // содержит вершины, в которые уже точно можно не заходить

    while (true){
        cout << endl;
        cout << "Current path: ";
        for (auto & i : path)
            cout << i;
        cout << endl;
        cout << "Current stop list: ";
        for (auto & i : stop)
            cout << i;
        cout << endl;

        char current = path[path.size()-1]; // последняя просмотренная вершина
        cout << "Current vertex: " << current << endl;
        if (current == finish){
            cout << "Finish!" << endl;
            break;
        }

        Vertex *v = returnVertex(graph, current);
        if (v == nullptr){
            // текущая вершина - лист
            cout << current << " is empty"<< endl;
            cout << "Pop " << current << " from current path and push it in stop list" << endl;
            stop.push_back(current);
            path.pop_back();
            continue;
        }
        char min = findMin(*returnVertex(graph, current), stop);
        if (min == current){
            // из вершины больше некуда идти, все просмотрено
            cout << "No more ways from " << current << endl;
            stop.push_back(current);
            path.pop_back();
            continue;
        }
        returnVertex(graph, current)->seen.push_back(min);
        if (inVect(path, min)){
            if (allSeen(*v)){
                stop.push_back(current);
                path.pop_back();
                continue;
            }
        }
        path.push_back(min);
        cout << "Push " << min << endl;
    }

}

int main() {

    /*   ifstream fin("../input.txt");
       if (!fin)
           return 0;
   */

    while (true){
        cout << "Enter data:" << endl;
        char start, end;
        //    if (!(fin >> start))
        //        break;
        //    fin >> end;
        vector <Vertex*> graph;
        if (!(cin >> start) || start == '!')
            break;
        cin >> end;

        while (true){
            char v1, v2;
            double weight;
            //    if (!(fin >> v1) || v1 == '.')
            //        break;
            if (!(cin >> v1) || v1 == '.' || v1 == '!')
                break;
            //    fin >> v2 >> weight;
            cin >> v2 >> weight;

            if (auto *v = returnVertex(graph, v1)){
                v->edges[v2] = weight;
            } else {
                v = new Vertex;
                v->name = v1;
                v->edges[v2] = weight;
                graph.push_back(v);
            }
        }

        cout << "Graph:" << endl;
        for (auto & vert : graph){
            cout << vert->name << ": ";
            for (auto & edge : vert->edges)
                cout << edge.first << "(" << edge.second << ") ";
            cout << endl;
        }

        vector <char> path;
        findPath(graph, start, end, path);
        cout << "\nPath from " << start << " to " << end << ":" << endl;
        for (auto & i : path)
            cout << i;
        cout << endl << endl;
    }

    //  fin.close();

    return 0;
}