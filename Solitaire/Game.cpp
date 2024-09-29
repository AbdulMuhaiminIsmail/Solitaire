#pragma once
#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>    
#include <ctime>  
#include "Card.cpp"
#include "Enums.hpp"
#include "Stack.cpp"
#include "List.cpp"

using namespace std;

class Game {
private:
    Stack<Card*> stockPile;
    Stack<Card*> wastePile;
    Stack<Card*> foundations[4];    
    DLL<Card*> tableaus[7];

public:
    Game() {
        setup();
    }

    void setup() {
        // Creating deck

        vector<Card*> vec;
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                Card* card = new Card(suit, rank, true);
                vec.push_back(card);       
            }
        }

        // Shuffle deck

        random_device rd; // Gets a random number from hardware
        mt19937 engine(rd()); // Seeds this number to the generator
        shuffle(vec.begin(), vec.end(), engine);

        //Pushes to stock

        for (int i = 0; i < vec.size(); i++) {
                stockPile.push(vec[i]);
        }

        //Distributes the cards among tableaus

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j <= i; j++) {
                tableaus[i].insertTail(stockPile.pop());
                if (i == j) tableaus[i].getTail()->flipHiddenStatus();
            }
        }

        //for (int i = 0; i < 7;i++) {
        //    for (auto it = tableaus[i].fbegin(); it != tableaus[i].fend(); ++it) {
        //        (*it)->display();
        //    }
        //    cout << endl;
        //}

        //while (!stockPile.isEmpty()) {
        //    Card* card = stockPile.pop();
        //    card->display();
        //}


    }

    void print() {
        cout << "Solitaire" << endl << endl << endl;

        cout << "Stock\t\tWaste\t\t\t\tFoundation1 \tFoundation2 \tFoundation3 \tFoundation4" << endl;

        // Displaying top card of each stack
        
        if (stockPile.getHead()) {
            stockPile.getHead()->display();
        }
        else {
            cout << "[ ]";
        }
        
        cout  << "\t\t" ;

        if (wastePile.getHead()) {
            wastePile.getHead()->display();
        }
        else {
            cout << "[ ]";
        }
        
        cout << "\t\t\t\t";

        for (int i = 0; i < 4; i++) {          
            Card* topCard = foundations[i].getHead();
            if (topCard) {
                topCard->display();
            }
            else {
                cout << "[ ]";
            }

            if (i != 3) cout << "\t\t";
        }

        cout << endl;

        // Displaying number of cards in each stack
        
        cout << "(" << stockPile.getSize() << " cards)\t(" << wastePile.getSize() << " cards)\t\t\t";

        for (int i = 0; i < 4; i++) {
            cout << "(" << foundations[i].getSize() << " cards)";
            if (i != 3) cout << "\t";
        }

        cout << endl << endl;

        // Displaying column numbers

        for (int i = 0; i < 7; i++) {
            cout << "Column " << i + 1;
            if (i != 7) cout << "\t";
        }

        cout << endl;
        
        // Displaying cards in each tableau

        for (int i = 0; i < 7; i++) {
            cout << "(" << tableaus[i].size() << " cards)";
            if (i != 7) cout << "\t";
        }



    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);  // Set the console to UTF-8
    Game game;   
    game.print();             
    return 0;
}