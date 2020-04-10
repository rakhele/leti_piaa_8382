#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <functional>

using namespace std;

//вершина графа
typedef struct Vertex{
    map <char, double> edges; // соседние вершины с длинами путей к ним
    bool isSeen; // показывает, обработа ли вершина
    double pathFromStart; // g(x) - расстояние от начальной вершины до текущей
    char name;
} Vertex;

// эвристическая оценка расстояния до конечной вершины
double h(char current, char end){
    return end - current;
}

// возвращает указатель на вершину их графа с именем name
Vertex* returnVertex(const vector <Vertex*>& graph, char name){
    if (graph.empty())
        return nullptr;
    for (auto & vert : graph){
        if (vert->name == name)
            return vert;
    }
    return nullptr;
}

// находится ли вершина curr в векторе vect
bool inVect(const vector <Vertex*>& vect, Vertex *curr){
    if (vect.empty())
        return false;
    for (auto & v : vect){
        if (v->name == curr->name)
            return true;
    }
    return false;
}

// ищем вершину с самой низкой оценкой f(x) = g(x) + h(x)
Vertex* findMin(vector <Vertex*>& openset, char end){
    Vertex *cmp = openset[0];
    int ind = 0;
    int i = 0;
    double f = cmp->pathFromStart + h(cmp->name, end);
    for (auto & vert : openset){
        if (((vert->pathFromStart + h(vert->name, end)) < f) || (((vert->pathFromStart + h(vert->name, end)) == f) &&
                                                                 vert->name > cmp->name)){
            f = vert->pathFromStart + h(vert->name, end);
            cmp = vert;
            ind = i;
        }
        i++;
    }
    // удаляем эту вершину их openset
    openset.erase(openset.begin() + ind);
    return cmp;
}

void printPath(map <Vertex*, Vertex*>& fromset, char start, char end, vector <Vertex*>& graph){
    if (end == start){
        cout << end;
        return;
    }
    printPath(fromset, start, (fromset[returnVertex(graph, end)])->name, graph);
    cout << end;
}

int checkCurrent(vector <Vertex*>& graph, vector <Vertex*>* openset, map <Vertex*, Vertex*>* fromset, char start, char end){

    auto currentVert = findMin(*openset, end);
    cout << "Current vertex: " << currentVert->name << endl;
    if (currentVert->name == end){
        cout << "Path to (" << end << ") was found" << endl;
        printPath(*fromset, start, end, graph);
        return 1;
    }

    currentVert->isSeen = true; // всё, показываем, что вершина просмотрена
    cout << "Neighbours:" << endl;
    for (auto & edge : currentVert->edges) { // цикл по соседям
        cout << edge.first;
        auto neighbour = returnVertex(graph, edge.first);
        if (neighbour->isSeen) {
            cout << " has already been viewed" << endl;
            continue;
        }

        double tentative_g = currentVert->pathFromStart + edge.second; // текущая оценка g

        bool tentativeIsBetter;
        if (!inVect(*openset, neighbour)) {
            cout << " add to openset" << endl;
            openset->push_back(neighbour);
            tentativeIsBetter = true; // ее не было в openset, поэтому точно придется обновлять свойства
        } else {
            tentativeIsBetter = tentative_g < neighbour->pathFromStart; // true - найден лучший путь до этого соседа
        }

        if (tentativeIsBetter) { // обновляем свойства вершины
            (*fromset)[neighbour] = currentVert; // добавляем в карту
            neighbour->pathFromStart = tentative_g;
        }
    }
    return 0;
}

void findPath(vector <Vertex*>& graph, char start, char end1, char end2){

    bool toEnd2 = true;
    bool toEnd1 = true;
    if (end2 == '\0') // отсутсвует вторая кончная вершина
        toEnd2 = false;

    vector <Vertex*> openset1; // вершины, которые стоит обработать
    map <Vertex*, Vertex*> fromset1; // карта пройденных вершин
    returnVertex(graph, start)->pathFromStart = 0;
    openset1.push_back(returnVertex(graph, start)); // сразу добавляем стартовую точку

    vector <Vertex*> openset2;
    map <Vertex*, Vertex*> fromset2;
    if (toEnd2){
        openset2.push_back(returnVertex(graph, start));
    }

    while (true){
        if (openset1.empty()){
            if (!toEnd2 || (toEnd2 && openset2.empty()))
                break;
            toEnd1 = false;
        }
        if (toEnd2 && openset2.empty()){
            toEnd2 = false;
        }

        cout << "============" << endl;
        cout << "To first end:" << endl << endl;
        if (toEnd1 && checkCurrent(graph, &openset1, &fromset1, start, end1)){
            return;
        }

        if (toEnd2){
            cout << "=============" << endl;
            cout << "To second end:" << endl << endl;
            if (checkCurrent(graph, &openset2, &fromset2, start, end2))
                return;
        }
    }
}


int main(){
/*
    ifstream fin("../input.txt");
    if (!fin)
        return 0;*/

    string s;
    while (true){
        cout << "Enter data" << endl;
        char start, end1, end2;
        if (!(cin >> start) || start == '!')
            break;
        getline(cin, s);
        end1 = s[1];
        if (s[3]){
            end2 = s[3];
        } else {
            end2 = '\0';
            cout << "\nWithout second end." << endl;
        }
        vector <Vertex*> graph;

        while (true){
            char v1, v2;
            double weight;
            if (!(cin >> v1) || v1 == '.')
                break;
            cin >> v2 >> weight;

            if (auto *v = returnVertex(graph, v1)){
                v->edges[v2] = weight;
            } else {
                v = new Vertex;
                v->name = v1;
                v->edges[v2] = weight;
                v->isSeen = false;
                graph.push_back(v);
            }
            if (auto *v = returnVertex(graph, v2)){
                continue;
            } else {
                v = new Vertex;
                v->name = v2;
                v->isSeen = false;
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
        findPath(graph, start, end1, end2);
        cout << endl << endl;
    }

   // fin.close();

    return 0;
}
