/*
 * Implements the behavior in player.h
 */

#include "player.h"
#include <vector>
#include <math.h>

player::player() {
    name = "Turd Furgason";
}

action player::doYourThing (const sensors &s) {
    analyzeEnemyPosition(s);
    if(s.turn >1 && !(s.myAmmo < 0)){
        return predictiveFire(s);

    }
    else {
        return evasion(s);
    }
    //return fireAtOpp(s);  // right now calls base class fireAtOpp method -
                            // feel free to override that method
                            // with one that fires at where you think they will be
}

string player::taunt(const string &otherguy) const{
    return "You couldn't code your way out of a wet paper bag, " + otherguy;
}

action player::predictiveFire(const sensors &s){
    action move;
    location guessNextEnemyPosition;
    int differenceColumn = 0;
    int differenceRow = 0;
    location lastEnemyPosition = positionList.at(s.turn-1);

    cout << "lastenemy: " << lastEnemyPosition.c << " : " << lastEnemyPosition.r << endl;
    cout << "currentEnemy: " << s.opp.c << " : " << s.opp.r << endl;


    if(lastEnemyPosition != s.opp){
        differenceColumn =  s.opp.c - lastEnemyPosition.c;
        differenceRow = s.opp.r -lastEnemyPosition.r;

        //difference = s.opp - lastEnemyPosition;
        //guessNextEnemyPosition = s.opp + difference;
        guessNextEnemyPosition.c = s.opp.c + differenceColumn;
        guessNextEnemyPosition.r = s.opp.r + differenceRow;
    }
    else{
        guessNextEnemyPosition = s.opp;
    }
    cout << "guess: " << guessNextEnemyPosition.c << " r:" << guessNextEnemyPosition.r << endl;
    move.aim = guessNextEnemyPosition;
    move.theMove = fire;
    return move;
}

void player::analyzeEnemyPosition(const sensors &s){
    positionList.insert(make_pair(s.turn,s.opp));
}

double player::enemyDistance(int tempCol, int tempRow, const sensors &s){

    double distanceRow = abs(tempRow - s.opp.r);
    double distanceCol = abs(tempCol - s.opp.c);

    return sqrt(pow(distanceRow,2.0) + pow(distanceCol, 2.0));
}


bool player::inBounds(int col, int row) {
   return row > 0 && col > 0 && row < BOARD_ROWS && col < BOARD_COLS;
}


action player::evasion(const sensors &s){
    action move;
    int x = s.me.c;
    int y = s.me.r;
    double closestDistance = 0.0;
    int closestIndex;
    vector<int> xValues     {x - 1,   x + 1,    x,      x - 1,  x + 1,  x,      x - 1,  x + 1};
    vector<int> yValues     {y - 1,   y - 1,    y - 1,  y + 1,  y + 1,  y + 1,  y,      y};
    vector<moves> directions{moveNW, moveNE,   moveN,  moveSW, moveSE, moveS,  moveW,  moveE};
    for (int i = 0; i < 8; i++) {
        if (inBounds(xValues[i], yValues[i])) {
            cout << "Index: " << i << " : position: " << xValues[i] << " y : " << yValues[i] << endl;
            cout << "Inbounds" << endl;
            double tempEnemyDistance = enemyDistance(xValues[i], yValues[i], s);
            if (tempEnemyDistance > closestDistance || closestDistance == 0.0) {
                cout << "Changed distance" << endl;
                closestDistance = tempEnemyDistance;
                closestIndex = i;
            }
        }
    }
    cout << "Direction: " << directions[closestIndex] << endl;
    move.theMove = directions[closestIndex];
    return move;


}
