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

    cout << "lastenemy: " << lastEnemyPosition.c << " : " << lastEnemyPosition.r << endl;
    cout << "currentEnemy: " << s.opp.c << " : " << s.opp.r << endl;


    if(lastEnemyPosition != s.opp){
        differenceColumn =  s.opp.c- lastEnemyPosition.c;
        differenceRow = s.opp.r -lastEnemyPosition.r;

        //difference = s.opp - lastEnemyPosition;
        //guessNextEnemyPosition = s.opp + difference;
        guessNextEnemyPosition.c = s.opp.c + differenceColumn;
        guessNextEnemyPosition.r = s.opp.r + differenceRow;
        cout << "Testing: " << endl;
    }
    else{
        guessNextEnemyPosition = s.opp;
        cout << "Hej" << endl;
    }
    cout << "yolo c:" << guessNextEnemyPosition.c << " r:" << guessNextEnemyPosition.r << endl;
    move.aim = guessNextEnemyPosition;
    move.theMove = fire;
    return move;
}

void Tank::analyzeEnemyPosition(map<int, location>& positionList, const sensors &s){
    positionList.insert(make_pair(s.turn,s.opp));
}

int enemyDistance(const sensors &s){

}
