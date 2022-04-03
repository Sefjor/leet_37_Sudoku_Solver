#include <vector>
#include <stdint.h>
#include <iostream>

using namespace std;

enum FLAGS_SHIFT {
    //from 0 to 8 there are digits 1-9
    is_preset = 9,
    is_guess = 10,
    is_solved = 11,
};
typedef vector<vector <uint16_t>> SOLUTION;
typedef vector<vector <char>> BOARD;

class Solution {
public:
    void PrepareSolution(const BOARD &board, SOLUTION &solution)
    {
        for (unsigned int i = 0; i < board.size(); ++i)
            for (unsigned int j = 0; j < board.size(); ++j) {
                if (board[i][j] == '.') {
                    solution[i][j] = 0x01FF; // we place 1-9 here
                } else {
                    solution[i][j] = board[i][j] - '0';
                    WriteBit(solution[i][j], is_preset, true); //write preset flag
                    WriteBit(solution[i][j], is_solved, true); //write preset flag
                }
            }
    }

    void solveSudoku(vector<vector<char>>& board)
    {
        PrepareSolution(board, solution);

        for (unsigned int i = 0; i < board.size(); ++i)
            for (unsigned int j = 0; j < board.size(); ++j)
                RemoveInvalidDigits(solution, i, j);

        DisplaySolution(solution);
    }
private:

    SOLUTION solution {SIZE, {SIZE, 0}};

    bool ReadBit (uint16_t value, int shift)
    {
        return  (value >> shift) & 0x1;
    }
    void WriteBit (uint16_t &target, int shift, bool value)
    {
        if(value)
            target |= 0x1 << shift;
        else
            target &= ~(1UL << shift);
    }
    void DisplaySolution (const SOLUTION &solution)
    {
        for (unsigned int i = 0; i < solution.size(); ++i)
            for (unsigned int j = 0; j < solution.size(); ++j) {

                if(j == 0)
                    cout << endl;
                if (ReadBit(solution[i][j], is_preset)) {
                    cout << "    " << (solution[i][j] & 0x00F) << "     ";
                } else {
                    for (int k = 1; k < 10; k++) {
                        bool  is_digit = (solution[i][j] >> (k - 1)) & 0x1;
                        cout << (is_digit ? k : ' ');
                    }
                    cout  << ' ';
                }
            }
    void RemoveInvalidDigits(SOLUTION &solution, int i, int j)
    {
        if(!ReadBit(solution[i][j], is_solved))
            return;
        int value = solution[i][j] & 0x00F;
        int shift = value - 1;

        //column
        for (int k = 0; k < SIZE; ++k) {
            if(!ReadBit(solution[k][j], is_solved))
                WriteBit(solution[k][j], shift, false);
        }
        //row
        for (int k = 0; k < SIZE; ++k) {
            if(!ReadBit(solution[i][k], is_solved))
                WriteBit(solution[i][k], shift, false);
        }

        //3x3 square
        const int sz = 3;

        int i_rounded = i - i % 3;
         int j_rounded = j - j % 3;

        for (int k = 0; k < sz; ++k)
            for (int l = 0; l < sz; ++l) {
            if(!ReadBit(solution[k+i_rounded][l + j_rounded], is_solved))
                WriteBit(solution[k+i_rounded][l + j_rounded], shift, false);
        }
    }
};

int main()
{
    vector<vector<char>> board = {{'5','3','.','.','7','.','.','.','.'},{'6','.','.','1','9','5','.','.','.'},{'.','9','8','.','.','.','.','6','.'},{'8','.','.','.','6','.','.','.','3'},{'4','.','.','8','.','3','.','.','1'},{'7','.','.','.','2','.','.','.','6'},{'.','6','.','.','.','.','2','8','.'},{'.','.','.','4','1','9','.','.','5'},{'.','.','.','.','8','.','.','7','9'}};
    Solution sol;
    sol.solveSudoku(board);
}
