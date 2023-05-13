
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

void createChild(Path* path, Board board){
    if(path->depth == 0){
        //cout << " depth == 0 FINAL" << endl;
        return;
    }

    list<Movement*> moves = board.generate_movements(path->player);
    if(moves.empty()){
        cout << "MOVES ARE EMPTY !!!!!!!" << endl;
        return;
    }

    for(Movement* m : moves){
        int current_depth = path->depth - 1;

        Path* child = new Path(m, current_depth, changePlayer(path->player));

        //cout << " Zostal stworzony path depth=" << child->depth << " player=" << child->player <<endl; 
        
        Board b = Board(board.fields);
        b.updateMovement(m);
        //b.print();

    
        createChild(child, b);
        path->addChild(child);

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


int main(){
    int depth = 2;
    int player = 2;

    // create board
    Board initial_board = Board();

    // create paths
    Path* root_node = new Path(depth, player);

    createChild(root_node, initial_board);

    cout << "suma dzieci=" << counting_final_children(root_node) << endl;

    printPath(root_node);
}