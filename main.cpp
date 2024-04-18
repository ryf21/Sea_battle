#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "player.h"

const int BLOCK_SIDE = 32;
const int PLAYER_FIELD_OFFSET[] = {3 * BLOCK_SIDE, 5 * BLOCK_SIDE};
const int ENEMY_FIELD_OFFSET[] = {16 * BLOCK_SIDE, 5 * BLOCK_SIDE};
const int ENEMY_SHIPS_INFO[] = {28 * BLOCK_SIDE, 5 * BLOCK_SIDE};
const int BUTTONS[] = {2 * BLOCK_SIDE, 16 * BLOCK_SIDE};
const int MOVEMENTS_TRANSLATION[] = {1, 3, 0, 5, 4, 2};
const int SHOT_MISS_DELAY = 1000;
const int FPS = 60;

void update_scene(sf::RenderWindow& window, Player player, Player enemy, sf::Sprite* ship_sprites,
                  const sf::Sprite& scene_sprite, sf::Sprite transparent_grey_sprite, sf::Sprite shot_sprite,
                  const bool* buttons = nullptr) {
    window.draw(scene_sprite);

    Player::Ship* player_ships = player.getShips();
    for (int i = 0; i < player.ships_size; i++) {
        int x = player_ships[i].coords.y + (player_ships[i].angle >= 2) +
                player_ships[i].length() * (player_ships[i].angle == 1) -
                player_ships[i].length() * (player_ships[i].angle == 3);
        int y = player_ships[i].coords.x + (player_ships[i].angle == 3) +
                player_ships[i].length() * (player_ships[i].angle == 2) -
                (player_ships[i].length() - 1) * (player_ships[i].angle == 0);
        sf::Sprite ship_sprite = ship_sprites[player_ships[i].length() - 1];
        ship_sprite.rotate(player_ships[i].angle * 90);
        ship_sprite.setPosition(PLAYER_FIELD_OFFSET[0] + x * BLOCK_SIDE, PLAYER_FIELD_OFFSET[1] + y * BLOCK_SIDE);
        window.draw(ship_sprite);
        for (int j = 0; j < player_ships[i].length(); j++)
            if (player_ships[i].damaged[j]) {
                x = player_ships[i].coords.y + j * (player_ships[i].angle % 2 != 0) * 2 * ((player_ships[i].angle == 1) - 0.5);
                y = player_ships[i].coords.x + j * (player_ships[i].angle % 2 == 0) * 2 * ((player_ships[i].angle == 2) - 0.5);
                shot_sprite.setPosition(PLAYER_FIELD_OFFSET[0] + x * BLOCK_SIDE, PLAYER_FIELD_OFFSET[1] + y * BLOCK_SIDE);
                window.draw(shot_sprite);
            }
    }

    Player::Ship* enemy_ships = enemy.getShips();
    for (int i = 0; i < enemy.ships_size; i++) {
        sf::Sprite ship_sprite = ship_sprites[enemy_ships[i].length() - 1];
        ship_sprite.rotate(90);
        ship_sprite.setPosition(ENEMY_SHIPS_INFO[0] + enemy_ships[i].length() * BLOCK_SIDE,
                                ENEMY_SHIPS_INFO[1] + i * BLOCK_SIDE);
        window.draw(ship_sprite);
        for (int j = 0; j < enemy_ships[i].length(); j++)
            if (enemy_ships[i].damaged[j]) {
                shot_sprite.setPosition(ENEMY_SHIPS_INFO[0] + j * BLOCK_SIDE, ENEMY_SHIPS_INFO[1] + i * BLOCK_SIDE);
                window.draw(shot_sprite);
            }
    }

    if (buttons != nullptr) {
        for (int i = 0; i < 6; i++)
            if (!buttons[i]) {
                transparent_grey_sprite.setPosition(BUTTONS[0] + 2 * i * BLOCK_SIDE, BUTTONS[1]);
                window.draw(transparent_grey_sprite);
            }
    } else
        for (int i = 0; i < 6; i++) {
            transparent_grey_sprite.setPosition(BUTTONS[0] + 2 * i * BLOCK_SIDE, BUTTONS[1]);
            window.draw(transparent_grey_sprite);
        }
}

int main() {
//    Окно
    sf::RenderWindow window(sf::VideoMode(1120, 640), "Sea Battle",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(FPS);

//    Загрузка текстур
    sf::Texture scene_texture, menu_texture, transparent_green_texture, transparent_grey_texture, shot_texture, miss_texture;

    if (!scene_texture.loadFromFile("textures/scene.png") ||
        !menu_texture.loadFromFile("textures/menu.png") ||
        !transparent_green_texture.loadFromFile("textures/transparent_green_x2.png") ||
        !transparent_grey_texture.loadFromFile("textures/transparent_grey_x4.png") ||
        !shot_texture.loadFromFile("textures/shot.png") ||
        !miss_texture.loadFromFile("textures/miss.png")) {
        std::cerr << "Failed to load textures" << std::endl;
        return 1;
    }

    sf::Texture *ship_textures = new sf::Texture[4];
    for (int i = 0; i < 4; i++) {
        sf::Texture texture;
        if (!texture.loadFromFile("textures/ship_" + std::to_string(i + 1) + ".png")) {
            std::cerr << "Failed to load ship texture" << std::endl;
            return 1;
        }
        ship_textures[i] = texture;
    }
//    Создание спрайтов
    sf::Sprite scene_sprite(scene_texture), menu_sprite(menu_texture),
    transparent_green_sprite(transparent_green_texture),
    transparent_grey_sprite(transparent_grey_texture), shot_sprite(shot_texture),
    miss_sprite(miss_texture);

    sf::Sprite* ship_sprites = new sf::Sprite[4];
    for (int i = 0; i < 4; i++) {
        ship_sprites[i].setTexture(ship_textures[i]);
    }
//    Игровой процесс
    Player player = Player();
    Player enemy = Player();

    int state_of_game = 5;

    sf::Event event;
    bool ship_is_selected;
    int index;
    bool* movements;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (state_of_game == 5) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = mousePos.x / BLOCK_SIDE;
                    int y = mousePos.y / BLOCK_SIDE;
                    if (x >= 9 && x < 25 && y >= 12 && y < 14) {
                        state_of_game = 0;
                        window.clear();
                        update_scene(window, player, enemy, ship_sprites, scene_sprite, transparent_grey_sprite, shot_sprite);
                        window.display();
                        ship_is_selected = false;
                        index = -1;
                        movements = nullptr;
                    } else if (x >= 9 && x < 25 && y >= 16 && y < 18) {
                        window.close();
                    }
                }
            } else if (state_of_game == 0 && event.type == sf::Event::MouseButtonPressed) {
                if (index != -1 && ship_is_selected) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = (mousePos.x - BUTTONS[0]) / (2 * BLOCK_SIDE);
                    if ((mousePos.y - BUTTONS[1]) / (2 * BLOCK_SIDE) == 0 && x >= 0 && x < 6) {
                        if (movements[x]) {
                            window.clear();
                            player.moveShip(index, MOVEMENTS_TRANSLATION[x]);
                            update_scene(window, player, enemy, ship_sprites, scene_sprite, transparent_grey_sprite,
                                         shot_sprite);
                            state_of_game = 1;
                            ship_is_selected = false;
                            window.display();
                        }
                    }
                }
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = (mousePos.x - PLAYER_FIELD_OFFSET[0]) / BLOCK_SIDE;
                int y = (mousePos.y - PLAYER_FIELD_OFFSET[1]) / BLOCK_SIDE;
                index = player.isShip(y, x);
                if (index != -1) {
                    movements = new bool[6];
                    int *possible_movements = player.possibleMovements(index);
                    for (int i = 0; i < 6; i++) {
                        movements[i] = false;
                        for (int j = 0; j < possible_movements[0]; j++)
                            if (MOVEMENTS_TRANSLATION[i] == possible_movements[j + 1])
                                movements[i] = true;
                    }
                    window.clear();
                    update_scene(window, player, enemy, ship_sprites, scene_sprite, transparent_grey_sprite,
                                 shot_sprite, movements);
                    Player::Ship selected_ship = player.getShips()[index];
                    for (int i = 0; i < selected_ship.length(); i++) {
                        int x = i * (selected_ship.angle % 2 != 0) * 2 * ((selected_ship.angle == 1) - 0.5);
                        int y = i * (selected_ship.angle % 2 == 0) * 2 * ((selected_ship.angle == 2) - 0.5);
                        transparent_green_sprite.setPosition(
                                PLAYER_FIELD_OFFSET[0] + (selected_ship.coords.y + x) * BLOCK_SIDE,
                                PLAYER_FIELD_OFFSET[1] + (selected_ship.coords.x + y) * BLOCK_SIDE);
                        window.draw(transparent_green_sprite);
                    }
                    window.display();
                    ship_is_selected = true;
                }
            } else if (state_of_game == 1) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = (mousePos.x - ENEMY_FIELD_OFFSET[0]) / BLOCK_SIDE;
                    int y = (mousePos.y - ENEMY_FIELD_OFFSET[1]) / BLOCK_SIDE;
                    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                        window.clear();
                        update_scene(window, player, enemy, ship_sprites, scene_sprite, transparent_grey_sprite, shot_sprite);
                        int* shoot = enemy.shoot(y, x);
                        bool is_hit = shoot[0] + shoot[1] + shoot[2];
                        if (is_hit) {
                            shot_sprite.setPosition(ENEMY_FIELD_OFFSET[0] + x * BLOCK_SIDE, ENEMY_FIELD_OFFSET[1] + y * BLOCK_SIDE);
                            window.draw(shot_sprite);
                        } else {
                            miss_sprite.setPosition(ENEMY_FIELD_OFFSET[0] + x * BLOCK_SIDE, ENEMY_FIELD_OFFSET[1] + y * BLOCK_SIDE);
                            window.draw(miss_sprite);
                        }
                        window.display();
                        sf::sleep(sf::milliseconds(SHOT_MISS_DELAY));
                        state_of_game = is_hit ? 0 : 2;
                    }
                }
            }
            else if (state_of_game == 2) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> indexDist(0, enemy.ships_size);
                int enemy_index = indexDist(gen);
                int* possible_movements = enemy.possibleMovements(enemy_index);
                std::uniform_int_distribution<> movementDist(1, possible_movements[0]);
                int movement = movementDist(gen);
                enemy.moveShip(enemy_index, possible_movements[movement]);
                std::uniform_int_distribution<> coordDist(0, 9);
                int x = coordDist(gen);
                int y = coordDist(gen);
                int* shoot = player.shoot(y, x);
                bool is_hit = shoot[0] + shoot[1] + shoot[2];
                window.clear();
                update_scene(window, player, enemy, ship_sprites, scene_sprite, transparent_grey_sprite, shot_sprite);
                if (!is_hit) {
                    miss_sprite.setPosition(PLAYER_FIELD_OFFSET[0] + x * BLOCK_SIDE, PLAYER_FIELD_OFFSET[1] + y * BLOCK_SIDE);
                    window.draw(miss_sprite);
                    sf::sleep(sf::milliseconds(SHOT_MISS_DELAY));
                }
                window.display();
                state_of_game = is_hit ? 2 : 0;
            }
        }
        if (state_of_game == 5) {
            window.draw(menu_sprite);
            window.display();
        }
    }
    return 0;
}