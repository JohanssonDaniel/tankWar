/*
 * En klass som representerar beteendet som vår tank ska ha under spelets gång
 */

#include "Board.h"
#include "Tank.h"

class danjo732_piehe154: public Tank {

public:
    danjo732_piehe154();
    ~danjo732_piehe154() = default;

    action doYourThing(const sensors&) override;
    string taunt(const string&) const override;

    /**
     * @brief predictiveFire        Jämför fiendens nuvarande position med dess förra och använder det för att lista ut vart den kommer
     *                              att vara nästa omgång och skjuter där
     * @return                      action.aim
     */
    action predictiveFire(const sensors&);

    /**
     * @brief evasion               Jämför spelarens position och fiendens föra tt avgöra vilken riktning som ökar avståndet mest
     * @return                      action.theMove
     */
    action evasion(const sensors&);

    /**
     * @brief uppdaterar spelarens positionList genom att lägga till motsåndarens nuvarande position
     */
    void analyzeEnemyPosition(const sensors&);

    /**
     * @brief enemyDistance     räknar ut motsåndarens avstånd med hjälp av pythagoras sats
     * @param col               numeriskt värde för motsåndarens nuvarande kolumn
     * @param row               numeriskt värde för motsåndarens nuvarande rad
     * @return                  numeriskt värde för motsåndarens avstånd
     *
     * Användning: double tempDistace = enemyDistance(int col, int row);
     */
    double enemyDistance(int col, int row, const sensors&) const;

    /**
     * @brief inBounds          Jämför spelarens position med BOARD_ROWS samt BOARD_COLS för att avgöra om spelaren är på väg att åka
     *                          utanför banan.
     * @param col               numeriskt värde för motsåndarens nuvarande kolumn
     * @param row               numeriskt värde för motsåndarens nuvarande rad
     * @return                  bools värde
     */
    bool inBounds(int row, int col) const;

private:
    /**
     * @brief positionList list med fiendens positioner som indexseras med hjälp av omgångens nummer
     */
    map<int, location> positionList;

};
