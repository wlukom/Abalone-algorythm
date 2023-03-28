#include <iostream>

using namespace std;

class Field{
    public :
        int id;
        int player = -1; // 1 or 2
        
        Field(){};
        Field(int id, int player = 0): id(id), player(player) {  };
        void print(){
            cout << "id: " << this->id << " player: " << this->player << endl;
        }
};