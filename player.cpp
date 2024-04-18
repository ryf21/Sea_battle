#include "player.h"

int *Player::possibleMovements(int index) {
    eraseShip(index);
    int movements_count = 0;
    for (int i = 0; i < 6; i++) {
        if (possible(index, Coords(movements(index, i)[0], movements(index, i)[1]), movements(index, i)[2])) {
            movements_count++;
        }
    }
    int* poss_movements = new int[movements_count + 1];
    poss_movements[0] = movements_count;
    int counter = 1;
    for (int i = 0; i < 6; i++) {
        if (possible(index, Coords(movements(index, i)[0], movements(index, i)[1]), movements(index, i)[2])) {
            poss_movements[counter] = i;
            counter++;
        }
    }
    placeShip(index, ships[index].coords, ships[index].angle);
    return poss_movements;
}

void Player::moveShip(int index, int movement) {
    eraseShip(index);
    placeShip(index, Coords(movements(index, movement)[0], movements(index, movement)[1]), movements(index, movement)[2]);
}

int *Player::movements(int index, int movement) {
    int angle = ships[index].angle;
    Coords coords(ships[index].coords.x, ships[index].coords.y);
    int longShipOffset = (ships[index].length() - 1) / 2;
    int newAngle = angle;
    Coords newCoords;
    if (movement == 0) {
        newCoords = Coords(coords.x + std::round(std::cos((2 - angle) * M_PI / 2)),
                           coords.y + std::round(std::sin((2 - angle) * M_PI / 2)));
    } else if (movement == 1) {
        int angleOffset = (angle + 3) % 4;
        newCoords = Coords(coords.x + std::round(std::cos((2 - angleOffset) * M_PI / 2)),
                           coords.y + std::round(std::sin((2 - angleOffset) * M_PI / 2)));
    } else if (movement == 2) {
        int angleOffset = (angle + 1) % 4;
        newCoords = Coords(coords.x + std::round(std::cos((2 - angleOffset) * M_PI / 2)),
                           coords.y + std::round(std::sin((2 - angleOffset) * M_PI / 2)));
    } else if (movement == 3) {
        newAngle = (angle + 3) % 4;
        int angleOffset = (angle + 1) % 4;
        newCoords = Coords(coords.x + longShipOffset * std::round(std::cos((2 - angleOffset) * M_PI / 2) + std::cos((2 - angle) * M_PI / 2)),
                           coords.y + longShipOffset * std::round(std::sin((2 - angleOffset) * M_PI / 2) + std::sin((2 - angle) * M_PI / 2)));
    } else if (movement == 4) {
        newAngle = (angle + 1) % 4;
        int angleOffset = (angle + 3) % 4;
        newCoords = Coords(coords.x + longShipOffset * std::round(std::cos((2 - angleOffset) * M_PI / 2) + std::cos((2 - angle) * M_PI / 2)),
                           coords.y + longShipOffset * std::round(std::sin((2 - angleOffset) * M_PI / 2) + std::sin((2 - angle) * M_PI / 2)));
    } else {
        newCoords = Coords(coords.x + 2 * longShipOffset * std::round(std::cos((2 - angle) * M_PI / 2)),
                           coords.y + 2 * longShipOffset * std::round(std::sin((2 - angle) * M_PI / 2)));
        newAngle = (angle + 2) % 4;
    }
    int* result = new int[3];
    result[0] = newCoords.x;
    result[1] = newCoords.y;
    result[2] = newAngle;
    return result;
}

int *Player::shoot(int x, int y) {
    int* result = new int[3];
    for (int index = 0; index < ships_size; index++) {
        for (int i = 0; i < ships[index].length(); i++) {
            int shipX = ships[index].coords.x + i * std::round(std::cos((2 - ships[index].angle) * M_PI / 2));
            int shipY = ships[index].coords.y + i * std::round(std::sin((2 - ships[index].angle) * M_PI / 2));
            if (x == shipX && y == shipY) {
                ships[index].damaged[i] = true;
                int destroyedShipSize = ships[index].length();
                placeShip(index, ships[index].coords, ships[index].angle);
                result[0] = index;
                result[1] = destroyedShipSize;
                result[2] = i;
                return result;
            }
        }
    }
    result[0] = 0;
    result[1] = 0;
    result[2] = 0;
    return result;
}

int **Player::getField() {
    return field;
}

Player::Ship *Player::getShips() {
    return ships;
}

int Player::isShip(int x, int y) const {
    if (x >= 0 && x < SIDE_OF_FIELD && y >= 0 && y < SIDE_OF_FIELD) {
        for (int index = 0; index < ships_size; index++) {
            for (int i = 0; i < ships[index].length(); i++) {
                int shipX = ships[index].coords.x + i * std::round(std::cos((2 - ships[index].angle) * M_PI / 2));
                int shipY = ships[index].coords.y + i * std::round(std::sin((2 - ships[index].angle) * M_PI / 2));
                if (x == shipX && y == shipY) {
                    return index;
                }
            }
        }
    }
    return -1;
}

bool Player::possible(int index, Player::Coords ship_coords, int ship_angle) {
    for (int i = 0; i < ships[index].length(); i++) {
        int x = ship_coords.x + i * std::round(std::cos((2 - ship_angle) * M_PI / 2));
        int y = ship_coords.y + i * std::round(std::sin((2 - ship_angle) * M_PI / 2));
        if (x < 0 || x >= SIDE_OF_FIELD || y < 0 || y >= SIDE_OF_FIELD || field[x][y] != -1) {
            return false;
        }
        for (int offset_x : {-1, 1}) {
            if (x + offset_x >= 0 && x + offset_x < SIDE_OF_FIELD && field[x + offset_x][y] != -1) {
                return false;
            }
        }
        for (int offset_y : {-1, 1}) {
            if (y + offset_y >= 0 && y + offset_y < SIDE_OF_FIELD && field[x][y + offset_y] != -1) {
                return false;
            }
        }
    }
    return true;
}

void Player::eraseShip(int index) {
    for (int i = 0; i < ships[index].length(); i++) {
        int x = ships[index].coords.x + i * std::round(std::cos((2 - ships[index].angle) * M_PI / 2));
        int y = ships[index].coords.y + i * std::round(std::sin((2 - ships[index].angle) * M_PI / 2));
        field[x][y] = -1;
    }
}

void Player::placeShip(int index, Player::Coords ship_coords, int ship_angle) {
    bool isDestroyed = true;
    for (int i = 0; i < ships[index].length(); i++) {
        if (!ships[index].damaged[i]) {
            isDestroyed = false;
            break;
        }
    }
    if (isDestroyed) {
        eraseShip(index);
        for (int i = index; i < ships_size - 1; i++) {
            *(ships + i) = *(ships + i + 1);
        }
//        delete &ships[ships_size];
        ships_size--;
        return;
    }
    for (int i = 0; i < ships[index].length(); i++) {
        int x = ship_coords.x + i * std::round(std::cos((2 - ship_angle) * M_PI / 2));
        int y = ship_coords.y + i * std::round(std::sin((2 - ship_angle) * M_PI / 2));
        if (ships[index].damaged[i]) {
            field[x][y] = 4;
        } else {
            field[x][y] = ship_angle;
        }
    }
    ships[index].setCoords(ship_coords);
    ships[index].setAngle(ship_angle);
}

void Player::drawField() {
    for (int i = 0; i < SIDE_OF_FIELD; i++) {
        for (int j = 0; j < SIDE_OF_FIELD; j++)
            std::cout << FIELD_TRANSITION[field[i][j] + 1] << ' ';
        std::cout << std::endl;
    }
}

void Player::addShip(int ship_size, Player::Coords ship_coords, int ship_angle) {
    Ship* new_ships = new Ship[ships_size + 1];
    for (int i = 0; i < ships_size; i++)
        *(i + new_ships) = *(i + ships);
    new_ships[ships_size] = Ship(ship_size);
    ships = new_ships;
    placeShip(ships_size, ship_coords, ship_angle);
    ships_size++;
}

Player::Player(bool fill_in_the_field) {
    field = new int*[SIDE_OF_FIELD];
    for (int i = 0; i < SIDE_OF_FIELD; i++) {
        field[i] = new int[SIDE_OF_FIELD];
        for (int j = 0; j < SIDE_OF_FIELD; j++)
            field[i][j] = -1;
    }
    ships_size = 0;
    if (!fill_in_the_field)
        return;
    ships_size = 10;
    ships = new Ship[ships_size];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coordDist(0, SIDE_OF_FIELD - 1);
    std::uniform_int_distribution<> angleDist(0, 3);
    int index = 0;
    for (int i = 4; i > 0; i--)
        for (int j = i; j < 5; j++) {
            ships[index] = Ship(i);
            Coords coords(coordDist(gen), coordDist(gen));
            int angle = angleDist(gen);
            while (!possible(index, coords, angle)) {
                coords = Coords(coordDist(gen), coordDist(gen));
                angle = angleDist(gen);
            }
            placeShip(index, coords, angle);
            index++;
        }
}

Player::~Player() {
    delete[] ships;
    delete[] field;
}

Player::Coords::Coords() : x(0), y(0) {}

Player::Coords::Coords(int x, int y) : x(x), y(y) {}

Player::Coords::Coords(Player::Coords &&other) noexcept: x(other.x), y(other.y) {
    other.x = 0;
    other.y = 0;
}

Player::Coords &Player::Coords::operator=(Player::Coords copy) {
    swap(copy);
    return *this;
}

void Player::Coords::swap(Player::Coords &other) {
    std::swap(x, other.x);
    std::swap(y, other.y);
}

Player::Ship::Ship(int size, int angle) : size(size), damaged(new bool[size]), angle(angle) {
    for (int i = 0; i < size; i++)
        damaged[i] = false;
}

Player::Ship::Ship(const Player::Ship &other) : angle(other.angle), coords(other.coords), damaged(new bool[other.size]), size(other.size) {
    for (int i = 0; i < size; i++)
        *(damaged + i) = *(other.damaged + i);
}

Player::Ship::Ship(Player::Ship &&other) noexcept: size(other.size), damaged(other.damaged), angle(other.angle) {
    other.damaged = new bool[size];
    for (int i = 0; i < size; i++)
        other.damaged[i] = false;
    other.angle = 0;
}

Player::Ship &Player::Ship::operator=(Player::Ship copy) {
    swap(copy);
    return *this;
}

Player::Ship::~Ship() {
    delete[] damaged;
}

int Player::Ship::length() const { return size; }

void Player::Ship::setCoords(const Player::Coords &ship_coord) { coords = ship_coord; }

void Player::Ship::setAngle(int ship_angle) { angle = ship_angle; }

void Player::Ship::swap(Player::Ship &other) {
    std::swap(coords, other.coords);
    std::swap(size, other.size);
    std::swap(damaged, other.damaged);
    std::swap(angle, other.angle);
}

Player::Ship::Ship() : size(0), angle(0) {}
