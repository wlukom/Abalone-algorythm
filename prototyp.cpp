
#include<iostream>
#include<fstream>
#include <iterator>


#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <list>

#include<chrono>
#include<thread>

#include "path.cpp"
//#include "board.cpp"

using namespace std;

class Algorithm{

    int started_player;
    int depth;
    Board initial_board;
    Path* root_node;

    public:

        Algorithm(list<Field*> fields, int player, int depth = 3){
            this->started_player = player;
            this->depth = depth;
            this->initial_board = Board(fields);

            root_node = new Path(depth, changePlayer(started_player));

        }
        array<int, 2> getTheBestMovement(){

            array<int, 2> the_best_movement;

            alfabeta(root_node, initial_board, -10, 20);

            Movement* m = choosingTheBestMovement(root_node);

            the_best_movement[0] = m->getFieldId();
            the_best_movement[1] = m->getDirection();

            m->print();
            initial_board.updateMovement(m);
            initial_board.print();
            
            return the_best_movement;

        }

        float alfabeta(Path* path, Board board, float alfa, float beta){
            if(path->getDepth() == 0){
                //board.print();
                return board.rate(started_player);
            }


            int child_player = changePlayer(path->getPlayer());
            float forwarded_value = 20;

            if(child_player == started_player)
                forwarded_value = -10;

            for(Movement* m : board.generate_movements(child_player)){

                Path* child = new Path(m, path->getDepth() - 1);
                path->addChild(child);

                Board b = Board(board.fields);
                b.updateMovement(m);

                float rate = alfabeta(child, b, alfa, beta);
                m->setRate(rate);   

                if(child_player == started_player){
                    forwarded_value = max(forwarded_value, rate);
                    alfa = max(alfa, forwarded_value);
                }
                else{
                    forwarded_value = min(forwarded_value, rate);
                    beta = min(beta, forwarded_value);
                }

                if(alfa >= beta)
                    break;
            }
            return forwarded_value;

        }
        Movement* choosingTheBestMovement(Path* path){
            list<Movement*> good_movements;
            float the_highest_rate = 0;

            for(Path* p : path->getChildren()){
                if(p->getMovement()->getRate() > the_highest_rate){
                    the_highest_rate = p->getMovement()->getRate();
                    good_movements.clear();
                    good_movements.push_back(p->getMovement());
                }
                else if(p->getMovement()->getRate() == the_highest_rate){
                    good_movements.push_back(p->getMovement());
                }
            }
            srand ( time(NULL) );
            int rand_number = rand() % good_movements.size();
            cout << "ilosc dobrych ruchow=" << good_movements.size() << endl;
            for(Movement* m : good_movements){
                m->print();
            }
            cout << "Koneic" << endl;

            return *(next(good_movements.begin(), rand_number));
        }
        int changePlayer(int player){
            if(player == 2) 
                return 1;
            return 2;
        }

};

int main(){
    cout << "XDD" << endl;
    Board b = Board();
    Algorithm a = Algorithm(b.fields, 2);
    a.getTheBestMovement();

}