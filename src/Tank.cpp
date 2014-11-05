/*
 * Implements the behavior in Tank.h
 */

#include "Tank.h"

Tank::Tank() {
    name = "SET YOUR NAME IN YOUR CONSTRUCTOR";
}

void Tank::setPlayer(int playNum) {
    pnum = playNum;
}

string Tank::getName() const{
    return name;
}

int Tank::getPlayer() const{
    return pnum;
}

/*
 * Tank::goToOppBase(sensors s)
 * This method returns the action required to proceed to the enemy base.
 * It uses the sensor information to determine the current poisiton, calls the board class
 * static method getLine, and returns the move necessary to proceed along the line.
 * If it is already in the enemy base, it will sit.
 */
action Tank::goToOppBase(const sensors &s) {
    action move;
    list<location> myPath;
    myPath = Board::getLine(s.me, s.oppBase);
    list<location>::iterator i;

    if (s.me != s.oppBase) {
        i = myPath.begin();
        if ((i->c == s.me.c-1) && (i->r == s.me.r-1)) move.theMove = moveNW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r-1)) move.theMove = moveNE;
        if ((i->c == s.me.c) && (i->r == s.me.r-1)) move.theMove = moveN;
        if ((i->c == s.me.c-1) && (i->r == s.me.r+1)) move.theMove = moveSW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r+1)) move.theMove = moveSE;
        if ((i->c == s.me.c) && (i->r == s.me.r+1)) move.theMove = moveS;
        if ((i->c == s.me.c-1) && (i->r == s.me.r)) move.theMove = moveW;
        if ((i->c == s.me.c+1) && (i->r == s.me.r)) move.theMove = moveE;
    }
    else {
        move.theMove = sit;
    }
    return move;
}

/*
 * Tank::fireAtOpp(sensors s)
 * This method returns the action required to fire at the enemy.
 * PLEASE NOTE: This will only have the possibility of hitting the enemy if
 * (s)he 'sits' during his/her subsequent move
 */
action Tank::fireAtOpp(const sensors &s) {
    action move;
    move.aim = s.opp;
    move.theMove = fire;
    return move;
}

action Tank::predictiveFire(map<int, location>& enemyPositions, const sensors &s){
    action move;
    location guessNextEnemyPosition;
    int differenceColumn = 0;
    int differenceRow = 0;
    location lastEnemyPosition = enemyPositions.at(s.turn-1);

    if((abs(lastEnemyPosition.c - s.opp.c == 1) && (abs(lastEnemyPosition.r - s.opp.c) == 1))){
        differenceColumn = lastEnemyPosition.c - s.opp.c;
        differenceRow = lastEnemyPosition.r - s.opp.r;
        guessNextEnemyPosition.c = s.opp.c + differenceColumn;
        guessNextEnemyPosition.r = s.opp.r + differenceRow;
    }
    else{
        guessNextEnemyPosition = s.opp;
    }
    move.aim = guessNextEnemyPosition;
    move.theMove = fire;
    return move;
}

void Tank::analyzeEnemyPosition(map<int, location>& positionList, const sensors &s){
    positionList.insert(make_pair(s.turn,s.opp));
}
