#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

const int SIDE_OF_FIELD = 10;
const char FIELD_TRANSITION[] = {'_', '^', '>', 'v', '<', '*'};

/**
 * \brief Класс, реализующий функционал поля и кораблей для игры Sea Battle.
 */
class Player {
public:

    //! Вспомогательный класс, который используется для работы с координатами.
    class Coords {
    public:
        int x, y;

        Coords();

        Coords(int x, int y);

        Coords(const Coords& other) = default;

        Coords(Coords&& other) noexcept;

        Coords& operator=(Coords copy);

        ~Coords() = default;
    private:
        void swap(Coords& other);
    };

    //! Вспомогательный класс, который используется для работы с кораблями.
    class Ship {
    public:
        int angle;
        Coords coords{};
        bool* damaged{};

        Ship();

        explicit Ship(int size, int angle = 0);

        Ship(const Ship& other);

        Ship(Ship&& other)  noexcept;

        Ship& operator=(Ship copy);

        ~Ship();

        //! Длина корабля.
        [[nodiscard]] int length() const;

        //! Установка координат корабля.
        void setCoords(const Coords& ship_coord);

        //! Установка угла корабля.
        void setAngle(int ship_angle);
    private:
        int size;

        void swap(Ship& other);
    };

    int ships_size;

    explicit Player(bool fill_in_the_field = true);

    ~Player();

    //! Возможные движения выбранного корабля.
    int* possibleMovements(int index);

    //! Метод, передвигающий выбранный корабль.
    void moveShip(int index, int movement);

    //! функция, реализующая движение корабля.
    int* movements(int index, int movement);

    //! Функция выстрела по полю игрока.
    int* shoot(int x, int y);

    //! Функция, возвращающая поле игрока.
    int** getField();

    //! Функция, возвращающая корабли игрока.
    Ship* getShips();

    //! Функция, проверяющая точку поля на нахождение в ней корабля.
    [[nodiscard]] int isShip(int x, int y) const;

    //! Метод, добавляющий корабль на поле игрока.
    void addShip(int ship_size, Coords ship_coords, int ship_angle);

    //! Метод, отрисовывающий поле игрока.
    [[maybe_unused]] void drawField();
private:
    Ship* ships;
    int** field = nullptr;

    //! Функция, проверяющая возможность установки на поле корабля с заданными характеристиками.
    bool possible(int index, Coords ship_coords, int ship_angle);

    //! Метод, стирающий корабль с поля игрока.
    void eraseShip(int index);
    
    //! Метод, устанавливающий корабль на поле игрока.
    void placeShip(int index, Coords ship_coords, int ship_angle);
};
