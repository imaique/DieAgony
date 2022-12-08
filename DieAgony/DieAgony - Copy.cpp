// DieAgony.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>

struct DieFace {
    bool hasValue;
    double value;

    DieFace() {
        hasValue = false;
    }

    DieFace(const double value) : value(value), hasValue(true) {
    }

    DieFace(const DieFace& face) {
        hasValue = face.hasValue;
        if (hasValue) value = face.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const DieFace& d) {
        if (d.hasValue) out << d.value;
        else out << "No value";
        return out;
    }
};

struct Position {
    enum Move {
        UP = 0,
        LEFT,
        RIGHT,
        DOWN
    };

    DieFace up;
    DieFace bottom;
    DieFace north;
    DieFace east;
    DieFace west;
    DieFace south;

    int x;
    int y;
    int score;
    int n;

    friend std::ostream& operator<<(std::ostream& out, const Position& p) {
        out << "up: " << p.up << "\n";
        out << "bottom: " << p.bottom << "\n";
        out << "north: " << p.north << "\n";
        out << "east: " << p.east << "\n";
        out << "west: " << p.west << "\n";
        out << "south: " << p.south << "\n";

        out << "score: " << p.score << "\n";
        out << "n: " << p.n << "\n";

        out << "x, y: " << p.x << ", " << p.y << "\n";
        return out;
    }

    Position(DieFace up, DieFace bottom, DieFace north, DieFace east, DieFace west, DieFace south, int y, int x, int score, int n) : 
        up(up), bottom(bottom), north(north), east(east), west(west), south(south), y(y), x(x), score(score), n(n) { }
};

void push_if_not_nullptr(std::queue<Position*>& queue, Position* pos) {
    if (pos != nullptr) {
        queue.push(pos);
    }
}

bool test_up(DieFace* up, int y, int x, int n, const int grid[6][6], const Position& origin) {
    if (x < 0 || y < 0 || x >= 6 || y >= 6) return false;
    if (up->hasValue) {
        double dscore = origin.score + up->value * n;
        if (double(grid[y][x]) != dscore) return false;
    }
    else {
        double facetest = (grid[y][x] - origin.score) / double(n);
        if (floor(facetest) == facetest) *up = DieFace(int(facetest));
        else return false;
    }
    return true;
}

Position* getNewPosition(const Position& origin, Position::Move move, const int grid[6][6]) {

    int x = origin.x, y = origin.y, n = origin.n + 1;
    DieFace up, bottom, north, east, west, south;

    switch (move) {
    case Position::Move::DOWN:
        y--;

        up = DieFace(origin.north);
        if (!test_up(&up, y, x, n, grid, origin)) return nullptr;

        bottom = DieFace(origin.south);
        north = DieFace(origin.bottom);
        south = DieFace(origin.up);

        west = DieFace(origin.west);
        east = DieFace(origin.east);
        break;
    case Position::Move::UP:
        y++;

        up = DieFace(origin.south);
        if (!test_up(&up, y, x, n, grid, origin)) return nullptr;

        bottom = DieFace(origin.north);
        north = DieFace(origin.up);
        south = DieFace(origin.bottom);

        west = DieFace(origin.west);
        east = DieFace(origin.east);
        break;
    case Position::Move::RIGHT:
        x++;

        up = DieFace(origin.west);
        if (!test_up(&up, y, x, n, grid, origin)) return nullptr;

        bottom = DieFace(origin.east);
        west = DieFace(origin.bottom);
        east = DieFace(origin.up);

        north = DieFace(origin.north);
        south = DieFace(origin.south);

        break;
    case Position::Move::LEFT:
        x--;

        up = DieFace(origin.east);
        if (!test_up(&up, y, x, n, grid, origin)) return nullptr;

        bottom = DieFace(origin.east);
        west = DieFace(origin.up);
        east = DieFace(origin.bottom);

        north = DieFace(origin.north);
        south = DieFace(origin.south);
        break;
    }
    int score = grid[y][x];
    Position* newPosition = new Position(up, bottom, north, east, west, south, y, x, score, n);
    std::cout << *newPosition << "\n";
    return newPosition;
}


int main()
{
    const int grid[6][6] = {
        {0, 77, 32, 403, 337, 452},
        {5, 23, -4, 592, 445, 620},
        {-7, 2, 357, 452, 317, 395},
        {186, 42, 195, 704, 452, 228},
        {81, 123, 240, 443, 353, 508},
        {57, 33, 132, 268, 492, 732}
    };

    std::queue<Position*> queue;
    Position* startPosistion = new Position(DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), 0, 0, 0, 0);
    queue.push(startPosistion);

    while (!queue.empty()) {
        const Position& origin = *queue.front();
        if (origin.x == 5 && origin.y == 5) {
            std::cout << origin;
            break;
        }

        queue.pop();
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::DOWN, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::UP, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::LEFT, grid));
        push_if_not_nullptr(queue, getNewPosition(origin, Position::Move::RIGHT, grid));
    }
}
