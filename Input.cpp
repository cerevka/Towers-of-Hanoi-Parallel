/* 
 * File:   Input.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 * 
 * Created on November 2, 2011, 10:28 AM
 */

#include <string.h>
#include <stdlib.h>

#include "Input.h"

Input::Input(Board* _board) {
    board = _board;
}

Input::~Input() {
}

void Input::parseArguments(int _count, char** _args) { 
    if (_count < 2) {
        throw "To few arguments!";
    }
    
    bool helpSwitch = false;
    bool tokensCountSwitch = false;
    bool towersCountSwitch = false;
    bool targetTowerSwitch = false;
    bool maxDepthSwitch = false;
    bool fileNameSwitch = false;
    
    for (int i = 1; i < _count; i = i + 2) {       
        if (strcmp(_args[i], "-h") == 0) {
            // Napoveda.
            helpSwitch = true;
            printHelp();
        } else if (strcmp(_args[i], "-n") == 0) {
            // Pocet tokenu.
            tokensCount = atoi(_args[i + 1]);
            tokensCountSwitch = true;
        } else if (strcmp(_args[i], "-s") == 0) {
            // Pocet vezi.
            towersCount = atoi(_args[i + 1]);
            towersCountSwitch = true;
        } else if (strcmp(_args[i], "-r") == 0) {
            // Cilova vez.
            targetTower = atoi(_args[i + 1]);
            targetTowerSwitch = true;
        } else if (strcmp(_args[i], "-d") == 0) {
            // Maximalni hloubka.
            maxDepth = atoi(_args[i + 1]);
            maxDepthSwitch = true;
        } else if (strcmp(_args[i], "-f") == 0) {
            fileName = _args[i + 1];
            fileNameSwitch = true;
        } else {
            throw "Unknown arguments. Use -h for help.";
        }
    }
    
    if (helpSwitch == false) {
        if (fileNameSwitch == true) {
            // Data se nacitai ze souboru.
            parseFile();            
        } else {
            // Data se generuji.
            if (tokensCountSwitch && towersCountSwitch && targetTowerSwitch && maxDepthSwitch) {
                generateBoard();
            } else {
                throw "Incomplete input!";
            }
        }
        printTask();
    }
}

void Input::generateBoard() {
    // Inicilizuje veze na desce.
    board->init(towersCount);    
    
    // Inicializuje randomizer.
    srand(time(NULL));
    
    for (int i = tokensCount; i > 0; --i) {
        // Nahodne se urci vez.
        int tower = rand() % towersCount;
        board->addTowerTop(tower, i);
    }
}

void Input::parseFile() {
    ifstream file(fileName.c_str());
    
    string line;
    getline(file, line);
    stringstream config(line);
    
    // Pocet vezi, cilova vez, omezeni hloubky.
    config >> towersCount >> targetTower >> maxDepth;
    
    // Inicializuje se deska.
    board->init(towersCount);
    
    // Na jednotlive veze se nasazi tokeny.
    for (int i = 0; i < towersCount; ++i) {
        getline(file, line);
        stringstream tokens(line);
        
        while (tokens.good()) {
            int token;
            tokens >> token;
            board->addTowerTop(i, token);
        }
    }
    file.close();
}

void Input::printTask() const {
    cout << "Task:" << endl;
    if (fileName.empty()) {
        cout << "Number of tokens: " << tokensCount << endl;
    } else {
        cout << "Loaded from file: " << fileName << endl;
    }
    cout << "Number of towers: " << towersCount << endl;
    cout << "Target tower: " << targetTower << endl;
    cout << "Max depth: " << maxDepth << endl;
    cout << "Board:" << endl;
    cout << (*board) << endl;
}

void Input::printHelp() const {
    cout << "Help:" << endl;
    cout << "A) From file:" << endl;
    cout << "   -f path to file" << endl;
    cout << "B) Generate:" << endl;
    cout << "   -s number of towers" << endl;
    cout << "   -n number of tokens" << endl;
    cout << "   -r target tower" << endl;
    cout << "   -d max depth" << endl;
}

