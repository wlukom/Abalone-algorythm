#include <list>
#include <algorithm>
#include <iterator>

#include <windows.h>


#include "movement.cpp"


int offTheBoard[] = {5,6,7,8,15,16,17,25,26,35,45,54,55,63,64,65,72,73,74,75};

class Board{
    public:
        list<Field*> fields;

        Board(){
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
                    this->fields.push_back(new Field(x,1));
                else if(contains(x, fieldsPlayer2, fieldsPlayer2Size))
                    this->fields.push_back(new Field(x,2));
                else
                    this->fields.push_back(new Field(x,0));
            }
        }

        Board(list<Field*> input_fields){
            for(Field* f : input_fields){
                this->fields.push_back(new Field(f->id, f->player));
            }
        }
        list<Field*> getFields(){
            return fields;
        }

        void updateMovement(Movement* movement){
            // this is from first Path in the tree
            if(movement->direction == 0){
                return;
            }

            //int initial_marbles_diff = countingMarblesDifference(initial_fields);
            

            Field* firstField = findField(movement->from_field->id);
            Field* secField = findField(firstField->id + movement->direction);

            //cout << firstField->id << " -> " << secField->id << endl;

            //while(secField)
            firstField->player = 0;

            while(!isNULL(secField) && secField->player == movement->player){
                firstField = secField;
                secField = findField(firstField->id + movement->direction);
                //cout << "while: sec id=" << secField->id << endl;
            }

            // next field NULL
            if(isNULL(secField)){
                //cout << "Jest NULL (1)" << endl;
                return;
            }

            // next field empty
            if(secField->player == 0){
                secField->player = movement->player;
                //cout << "set player=" << secField->player << endl;
                return;
            }

            firstField = secField;
            firstField->player = movement->player;

            secField = findField(firstField->id + movement->direction);

            int opposite_player = oppositePlayer(movement->player);

            while(!isNULL(secField) && secField->player == opposite_player){
                firstField = secField;
                secField = findField(firstField->id + movement->direction);
            }

            // next field NULL
            if(isNULL(secField))
                return;

            // next field empty
            if(secField->player == 0){
                secField->player = opposite_player;
            }
            return;
        }
        int rate(Board initial_board, int player){


                /* RODZAJE STRATEGII */


            // NUMBER OF MARBLES
            //cout << " ** NUMBER OF MARBLES **" << endl;


            int prev_marbles_balance = this->marbles_balance(player);
            int current_marbles_balance = initial_board.marbles_balance(player);
            // cout << "count1=" << prev_marbles_balance << endl;
            // cout << "count2=" << current_marbles_balance << endl;

            if(prev_marbles_balance > current_marbles_balance)
                return 2;
            if(prev_marbles_balance < current_marbles_balance)
                return -2;


            // DISTNACE FROM THE CENTER
            //cout << " ** DISTANCE FROM THE CENTER **" << endl;

            int prev_balance_points = initial_board.distance_from_center_balance(player);
            int current_balance_points = this->distance_from_center_balance(player);
            

            int points_balance = current_balance_points - prev_balance_points;

            if(abs(points_balance) > 1){ // gdy bedzie roznica 2
                if(points_balance)
                    return 1;
                else
                    return -1;
            }
            return 0;
        
        }

        int marbles_balance(int player){
            int player_marbles = 0;
            int opponent_marbles = 0;
            for(Field* marble : fields){
                if(marble->player == player)
                    player_marbles++;
                else if(marble->player != 0)
                    opponent_marbles++;
            }
            //cout << "me=" << player_marbles << " and my bro=" << opponent_marbles << endl;
            return player_marbles - opponent_marbles;
        }

        int distance_from_center_balance(int player){
            int center = 40;
            int specific_fields_rate_1[] = {11,12,23,33,52,61,68,69,47,57,19,28};
            int specific_fields_rate_2[] = {21,32,51,59,48,29};
            int useless_fields[] = {0,1,2,3,4,14,24,34,44,53,62,71,80,79,78,77,76,66,56,46,36,27,18,9};
            

            int player_points = 0;
            int opponent_points = 0;

            for(Field* marble : fields){
                if(marble->player == 0)
                    continue;
                //cout << "kula player id=" << marble->id << endl;

                if(find(begin(useless_fields), end(useless_fields), marble->id) != end(useless_fields)){
                    //cout << "jest bezuyteczna" << endl;
                    continue;
                }
                if(find(begin(specific_fields_rate_1), end(specific_fields_rate_1), marble->id) != end(specific_fields_rate_1)){
                    if(marble->player == player)
                        player_points++;
                    else
                        opponent_points++;
                    //cout << "dla id=" << marble->id << " dodajemy punkty=" << 1 << endl;
                    continue;
                }

                if(find(begin(specific_fields_rate_2), end(specific_fields_rate_2), marble->id) != end(specific_fields_rate_2)){
                    if(marble->player == player)
                        player_points += 2;
                    else
                        opponent_points += 2;
                    //cout << "dla id=" << marble->id << " dodajemy punkty=" << 2 << endl;
                    continue;
                }
                
                if(marble->id == center){
                    if(marble->player == player)
                        player_points += 4;
                    else
                        opponent_points += 4;
                    //cout << "dla id=" << marble->id << " dodajemy punkty=" << 5 << endl;
                    continue;
                }

                for(int dir: {-1, 1, -10, 10, -9, 9}){
                    for(int i : {1, 2, 3, 4}){
                        if(center + dir*i == marble->id){
                            if(marble->player == player)
                                player_points += (4 - i);
                            else
                                opponent_points += (4 - i);
                            //cout << "dla id=" << marble->id << " dodajemy punkty=" << 4-i << endl; 
                            continue;
                        }
                            
                    }
                }

                
            }
            //cout << "play=" << player_points << " oppo=" << opponent_points << endl;
            return player_points - opponent_points;

        }

        list<Movement*> generate_movements(int currentPlayer){ //player 1,2

            list<Movement*> movements;
            for(int direction: {-1, 1, -10, 10, -9, 9}){
                
                for(Movement* m : possibleMovesInLine(direction, currentPlayer)){
                    //m->print();
                    movements.push_back(m);
                }
                    
            }

            return movements;
        }

        list<Movement*> possibleMovesInLine(int direction, int player){
            list<Movement*> m;
            // cout << "aktualna plansza: " << endl;
            // printBoard(fields);
            //cout << "direction=" << direction << endl;

            for(Field* firstField : this->fields){

                //cout << "\tid field=" << firstField->id << " direction=" << direction << endl;
                // cout << "movements: " << endl;
                // for(Movement* mm : m)
                //     mm->print();



                if(firstField->player == player){
                    //cout << "TO JEST ID=" << firstField.id << endl;
                    Field* secField = findField(firstField->id + direction);
                    // 1 + NULL
                    if(isNULL(secField))
                        continue;
                    // 1 + 0
                    if(secField->player == 0){
                        m.push_back(new Movement(this->fields, firstField, direction));
                        continue;
                    }
                    // 2
                    else if(secField->player == player){ 
                        //cout << " 2 ";               
                        Field* thirdField = findField(secField->id + direction);
                        // 2 + NULL
                        if(isNULL(thirdField))
                            continue;
                        // 2 + 0
                        else if(thirdField->player == 0){
                            //cout << " 2+0 "; 
                            m.push_back(new Movement(this->fields, firstField, direction));
                            //cout << " 2+0 end";
                            continue;
                        }  
                        Field* firstOut = findField(thirdField->id + direction);
                        // 2 + 1
                        if(thirdField->player != player){
                            // 2 + 1 + NULL
                            if(isNULL(firstOut))
                                m.push_back(new Movement(this->fields, firstField, direction, 2));
                            // 2 + 1 + 0
                            else if(firstOut->player == 0)
                                m.push_back(new Movement(this->fields, firstField, direction));
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
                                m.push_back(new Movement(this->fields, firstField, direction));
                            // 4  -wrong
                            else if(firstOut->player == player)
                                continue;
                            // 3 + 1 opponent
                            else{
                                Field* secOut = findField(firstOut->id + direction);
                                // 3 + 1 + NULL
                                if(isNULL(secOut))
                                    m.push_back(new Movement(this->fields, firstField, direction, 2));
                                // 3 + 1 + 1  -wrong
                                else if(secOut->player == player)
                                    continue; 
                                // 3 + 1 + 0
                                else if(secOut->player == 0)
                                    m.push_back(new Movement(this->fields, firstField, direction));

                                // 3 + 2 opponent
                                else{
                                    Field* thirdOut = findField(secOut->id + direction);
                                    // 3 + 2 + NULL
                                    if(isNULL(thirdOut))
                                        m.push_back(new Movement(this->fields, firstField, direction, 2));
                                    // 3 + 2 + 0
                                    else if(thirdOut->player == 0)
                                        m.push_back(new Movement(this->fields, firstField, direction));
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

        void print(){
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

                Field* f = findField(row);
                while(!isNULL(f)){
                    if(f->player == 0)
                        SetConsoleTextAttribute(console_color, 7);
                    else if(f->player == 1)
                        SetConsoleTextAttribute(console_color, 2);
                    else if(f->player == 2)
                        SetConsoleTextAttribute(console_color, 12);
        
                    cout << f->id << " ";

                    f = findField(f->id + 10);
                }
                cout << endl;
            }
            SetConsoleTextAttribute(console_color, 7);
        }


        bool contains(int x, int tab[], int tabSize){
            for(int i = 0; i < tabSize; i++){
                if(x == tab[i])
                    return true;
            }
            return false;
        }

        bool isOffTheBoard(int x){
            if(x < 0 || x > 80)
                return false;
            for(int i : offTheBoard){
                if(i == x)
                    return false;
            }
            return true;
        }


        Field* findField(int id){
            for(Field* field : this->fields){
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

        int oppositePlayer(int p){
            if(p == 1)
                return 2;
            if(p == 2)
                return 1;
            return 0;
        }

};