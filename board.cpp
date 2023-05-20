#include <list>
#include <algorithm>
#include <iterator>
#include <vector>

#include <windows.h>


#include "movement.cpp"

int offTheBoard[] = {5,6,7,8,15,16,17,25,26,35,45,54,55,63,64,65,72,73,74,75};

class Board{
    vector<int> directions = {10, 1, -9, -10, -1, 9};
    public:
        list<Field*> fields;

        Board(){
            int fieldsPlayer1[] = {27,36,37,38,46,47,48,56,57,58,66,67,76,77};
            int fieldsPlayer2[] = {3,4,13,14,22,23,24,32,33,34,42,43,44,53};
            // int fieldsPlayer1[] = {27,36,37,38,46,47,48,49,56,57,58,66,67,77};
            // int fieldsPlayer2[] = {3,4,13,14,22,23,24,31,32,33,42,43,44,53};

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
        //int rate(Board initial_board, int player){
        float rate(int player){
            //cout << "player=" << player << endl;
            float score = 5;

            // NUMBER OF MARBLES
            score += getCountMarbles(player) * 1.0;
            score -= getCountMarbles(oppositePlayer(player)) * 1.0;
            
            //cout << "\t MARBLES BALANCE=" << score << endl;



            // DISTANCE FROM THE CENTER
            int d = summary_distance_from_center(player);

            score -= d * 0.01;
            //cout << "\t DISTANCE=" << d << endl;
            cout << "\t DISTANCE=" << score << endl;
            return score;

            // }
            //         // WARTOSC BEZWZGLEDNA Z BALANCE            !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            // // GROUPING
            // int prev_grouping = initial_board.initial_marbles_neighborhood_balance;
            // int current_grouping = marbles_neighborhood_balance(player);

            // int grouping_balance = current_grouping - prev_grouping;
            // cout << "\t GROUP BALANCE=" << grouping_balance<< endl;
            // if(abs(grouping_balance) > 5){
            //     if(grouping_balance)
            //         return grouping_balance;
            //     else
            //         return 2;
            // }
            // Attacking by group

            return score;
        
        }
        int getCountMarbles(int player){
            int points = 0;
            for(Field* marble : fields){
                if(marble->player == player){
                    points++;
                }
            }
            return points;
        }
        int marbles_neighborhood_balance(int player){
            int player_points = 0;
            int opponent_points = 0;

            for(Field* marble : fields){
                if(marble->player == player){
                    for(int dir : this->directions){
                        Field* neighborn = getField(marble->id + dir);
                        if(neighborn != nullptr && neighborn->player == player){
                            player_points++;
                        }
                    }
                }
                else if(marble->player != 0){
                    for(int dir : this->directions){
                        Field* neighborn = getField(marble->id + dir);
                        if(neighborn != nullptr && neighborn->player == oppositePlayer(player)){
                            opponent_points++;
                        }
                    }
                }
            }
            return player_points - opponent_points;
        }
        Field* getField(int id){
            for(Field* marble : fields){
                if(marble->id == id)
                    return marble;
            }
            return nullptr;
        }

        int get_marbles_balance(int player){
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


        int summary_distance_from_center(int player){
            int const center = 40;
            int const max_distance = 4;
            int summary_distance = 0;
            vector<int> marbles_ids = get_marbles_ids(player);
            
            for(int distance = 1; distance <= max_distance; distance++){
                vector<int> neighbors_ids = get_neighbors_ids(center, distance);
                for(int marble_id : marbles_ids){
                    if(list_contains_element(neighbors_ids, marble_id)){
                        //cout << "dla marble=" << marble_id << " distance=" << distance << endl;
                        summary_distance += distance;
                    }
                }
            }
            return summary_distance;
        }
        bool list_contains_element(vector<int> l, int element) const{
            if(find(begin(l), end(l), element) != end(l))
                return true;
            return false;
        }
        vector<int> get_neighbors_ids(int field_id, int distance = 1){
            vector<int> neighbors_ids;
            vector<int> basic_neighbors_ids;
            for(int dir : this->directions){
                int neighbor_id = field_id + (dir * distance);
                basic_neighbors_ids.push_back(neighbor_id);

            }
            
            for(int i = 0; i < this->directions.size(); i++){
                int begin_neighbor_id = basic_neighbors_ids.at(i);
                int end_neighbor_id = basic_neighbors_ids.at((i+1) % directions.size());

                int direction = directions.at((i+2)% directions.size());

                for(
                    int neighbor_id = begin_neighbor_id + direction; 
                    end_neighbor_id != neighbor_id; 
                    neighbor_id += direction)
                    {
                    neighbors_ids.push_back(neighbor_id);
                }
            }
            neighbors_ids.insert(neighbors_ids.end(), basic_neighbors_ids.begin(), basic_neighbors_ids.end());
            return neighbors_ids;
        }   
        vector<int> get_marbles_ids(int player){
            vector<int> marbles_ids;
            for(Field* marble : fields){
                if(marble->player == player)
                    marbles_ids.push_back(marble->id);
            }
            return marbles_ids;
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