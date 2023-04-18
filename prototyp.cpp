

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
//#include "branch.cpp"
#include "movement.cpp"
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
    return new Field();
}
bool isNULL(Field* object){

    if(object->id == -1)
        return true;
    return false;
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
    // cout << "aktualna plansza: " << endl;
    // printBoard(fields);
    cout << "direction=" << direction << endl;

    for(Field* firstField : fields){

        //cout << "\tid field=" << firstField->id << " direction=" << direction << endl;
        // cout << "movements: " << endl;
        // for(Movement* mm : m)
        //     mm->print();



        if(firstField->player == player){
            //cout << "TO JEST ID=" << firstField.id << endl;
            Field* secField = findField(fields, firstField->id + direction);
            // 1 + NULL
            if(isNULL(secField))
                continue;
            // 1 + 0
            if(secField->player == 0){
                m.push_back(new Movement(fields, firstField, direction));
                continue;
            }
            // 2
            else if(secField->player == player){ 
                cout << " 2 ";               
                Field* thirdField = findField(fields, secField->id + direction);
                // 2 + NULL
                if(isNULL(thirdField))
                    continue;
                // 2 + 0
                else if(thirdField->player == 0){
                    cout << " 2+0 "; 
                    m.push_back(new Movement(fields, firstField, direction));
                    cout << " 2+0 end";
                    continue;
                }  
                Field* firstOut = findField(fields, thirdField->id + direction);
                // 2 + 1
                if(thirdField->player != player){
                    // 2 + 1 + NULL
                    if(isNULL(firstOut))
                        m.push_back(new Movement(fields, firstField, direction, 2));
                    // 2 + 1 + 0
                    else if(firstOut->player == 0)
                        m.push_back(new Movement(fields, firstField, direction));
                    // 2 + 1 + 1 || 2 + 2
                    else
                        continue;
                }
                // 3               
                else{
                    // 3 + NULL
                    if(isNULL(firstOut))
                        continue;
                    // 3 + 0
                    else if(firstOut->player == 0)                    
                        m.push_back(new Movement(fields, firstField, direction));
                    // 4  -wrong
                    else if(firstOut->player == player)
                        continue;
                    // 3 + 1 opponent
                    else{
                        Field* secOut = findField(fields, firstOut->id + direction);
                        // 3 + 1 + NULL
                        if(isNULL(secOut))
                            m.push_back(new Movement(fields, firstField, direction, 2));
                        // 3 + 1 + 1  -wrong
                        else if(secOut->player == player)
                            continue; 
                        // 3 + 1 + 0
                        else if(secOut->player == 0)
                            m.push_back(new Movement(fields, firstField, direction));

                        // 3 + 2 opponent
                        else{
                            Field* thirdOut = findField(fields, secOut->id + direction);
                            // 3 + 2 + NULL
                            if(isNULL(thirdOut))
                                m.push_back(new Movement(fields, firstField, direction, 2));
                            // 3 + 2 + 0
                            else if(thirdOut->player == 0)
                                m.push_back(new Movement(fields, firstField, direction));
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

        while(!isNULL(secField) && secField->player != 0){
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
// list<Field*> change_board(list<Field*> initial_board, int from_id, int direction){

//     list<Field*> fields = copy(initial_board);

//     Field* firstField = findField(fields, from_id);
//     Field* secField = findField(fields, from_id + direction);

//     int pom_player = secField->player;
//     secField->player = firstField->player;
//     firstField->player = 0;

//     firstField = secField;
//     secField = findField(fields, firstField->id + direction);

//     // while(!isOffTheBoard(secField->id) || secField->player != 0){
//     while(!isNULL(secField) || secField->player != 0){
//         firstField = secField;
//         secField = findField(fields, firstField->id + direction);
//     }
//     // empty field
//     if(isNull(secField))
//         return fields;
//     if(secField->player != 0)
    
// }

list<Movement*> generate_movements(list<Field*> fields, int currentPlayer){ //player 1,2
    //int directions[] = {-1, 1, -10, 10, -9, 9};
    cout << " 1 " << endl;

    list<Movement*> movements;
    for(int direction: {-1, 1, -10, 10, -9, 9}){
        cout << " i ";
        list<Movement*> ms = possibleMovesInLine(fields, direction, currentPlayer);
        
        cout << " j ";
        for( Movement* m : ms)
            movements.push_back(m);
        //break; 
    }
    // cout << "Generate_movements size=" << movements.size() << endl;
    // for(Movement* m : movements){
    //     m->print();
    // }
    //cout << "Jest dostepnych: " << movements.size()<< endl;

    return movements;
}

list<Field*> copy(list<Field*> old_fields){
    list<Field*> new_fields;
    for(Field* f : old_fields){
        new_fields.push_back(new Field(f->id, f->player));
    }
    return new_fields;
}

Movement* selectMovement(list<Movement*> movements){
    int theBestRate = -3;
    list<Movement*> theBestMovements;
    for(Movement* m : movements){

        m->setRate();
        if(m->rate > theBestRate){
            theBestMovements.clear();
            theBestMovements.push_back(m);
            theBestRate = m->rate;
        }
        else if(m->rate == theBestRate)
            theBestMovements.push_back(m);

        m->print();
        printBoard(m->board);
    }
    // movement drawing
    int randNumber = int(rand() % theBestMovements.size());
    cout << "Rand number=" << randNumber << " a lista ma size=" << theBestMovements.size() << endl;;
    list<Movement*>::iterator it = theBestMovements.begin();
    advance(it, randNumber);
    return *it;
    //return theBestMovements[randNumber];


    // int counter = 0;
    // for(Movement* m : theBestMovements){
    //     if(counter == randNumber)
    //         return m;
    //     counter++;
    // }
}


int changePlayer(int player){
    if(player == 2) 
        return 1;
    return 2;
}
// Movement* finallyRate(list<Field*> initialBoard, int player, int depth){
//     cout << "ZACZYNAM FINALLYRATE   player=" << player << " depth=" << depth << endl;
//     printBoard(initialBoard);
//     if(depth == 0){
//         cout << "\tdepth == 0 czyli zwracam nullptr" << endl;
//         // return ocena danego ruchu
//         return nullptr;
//     }
//     list<Movement*> newMovements = generate_movements(initialBoard, player);

    
//     Movement* theBestMovement = newMovements.front();
//     int theBestRate = -3;
//     int i = 0;
//     for(Movement* & m : newMovements){

//         printBoard(m->board);

//         int currentRate = finallyRate(m->board, changePlayer(player), --depth)->rate;
//         if(i == 0)
//             theBestRate = currentRate;
//         if(currentRate > theBestRate){
//             theBestMovement = m;
//             theBestRate = currentRate;
//         }
//         i++;
//     }
//     theBestMovement->print();
//     return theBestMovement;
// }


Movement* finallyRate(list<Field*> initialBoard, int player){
    cout << "Stan pocxatkowy planszy:" << endl;
    printBoard(initialBoard);

    list<Movement*> newMovements = generate_movements(initialBoard, player);

    cout << "ilosc ruchow: " << newMovements.size() << endl;
    
    Movement* theBestMovement = selectMovement(newMovements);

    cout << "Najlepszy ruch: " << endl;
    theBestMovement->print();
    return theBestMovement;
}

int main(){
    cout << "MMM" <<endl;
    // starting Board
    list<Field*> datas = generateList();

    // glebokosc 1
    
    Movement* theBest = finallyRate(datas, 2);

    printBoard(theBest->board);

    // jakie strategie do rate powinienem uzyc? -------------------


}