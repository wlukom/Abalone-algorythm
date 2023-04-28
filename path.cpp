#include <iostream>
#include <vector>

//#include "movement.cpp"
//#include "field.cpp"
#include "board.cpp"

using namespace std;

class Path{
    public:
        //int id;
        int value;
        //list<Movement*> movements;
        Movement* movement;
        list<Path*> children;

        // Movement* head = nullptr;
        // Movement* last = nullptr;


        // Path(int id){
        //     //Board startBoard = Board();
        //     this->id = id;
        // }
        Path(){
            this->movement = new Movement();
        }
        Path(Movement* movement){
            //movements.push_back(movement);
            this->movement = movement;
        }

        void addPaths(list<Path*> new_paths){
            for(Path* p : new_paths){
                this->children.push_back(p);
            }
        }
        void print(){
            cout << "\tilosc dzieci=" << this->children.size() << endl;
            this->movement->print();
        }
        void printChildren(){
            cout << "--- Children:" << endl;
            for(Path* p : this->children){
                p->print();
                cout << endl;
            }
            cout << "--- Koniec:" << endl;
        }

};