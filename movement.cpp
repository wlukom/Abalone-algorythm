#include <iostream>
#include <list>

#include "field.cpp"
//#include "board.cpp"

using namespace std;

class Movement{

    Field* from_field;
    int direction;
    float rate; // 0-10

    public:

        Movement(){
            this->from_field = new Field(-1, -1);
            this->direction = 0;
        }
        Movement(int player){
            this->from_field = new Field(-1, player);
            this->direction = 0;
        }
        Movement(Field* from, int direction){
            this->from_field = from;
            this->direction = direction;
        }

        int getFieldId() { return this->from_field->id; };
        int getPlayer() { return this->from_field->player; };
        int getDirection() { return this->direction; };
        float getRate() { return this->rate; };

        void setRate(float x)  {this->rate = x;};

        string toString() {
            return "Movement z " + to_string(this->from_field->id) + " dir:" + to_string(this->direction) + " rate:" + to_string(this->rate);
        }
        
        void print(){
            cout << "Movement z " << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl; 
        }
};