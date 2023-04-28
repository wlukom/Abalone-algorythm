
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


Path* createPaths(Board board, int depth, int player){
    cout << " Create paths depth=" << depth << " player=" << player <<endl; 
    if(depth == 0){
        cout << " depth == 0 FINAL" << endl;
        return nullptr;//new Path();
    }

    Path* paths = new Path();

    for(Movement* m : board.generate_movements(player)){

        Board b = Board(board.fields);
        b.updateMovement(m);
        b.print();
        Path* new_path = new Path(m);

        // paths->children.push_back(new Path(m));

        Path* new_child = createPaths(b, depth - 1, changePlayer(player));

        if(new_child != nullptr){
            cout << "NIE JEST NULLPTR" << endl;
            new_path->children.push_back(new_child);
        }
        
        //cout << "A";
        cout << "new path:" << endl;
        new_path->print();
        //cout << "B";
        paths->children.push_back(new_path);
        cout << "paths:" << endl;
        paths->print();

        // if(new_child != NULL){


        //     //paths->children.back()->children = new_child->children;
        //     //paths->children.back()->addPaths(new_child->children);
        // }
        // else{
        //     cout << "c";
        // }

        

    }
    //int pom = counting_children(paths);
    //cout << "total count=" << 
    return paths;

}

void traverse_tree(Path* path) {
    path->movement->print();
    cout << endl;
    //cout << "Ruch: " << node.move << std::endl;
    for (Path* child : path->children) {
        traverse_tree(child);
    }
}

int counting_children(Path* path) {
    if(path->children.size() == 0){
        return 1;
    }

    int count = 0;
    for (Path* child : path->children) {
        count += counting_children(child);
    }
    return count;
}
void display_tree(Path* node, int depth = 0) {
    if(node == nullptr)
        return;
    // Wyświetlanie identyfikatora węzła wraz z poziomem drzewa
    // for (int i = 0; i < depth; ++i) {
    //     std::cout << "|  ";
    // }
    // std::cout << "|-- ";// << node->movement << std::endl;
    cout << " depth=" << depth << " ";
    node->print();

    // Wywołanie rekurencyjne dla dzieci węzła
    for (Path* child : node->children) {
        display_tree(child, depth + 1);
    }
}


int main(){

    cout << "decision treeeee" << endl;;
    // ToDo

    // - popracowac nad wyswietlniem drzewa - funkcja display

    // create board
    Board initial_board = Board();
    //initial_board.print();


    // create paths
    Path* p = createPaths(initial_board, 1,2);
    cout << "Koniec" <<endl;
    p->printChildren();
    //return 1;
    int pom = counting_children(p);
    cout << "suma dzieci=" << pom << endl;
    display_tree(p, 1);

    //traverse_tree(p);

}