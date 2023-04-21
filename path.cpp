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
        list<Movement*> movements;
        Movement* movement;
        list<Path*> paths;

        // Movement* head = nullptr;
        // Movement* last = nullptr;


        // Path(int id){
        //     //Board startBoard = Board();
        //     this->id = id;
        // }
        //Path(){}
        Path(Movement* movement){
            movements.push_back(movement);
            //this->movement = movement;
        }

        Movement* getLastMovement(){
            return movements.back();
        }

        void addPaths(list<Path*> new_paths){
            for(Path* p : new_paths){
                paths.push_back(p);
            }
        }
        void print(){
            cout << "\tPATH" << endl;
            for(Movement* m : this->movements){
                m->print();
            }
        }
        void print(ofstream &file){
            file << "\tPATH" << endl;
            for(Movement* m : this->movements){
                file << "\t\tfrom=" << m->from_field->id << " dir=" << m->direction << endl;
                //m->print();
            }
        }

        // Path(Movement* m){
        //     this->head = m;
        //     this->last = m;
        // }

        // void push(Movement* m){
        //     if(this->head == nullptr)
        //         this->head = m;
        //     else
        //         this->last->next = m;
            
        //     this->last = m;
        // }

        // int count(){
        //     if(this->head == nullptr)
        //         return 0;
        //     int counter = 1;
        //     for(Movement* current = this->head; current->next != NULL; current = current->next){
        //         counter++;
        //     }
        //     return counter;
        // }

};