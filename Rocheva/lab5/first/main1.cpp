#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::pair;

#define SIZE 5

struct BohrVertex{
    vector<int> nextVert;
    vector<int> goArray;
    int suffLink = -1;
    int up = -1;
    int parent;
    char toParent;
    int numVert; // номер вершины
    int numPattern = 0;
    bool isFinal;
};

class Bohr{
public:
    Bohr(){
        bohr.push_back(createVertex(-1, '0'));
        bohrAlp = {{'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4} };
    }
    BohrVertex *createVertex(int parent, char toParent){
        auto *vertex = new BohrVertex;
        vertex->nextVert.assign(SIZE, -1);
        vertex->goArray.assign(SIZE, -1);
        vertex->parent = parent;
        vertex->toParent = toParent;
        vertex->numVert = bohr.size();
        vertex->suffLink = -1;
        return vertex;
    }

    void addPattern(string &pattern){
        cout << "Add pattern [" << pattern << "] to bohr:" << endl;
        int currentVert = 0;
        for (auto & symb : pattern){
            if (bohr[currentVert]->nextVert[bohrAlp[symb]] == -1){
                auto *newVertex = createVertex(currentVert, symb);
                newVertex->isFinal = false;
                bohr.push_back(newVertex);
                bohr[currentVert]->nextVert[bohrAlp[symb]] = bohr.size()-1;
                cout << "Created vertex " << newVertex->numVert << " for edge " << symb << endl;
            }
            currentVert = bohr[currentVert]->nextVert[bohrAlp[symb]];
        }
        patterns[currentVert] = pattern;
        bohr[currentVert]->numPattern = patterns.size();
        numPatterns[patterns.size()] = pattern;
        bohr[currentVert]->isFinal = true;
    }

    int getAutoLink(int vertex, char path){
        cout << "finding autolink for vertex " << vertex << " with path " << path << endl;
        if (bohr[vertex]->goArray[bohrAlp[path]] == -1){
            if (bohr[vertex]->nextVert[bohrAlp[path]] != -1){
                cout << "get autolink to vertex " << bohr[vertex]->nextVert[bohrAlp[path]] << endl;
                bohr[vertex]->goArray[bohrAlp[path]] = bohr[vertex]->nextVert[bohrAlp[path]];
            }
            else if (bohr[vertex]->numVert == 0){
                cout << "get autolink to root" << endl;
                bohr[vertex]->goArray[bohrAlp[path]] = 0;
            }
            else {
                bohr[vertex]->goArray[bohrAlp[path]] = getAutoLink(getSuffLink(vertex), path);
            }
        }
        cout << "vertex " << vertex << " has autolink " << bohr[vertex]->goArray[bohrAlp[path]] << "with path" << path << endl;
        return bohr[vertex]->goArray[bohrAlp[path]];
    }

    int getUpLink(int vertex){
        cout << "finding compressed link for vertex " << vertex << endl;
        if (bohr[vertex]->up == -1) {
            int vert = getSuffLink(vertex);
            if (vert == 0) {
                cout << "get compressed link to root" << endl;
                bohr[vertex]->up = 0;
            }
            else {
                if (bohr[vert]->isFinal){
                    bohr[vertex]->up = vert;
                    cout << "get compressed link to " << vert << endl;
                }
                else {
                    cout << "get compressed link of vertex " << vert << endl;
                    bohr[vertex]->up = getUpLink(vert);
                }
            }
        }
        cout << "compressed link for " << vertex << " = " << bohr[vertex]->up << endl;
        return bohr[vertex]->up;
    }

    int getSuffLink(int vertex){
        cout << "finding suffix link for vertex " << vertex << endl;
        if (bohr[vertex]->suffLink == -1){
            if (bohr[vertex]->numVert == 0 || bohr[vertex]->parent == 0){
                cout << "get link to root" << endl;
                bohr[vertex]->suffLink = 0;
            }
            else {
                cout << "get link through parent" << endl;
                bohr[vertex]->suffLink = getAutoLink(getSuffLink(bohr[vertex]->parent), bohr[vertex]->toParent);
            }
        }
        cout << "suffix link for " << vertex << " = " << bohr[vertex]->suffLink << endl;
        return bohr[vertex]->suffLink;
    }

    void processText(string &text){
        cout << endl << "Finding patterns in text [" << text << "]" << endl;
        map<int, vector<int>> result; // для сортировки еще
        cout << "current vertex in bohr = root" << endl;
        int currentVertex = 0;
        int index = 0;
        for (auto & symb : text){
            currentVertex = getAutoLink(currentVertex, symb);
            cout << endl << "current vertex in bohr = " << currentVertex << endl;
            int i = index;
            cout << "Search with this start vertex:" << endl;
            for (int vert = currentVertex; vert != 0; vert = getUpLink(vert)){
                if (bohr[vert]->isFinal) {
                    cout << ">>final state for pattern [" << numPatterns[bohr[vert]->numPattern] << "] was found" << endl;
                    cout << "start index in text: " << i + 1 - patterns[vert].size() + 1 << endl << endl;
                    result[i + 1 - patterns[vert].size() + 1].push_back(bohr[vert]->numPattern);
                    i--;
                }
                i++;
            }
            index++;
        }
        cout << "end of text" << endl;

        cout << endl << "Bohr:" << endl;
        for (auto & vert : bohr){
            cout << "From ";
            vert->numVert == 0 ? cout << "root" : cout << "vertex " << vert->numVert;
            cout << endl;
            for (auto & next : vert->nextVert){
                if (next == -1)
                    continue;
                cout << "\tto vertex " << next << " with edge " << bohr[next]->toParent << endl;
            }
        }
        cout << endl << "Used suffix link:" << endl;
        for (auto & vert : bohr){
            cout << "From ";
            vert->numVert == 0 ? cout << "root" : cout << "vertex " << vert->numVert;
            cout << endl;
            if (vert->suffLink == -1)
                continue;
            cout << "\tto vertex " << vert->suffLink << endl;
        }

        cout << endl << "Result:" << endl;
        for (auto & res : result){
            sort(res.second.begin(), res.second.end());
            for (auto & num : res.second){
                cout << res.first << " " << num << endl;
            }
        }

        printResVar(result);
    }

    void printResVar(map<int, vector<int>> &result){

        cout << "Count of vertices: " << bohr.size() << endl;
        cout << "Intersecting patterns:" << endl;
        for (auto & res : result){
            for (auto & first : res.second) {
                for (auto & newRes : result) {
                    if (res.first >= newRes.first){
                        continue;
                    }
                    if (res.first + numPatterns[first].size() >= newRes.first){
                        for (auto & second : newRes.second){
                            cout << numPatterns[first] << " & " << numPatterns[second] << " (" << first << " & " << second << ")" << endl;
                        }
                    }
                }
            }
        }
    }

private:
    vector<BohrVertex*> bohr;
    map<int, string> patterns; // номер вершины - паттерн
    map<int, string> numPatterns;
    map<char, int> bohrAlp;
};

int main(){
    Bohr *bohr = new Bohr;
    string text;
    cin >> text;
    int num;
    cin >> num;
    vector<string> patterns;
    while (num){
        string pattern;
        cin >> pattern;
        patterns.push_back(pattern);
        num--;
    }
    for (auto & pattern : patterns){
        bohr->addPattern(pattern);
    }
    bohr->processText(text);
    return 0;
}
