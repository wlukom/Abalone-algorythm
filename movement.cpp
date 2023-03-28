#include <iostream>
#include <list>

#include "field.cpp"
//#include "board.cpp"

using namespace std;

class Movement{
    public:

        list<Field*> board;

        Field* from_field = NULL;
        int direction;
        int myMarblesPushed;
        int rate; // -2 1 0 1 2

        Movement* prev;
        Movement* next;

        Movement(){};
        Movement(Field* from, int direction, int rate=0){
            this->from_field = from;
            this->direction = direction;
            this->rate = rate;
            // Board b = Board();
            // this->move(b.board);
            
        }
        
        void setRate(int x)  {this->rate = x;};

        list<Field*> move(list<Field*> fields){

            cout << "id: "<< this->from_field->id <<" player: " << this->from_field->player << " dir: " << this->direction << endl;

            Field* firstField = findField(fields, this->from_field->id);//m->from_field;
            // firstField->print();
            Field* secField = findField(fields, firstField->id + this->direction);
            //secField->print();
            //  move only one marble
            if(secField->player == 0){
                secField->player = firstField->player;
                firstField->player = 0;
            }
            // move more marbles
            else{
                int tempPlayer = secField->player;

                while(secField != NULL && secField->player != 0){
                    //cout << "Wchodze" << endl;
                    tempPlayer = secField->player;
                    secField->player = firstField->player;
                    firstField->player = 0;
                    firstField = findField(fields, secField->id);
                    secField = findField(fields, secField->id + this->direction);
                    
                }
                if(secField->player == 0)
                    secField->player = tempPlayer;
            }
            Field* end = findField(fields, this->from_field->id);
            //cout << "id: "<< end->id <<" player: " << end->player << endl;

            return fields;
        }
        void print(){
            cout << "Ruch z id:" << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl;
        }
        void printBoard(list<Field*> fields){
            HANDLE console_color;
            console_color = GetStdHandle(STD_OUTPUT_HANDLE);

            int rows[] = {36,27,18,9,0,1,2,3,4};
            for(int row : rows){
                if(row == 9 || row == 1)
                    cout << " ";
                else if(row == 18 || row == 2)
                    cout << "  ";
                else if(row == 27 || row == 3)
                    cout << "   ";
                else if(row == 36 || row == 4)
                    cout << "    ";
                    
                Field* f = findField(fields, row);
                while(f != NULL){
                    if(f->player == 0)
                        SetConsoleTextAttribute(console_color, 7);
                    else if(f->player == 1)
                        SetConsoleTextAttribute(console_color, 2);
                    else if(f->player == 2)
                        SetConsoleTextAttribute(console_color, 12);
        
                    cout << f->id << " ";

                    f = findField(fields, f->id + 10);
                }
                cout << endl;
            }
            SetConsoleTextAttribute(console_color, 7);
        }

        Field* findField(list<Field*> fields, int id){
            for(Field* field : fields){
                if(field->id == id)
                    return field;
            }
            return NULL;
        }
};