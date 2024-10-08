#pragma once
#include <iostream>
#include <vector>
#include <algorithm> 
#include <random>    
#include <ctime>  
#include <string>
#include "Card.cpp"
#include "Enums.hpp"
#include "Stack.cpp"
#include "List.cpp"
#include "Command.cpp"

using namespace std;

class Game {
private:
    int moves;
    Stack<Card*> stockPile;
    Stack<Card*> wastePile;
    Stack<Card*> foundations[4];    
    DLL<Card*> tableaus[7];

public:
    Game() : moves(0) { setupGame(); }

    void setupGame() {
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

        // Make the stock pile visible as well

        for (auto it = stockPile.begin(); it != stockPile.end(); ++it) {
            (*it)->flipHiddenStatus();
        }
    }

    void printGameState() {
        cout << "\t\t\t\t\t\tSolitaire" << endl << endl;

        cout << "Moves: " << moves << endl << endl << endl;

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
        
        // Displaying number of cards in each tableau

        for (int i = 0; i < 7; i++) {
            cout << "(" << tableaus[i].size() << " cards)";
            if (i != 7) cout << "\t";
        }

        cout << endl;
        
        // Displaying cards in each tableau

        DLL<Card*>::ListIterator iters[7];
        for (int i = 0; i < 7; i++) {
            iters[i] = tableaus[i].fbegin();
        }

        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 7; j++) {
                if (iters[j] != DLL<Card*>::ListIterator(nullptr)) {
                    (*iters[j])->display();
                }

                cout << "\t\t";
                ++iters[j];
            }
            cout << endl;
        }
    }

    Command inputCommand() {
        string line;
        cout << "Please enter command with proper syntax, given in the instruction set: ";
        getline(cin, line);

        string cmd, src, dest;
        int count = 1;

        // If the length is wrong so is the syntax

        if (line.length() != 12 && line.length() != 9 && line.length() != 4) {
            return Command("", "", "", 0);
        }

        cmd = line.substr(0, 4);
        
        // Wrong command

        if (cmd != "draw" && cmd != "quit" && cmd != "exit" && cmd != "move") {
            return Command("", "", "", 0);
        }
        
        // These don't require any futher parameters
        
        if (cmd == "draw" || cmd == "quit" || cmd == "exit") {
            return Command(cmd, "", "", 0);
        }

        // Handling wastepile case and other cases

        if (line[5] == 'w') {
            src = 'w';
            dest = line.substr(7, 2);
        }
        else {
            src = line.substr(5, 2);
            dest = line.substr(8, 2);
            count = line[11];
        }

        return Command(cmd, src, dest, count);

    }
    
    void parseCommand(Command command) {
        if (command.getCmd() == "quit" || command.getCmd() == "exit") {
            cout << "Thanks for playing Solitaire! Hope to see you again soon! ";
            return;
        }

        else if (command.getCmd() == "draw") {
            drawCardFromStock();
            return;
        }

        else if (command.getCmd() == "move") {
            char src = command.getSrc()[0];
            int srcX = command.getSrc()[1] - 48;
            char dest = command.getDest()[0];
            int destX = command.getDest()[1] - 48;
            int n = command.getCount() - 48;

            // Ensuring proper valid tableau ids
            
            if ((src == 'c' &&  srcX < 0 || srcX > 7) || (dest == 'c' && destX < 0 || destX > 7)) {
                cout << "Invalid tableau ids. " << endl;
                return;
            }

            // Ensuring proper valid foundations ids

            if ((src == 'f' && (srcX < 0 || srcX > 4)) || (dest == 'f' && (destX < 0 || destX > 4))) {
                cout << "Invalid foundation ids. " << endl;
                return;
            }

            // Calling appropriate functions
            
            if (src == 'c' && dest == 'c') {
                moveFromTableauToTableau(srcX, destX, n);
                return;
            }

            if (src == 'c' && dest == 'f') {
                moveFromTableauToFoundation(srcX, destX);
                return;
            }

            if (src == 'f' && dest == 'c') {
                moveFromFoundationToTableau(srcX, destX);
                return;
            }

            if (src == 'w' && dest == 'f') {
                moveFromWasteToFoundation(destX);
                return;
            }

            if (src == 'w' && dest == 'c') {
                moveFromWasteToTableau(destX);
                return;
            } 
        }
        else {
            cout << "Invalid command or syntax. " << endl;
            return;
        }
    }
    
    void moveFromTableauToTableau(int src, int dest, int n) {
        --src, --dest;                                      // Converting tableau numbers to indices
        auto srcTab = tableaus[src].accessNode(n);          // Accessing ptr to head of sublist to append
        auto destTab = tableaus[dest].accessNode(1);        // Accessing ptr to tail of destination tableau

        // Ensuring source is populated with card(s)
        
        if (!srcTab) {
            cout << "You can not move cards from an empty tableau or the tableau does not have enough cards." << endl;
            return;
        }
        
        // Ensuring card(s) to be moved are not hidden

        if (srcTab->data->isHidden()) {
            cout << "You can not move hidden cards." << endl;
            return;
        }

        // Ensuring only King is placed as tableau starter

        if (!destTab && srcTab->data->getRank() != 13) {
            cout << "You can only start a tableau with a King card." << endl;
            return;
        }

        // Ensuring only King is placed as tableau starter

        if (!destTab && srcTab->data->getRank() == 13) {
            tableaus[dest].append(tableaus[src].extract(n), n);
            if (tableaus[src].getTail() && tableaus[src].getTail()->isHidden()) tableaus[src].getTail()->flipHiddenStatus();
            moves++;
            return;
        }

        // Ensuring difference of one in ranks

        if (destTab->data->getRank() - srcTab->data->getRank() != 1) {
            cout << "You can arrange cards only in proper descending order in tableaus." << endl;
            return;
        }

        // Ensuring opposite suits

        if ((srcTab->data->getSuit() == 1 || srcTab->data->getSuit() == 3) && (destTab->data->getSuit() != 2 && destTab->data->getSuit() != 4) ||
            (srcTab->data->getSuit() == 2 || srcTab->data->getSuit() == 4) && (destTab->data->getSuit() != 1 && destTab->data->getSuit() != 3)) {
            cout << "You can not place cards of the same suit together. " << endl;
            return;
        }

        // Finally moving card(s) between tableaus

        tableaus[dest].append(tableaus[src].extract(n), n);

        // Flipping last card of source destination (if required)

        if (tableaus[src].getTail() && tableaus[src].getTail()->isHidden()) tableaus[src].getTail()->flipHiddenStatus();
        moves++;

    }

    void moveFromTableauToFoundation(int tableauNumber, int foundationNumber) {
        // Ensuring tableau is not empty
        
        --tableauNumber;
        Card* tab = tableaus[tableauNumber].getTail();

        if (!tab) {
            cout << "You can not pick card from an empty tableau. " << endl;
            return;
        }

        // Ensuring same suits
        
        if (tab->getSuit() == 1 && foundationNumber != 1 ||
            tab->getSuit() == 2 && foundationNumber != 2 ||
            tab->getSuit() == 3 && foundationNumber != 3 ||
            tab->getSuit() == 4 && foundationNumber != 4) {
            cout << "You can only place cards with same suit as foundation. " << endl;
            return;
        }

        // Ensuring first card to be Ace

        --foundationNumber;

        if (foundations[foundationNumber].isEmpty() && tab->getRank() != 1) {
            cout << "You can only start a foundation with Ace. " << endl;
            return;
        }

        if (foundations[foundationNumber].isEmpty() && tab->getRank() == 1) {
            auto temp = tableaus[tableauNumber].removeTail();   
            foundations[foundationNumber].push(temp->data);    
            if (tableaus[tableauNumber].getTail() && tableaus[tableauNumber].getTail()->isHidden()) tableaus[tableauNumber].getTail()->flipHiddenStatus();
            moves++;
            return;
        }
        
        // Ensuring ascending order

        if (tab->getRank() - foundations[foundationNumber].getHead()->getRank() != 1) {
            cout << "You can place cards only in proper ascending order in foundations. " << endl;
            return;
        }

        auto temp = tableaus[tableauNumber].removeTail();   // Returns the tail by reference
        foundations[foundationNumber].push(temp->data);     // Pushes to the foundation as the new head

        // Flipping last card of source destination (if required)

        if (tableaus[tableauNumber].getTail() && tableaus[tableauNumber].getTail()->isHidden()) tableaus[tableauNumber].getTail()->flipHiddenStatus();
        moves++;
    }

    void moveFromFoundationToTableau(int foundationNumber, int tableauNumber) {
        // Ensuring the foundation has some card to place

        Stack<Card*> fnd = foundations[foundationNumber - 1];

        if (fnd.isEmpty()) {
            cout << "You can not pick card from an empty foundation. " << endl;
            return;
        }

        --tableauNumber;
        Card* tab = tableaus[tableauNumber].getTail();

        // Ensuring only King is placed as tableau starter

        if (!tab && fnd.getHead()->getRank() != 13) {
            cout << "You can only start a tableau with a King card." << endl;
            return;
        }

        // Ensuring only King is placed as tableau starter

        if (!tab && fnd.getHead()->getRank() == 13) {
            tableaus[tableauNumber].insertTail(fnd.pop());
            moves++;
            return;
        }

        // Ensuring difference of one in ranks

        if (tab->getRank() - fnd.getHead()->getRank() != 1) {
            cout << "You can arrange cards only in proper descending order in tableaus." << endl;
            return;
        }

        // Ensuring opposite suits

        if ((foundationNumber == 1 || foundationNumber == 3) && (tab->getSuit() != 2 && tab->getSuit() != 4) ||
            (foundationNumber == 2 || foundationNumber == 4) && (tab->getSuit() != 1 && tab->getSuit() != 3)) {
            cout << "You can not place cards of the same suit together. " << endl;
            return;
        }

        // Finally moving card from foundation to tableau

        tableaus[tableauNumber].insertTail(fnd.pop());
        moves++;
    }

    void moveFromWasteToFoundation(int foundationNumber) {
        if (foundationNumber < 1 || foundationNumber > 4) {
            cout << "You can only place card in a valid foundation." << endl;
            return;
        }
        
        // Ensuring wastePile is not empty
        
        if (wastePile.isEmpty()) {
            cout << "You can not pick card from an empty waste pile. " << endl;
            return;
        }
        
        // Ensuring cards have same suits
        
        Card* wasteHead = wastePile.getHead();

        if (wasteHead->getSuit() == 1 && foundationNumber != 1 ||
            wasteHead->getSuit() == 2 && foundationNumber != 2 ||
            wasteHead->getSuit() == 3 && foundationNumber != 3 ||
            wasteHead->getSuit() == 4 && foundationNumber != 4) {
            cout << "You can only place cards with same suit as foundation. " << endl;
            return;
        }

        // Ensuring first card to be Ace
        
        --foundationNumber;

        if (foundations[foundationNumber].isEmpty() && wasteHead->getRank() != 1) {
            cout << "You can only start a foundation with Ace. " << endl;
            return;
        }

        if (foundations[foundationNumber].isEmpty() && wasteHead->getRank() == 1) {
            Card* temp = wastePile.pop();
            foundations[foundationNumber].push(temp);
            return;
        }

        // Ensuring ascending order
        
        if (wasteHead->getRank() - foundations[foundationNumber].getHead()->getRank() != 1) {
            cout << "You can place cards only in proper ascending order in foundations. " << endl;
            return;
        }
        
        // Eventually adding on top of existing foundation
        
        Card* temp = wastePile.pop();
        foundations[foundationNumber].push(temp);
        moves++;
    }

    void moveFromWasteToTableau(int tableauNumber) {
        // Ensuring wastepile has some card to place
        
        if (wastePile.isEmpty()) {
            cout << "You can not pick card from an empty waste pile. " << endl;
            return;
        }

        --tableauNumber;
        Card* tab = tableaus[tableauNumber].getTail();

        // Ensuring only King is placed as tableau starter

        if (!tab && wastePile.getHead()->getRank() != 13) {
            cout << "You can only start a tableau with a King card." << endl;
            return;
        }

        // Ensuring only King is placed as tableau starter

        if (!tab && wastePile.getHead()->getRank() == 13) {
            tableaus[tableauNumber].insertTail(wastePile.pop());
            return;
        }

        // Ensuring difference of one in ranks

        if (tab->getRank() - wastePile.getHead()->getRank() != 1) {
            cout << "You can arrange cards only in proper descending order in tableaus." << endl;
            return;
        }

        // Ensuring opposite suits

        if ((wastePile.getHead()->getSuit() == 1 || wastePile.getHead()->getSuit() == 3) && (tab->getSuit() != 2 && tab->getSuit() != 4) ||
            (wastePile.getHead()->getSuit() == 2 || wastePile.getHead()->getSuit() == 4) && (tab->getSuit() != 1 && tab->getSuit() != 3)) {
            cout << "You can not place cards of the same suit together. " << endl;
            return;
        }

        // Finally moving card from waste to tableau

        tableaus[tableauNumber].insertTail(wastePile.pop());
        moves++;
    }

    void drawCardFromStock() {
        // Ensuring circulation if stockpile is empty
        if (stockPile.isEmpty()) {
            while (!wastePile.isEmpty()) {
                Card* wasteHead = wastePile.pop();
                stockPile.push(wasteHead);
            }
        }

        // Pushing head of stockpile to wastepile
        Card* stockHead = stockPile.pop();
        wastePile.push(stockHead);
        moves++;
    }

    bool isGameWon() {
        return (stockPile.isEmpty() && wastePile.isEmpty() && foundations[0].getSize() == 13 && foundations[1].getSize() == 13 && foundations[2].getSize() == 13 && foundations[3].getSize() == 13);
    }

    void mainLoop() {
        while (!isGameWon()) {
            //Sleep(2000);
            printGameState();
            parseCommand(inputCommand());
        }
    }

};

int main() {
    SetConsoleOutputCP(CP_UTF8);  // Set the console to UTF-8
    Game game;
    game.mainLoop();

    return 0;
}