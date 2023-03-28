// #include <iostream>
// #include <cstdlib>
// #include <stdlib.h>
// #include <windows.h>
// #include <list>

// #include<chrono>
// #include<thread>

// #include "field.cpp"

// using namespace std;

// class Board{
//     public:
//         list<Field*> board;

//         Board(){
//             this->board = generateList();
//         }
//         Board(list<Field*> fields){
//             this->board = fields;
//         }

//         list<Field*> generateList(){
//             list<Field*> myList;
//             int offTheBoard[] = {5,6,7,8,15,16,17,25,26,35,45,54,55,63,64,65,72,73,74,75};
//             int fieldsPlayer1[] = {27,36,37,38,46,47,48,56,57,58,66,67,76,77};
//             int fieldsPlayer2[] = {3,4,13,14,22,23,24,32,33,34,42,43,44,53};

//             int offTheBoardSize = sizeof(offTheBoard)/sizeof(offTheBoard[0]);
//             int fieldsPlayer1Size = sizeof(fieldsPlayer1)/sizeof(fieldsPlayer1[0]);
//             int fieldsPlayer2Size = sizeof(fieldsPlayer2)/sizeof(fieldsPlayer2[0]);

//             int x = 0;
//             for(int i = 0; i < 61; i++, x++){
//                 while(contains(x, offTheBoard, offTheBoardSize))
//                         x++;

//                 if(contains(x, fieldsPlayer1, fieldsPlayer1Size))
//                     myList.push_back(new Field(x,1));
//                 else if(contains(x, fieldsPlayer2, fieldsPlayer2Size))
//                     myList.push_back(new Field(x,2));
//                 else
//                     myList.push_back(new Field(x,0));
//             }
//             return myList;
//         }

//         bool contains(int x, int tab[], int tabSize){
//             for(int i = 0; i < tabSize; i++){
//                 if(x == tab[i])
//                     return true;
//             }
//             return false;
//         }

//         void print(list<Field*> fields){
//             HANDLE console_color;
//             console_color = GetStdHandle(STD_OUTPUT_HANDLE);

//             int rows[] = {36,27,18,9,0,1,2,3,4};
//             for(int row : rows){
//                 if(row == 9 || row == 1)
//                     cout << " ";
//                 else if(row == 18 || row == 2)
//                     cout << "  ";
//                 else if(row == 27 || row == 3)
//                     cout << "   ";
//                 else if(row == 36 || row == 4)
//                     cout << "    ";
                    
//                 Field* f = findField(fields, row);
//                 while(f != NULL){
//                     if(f->player == 0)
//                         SetConsoleTextAttribute(console_color, 7);
//                     else if(f->player == 1)
//                         SetConsoleTextAttribute(console_color, 2);
//                     else if(f->player == 2)
//                         SetConsoleTextAttribute(console_color, 12);
        
//                     cout << f->id << " ";

//                     f = findField(fields, f->id + 10);
//                 }
//                 cout << endl;
//             }
//             SetConsoleTextAttribute(console_color, 7);
//         }

//         Field* findField(list<Field*> fields, int id){
//             for(Field* field : fields){
//                 if(field->id == id)
//                     return field;
//             }
//             return NULL;
//         }
// };