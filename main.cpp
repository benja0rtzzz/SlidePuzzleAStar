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
    int g = 0, h; //G(h) and the heuristic

    // Compare states based on the board configuration
    bool operator<(const PuzzleState& other) const {
        return (g + h) > (other.g + other.h);
    }
};

// Goal state for a 3x3 puzzle
vector<vector<int>> getWinning(int n) {
    vector<vector<int>> actualMatrix(n, vector<int>(n));
    int value = 1;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n;j++) {
            actualMatrix[i][j] = value;
            value++;
        }
    }
    actualMatrix[n-1][n-1] = 0;

    return actualMatrix;
}

// Check if the board is the goal state
bool isGoalState(const vector<vector<int>>& board, const vector<vector<int>>& goalState) {
    int boardSize = board.size();
    for(int i = 0; i < boardSize;i++) {
        for(int j = 0; j < boardSize;j++) {
            if(board[i][j] != goalState[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool is_solvable(const vector<vector<int>>& board) {
    vector<int> flat;
    int n = board.size();
    int empty_row = 0;  // Row where the 0 (empty space) is located

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] == 0) {
                empty_row = i;
            } else {
                flat.push_back(board[i][j]);
            }
        }
    }

    // Count the inversions
    int inversions = 0;
    for (int i = 0; i < flat.size(); ++i) {
        for (int j = i + 1; j < flat.size(); ++j) {
            if (flat[i] > flat[j]) {
                inversions++;
            }
        }
    }

    // In a 4x4 board, if the number of inversions is even and the empty space is in an odd row from the bottom, it's solvable
    if (n % 2 == 0) {
        return (inversions % 2 == 0) == ((n - empty_row) % 2 == 1);
    }
    return inversions % 2 == 0;
}


//Heuristic Function
int getHeuristic(vector<vector<int>> board) {
    int heuristic = 0;
    int boardSize = board.size();
    for(int i = 0; i < boardSize;i++) {
        for(int j = 0; j < boardSize;j++) {
            int value = board[i][j];
            //Using Manhattan distance
            if (value == 0) continue;  // Skip the empty space
            int goalX = (value-1) / boardSize;
            int goalY = (value-1) % boardSize;
            heuristic += abs(i-goalX) + abs(j-goalY);
        }
    }
    return heuristic;
}

// Generate possible moves from the current state
vector<PuzzleState> generateMoves(const PuzzleState current) {
    vector<PuzzleState> moves;
    int x = current.zeroPos.first;
    int y = current.zeroPos.second;
    int currentSize = current.board.size();
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // Up, Down, Left, Right
    vector<string> moveNames = {"Up", "Down", "Left", "Right"};

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i].first;
        int newY = y + directions[i].second;
        if (newX >= 0 && newX < currentSize && newY >= 0 && newY < currentSize) {
            PuzzleState newState = current;
            swap(newState.board[x][y], newState.board[newX][newY]);
            newState.zeroPos = {newX, newY};
            newState.path.push_back(moveNames[i]);  // Record the move
            newState.g++;
            newState.h = getHeuristic(newState.board);
            moves.push_back(newState);
        }
    }
    return moves;
}



// BFS algorithm to solve the puzzle
void solvePuzzle(const vector<vector<int>>& initialBoard, const vector<vector<int>>& winningBoard) {
    if(!is_solvable(initialBoard)) return;
    priority_queue<PuzzleState> q; //Min ordered
    set<vector<vector<int>>> visited;

    PuzzleState initialState;
    initialState.board = initialBoard;
    int sizeOfBoard = initialBoard.size();
    // Find the initial position of zero
    for (int i = 0; i < sizeOfBoard; ++i) {
        for (int j = 0; j < sizeOfBoard; ++j) {
            if (initialBoard[i][j] == 0) {
                initialState.zeroPos = {i, j};
                break;
            }
        }
    }

    q.push(initialState);
    visited.insert(initialState.board);

    while (!q.empty()) {
        PuzzleState current = q.top();
        q.pop();

        if (current.board == winningBoard) {
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

    int n;
    cin >> n;
    vector<vector<int>> winningCombination = getWinning(n);

    vector<vector<int>> initialCombination(n, vector<int>(n));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n;j++) {
            cin >> initialCombination[i][j];
        }
    }
    auto start = high_resolution_clock::now();

    /*
     * test case: 3 2 3 6 5 4 8 1 7 0 ctrl+c ctrl+v
     */
    solvePuzzle(initialCombination, winningCombination);

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Resolved in: " << duration.count()/1000000.00 << " seconds. " << endl;
    return 0;
}
