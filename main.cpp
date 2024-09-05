#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct PuzzleState {
    vector<vector<int>> board;
    pair<int, int> zeroPos;  // Position of the empty space
    vector<string> path;     // Stores the path of moves

    // Compare states based on the board configuration
    bool operator<(const PuzzleState& other) const {
        return board < other.board;
    }
};

// Goal state for a 3x3 puzzle
const vector<vector<int>> goalState = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// Check if the board is the goal state
bool isGoalState(const vector<vector<int>>& board) {
    return board == goalState;
}

// Generate possible moves from the current state
vector<PuzzleState> generateMoves(const PuzzleState& current) {
    vector<PuzzleState> moves;
    int x = current.zeroPos.first;
    int y = current.zeroPos.second;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // Up, Down, Left, Right
    vector<string> moveNames = {"Up", "Down", "Left", "Right"};

    for (int i = 0; i < directions.size(); ++i) {
        int newX = x + directions[i].first;
        int newY = y + directions[i].second;
        if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
            PuzzleState newState = current;
            swap(newState.board[x][y], newState.board[newX][newY]);
            newState.zeroPos = {newX, newY};
            newState.path.push_back(moveNames[i]);  // Record the move
            moves.push_back(newState);
        }
    }
    return moves;
}

// BFS algorithm to solve the puzzle
void solvePuzzle(const vector<vector<int>>& initialBoard) {
    queue<PuzzleState> q;
    set<vector<vector<int>>> visited;

    PuzzleState initialState;
    initialState.board = initialBoard;

    // Find the initial position of zero
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (initialBoard[i][j] == 0) {
                initialState.zeroPos = {i, j};
                break;
            }
        }
    }

    q.push(initialState);
    visited.insert(initialState.board);

    while (!q.empty()) {
        PuzzleState current = q.front();
        q.pop();

        if (isGoalState(current.board)) {
            cout << "Solved in " << current.path.size() << " moves." << endl;
            for (const string& move : current.path) {
                cout << move << " ";
            }
            cout << endl;
            return;
        }

        vector<PuzzleState> moves = generateMoves(current);
        for (const auto& move : moves) {
            //If the .find() function does not encounter the state in the set, it will return visited.end()
            //So this if is saying. If you dont encounter the state in the set, push the moves to the queue
            //and insert the new move to the visited vector
            if (visited.find(move.board) == visited.end()) {
                q.push(move);
                visited.insert(move.board);
            }
        }
    }

    cout << "No solution found." << endl;
}

int main() {
    vector<vector<int>> initialBoard = {
        {5,8,3}, {1,0,4}, {7,6,2}
    };
    auto start = high_resolution_clock::now();

    solvePuzzle(initialBoard);

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Resolved in: " << duration.count()/1000000.00 << " seconds. " << endl;
    return 0;
}
