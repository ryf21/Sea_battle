#include <cassert>
#include "player.h"

int main() {

//    Добавление корабля на поле
    Player player_0 = Player(false);
    player_0.addShip(3, Player::Coords(1, 1), 1);
    assert(player_0.getField()[1][1] == 1);
    assert(player_0.getField()[1][2] == 1);
    assert(player_0.getField()[1][3] == 1);

//    Поворот корабля
    Player player_1 = Player(false);
    player_1.addShip(4, Player::Coords(2, 2), 1);
    player_1.moveShip(0, 4);
    assert(player_1.getField()[1][3] == 2);
    assert(player_1.getField()[2][3] == 2);
    assert(player_1.getField()[3][3] == 2);
    assert(player_1.getField()[4][3] == 2);

//    Разворот корабля
    Player player_3 = Player(false);
    player_3.addShip(4, Player::Coords(2, 2), 1);
    player_3.moveShip(0, 5);
    assert(player_3.getField()[2][1] == 3);
    assert(player_3.getField()[2][2] == 3);
    assert(player_3.getField()[2][3] == 3);
    assert(player_3.getField()[2][4] == 3);

//    Движение корабля вперёд
    Player player_4 = Player(false);
    player_4.addShip(2, Player::Coords(3, 1), 0);
    player_4.moveShip(0, 0);
    assert(player_4.getField()[1][1] == 0);
    assert(player_4.getField()[2][1] == 0);

//    Движение корабля вбок
    Player player_5 = Player(false);
    player_5.addShip(3, Player::Coords(1, 2), 1);
    player_5.moveShip(0, 2);
    assert(player_5.getField()[2][2] == 1);
    assert(player_5.getField()[2][3] == 1);
    assert(player_5.getField()[2][4] == 1);

//    Выстрел по кораблю
    Player player_6 = Player(false);
    player_6.addShip(3, Player::Coords(1, 1), 1);
    player_6.shoot(1, 2);
    assert(player_6.getField()[1][1] == 1);
    assert(player_6.getField()[1][2] == 4);
    assert(player_6.getField()[1][3] == 1);

//    Уничтожение корабля
    Player player_7 = Player(false);
    player_7.addShip(2, Player::Coords(1, 2), 3);
    player_7.shoot(1, 1);
    player_7.shoot(1, 2);
    assert(player_7.getField()[1][1] == -1);
    assert(player_7.getField()[1][2] == -1);

//    Промах
    Player player_8 = Player(false);
    player_8.addShip(2, Player::Coords(1, 2), 3);
    int* shoot = player_8.shoot(0, 0);
    for (int i = 0; i < 3; i++)
        assert(shoot[i] == 0);
}
