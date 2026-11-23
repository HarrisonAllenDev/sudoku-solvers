#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Function to check if it is safe to place num at mat[row][col]
bool isSafe(vector<vector<int>> &mat, int row, int col, int num, int size) {

    // Check if num exist in the row
    for (int x = 0; x <= (size - 1); x++)
        if (mat[row][x] == num)
            return false;

    // Check if num exist in the col
    for (int x = 0; x <= (size - 1); x++)
        if (mat[x][col] == num)
            return false;

    // Check if num exist in the 3x3 sub-matrix
    int startRow = row - (row % int(sqrt(size))), startCol = col - (col % int(sqrt(size)));

    for (int i = 0; i < int(sqrt(size)); i++)
        for (int j = 0; j < int(sqrt(size)); j++)
            if (mat[i + startRow][j + startCol] == num)
                return false;

    return true;
}

// Function to solve the Sudoku problem
bool solveSudokuRec(vector<vector<int>> &mat, int row, int col, int size) {
    int n = mat.size();

    // base case: Reached nth column of last row
    if (row == n - 1 && col == n)
        return true;

    // If last column of the row go to next row
    if (col == n) {
        row++;
        col = 0;
    }

    // If cell is already occupied then move forward
    if (mat[row][col] != 0)
        return solveSudokuRec(mat, row, col + 1, size);

    for (int num = 1; num <= n; num++) {

        // If it is safe to place num at current position
        if (isSafe(mat, row, col, num, size)) {
            mat[row][col] = num;
            if (solveSudokuRec(mat, row, col + 1, size))
                return true;
            mat[row][col] = 0;
        }
    }
  
  	return false;
}

void solveSudoku(vector<vector<int>> &mat, int size) {
  	solveSudokuRec(mat, 0, 0, size);
}

int main() {
    
    vector<vector<int>> mat = {
        {0, 0, 0, 0, 3, 0, 0, 19, 0, 0, 0, 0, 0, 13, 0, 24, 20, 2, 22, 0, 0, 0, 8, 6, 0},
	{0, 15, 0, 0, 0, 5, 4, 0, 0, 8, 0, 6, 19, 0, 0, 23, 9, 0, 0, 0, 24, 14, 0, 25, 17},
	{22, 0, 0, 20, 0, 9, 2, 18, 15, 3, 0, 11, 23, 0, 10, 21, 0, 0, 17, 14, 0, 13, 7, 0, 0},
	{8, 0, 6, 4, 9, 0, 0, 0, 24, 0, 0, 0, 3, 15, 0, 0, 0, 11, 18, 25, 23, 0, 21, 10, 0},
	{17, 24, 14, 5, 7, 11, 0, 0, 0, 22, 12, 0, 0, 21, 4, 0, 0, 10, 0, 13, 9, 2, 3, 20, 16},
	{0, 0, 0, 0, 0, 0, 0, 0, 9, 25, 0, 0, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 13, 0},
	{0, 0, 21, 7, 19, 18, 0, 0, 0, 6, 11, 10, 24, 25, 9, 8, 17, 0, 0, 16, 20, 12, 0, 15, 0},
	{0, 0, 0, 0, 23, 0, 14, 0, 0, 13, 0, 1, 6, 2, 17, 0, 22, 0, 0, 19, 0, 11, 5, 0, 0},
	{0, 9, 0, 14, 0, 0, 3, 0, 16, 19, 0, 0, 15, 0, 12, 4, 6, 0, 2, 11, 0, 10, 24, 8, 23},
	{0, 16, 0, 0, 2, 24, 11, 17, 22, 12, 0, 0, 20, 14, 7, 0, 3, 25, 21, 0, 0, 0, 1, 0, 6},
	{0, 0, 23, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 16, 0, 7, 18, 0, 0, 0, 0, 0},
	{0, 0, 0, 2, 8, 0, 20, 0, 25, 0, 0, 22, 13, 0, 11, 0, 4, 9, 0, 12, 21, 0, 0, 3, 18},
	{0, 0, 18, 0, 22, 0, 15, 0, 14, 23, 0, 25, 16, 0, 0, 2, 0, 0, 11, 1, 0, 7, 0, 17, 0},
	{19, 10, 11, 0, 24, 21, 9, 0, 0, 18, 0, 0, 0, 1, 0, 0, 23, 0, 0, 0, 4, 25, 13, 22, 20},
	{0, 0, 0, 0, 5, 0, 24, 0, 3, 0, 0, 17, 14, 0, 6, 0, 19, 0, 0, 0, 15, 0, 2, 1, 0},
	{4, 0, 19, 0, 0, 23, 0, 0, 1, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 21, 0, 0, 0},
	{21, 17, 0, 0, 18, 2, 0, 14, 12, 0, 6, 24, 10, 4, 1, 0, 11, 0, 0, 7, 13, 15, 20, 0, 0},
	{12, 22, 7, 15, 0, 0, 8, 0, 20, 10, 0, 19, 25, 3, 0, 0, 2, 16, 23, 21, 0, 24, 0, 0, 1},
	{10, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 14, 0, 0, 0, 12, 0, 0, 20, 6, 5, 3, 0, 0, 9},
	{5, 2, 0, 6, 0, 0, 0, 0, 0, 0, 7, 0, 0, 9, 20, 1, 14, 0, 0, 15, 22, 0, 10, 23, 0},
	{0, 0, 22, 17, 10, 0, 12, 5, 0, 0, 0, 0, 9, 20, 25, 0, 0, 4, 14, 24, 6, 0, 16, 19, 21},
	{0, 3, 0, 0, 0, 0, 17, 7, 0, 0, 0, 0, 4, 10, 0, 0, 21, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 8, 0, 16, 25, 0, 0, 20, 13, 7, 0, 11, 19, 18, 0, 0, 3, 0, 0, 4, 23, 24, 0},
	{6, 23, 2, 1, 0, 0, 0, 15, 19, 0, 0, 0, 22, 17, 0, 0, 10, 13, 25, 0, 0, 5, 18, 12, 0},
	{0, 0, 13, 25, 0, 0, 23, 0, 0, 0, 14, 0, 0, 6, 0, 16, 0, 0, 19, 20, 0, 0, 15, 0, 0}
        };

    
    int size = mat.size();
    cout << "Solving size " << size << " sudoku grid..." << endl;

    auto start = high_resolution_clock::now();
	solveSudoku(mat, size);
    auto stop = high_resolution_clock::now();

    
  	for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat.size(); j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }

    auto duration = duration_cast<microseconds>(stop - start);
    cout << endl << "Solve Time: " << duration.count() << " microseconds." << endl;

    return 0;
}