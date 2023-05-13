#include <iostream>
#include <list>

#include "field.cpp"
//#include "board.cpp"

using namespace std;

class Movement{
    public:

        list<Field*> board;

        Field* from_field;
        int player = 0;
        int marblesDifference;
        int direction;
        int myMarblesPushed;
        int rate; // -2 1 0 1 2


        Movement(){
            this->from_field = new Field(-1, -1);
            this->direction = 0;
        }
        Movement(list<Field*> board_before_movement, Field* from, int direction, int marblesDiference = 0, int rate=0){
            this->player = from->player;
            this->from_field = from;
            this->direction = direction;
            this->marblesDifference = marblesDiference;
            this->rate = rate;

            HANDLE console_color;
            console_color = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(console_color, 7);
        }
        string toString() {
            return "Movement z " + to_string(this->from_field->id) + " dir:" + to_string(this->direction) + " rate:" + to_string(this->rate);
        }
        
        void setRate(int x)  {this->rate = x;};

        int oppositePlayer(int p){
            if(p == 1)
                return 2;
            if(p == 2)
                return 1;
            return 0;
        }

        int countingMarblesDifference(list<Field*> fields){
            int myMarbles = 0;
            int opponentMarbles = 0;

            for(Field* f : fields){
                if(f->player == this->player)
                    myMarbles++;
                else if(f->player != 0)
                    opponentMarbles++;
            }
            return (myMarbles - opponentMarbles);
        };

        list<Field*> copy(list<Field*> old_fields){
            list<Field*> new_fields;
            for(Field* f : old_fields){
                new_fields.push_back(new Field(f->id, f->player));
            }
            return new_fields;
        }

        // void print(){
        //     cout << "Ruch z id:" << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl;
        // }
        void print(){
            cout << "\tMovement z " << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl;
            return;
            if(this->board.empty())
                cout << "\tMovemnt NULL" << endl;
            //this->from_field->print();
            // else{
            //     cout << "\tMovement z " << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl;
            // }
            
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
                while(!isNULL(f)){
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
            return new Field();
        }
        bool isNULL(Field* object){

            if(object->id == -1)
                return true;
            return false;
        }


        void setRate(){ // -2 -1 0 1 2 

            /* RODZAJE STRATEGII */

        
        //        PRZECIWNIK BLISKO
        // - wypchniecie kuli przeciwnika najbardziej punktowane
        /*
            if(this->marblesDifference > 0){
                this->rate = 2;
                return;
            }
            if(this->marblesDifference < 0){
                this->rate = -2;
                return;
            }
            this->rate = 0;
        */
        // - podchodzimy blizej starajac sie miec 3 kule w rzędzie
        // - sprawdzamy czy mozemy przepchnąć kolumne przeciwnika


        
            
            /*
                PRZECIWNIK DALEKO (rozpoczecie gry)

            - kierunek do środka planszy (pole 40)
            - jak najwiecej sasiadow tego samego koloru
            
            */

        /*
                PRZECIWNIK DALEKO (idzie skrzydlem)

            - kierunek do środka planszy (pole 40)
            - przygotowujemy sie na atak z boku
                - wiecej kul dajemy z przeciwnej strony
            - jak najwiecej sasiadow tego samego koloru
        
        */

            /*
                PRZECIWNIK BLISKO (odleglosc 2 kule od naszej)

            - sprawdzamy zagrozenie:
                    - ilosc kul przeciwnika vs nasze
                    - czy wypchnie nasze kule za plansze
                    - czy mozemy rozwalic jego atakujacy szyk (z boku)
            - korygujemy nasze ustawienie

            */

        }
};