// DieAgony.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>

struct Position {
    enum Move {
        UP = 0,
        LEFT,
        RIGHT,
        DOWN
    };

    int* up;
    int* bottom;
    int* north;
    int* east;
    int* west;
    int* south;

    int x;
    int y;
    int score;
    int n;

    friend std::ostream& operator<<(std::ostream& out, const Position& p) {
        if(p.up) out << "up: " << *p.up << "\n";
        if (p.bottom) out << "bottom: " << *p.bottom << "\n";
        if (p.north) out << "north: " << *p.north << "\n";
        if (p.east) out << "east: " << *p.east << "\n";
        if (p.west) out << "west: " << *p.west << "\n";
        if (p.south) out << "south: " << *p.south << "\n";

        out << "score: " << p.score << "\n";
        out << "n: " << p.n << "\n";
        return out;
    }

    Position(int* up, int* bottom, int* north, int* east, int* west, int* south, int y, int x, int score, int n) : 
        up(up), bottom(bottom), north(north), east(east), west(west), south(south), y(y), x(x), score(score), n(n) { }
};

void push_if_not_nullptr(std::queue<Position*>& queue, Position* pos) {
    if (pos != nullptr) {
        std::cout << *pos << "\n";
        queue.push(pos);
    }
}

void copy_if_not_nullptr(int** a, int* b) {
    if (b == nullptr) *a = nullptr;
    else **a = *b;
}

bool test_up(int* up, int* score, int y, int x, int n, const int grid[3][3], const Position& origin) {
    if (x < 0 || y < 0 || x >= 3 || y >= 3) return false;
    if (up) {
        double dscore = origin.score + *up * n;
        if (double(grid[y][x]) != dscore) return false;
        *score = int(dscore);
    }
    else {
        double facetest = (grid[y][x] - origin.score) / double(n);
        if (floor(facetest) == facetest) up = new int(facetest);
        else return false;
        *score = grid[y][x];
    }
    return true;
}

Position* getNewPosition(const Position& origin, Position::Move move, const int grid[3][3]) {

    int x = origin.x, y = origin.y, n = origin.n + 1, score;
    int* up = nullptr, * bottom = nullptr, * north = nullptr, * east = nullptr, * west = nullptr, * south = nullptr;

    switch (move) {
    case Position::Move::DOWN:
        y--;

        copy_if_not_nullptr(&up, origin.north);
        if (!test_up(up, &score, y, x, n, grid, origin)) return nullptr;

        copy_if_not_nullptr(&bottom, origin.south);
        copy_if_not_nullptr(&north, origin.bottom);
        copy_if_not_nullptr(&south, origin.up);

        copy_if_not_nullptr(&west, origin.west);
        copy_if_not_nullptr(&east, origin.east);
        break;
    case Position::Move::UP:
        y++;


        if (!test_up(up, &score, y, x, n, grid, origin)) return nullptr;

        copy_if_not_nullptr(&bottom, origin.north);
        copy_if_not_nullptr(&north, origin.up);
        copy_if_not_nullptr(&south, origin.bottom);

        copy_if_not_nullptr(&west, origin.west);
        copy_if_not_nullptr(&east, origin.east);
        break;
    case Position::Move::RIGHT:
        x++;

        copy_if_not_nullptr(&up, origin.west);
        if (!test_up(up, &score, y, x, n, grid, origin)) return nullptr;

        copy_if_not_nullptr(&bottom, origin.east);
        copy_if_not_nullptr(&west, origin.bottom);
        copy_if_not_nullptr(&east, origin.up);

        copy_if_not_nullptr(&north, origin.north);
        copy_if_not_nullptr(&south, origin.south);

        break;
    case Position::Move::LEFT:
        x--;

        copy_if_not_nullptr(&up, origin.east);
        if (!test_up(up, &score, y, x, n, grid, origin)) return nullptr;

        copy_if_not_nullptr(&bottom, origin.east);
        copy_if_not_nullptr(&west, origin.bottom);
        copy_if_not_nullptr(&east, origin.up);

        copy_if_not_nullptr(&north, origin.north);
        copy_if_not_nullptr(&south, origin.south);
        break;
    }

    return new Position(up, bottom, north, east, west, south, y, x, score, n);
}


int main()
{
    const int grid[3][3] = {
        {3, 4, 5},
        {2, 3, 4},
        {1, 2, 3}
    };

    std::queue<Position*> queue;
    Position* startPosistion = new Position(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0);
    queue.push(startPosistion);

    while (!queue.empty()) {
        const Position& origin = *queue.front();
        if (origin.x == 2 && origin.y == 2) std::cout << origin;

        queue.pop();
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::DOWN, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::UP, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::LEFT, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::RIGHT, grid));
    }
}
