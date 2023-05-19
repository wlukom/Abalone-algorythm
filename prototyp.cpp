
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

int started_player = 2;

Board initial_board = Board();
initial_board_evaluation rating_initial_board = {
    initial_board.get_marbles_balance(started_player),
    initial_board.distance_from_center_balance(started_player),
    initial_board.marbles_neighborhood_balance(started_player)
};



Movement* getTheBestMovement(Path* path){
    list<Movement*> good_movements;
    int the_highest_rate = -2;

    for(Path* p : path->children){
        if(p->movement->rate > the_highest_rate){
            the_highest_rate = p->movement->rate;
            good_movements.clear();
            good_movements.push_back(p->movement);
        }
        else if(p->movement->rate == the_highest_rate){
            good_movements.push_back(p->movement);
        }
    }
    int number_of_the_best_movement = rand() % good_movements.size();
    cout << "ilosc dobrych ruchow=" << good_movements.size() << endl;
    //list<Movement*>::iterator it = good_movements.begin();
    return *(next(good_movements.begin(), number_of_the_best_movement));
}
int changePlayer(int player){
    if(player == 2) 
        return 1;
    return 2;
}


int alfabeta(Path* path, Board board, int alfa, int beta){
    if(path->depth == 1)
        return board.rate(rating_initial_board, path->player);

    if(path->player != started_player){
        for(Movement* m : board.generate_movements(path->player)){

            Path* child = new Path(m, path->depth - 1, changePlayer(path->player));
            path->addChild(child);

            Board b = Board(board.fields);
            b.updateMovement(m);
            

            beta = min(beta, alfabeta(child, b, alfa, beta));
            m->setRate(beta);

            if(alfa >= beta)
                break;
        }
        return beta;
    }
    else{ // path->player == started_player
        for(Movement* m : board.generate_movements(path->player)){

            Path* child = new Path(m, path->depth - 1, changePlayer(path->player));
            path->addChild(child);

            Board b = Board(board.fields);
            b.updateMovement(m);

            alfa = max(alfa, alfabeta(child, b, alfa, beta));
            m->setRate(alfa);
            if(alfa >= beta)
                break;
        }
        return alfa;
    }

}

void printPath(Path* node) {
    for (int i = 0; i < node->depth; i++) {
        cout << "\t";
    }
    cout << "- " << node->toString() << endl;
    for (Path* child : node->children) {
        printPath(child);
    }
}
int counting_final_children(Path* node) {
    if (node->children.empty())
        return 1;
    else{
        int count = 0;
        for (Path* child : node->children)
            count += counting_final_children(child);

        return count;
    }
}
void printTree(Path* node) {
    if (node == nullptr) {
        return;
    }

    for (int i = 1; i < node->depth; ++i) {
        std::cout << "|  "; // Wcięcie dla lepszego wyglądu
    }

    std::cout << "|--" ;
    node->print();

    for (const auto& child : node->children) {
        printTree(child);
    }
}



int main(){
    exit(0);
    // DODAC SYSTEM OCENY: BYCIE W GRUPIE, ATAKOWANIE ZBIOROWE
    //I SPRAWDZIC TO

    int depth = 4; // nieparzyste
    //int started_player = 2;

    Path* root_node = new Path(depth, started_player);
    
    
    alfabeta(root_node, initial_board, -10, 10);
    root_node->printChildren();
    //cout << "x=" << x;
    // create paths

    cout << "suma dzieci=" << counting_final_children(root_node) << endl;
    Movement* m = getTheBestMovement(root_node);
    m->print();
    //printTree(root_node);
}