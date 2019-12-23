#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
using namespace std;

bool mainMenu();
void playTicTakToe(bool);
int inputInt(int, int);
double inputDouble(double, double);
int randIntInRange(int, int);

class Node {
    public:
        static const int NOONE_SYMBOL = 0;
        static const int MY_SYMBOL = 1;
        static const int OPPONENT_SYMBOL = 2;

        bool isLeaf;
        int fitness;
        int *cells;
        bool isMaxNode;
        int fullCellsNumber;
        vector<Node> children;

    Node(bool isMaxNode) {
        this->isMaxNode = isMaxNode;
        cells = new int[9];
        for(int i=0; 9>i; i++) cells[i] = NOONE_SYMBOL;
        fullCellsNumber = 0;
        isLeaf = false;
        fitness = -1;
        extend();
    }

    Node(int parentCells[], int parentFullCellsNumber, int index, int symbole) {
    	fitness = -1;
        cells = new int[9];
        for(int i=0; 9>i; i++) cells[i] = parentCells[i];
        if(cells[index] != NOONE_SYMBOL) throw string("WrongIndex");
        if(symbole != MY_SYMBOL && symbole != OPPONENT_SYMBOL) throw string("WrongSymbole");
        cells[index] = symbole;
        fullCellsNumber = parentFullCellsNumber + 1;
        isMaxNode = symbole == OPPONENT_SYMBOL;
        extend();
    }

    void extend() {
        if(isFinished()) return;
        for(int i=0; 9>i; i++) {
            if(cells[i] == NOONE_SYMBOL) {
                Node newNode(cells, fullCellsNumber, i,
                        isMaxNode ? MY_SYMBOL : OPPONENT_SYMBOL);
                children.push_back(newNode);
            }
        }
    }

    int whoIsWinner(){
        if(cells[0] != NOONE_SYMBOL && cells[0] == cells[1] && cells[0] == cells[2]) return cells[0];
        if(cells[3] != NOONE_SYMBOL && cells[3] == cells[4] && cells[3] == cells[5]) return cells[3];
        if(cells[6] != NOONE_SYMBOL && cells[6] == cells[7] && cells[6] == cells[8]) return cells[6];
        if(cells[0] != NOONE_SYMBOL && cells[0] == cells[3] && cells[0] == cells[6]) return cells[0];
        if(cells[1] != NOONE_SYMBOL && cells[1] == cells[4] && cells[1] == cells[7]) return cells[1];
        if(cells[2] != NOONE_SYMBOL && cells[2] == cells[5] && cells[2] == cells[8]) return cells[2];
        if(cells[0] != NOONE_SYMBOL && cells[0] == cells[4] && cells[0] == cells[8]) return cells[0];
        if(cells[2] != NOONE_SYMBOL && cells[2] == cells[4] && cells[2] == cells[6]) return cells[2];

        return NOONE_SYMBOL;
    }

    bool isFinished(){
        int who = whoIsWinner();
        if(who == MY_SYMBOL){
            fitness = 1 + (9 - fullCellsNumber);
            isLeaf = true;
            return true;
        }
        else if(who == OPPONENT_SYMBOL){
            fitness = -1 - (9 - fullCellsNumber);
            isLeaf = true;
            return true;
        }

        if(fullCellsNumber >= 9){
            fitness = 0;
            isLeaf = true;
            return true;
        }
        return false;
    }

    void print(){
        for(int i=0; 9>i; i++) {
            if(i % 3 == 0){
                for(int j=0; 3 * 4 + 1 > j; j++) cout<<"-";
                cout<<endl;
            }
            if(i % 3 == 0) cout<<"| ";
            if(cells[i] == MY_SYMBOL) cout<<"*";
            else if(cells[i] == OPPONENT_SYMBOL) cout<<"o";
            else cout<<" ";
            cout<<" | ";
            if((i+1) % 3 == 0) cout<<endl;
        }
        for(int j=0; 3 * 4 + 1 > j; j++) cout<<"-";
        cout<<endl;
    }

    int evaluate(){
        if(!isLeaf) {
            int min = INT_MAX;
            int max = INT_MIN;

            for(int i=0; children.size() > i; i++) {
                int f = children.at(i).evaluate();
                if (f < min) min = f;
                if (f > max) max = f;
            }

            if(isMaxNode) fitness = max;
            else fitness = min;
            //cout<<"Fit: "<<fitness<<endl;
        }
        return fitness;
    }

    Node getNextMaxNode(){
        int max = INT_MIN;
        vector<int> list;
        for(int i=0; children.size()>i; i++){
            if(children.at(i).fitness > max){
                max = children.at(i).fitness;
                list.clear();
                list.push_back(i);
            }
            else if(children.at(i).fitness == max){
                list.push_back(i);
            }
        }

        int randIndex = randIntInRange(0, list.size() - 1);
        return children.at(list.at(randIndex));
    }

    Node getNextMinMode(int index) {
        if(cells[index] != NOONE_SYMBOL) throw string("NullRet");
        for(int i=0; children.size()>i; i++){
            if(children.at(i).cells[index] != NOONE_SYMBOL) return children.at(i);
        }
        throw string("Error!");
    }
};

int main() {
    srand(time(0));
    while(mainMenu());
    return 0;
}

bool mainMenu(){
    cout<<endl<<"Menu:"<<endl;
    cout<<"1: Play TicTakToe (You are the first)"<<endl;
    cout<<"2: Play TicTakToe (CPU is the first)"<<endl;
    cout<<"3: Exit"<<endl;
    cout<<endl<<"Enter Your Choice: ";
    int choice = inputInt(1, 3);

    if(choice==1) playTicTakToe(false);
    else if(choice == 2) playTicTakToe(true);
    else if(choice == 3) return false;
    return true;
}

void playTicTakToe(bool isMaxNode){
    cout<<endl;
    try {
        Node rootNode(isMaxNode);
        cout<<"Game Started: "<<endl;
        rootNode.print();
        cout<<endl;

        rootNode.evaluate();
        Node currentNode = rootNode;
        while(!currentNode.isLeaf){
            if(currentNode.isMaxNode) {
                cout<<"CPU Turn: "<<endl;
                currentNode = currentNode.getNextMaxNode();
            }
            else {
                cout<<"Enter your move: ";
                bool isNull = true;
                while(isNull){
                    int choice = inputInt(1, 9);
                    choice--;
                    try{
                        Node newNode = currentNode.getNextMinMode(choice);
                        currentNode = newNode;
                        isNull = false;
                    }
                    catch (string e){
                        cout<<"Wrong choice! Please enter a new move: ";
                    }
                }
                cout<<endl<<"Your Turn: "<<endl;
            }
            currentNode.print();
            cout<<endl;
        }

        int whoIsWinner = currentNode.whoIsWinner();
        if(whoIsWinner == Node::MY_SYMBOL) cout<<"Sorry I win :)"<<endl;
        else if(whoIsWinner == Node::OPPONENT_SYMBOL)
            cout<<"Ooops You are the winner :("<<endl;
        else cout<<"OK Its a draw :("<<endl;
    }
    catch (string e) {
        cout<<"Error: "<<e<<endl;
    }
}

int inputInt(int min, int max){
    while(true){
        try{
        	string str;
    		getline(cin, str);
            int choice = atoi(str.c_str());
            if(choice<min || choice>max){
                cout<<"Wrong range! Try Again: ";
                continue;
            }
            return choice;
        }
        catch (invalid_argument const &e){
            cout <<"Wrong format! Try Again: ";
        }
        catch (out_of_range const &e){
            cout <<"Wrong format! Try Again: ";
        }
    }
}

double inputDouble(double min, double max){
    while(true){
        try{
        	string str;
    		getline(cin, str);
            double value = atoi(str.c_str());
            if(value<min || value>max){
                cout<<"Wrong range! Try Again: ";
                continue;
            }
            return value;
        }
        catch (invalid_argument const &e){
            cout <<"Wrong format! Try Again: ";
        }
        catch (out_of_range const &e){
            cout <<"Wrong format! Try Again: ";
        }
    }
}

int randIntInRange(int min, int max){
    return min + (rand() % (max - min + 1));
}


