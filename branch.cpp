#include <iostream>
#include <vector>

#include "movement.cpp"
//#include "field.cpp"
//#include "board.cpp"

using namespace std;

class Branch{
    public:
        int id;
        int value;

        Movement* head = nullptr;
        Movement* last = nullptr;


        Branch(int id){
            //Board startBoard = Board();

        }
        Branch(Movement* m){
            this->head = m;
            this->last = m;
        }

        void push(Movement* m){
            if(this->head == nullptr)
                this->head = m;
            else
                this->last->next = m;
            
            this->last = m;
        }

        int count(){
            if(this->head == nullptr)
                return 0;
            int counter = 1;
            for(Movement* current = this->head; current->next != NULL; current = current->next){
                counter++;
            }
            return counter;
        }

};