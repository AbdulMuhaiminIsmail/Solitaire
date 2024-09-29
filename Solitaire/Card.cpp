#pragma once
#include <iostream>
#include <windows.h>
#include "Enums.hpp"

using namespace std;


class Card {
private:
    bool hidden;
    Ranks rank;
    Suits suit;

public:
    Card(Suits suit, Ranks rank, bool hidden) : suit(suit), rank(rank), hidden(hidden) {}

    char getRank() const {
        return rank;
    }

    void setRank(Ranks r) {
        rank = r;
    }

    Suits getSuit() const {
        return suit;
    }

    void setSuit(Suits s) {
        suit = s;
    }

    bool isHidden() const {
        return hidden;
    }

    void flipHiddenStatus() {
        hidden = !hidden; 
    }

    void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    void displayRank(Ranks rank) {
        switch(rank) {
            case Ace:
                cout << 'A';    // Ace
                break;
            case Two:
                cout << '2';    // 2
                break;
            case Three:
                cout << '3';    // 3
                break;
            case Four:
                cout << '4';    // 4
                break;
            case Five:
                cout << '5';    // 5
                break;
            case Six:
                cout << '6';    // 6
                break;
            case Seven:
                cout << '7';    // 7
                break;
            case Eight:
                cout << '8';    // 8
                break;
            case Nine:
                cout << '9';    // 9
                break;
            case Ten:
                cout << "10";   // 10
                break;
            case Jack:
                cout << 'J';    // Jack
                break;
            case Queen:
                cout << 'Q';    // Queen
                break;
            case King:
                cout << 'K';    // King
                break;
            default:
                cout << "?";     // If something goes wrong
                break;
        }
    }

    void displaySuit(Suits suit) {
        switch(suit) {
            case Heart:
                setColor(12); // Set color to red
                cout << u8"\u2665"; // ♥ (heart symbol)
                setColor(7); // Reset to white
                break;
            
            case Diamond:
                setColor(12); // Set color to red
                cout << u8"\u2666"; // ♦ (diamond symbol)
                setColor(7); // Reset to white
                break;
            
            case Spade:
                cout << u8"\u2660"; // ♠ (spade symbol)
                break;
            
            case Club:
                cout << u8"\u2663"; // ♣ (club symbol)
                break;
            
            default:
                cout << "?"; // If an unknown suit is provided
                break;
        }
    }


    void display() {
        if (hidden) {
            cout << "[ ]";
            return; 
        }

        displayRank(rank);
        displaySuit(suit);
    }
};

