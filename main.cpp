/* 
 * File:   main.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on October 31, 2011, 3:21 PM
 */

#include <cstdlib>
#include <string>
//#include <mpi.h>

#include "Input.h"

using namespace std;

/*
 * Vstupni bod programu.
 */
int main(int argc, char** argv) {
    try {
        cout << "Towers of Hanoi" << endl;
    
        // Priprav se deska.
        Board board;
        Input input(&board);
        input.parseArguments(argc, argv); 
        return 0;
        
    } catch (const char* exception) {
        cout << exception << endl;
        return 1;
    }
    
    
}

