#include <iostream>
#include <vector>

#include "board.cpp"

using namespace std;

class Path{

    int depth;
    int player;
    Movement* movement;
    list<Path*> children;

    public:
        Path(int depth, int player){
            this->depth = depth;
            this->player = player;
            this->movement = new Movement(player);
        }

        Path(Movement* movement, int depth){
            this->movement = movement;
            this->depth = depth;
            this->player = movement->getPlayer();
        }

        int getDepth() { return this->depth; };
        int getPlayer() { return this->player; };
        Movement* getMovement() { return this->movement; };
        list<Path*> getChildren() { return this->children; }; 

        string toString() {
            return this->movement->toString();
        }

        void addChild(Path* new_path){
            this->children.push_back(new_path);
        }

        void print(){
            this->movement->print();
            cout << "\tilosc dzieci=" << this->children.size() << endl;
        }
        // void printChildren(){
        //     cout << "--- Children:" << endl;
        //     for(Path* p : this->children){
        //         p->print();
        //     }
        //     cout << "--- Koniec:" << endl;
        // }

};