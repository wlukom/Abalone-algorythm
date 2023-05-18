
#include<iostream>
#include<fstream>


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


// Movement* selectMovement(list<Movement*> movements){
//     int theBestRate = -3;
//     list<Movement*> theBestMovements;
//     for(Movement* m : movements){

//         m->setRate();
//         if(m->rate > theBestRate){
//             theBestMovements.clear();
//             theBestMovements.push_back(m);
//             theBestRate = m->rate;
//         }
//         else if(m->rate == theBestRate)
//             theBestMovements.push_back(m);

//         m->print();
//         printBoard(m->board);
//     }
//     // movement drawing
//     int randNumber = int(rand() % theBestMovements.size());
//     cout << "Rand number=" << randNumber << " a lista ma size=" << theBestMovements.size() << endl;;
//     list<Movement*>::iterator it = theBestMovements.begin();
//     advance(it, randNumber);
//     return *it;
//     //return theBestMovements[randNumber];


//     // int counter = 0;
//     // for(Movement* m : theBestMovements){
//     //     if(counter == randNumber)
//     //         return m;
//     //     counter++;
//     // }
// }
int changePlayer(int player){
    if(player == 2) 
        return 1;
    return 2;
}
Board initial_board = Board();
int started_player = 2;

int alfabeta(Path* path, Board board, int alfa, int beta){
    if(path->depth == 1)
        return board.rate(initial_board, path->player);

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
    int depth = 4; // nieparzyste
    //int started_player = 2;

    Path* root_node = new Path(depth, started_player);
    
    alfabeta(root_node, initial_board, -10, 10);
    root_node->printChildren();
    //cout << "x=" << x;
    // create paths

    cout << "suma dzieci=" << counting_final_children(root_node) << endl;
    //printTree(root_node);
}