#include <iostream>

struct DieFace {
    bool hasValue;
    double value;

    DieFace() : hasValue(false) {}

    DieFace(const double value) : value(value), hasValue(true) {}

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
// Each of the die's faces are represented by the direction they are facing at the current square.
// visited and vis are redundant but a double check to make sure the solution is correct.
bool dfs(int y, int x, int prevScore, int n, DieFace up, DieFace bottom, DieFace north, DieFace south, DieFace east, DieFace west, const int grid[6][6], int visited[6][6], bool vis[6][6]) {
    if (x < 0 || y < 0 || x >= 6 || y >= 6) return false;

    // The score always needs to be the value of the square
    int score = grid[y][x];

    // This is the value "N times the value of the die facing up after the move" by which we increase the score by
    int increase = score - prevScore;

    // If we are currently in the blue corner, then we've found the correct journey
    if (y == 5 && x == 5) {
        visited[5][5] = true;

        // Calculate the total of the unvisited squares (first check)
        int total = 0;
        for (size_t i = 0; i < 6; i++) {
            for (size_t j = 0; j < 6; j++) {
               std::cout << visited[i][j] << ", \t";
                if (!visited[i][j]) total+= grid[i][j];
            }
            std::cout << " \n";
        }
        std::cout << total << "\n";

        // Save this current square as part of the correct path (second check)
        vis[y][x] = true;
        return true;
    }

    // If the face facing up already has a value assigned, then it has the be equal to the increase when multiplied by n.
    // Otherwise assign that value.
    if (up.hasValue) {
        if (up.value * n != increase) return false;
    }
    else {
        up.value = double(increase) / n;
        up.hasValue = true;
    }
    
    // Increment and decrement the current square in the visited grid. Not a boolean grid as you could technically return from the same square more than once in a path (first check).
    visited[y][x]++;
    if (
        // Explore all orthogonal neighbors, adjusting each of the faces of the square.
        dfs(y + 1, x, score, n + 1, south, north, up, bottom, east, west, grid, visited, vis) ||
        dfs(y - 1, x, score, n + 1, north, south, bottom, up, east, west, grid, visited, vis) ||
        dfs(y, x + 1, score, n + 1, west, east, north, south, up, bottom, grid, visited, vis) ||
        dfs(y, x - 1, score, n + 1, east, west, north, south, bottom, up, grid, visited, vis)) {

        // If one of the dfs was fruitful, it means that this square is part of die's journey (second check).
        vis[y][x] = true;
        return true;
    }
    visited[y][x]--;
    return false;
    

}

int main()
{
    // Initialize the two arrays that keep track of which squares we've visited
    bool vis[6][6];
    int visited[6][6];
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 6; j++) {
            vis[i][j] = false;
            visited[i][j] = 0;
        }
    }

    const int grid[6][6] = {
        {0, 77, 32, 403, 337, 452},
        {5, 23, -4, 592, 445, 620},
        {-7, 2, 357, 452, 317, 395},
        {186, 42, 195, 704, 452, 228},
        {81, 123, 240, 443, 353, 508},
        {57, 33, 132, 268, 492, 732}
    };

    dfs(1, 0, 0, 1, DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), grid, visited, vis);
    dfs(0, 1, 0, 1, DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), DieFace(), grid, visited, vis);

    // Calculate the total of the unvisited squares (second check)
    int total = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (!vis[i][j]) total += grid[i][j];
        }
    }
    std::cout << total << "\n";
}
