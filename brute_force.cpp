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
        {0, 0, 6, 9, 0, 0, 16, 0, 0, 11, 2, 0, 0, 0, 0, 0},
	{4, 15, 3, 0, 12, 11, 0, 9, 0, 6, 0, 7, 16, 1, 0, 0},
	{11, 0, 12, 0, 0, 8, 0, 0, 9, 0, 15, 3, 0, 4, 6, 0},
	{0, 14, 5, 0, 13, 2, 3, 6, 8, 0, 0, 0, 9, 11, 15, 12},
	{7, 13, 0, 0, 0, 3, 15, 5, 11, 0, 8, 0, 0, 0, 2, 4},
	{0, 12, 9, 0, 1, 14, 4, 10, 0, 0, 7, 16, 0, 0, 13, 11},
	{3, 0, 10, 5, 0, 0, 0, 0, 4, 14, 0, 0, 8, 16, 0, 1},
	{0, 0, 4, 0, 16, 7, 0, 8, 0, 0, 0, 9, 3, 0, 10, 0},
	{14, 0, 8, 10, 3, 4, 0, 2, 16, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 7, 13, 0, 0, 0, 0, 10, 0, 12, 0, 0, 2, 0, 0},
	{2, 0, 11, 0, 10, 0, 1, 0, 6, 9, 13, 8, 0, 15, 0, 14},
	{5, 3, 16, 0, 0, 0, 6, 0, 0, 7, 1, 4, 13, 8, 0, 0},
	{16, 6, 2, 4, 9, 0, 0, 13, 0, 0, 0, 11, 0, 0, 1, 0},
	{12, 0, 1, 3, 15, 0, 0, 0, 14, 0, 10, 0, 0, 13, 0, 0},
	{10, 5, 0, 0, 0, 0, 0, 0, 1, 12, 9, 0, 2, 7, 0, 0},
	{9, 0, 0, 14, 0, 1, 0, 3, 0, 0, 16, 6, 0, 10, 11, 0}
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