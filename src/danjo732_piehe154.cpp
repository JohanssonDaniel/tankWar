/*
 * Implements the behavior in danjo732_piehe154.h
 *
 */

#include "danjo732_piehe154.h"
#include <vector>
#include <math.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

danjo732_piehe154::danjo732_piehe154() {
    name = "danjo732_piehe154";
}

action danjo732_piehe154::doYourThing (const sensors &s) {
    analyzeEnemyPosition(s);

    if(s.turn > 2 && !(s.myAmmo < 0)){
        return predictiveFire(s);
    }
    else {
        if (s.me != s.oppBase) {
            return moveToOppBase(s);
        }
        else {
            action move;
            move.theMove = sit;
            return move;
        }
    }

}

string danjo732_piehe154::taunt(const string &otherguy) const{
    return "Hej svejs i lingonskogen " + otherguy;
}

/*
 * Förutsäger motsåndarens position genom att se hur den har rörtt sig från förra omgången
 * Förutsätter att motståndaren rör sig i samma riktning från förra omgången
 *
*/
action danjo732_piehe154::predictiveFire(const sensors &s){
    action move;
    location guessNextEnemyPosition;
    int differenceColumn = 0;
    int differenceRow = 0;
    location lastEnemyPosition = positionList.at(s.turn-1);
    //Om motsåndaren inte står på samma position som förra omgången
    if(lastEnemyPosition != s.opp){
        //Räkna ut differensen
        differenceColumn =  s.opp.c - lastEnemyPosition.c;
        differenceRow = s.opp.r -lastEnemyPosition.r;
        guessNextEnemyPosition.c = s.opp.c + differenceColumn;
        guessNextEnemyPosition.r = s.opp.r + differenceRow;
    }
    //Annars anta att han kommer stå stilla nästa omgång
    else{
        guessNextEnemyPosition = s.opp;
    }
    move.aim = guessNextEnemyPosition;
    move.theMove = fire;
    return move;
}

void danjo732_piehe154::analyzeEnemyPosition(const sensors &s){
    positionList.insert(make_pair(s.turn,s.opp));
}
/*
 * Räknar ut avståndet till fienden med pytagoras stats
 */
double danjo732_piehe154::enemyDistance(int tempCol, int tempRow, int targetCol, int targetRow) const{

    double distanceRow = abs(tempRow - targetRow);
    double distanceCol = abs(tempCol - targetCol);

    return sqrt(pow(distanceRow,2.0) + pow(distanceCol, 2.0));
}


bool danjo732_piehe154::inBounds(int col, int row) const{
   return row > 0 && col > 0 && row < BOARD_ROWS && col < BOARD_COLS;
}

/*
 * Jämför alla positioner runt om spelaren och ser vilken
 * riktning som är närmast/näst närmast (strafe) mot motståndarens bas
 */
action danjo732_piehe154::moveToOppBase(const sensors &s){
    action move;
    int x = s.me.c;
    int y = s.me.r;
    double shortestDistance = 0.0;
    int closestIndex;
    double nextShortestDistance = 0.0;
    int nextClosestIndex;

    //Håller värderna för rader och kolumner runt om spelaren, representerar samma riktning som ligger i vektorn under

    vector<int> xValues     {x - 1,   x + 1,    x,      x - 1,  x + 1,  x,      x - 1,  x + 1};
    vector<int> yValues     {y - 1,   y - 1,    y - 1,  y + 1,  y + 1,  y + 1,  y,      y};

    vector<moves> directions{moveNW,  moveNE,   moveN,  moveSW, moveSE, moveS,  moveW,  moveE};

    for (int i = 0; i < 8; i++) {
        if (inBounds(xValues[i], yValues[i])) {
            double tempEnemyDistance = enemyDistance(xValues[i], yValues[i], s.oppBase.c, s.oppBase.r);
            if (tempEnemyDistance < shortestDistance || shortestDistance == 0.0) {
                //Sparar undan det kortaste avståndet och dess index
                nextShortestDistance = shortestDistance;
                nextClosestIndex = closestIndex;
                shortestDistance = tempEnemyDistance;
                closestIndex = i;
            }
        }
    }

    int randMove;
    /* initialize random seed: */
    srand (time(NULL));
    /* generate secret number between 1 and 10: */
    randMove = rand() % 2 + 1;

    if (randMove == 1) {
        move.theMove = directions[nextClosestIndex];
    } else {
        move.theMove = directions[closestIndex];
    }
    return move;
    //Väljer ut rätt riktning med det sparade indexet
}

/*
 * Jämför alla positioner runt om spelaren och ser vilken riktning som ökar avståndet mest
 */
action danjo732_piehe154::evasion(const sensors &s){
    action move;
    int x = s.me.c;
    int y = s.me.r;
    double longestDistance = 0.0;
    int closestIndex;

    //Håller värderna för rader och kolumner runt om spelaren, representerar samma riktning som ligger i vektorn under

    vector<int> xValues     {x - 1,   x + 1,    x,      x - 1,  x + 1,  x,      x - 1,  x + 1};
    vector<int> yValues     {y - 1,   y - 1,    y - 1,  y + 1,  y + 1,  y + 1,  y,      y};

    vector<moves> directions{moveNW,  moveNE,   moveN,  moveSW, moveSE, moveS,  moveW,  moveE};

    for (int i = 0; i < 8; i++) {
        if (inBounds(xValues[i], yValues[i])) {
            double tempEnemyDistance = enemyDistance(xValues[i], yValues[i], s.opp.c, s.opp.r);
            if (tempEnemyDistance > longestDistance || longestDistance == 0.0) {
                //Sparar undan det längsta avståndet och dess index
                longestDistance = tempEnemyDistance;
                closestIndex = i;
            }
        }
    }
    //Väljer ut rätt riktning med det sparade indexet
    move.theMove = directions[closestIndex];
    return move;
}
