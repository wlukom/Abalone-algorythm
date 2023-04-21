
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
// Path* createPaths(Board board, int depth, int player){
//     cout << " Create paths depth=" << depth << " player=" << player <<endl; 
//     if(depth == 0){
//         cout << " depth == 1 FINAL" << endl;
//         return new Path();
//     }

//     Path* paths = new Path();
    
//     for(Movement* m : board.generate_movements(player)){
//         m->print();

//         Board b = Board(board.board);
//         b.updateMovement(m);

//         Path* new_path = createPaths(b, depth - 1, changePlayer(player));

//         paths->paths.push_back(new Path(m));
//         paths->paths.back()->paths = new_path->paths;
//         // paths.add(new Path([m]));
//         //paths.push_back(new Path(m));
//     }
//     cout << "KONIEC PRAWIE";
//     return paths;

// }

list<Path*> createPaths(Board board, int depth, int player){
    //cout << " Create paths depth=" << depth << " player=" << player <<endl; 

    list<Path*> paths;
    
    for(Movement* m : board.generate_movements(player)){
        // paths.add(new Path([m]));
        paths.push_back(new Path(m));
    }

    if(depth == 1){
        //cout << " depth == 1 FINAL" << endl;
        return paths;
    }

    Path* theLastPath = paths.back();

    //return paths;
    for (list<Path*>::iterator path = paths.begin(); path != paths.end(); ++path) {
        //cout << "a";
        //(*path)->print();

        if(*path == theLastPath){
            cout << "to ja juz wychodze" << endl;
            break;          
        }
        //std::cout<<(*it)->name;

        

        Board b = Board(board.board);
        b.updateMovement((*path)->getLastMovement());
        //b.print();

        for(Path* p : createPaths(b, depth - 1, changePlayer(player))){
            p->movements.push_front((*path)->getLastMovement());
            paths.push_back(p);
        }
        //cout << " B " << endl;
    }
///////////////////////////////////////////////////////////////////

    // for(Path* path: paths){
    //     //path->print();
    //     // do przekywania path dalej - zrobic drzewko nizej(skopiowac do tej pory wszystkei ruchy)
    //     Board b = Board(board.board);
        
    //     //cout << " A " << endl;
    //     //Board b = board.copy();
        
    //     b.updateMovement(path->getLastMovement());
    //     //b.print();
    //     //paths.addPaths(createPaths(b, changePlayer(player), depth - 1));
    //     for(Path* p : createPaths(b, depth - 1, changePlayer(player))){
    //         paths.push_back(p);
    //     }
    //     //paths = createPaths(b, depth - 1, changePlayer(player));
    //     //cout << " B " << endl;
    // }
    
}


// list<Path*> createPaths(list<Field*> board, int depth){

//     Board b = initial_boards;
//     b.generate_movements(2)

//     list<Path*> paths;

//     for(Path p: paths){
//         auto board = b;
//         for(int i = 0; i < depth i++){
//             list<Movement*> possible_moves = board.generate_movements(board, 2);
//             Movement* move = losuje z possible;
//             p.add(move);
//             board.apply(move)
//         }
//     }


    
// list<Movement*> possible_moves = generate_movements(board, 2);
    
//     for(Movement* m : possible_moves){
//         paths.push_back(new Path(m));
//     }

//     for(Path* p : paths){
//         // dla kazdego path generate_movements
//     }
//     return paths;
// }

int main(){

    cout << "decision treeeee";

    // create board
    Board initial_board = Board();
    //initial_board.print();


    // create paths
    list<Path*> paths = createPaths(initial_board, 2, 2);
    //Path* p = createPaths(initial_board, 2,2);
    //return 10;

    cout << "ilosc=" << paths.size();
    for(Path* p : paths){
        Board b = Board();
        for(Movement* m : p->movements){
            b.updateMovement(m);
            b.print();
        }
        cout << endl << endl;
            
    }
    // return 1;
    // ofstream s("dane.txt");
    // for(Path* p : paths){
        
    //     Board b = Board();
        

    //     b.updateMovement(p->getLastMovement());
    //     p->print(s);
    //     s << endl;
    //    // break;
    // }
    // s.close();
    // cout << "Koniec" ;


}