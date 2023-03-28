

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <list>
//#include <experimental/random>
//#include <random>

#include<chrono>
#include<thread>

//#include "field.cpp"
#include "branch.cpp"

using namespace std;

// class Field
// {
//  public :
//     int id;
//     int player = -1; // 1 or 2
//     Field(){};
//     Field(int id, int player = 0): id(id), player(player) {  };
//     void print(){
//         cout << "id: " << this->id << " player: " << this->player << endl;
//     }
// };

bool contains(int x, int tab[], int tabSize){

    for(int i = 0; i < tabSize; i++){
        if(x == tab[i])
            return true;
    }
    return false;
}

list<Field*> generateList(){
    list<Field*> myList;
    int offTheBoard[] = {5,6,7,8,15,16,17,25,26,35,45,54,55,63,64,65,72,73,74,75};
    int fieldsPlayer1[] = {27,36,37,38,46,47,48,56,57,58,66,67,76,77};
    int fieldsPlayer2[] = {3,4,13,14,22,23,24,32,33,34,42,43,44,53};

    int offTheBoardSize = sizeof(offTheBoard)/sizeof(offTheBoard[0]);
    int fieldsPlayer1Size = sizeof(fieldsPlayer1)/sizeof(fieldsPlayer1[0]);
    int fieldsPlayer2Size = sizeof(fieldsPlayer2)/sizeof(fieldsPlayer2[0]);

    int x = 0;
    for(int i = 0; i < 61; i++, x++){
        while(contains(x, offTheBoard, offTheBoardSize))
                x++;

        if(contains(x, fieldsPlayer1, fieldsPlayer1Size))
            myList.push_back(new Field(x,1));
        else if(contains(x, fieldsPlayer2, fieldsPlayer2Size))
            myList.push_back(new Field(x,2));
        else
            myList.push_back(new Field(x,0));
    }
    return myList;
}



Field* findField(list<Field*> fields, int id){
    for(Field* field : fields){
        if(field->id == id)
            return field;
    }
    return NULL;
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



// class Movement{
//     public:
//         Field* from_field = NULL;
//         int direction;
//         int myMarblesPushed;
//         int rate; // -2 1 0 1 2
//         Movement(){};
//         Movement(Field* from, int direction, int rate=0){
//             this->from_field = from;
//             this->direction = direction;
//             this->rate = rate;
//         }
//         void setRate(int x)  {this->rate = x;};
//         void print(){
//             cout << "Ruch z id:" << this->from_field->id << " dir:" << this->direction << " rate:" << this->rate << endl;
//         }
// };


int offTheBoard[] = {5,6,7,8,15,16,17,25,26,35,45,54,55,63,64,65,72,73,74,75};
bool isOffTheBoard(int x){
    if(x < 0 || x > 80)
        return false;
    for(int i : offTheBoard){
        if(i == x)
            return false;
    }
    return true;
}

list<Movement*> possibleMovesInLine(list<Field*> fields, int direction, int player){
    list<Movement*> m;

    for(Field* firstField : fields){
        if(firstField->player == player){
            Field* secField = findField(fields, firstField->id + direction);
            // 1 + NULL
            if(secField == NULL)
                continue;
            // 1 + 0
            if(secField->player == 0)
                m.push_back(new Movement(firstField, direction));
            // 2
            else if(secField->player == player){                
                Field* thirdField = findField(fields, secField->id + direction);
                // 2 + NULL
                if(thirdField == NULL)
                    continue;
                // 2 + 0
                else if(thirdField->player == 0){
                    m.push_back(new Movement(firstField, direction));
                    continue;
                }  
                Field* firstOut = findField(fields, thirdField->id + direction);
                // 2 + 1
                if(thirdField->player != player){
                    // 2 + 1 + NULL
                    if(firstOut == NULL)
                        m.push_back(new Movement(firstField, direction, 2));
                    // 2 + 1 + 0
                    else if(firstOut->player == 0)
                        m.push_back(new Movement(firstField, direction));
                    // 2 + 1 + 1 || 2 + 2
                    else
                        continue;
                }
                // 3               
                else{
                    // 3 + NULL
                    if(firstOut == NULL)
                        continue;
                    // 3 + 0
                    else if(firstOut->player == 0)                    
                        m.push_back(new Movement(firstField, direction));
                    // 4  -wrong
                    else if(firstOut->player == player)
                        continue;
                    // 3 + 1 opponent
                    else{
                        Field* secOut = findField(fields, firstOut->id + direction);
                        // 3 + 1 + NULL
                        if(secOut == NULL)
                            m.push_back(new Movement(firstField, direction, 2));
                        // 3 + 1 + 1  -wrong
                        else if(secOut->player == player)
                            continue; 
                        // 3 + 1 + 0
                        else if(secOut->player == 0)
                            m.push_back(new Movement(firstField, direction));

                        // 3 + 2 opponent
                        else{
                            Field* thirdOut = findField(fields, secOut->id + direction);
                            // 3 + 2 + NULL
                            if(thirdOut == NULL)
                                m.push_back(new Movement(firstField, direction, 2));
                            // 3 + 2 + 0
                            else if(thirdOut->player == 0)
                                m.push_back(new Movement(firstField, direction));
                            // 3 + 3 || 3 + 2 + 1
                            else
                                continue;
                        }
                    }
                }
            }
        }
    }

    return m;
}
list<Field*> move(list<Field*> fields, Movement* m){

    cout << "id: "<< m->from_field->id <<" player: " << m->from_field->player << " dir: " << m->direction << endl;

    Field* firstField = findField(fields, m->from_field->id);//m->from_field;
    // firstField->print();
    Field* secField = findField(fields, firstField->id + m->direction);
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
            secField = findField(fields, secField->id + m->direction);
            
        }
        if(secField->player == 0)
            secField->player = tempPlayer;
    }
    Field* end = findField(fields, m->from_field->id);
    //cout << "id: "<< end->id <<" player: " << end->player << endl;

    return fields;
}

list<Movement*> generate_movements(list<Field*> fields, int currentPlayer){ //player 1,2
    int directions[] = {-1, 1, -10, 10, -9, 9};

    list<Movement*> movements;
    for(int direction: {-1, 1, -10, 10, -9, 9}){
        list<Movement*> ms = possibleMovesInLine(fields, direction, currentPlayer);
        for( Movement* m : ms)
            movements.push_back(m);
        
    }

    for(Movement* m : movements){
        m->print();
    }
    cout << "Jest dostepnych: " << movements.size()<< endl;

    return movements;
}

list<Movement*> setRate(list<Movement*> movements, list<Field*> board){

    int player = movements.front()->from_field->player;

    for(Movement* m : movements){
        ;
    }

    /* RODZAJE STRATEGII */
    
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

   /*
        PRZECIWNIK BLISKO

    - podchodzimy blizej starajac sie miec 3 kule w rzędzie
    - sprawdzamy czy mozemy przepchnąć kolumne przeciwnika
   
    - wypchniecie kuli przeciwnika najbardziej punktowane
   */


    return list<Movement*>();
}
Movement* selectMove(list<Movement*> movements){
    Movement* returned = movements.back();
    for(Movement* m : movements){
        if(m->rate > returned->rate)
            returned = m;
    }
    int counter = 0;
    for(Movement* m : movements){
        if(returned->rate == m->rate)
            counter++;
    }
    if(counter > 0){
        int randNumber = int(rand() % counter);
        counter = 0;
        for(Movement* m : movements){
            if(m->rate == returned->rate)
                if(counter == randNumber)
                    return m;
                counter++;

        }
    }

    return returned;
}

int algorythm(Branch* branch, int depth, int player){

    if(depth == 0)
        return 2;

    

    return 1;
}

int main(){

    cout << "Maad\n";

    // starting Board
    list<Field*> datas = generateList();
    printBoard(datas);

    list<Branch*> branches;

    //first predicting
    for(Movement* & m : generate_movements(datas, 2)){
        branches.push_back(new Branch(m));
    }

    //next predicting
    for(Branch* & branch : branches){
        // ? co dalej?
    }
        
        //Movement* chosenMove = selectMove(generate_movements(datas, 2));
    
    
    //Branch b = new Branch();
    

    // printBoard(datas);

    // Movement* chosenMove = selectMove(generate_movements(datas, 2));

    

    // list<Field*> fields = datas;

    // for(int i = 1; i < 10; i++){
    //     if(i%2 == 1)
    //         chosenMove = selectMove(generate_movements(fields, 1));
    //     else
    //         chosenMove = selectMove(generate_movements(fields, 2));

    //     fields = move(fields, chosenMove);

    //     this_thread::sleep_for(chrono::milliseconds(5000));

    //     printBoard(fields);
    // }

}