#include <cstring>
#include <iostream>
#include <map>
#include <vector>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::pair;

#define SIZE 5

// структура вершины
struct BohrVertex{
    vector<int> nextVerts;
    vector<int> goArray;
    int parent;
    vector<int> patternNumber;
    int suffLink;
    int up;
    char toParent;
    bool isFinal;
};


class Bohr{
public:
    Bohr(){
        bohrAlp = {{'A', 0}, {'C', 1}, {'G', 2}, {'T', 3}, {'N', 4} };
        bohr.push_back(createVertex(0, '0'));
        wordLen = 0;
        countPatterns = 0;
    }

    void setJoker(char joker){
        this->joker = joker;
    }

    BohrVertex *createVertex(int parent, char toParent){
        auto *newVertex = new BohrVertex;
        newVertex->nextVerts.assign(SIZE, -1);
        newVertex->goArray.assign(SIZE, -1);
        newVertex->suffLink = -1;
        newVertex->up = -1;
        newVertex->parent = parent;
        newVertex->toParent = toParent;
        newVertex->isFinal = false;
        return newVertex;
    }

    void addPattern(string &text){

        int index = 0;
        int count = 0;
        wordLen = text.size();

        // split
        for (int i = 0; i < text.length(); i = index){
            string buff = "";
            while (index < text.length() && text[index] == joker)
                index++;
            if (index == text.length())
                break;
            int pos = index;
            while (index < text.length() && text[index] != joker)
                buff += text[index++];
            if (!buff.empty()){
                count++;
                positions.push_back(pos);
                patterns.push_back(buff);
            }
        }
        // create bohr
        for (auto & pattern : patterns){
            cout << "Add pattern [" << pattern << "] to bohr:" << endl;
            int j = 0;
            for (char i : pattern){
                if (bohr[j]->nextVerts[bohrAlp[i]] == -1){
                    auto *newVert = createVertex(j, i);
                    bohr.push_back(newVert);
                    bohr[j]->nextVerts[bohrAlp[i]] = bohr.size() - 1;
                    cout << "Created vertex " << bohr.size()-1 << " for edge " << i << endl;
                }
                j = bohr[j]->nextVerts[bohrAlp[i]];
            }
            bohr[j]->isFinal = true;
            bohr[j]->patternNumber.push_back(countPatterns);
            countPatterns++;
        }

    }

    int getSuffLink(int vert){
        cout << "finding suffix link for vertex " << vert << endl;
        if (bohr[vert]->suffLink == -1) {
            if (vert == 0 || bohr[vert]->parent == 0) {
                cout << "get link to root" << endl;
                bohr[vert]->suffLink = 0;
            }
            else {
                cout << "get link through parent" << endl;
                bohr[vert]->suffLink = getAutoLink(getSuffLink(bohr[vert]->parent), bohrAlp[bohr[vert]->toParent]);
            }
        }
        cout << "suffix link for " << vert << " = " << bohr[vert]->suffLink << endl;
        return bohr[vert]->suffLink;
    }

    int getAutoLink(int vertex, int index){
        cout << "finding autolink for vertex " << vertex << endl;
        if (bohr[vertex]->goArray[index] == -1){
            if (bohr[vertex]->nextVerts[index] != -1) {
                cout << "get autolink to vertex " << bohr[vertex]->nextVerts[index] << endl;
                bohr[vertex]->goArray[index] = bohr[vertex]->nextVerts[index];
            }
            else {
                if (vertex == 0){
                    cout << "get autolink to root" << endl;
                    bohr[vertex]->goArray[index] = 0;
                }
                else {
                    bohr[vertex]->goArray[index] = getAutoLink(getSuffLink(vertex), index);
                }
            }
        }
        cout << "vertex " << vertex << " has autolink " << bohr[vertex]->goArray[index] << endl;
        return bohr[vertex]->goArray[index];
    }

    int getUpLink(int vertex){
        cout << "finding compressed link for vertex " << vertex << endl;
        if (bohr[vertex]->up == -1){
            int vert = getSuffLink(vertex);
            if (bohr[vert]->isFinal) {
                cout << "get compressed link to " << vert << endl;
                bohr[vertex]->up = vert;
            }
            else {
                if (vert == 0) {
                    cout << "get compressed link to root" << endl;
                    bohr[vertex]->up = 0;
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

    void processText(string& text){
        for (int i = 0; i < text.size(); i++)
            arr.push_back(0);
        cout << endl << "Finding split patterns in text [" << text << "]" << endl;
        int currentVert = 0;
        for (int i = 0; i < text.length(); i++){
            currentVert = getAutoLink(currentVert, bohrAlp[text[i]]);
            int index = i;
            for (int j = currentVert; j != 0; j = getUpLink(j)){
                if (bohr[j]->isFinal){
                    for (auto & in: bohr[j]->patternNumber){
                        cout << ">>final state for pattern [" << patterns[in] << "] was found" << endl;
                        cout << "start index in text: " << i + 1 - patterns[in].size() + 1 << endl << endl;
                        int tmp = index + 1 - patterns[in].size() - positions[in];
                        if (tmp >= 0 && tmp <= text.length() - wordLen){
                            arr[tmp]++;
                        }
                    }
                }
            }
        }
        cout << endl <<  "Result:" << endl;
        for (int i = 0; i < text.size(); i++){
            if (arr[i] == patterns.size())
                cout << i + 1 << endl;
        }

        cout << "Count of vertices: " << bohr.size() << endl;
    }


private:
    vector<BohrVertex*> bohr;
    vector<string> patterns;
    int wordLen;
    map<char, int> bohrAlp;
    char joker;
    vector<int> positions;
    int countPatterns;
    vector<int> arr;
};



int main() {
    string text;
    cin >> text;
    string pattern;
    cin >> pattern;
    char joker;
    cin >> joker;

    Bohr bohr;
    bohr.setJoker(joker);
    bohr.addPattern(pattern);
    bohr.processText(text);

    return 0;
}