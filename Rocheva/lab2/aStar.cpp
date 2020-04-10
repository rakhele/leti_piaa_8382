#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <functional>
#include <cmath>

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
Vertex* findMin(vector <Vertex*>& openset, char end1, char end2){
    Vertex *cmp = openset[0];
    int ind = 0;
    int i = 0;
    double hCmpEnd1 = h(cmp->name, end1);
    double hCmpEnd2 = h(cmp->name, end2);
    double f;
    if (end2 == '\0'){
        f = hCmpEnd1 + cmp->pathFromStart;
    }
    else {
        if (fabs(hCmpEnd1) < fabs(hCmpEnd2))
            f = hCmpEnd1 + cmp->pathFromStart;
        else
            f = hCmpEnd2 + cmp->pathFromStart;
    }
    for (auto & vert : openset){
        cout << "Vertex: " << vert->name << endl;
        double hEnd1 = h(vert->name, end1);
        double hEnd2 = h(vert->name, end2);
        cout << "to (" << end1 << ") = " << hEnd1 << "; to (" << end2 << ") = " << hEnd2 << endl;
        double hVert;
        if (end2 == '\0'){
            hVert = hEnd1;
        }
        else {
            if (fabs(hEnd1) < fabs(hEnd2))
                hVert = hEnd1;
            else
                hVert = hEnd2;
        }
        cout << "choose " << hVert << endl;
        cout << "h(x) for: " << vert->name << " = " << hVert << ", abs (" << vert->name << ") = " << fabs(hVert) << endl;
        hVert = fabs(hVert);
        if (((vert->pathFromStart + hVert) < f) || (((vert->pathFromStart + hVert) == f) &&
                                                    vert->name > cmp->name)){
            f = vert->pathFromStart + hVert;
            cout << "new min: " << vert->name << endl;
            cmp = vert;
            ind = i;
        }
        i++;
    }
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

int checkCurrent(vector <Vertex*>& graph, vector <Vertex*>* openset, map <Vertex*, Vertex*>* fromset, char start, char end1, char end2){

    auto currentVert = findMin(*openset, end1, end2);
    cout << "Current vertex: " << currentVert->name << endl;
    if (currentVert->name == end1){
        cout << "Path to (" << end1 << ") was found" << endl;
        printPath(*fromset, start, end1, graph);
        return 1;
    }
    if (end2 != '\0' && currentVert->name == end2){
        cout << "Path to (" << end2 << ") was found" << endl;
        printPath(*fromset, start, end2, graph);
        return 1;
    }

    currentVert->isSeen = true;
    cout << "Neighbours:" << endl;
    for (auto & edge : currentVert->edges) {
        cout << edge.first;
        auto neighbour = returnVertex(graph, edge.first);
        if (neighbour->isSeen) {
            cout << " has already been viewed" << endl;
            continue;
        }

        double tentative_g = currentVert->pathFromStart + edge.second;

        bool tentativeIsBetter;
        if (!inVect(*openset, neighbour)) {
            cout << " add to openset" << endl;
            openset->push_back(neighbour);
            tentativeIsBetter = true;
        } else {
            tentativeIsBetter = tentative_g < neighbour->pathFromStart;
        }

        if (tentativeIsBetter) {
            (*fromset)[neighbour] = currentVert;
            neighbour->pathFromStart = tentative_g;
        }
    }
    return 0;
}

void findPath(vector <Vertex*>& graph, char start, char end1, char end2) {

    vector<Vertex *> openset1; // вершины, которые стоит обработать
    map<Vertex *, Vertex *> fromset1; // карта пройденных вершин
    returnVertex(graph, start)->pathFromStart = 0;
    openset1.push_back(returnVertex(graph, start)); // сразу добавляем стартовую точку

    while (!openset1.empty()) {
        cout << "============" << endl;
        if (checkCurrent(graph, &openset1, &fromset1, start, end1, end2)) {
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
