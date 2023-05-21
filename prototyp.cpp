
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

Movement* getTheBestMovement(Path* path){
    list<Movement*> good_movements;
    float the_highest_rate = 0;

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
    srand ( time(NULL) );
    int rand_number = rand() % good_movements.size();
    cout << "ilosc dobrych ruchow=" << good_movements.size() << endl;

    for(Movement* m : good_movements){
        m->print();
    }
    cout << "KONIeC" << endl;
    return *(next(good_movements.begin(), rand_number));
}
int changePlayer(int player){
    if(player == 2) 
        return 1;
    return 2;
}

float alfabeta(Path* path, Board board, float alfa, float beta){
    if(path->depth == 0){
        board.print();
        return board.rate(started_player);
    }

    float forwarded_value = 20;
    if(path->player == started_player)
        forwarded_value = -10;

    for(Movement* m : board.generate_movements(path->player)){

        Path* child = new Path(m, path->depth - 1, changePlayer(path->player));
        path->addChild(child);

        Board b = Board(board.fields);
        b.updateMovement(m);

        float rate = alfabeta(child, b, alfa, beta);
        m->setRate(rate);   
        //cout << "ocena dla ruchu m =" << m->rate << endl;

        if(path->player == started_player){
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

    for (int i = 0; i < node->depth; ++i) {
        std::cout << "|  "; // Wcięcie dla lepszego wyglądu
    }

    std::cout << "|--" ;
    node->print();

    for (const auto& child : node->children) {
        printTree(child);
    }
}



int main(){
    // initial_board.rate(rating_initial_board, started_player);
    // initial_board.print();
    // exit(0);

    int depth = 1; // min 1 nieparzyste
    //int started_player = 2;

    Path* root_node = new Path(depth, started_player);
    
    
    float x = alfabeta(root_node, initial_board, -10, 20);
    cout << "OCENA=" << x << endl;
    //exit(0);
    root_node->printChildren();
    //cout << "x=" << x;
    // create paths

    cout << "suma dzieci=" << counting_final_children(root_node) << endl;
    Movement* m = getTheBestMovement(root_node);
    m->print();
    initial_board.updateMovement(m);
    // initial_board.printRating(started_player);
    initial_board.print();
    //printTree(root_node);
}