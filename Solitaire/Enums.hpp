#pragma once
#include <vector>

using namespace std;

enum Suits { Club, Diamond, Spade, Heart };

enum Ranks { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

extern vector<Suits> suits;
extern vector<Ranks> ranks;