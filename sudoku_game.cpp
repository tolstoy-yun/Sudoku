// Copyright 2021 Zhu Yun & Yu Haoyu
// Author: Zhu Yun & Yu Haoyu

#include "../sudoku_game/include/sudoku_game.h"
#include<string.h>
#include<iostream>
#include <fstream>
#include<string>
#include <algorithm>
#include <ctime>
using namespace std; // NOLINT

long get_file_size(const char* str_file_name) { // NOLINT
    FILE * fp = fopen(str_file_name, "r");
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp); // NOLINT
    fclose(fp);
    return size;
}

// Write sudoku to a file.
void SudokuGame::write_game(string file_name, char* game) {
    fstream f;
    f.open(file_name, ios::out | ios::app);
    for (unsigned int i = 0; i < strlen(game); i++) {
        f << game[i];
    }
    f << endl;
    f.close();
}

// Read Sudoku from the document.
char** SudokuGame::read_game(string file_name, long int n) { // NOLINT
    char **sudoku_array = new char*[9];
    for (int i = 0; i < 9; i++) {
        sudoku_array[i] = new char[9];
        memset(sudoku_array[i], '$', sizeof(char)*(9));
    }
    fstream f;
    f.open(file_name);
    long int pos = 173 * n; // NOLINT
    f.seekg(pos, ios::cur);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 18; j++) {
            if (j % 2 == 0) {
                f >> sudoku_array[i][j / 2];
            }
        }
    }
    return sudoku_array;
}

// Generate the first template.
void SudokuGame::first_model() {
    char number[N] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    // Randomly determine the initial permutation.
    int ran = 0 + rand() % 362880; // NOLINT
    for (int i = 0; i < ran; i++) {
        next_permutation(number, number + N);  // The whole arrangement.
    }
    for (int i = 0; i < N; i++) {
        first_row[i] = number[i];
    }
    first_row[N] = '\0';
    // Generate a Sudoku template.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int k = (j + offset[i]) % N;  // Calculate offset coordinates.
            model[i][j] = first_row[k];
        }
    }
}

// Update the template.
void SudokuGame::refresh_model() {
    next_permutation(first_row, first_row + N);
    // Generate a Sudoku template.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int k = (j + offset[i]) % N;  // Calculate offset coordinates.
            model[i][j] = first_row[k];
        }
    }
}

/*
*  Transformation is made on the basis of the original model and 36 permutations are saved.
*  For option 1, the end disk is saved to the folder;
*  for option 2, the end disk is randomly holed out
*/
void SudokuGame::conversion_model(int option, int p, int q) {
    int A[3] = { 4, 5, 6 };  // 4, 5 and 6 are in a group
    int B[3] = { 7, 8, 9 };  // 7, 8 and 9 are in a group
    // The first three rows don't change.
    char first_three_row[6 * N];  // Save rows 1, 2 and 3.
    int first_index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < N - 1; j++) {
            first_three_row[first_index++] = (model[i][j]);
            first_three_row[first_index++] = ' ';
        }
        first_three_row[first_index++] = (model[i][N - 1]);
        first_three_row[first_index++] = '\n';  // A newline.
    }
    for (int a = 0; a < 6; a++) {
        // Determine permutation in Group A rows 4, 5 and 6.
        char mid_three_row[6 * N];  // Save rows 4, 5 and 6.
        int mid_index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < N - 1; j++) {
                // Each number generated is saved directly.
                mid_three_row[mid_index++] = (model[A[i] - 1][j]);
                mid_three_row[mid_index++] = ' ';
            }
            mid_three_row[mid_index++] = (model[A[i] - 1][N - 1]);
            mid_three_row[mid_index++] = '\n';  // A newline.
        }
        // There can be 6 different permutations of the same A.
        for (int b = 0; b < 6; b++) {
            // Determine a permutation of Group B rows 7, 8 and 9.
            char last_three_row[6 * N];
            int last_index = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < N - 1; j++) {
                    // Each number generated is saved directly.
                    last_three_row[last_index++] = (model[B[i] - 1][j]);
                    last_three_row[last_index++] = ' ';
                }
                last_three_row[last_index++] = (model[B[i] - 1][N - 1]);
                last_three_row[last_index++] = '\n';  // A newline.
            }
            // Our change in residual quantity is minus 1.
            c--;
            // Save it in an array.
            save_array(first_three_row, mid_three_row, last_three_row);
            // Save it to a file.
            if (option == 1) {
                write_game("endgame.txt", sudoku_endgame);
            }
            // Random hollowed out.
            if (option == 2) {
                if (dig_hole(p, q) == false) {
                    cout << "Parameter setting error!" << endl;
                    exit(0);
                }
                write_game("game.txt", sudoku_endgame);
            }
            // The only solution.
            if (option == 3) {
                if (dig_hole(p, q) == false) {
                    cout << "Parameter setting error!" << endl;
                    exit(0);
                }
                char **only_sudoku = new char*[9];
                for (int i = 0; i < 9; i++) {
                    only_sudoku[i] = new char[9];
                    memset(only_sudoku[i], '$', sizeof(char)*(9));
                }
                int k = 0;
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        only_sudoku[i][j] = sudoku_endgame[k];
                        k += 2;
                    }
                }
                char **select_only_sudoku = new char*[9];
                for (int i = 0; i < 9; i++) {
                    select_only_sudoku[i] = new char[9];
                    memset(select_only_sudoku[i], '$', sizeof(char)*(9));
                }
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        select_only_sudoku[i][j] = only_sudoku[i][j];
                    }
                }
                sign_O = false;
                OnlyOneA = true;
                flag = 0;
                select_only(0, select_only_sudoku);
                // If it's a unique solution.
                if (OnlyOneA == true) {
                    char result[2 * N * N + 1];
                    int h = 0;
                    for (int i = 0; i < 9; i++) {
                        for (int j = 0; j < 8; j++) {
                            result[h++] = only_sudoku[i][j];
                            result[h++] = ' ';
                        }
                        result[h++] = only_sudoku[i][N - 1];
                        result[h++] = '\n';
                    }
                    result[2 * N * N] = '\0';
                    write_game("game.txt", result);
                } else {
                    c++;
                }
            }
            if (c == 0) return;  // Zero ends the generation.
            // The next permutation of B.
            next_permutation(B, B + 3);
        }
        // The next permutation of A.
        next_permutation(A, A + 3);
    }
}

// Save the first, middle and last three lines of Sudoku in a large array.
void SudokuGame::save_array(char a[], char b[], char c[]) {
    int k = 0;
    for (int i = 0; i < 6 * N; i++) {
        sudoku_endgame[k++] = a[i];
    }
    for (int i = 0; i < 6 * N; i++) {
        sudoku_endgame[k++] = b[i];
    }
    for (int i = 0; i < 6 * N; i++) {
        sudoku_endgame[k++] = c[i];
    }
    sudoku_endgame[2 * N * N] = '\0';
}

// To generate the final.
void SudokuGame::generate_endgame(int num) {
    c = num;
    first_model();
    while (c > 0) {
        conversion_model(1);
        refresh_model();
    }
}

// Random holing, the number of random holing is p~q.
bool SudokuGame::dig_hole(int p, int q) {
    // If the range is 0, no holes will be dug, and return.
    if (p == 0 && q == 0) {
        return false;
    }
    if (p > q) {
        return false;
    }
    if (p > 80 || q > 80) {
        return false;
    }
    // Generates the number within the range of p~q.
    int hole_num = (rand() % (q - p + 1)) + p; // NOLINT
    // Generates the location of a random hole dug.
    int *ran_pos = new int[hole_num];
    for (int i = 0; i < hole_num; i++) {
        ran_pos[i] = rand() % 161; // NOLINT
        // Generate again if the location of the generated random hole
        // is equal to an odd number.
        if (ran_pos[i] % 2 != 0) {
            i--;
        }
        // If a repeated random number is generated, it is regenerated.
        for (int j = 0; j < i; j++) {
            if (ran_pos[j] == ran_pos[i]) {
                i--;
            }
        }
    }
    // Sort the generated random hole position.
    sort(ran_pos, ran_pos + hole_num);
    // The hole is dug according to the location of
    // the generated random hole.
    int j = 0;
    for (int i = 0; i < sizeof(sudoku_endgame); i++) {
        if (j == hole_num) break;
        if (i == ran_pos[j]) {
            sudoku_endgame[i] = '$';
            j++;
        }
    }
    delete[] ran_pos;
    return true;
}

// Generate a Sudoku game.
void SudokuGame::generate_game(int num, int p, int q) {
    c = num;
    first_model();
    while (c > 0) {
        conversion_model(2, p, q);
        refresh_model();
    }
}

// Deep search to solve Sudoku.
int SudokuGame::solve_dfs(int n, char **Array) {
    // When sudoku is filled, the recursion exits.
    if (n > 80) {
        sign_D = true;
        return 0;
    }
    // Skip if the current bit is not $.
    if (Array[n / 9][n % 9] != '$') {
        solve_dfs(n + 1, Array);
    } else {
        for (int i = 1; i <= 9; i++) {
            // Fill in the numbers if the conditions are met.
            if (check(n, (i + '0'), Array) == true) {
                Array[n / 9][n % 9] = i + '0';
                // Continue to search.
                solve_dfs(n + 1, Array);
                // If the construction succeeds on return, exit directly.
                if (sign_D == true) return 0;
                // If the construction is unsuccessful,
                // restore the current bit.
                Array[n / 9][n % 9] = '$';
            }
        }
    }
    return 0;
}

// solve sudoku
void SudokuGame::solute_sudoku(const char* file_path) {
    int sudoku_num = get_file_size(file_path) / 173;
    char **sudoku_array = new char*[9];
    for (int i = 0; i < 9; i++) {
        sudoku_array[i] = new char[9];
        memset(sudoku_array[i], '$', sizeof(char)*(9));
    }
    // Read each Sudoku, solve it and write it into a file.
    for (int i = 0; i < sudoku_num; i++) {
        sudoku_array = read_game(file_path, i);
        sign_D = false;
        solve_dfs(0, sudoku_array);
        char result[2 * N * N + 1];
        int k = 0;
        for (int h = 0; h < 9; h++) {
            for (int j = 0; j < 8; j++) {
                result[k++] = sudoku_array[h][j];
                result[k++] = ' ';
            }
            result[k++] = sudoku_array[h][N - 1];
            result[k++] = '\n';
        }
        result[2 * N * N] = '\0';
        write_game("sudoku.txt", result);
    }
}

// Filter unique solutions.
int SudokuGame::select_only(int n, char** Array) {
    if (flag > 1) {
        OnlyOneA = false;
        return 0;
    }
    // When sudoku is filled, the recursion exits.
    if (n > 80) {
        sign_O = true;
        flag += 1;
        return 0;
    }
    // Skip if the current bit is not $.
    if (Array[n / 9][n % 9] != '$') {
        select_only(n + 1, Array);
    } else {
        for (int i = 1; i <= 9; i++) {
            // Fill in the numbers if the conditions are met.
            if (check(n, (i + '0'), Array) == true) {
                Array[n / 9][n % 9] = i + '0';
                // Continue to search.
                select_only(n + 1, Array);
                // f the construction is unsuccessful,
                // restore the current bit.
                Array[n / 9][n % 9] = '$';
            }
        }
        // If the construction succeeds on return, exit directly.
        if (sign_O == true) return 0;
    }
    return 0;
}

// Generate a unique solution to a Sudoku.
void SudokuGame::generate_only_game(int num) {
    c = num;
    first_model();
    while (c > 0) {
        conversion_model(3, 10, 65);
        refresh_model();
    }
}

// Determine whether the value satisfies the condition
// when filling in position n.
bool SudokuGame::check(int n, char key, char **Array) {
    // Determine if n is a valid column.
    for (int i = 0; i < 9; i++) {
        // j is the vertical coordinate of n
        int j = n / 9;
        if (Array[j][i] == key) return false;
    }
    // Determine if n is a valid column.
    for (int i = 0; i < 9; i++) {
        // j is the x-coordinate of n
        int j = n % 9;
        if (Array[i][j] == key) return false;
    }
    // X is the vertical coordinate of the left vertex of
    // the small nine grid where n is located.
    int x = n / 9 / 3 * 3;
    // Y is the abscissa of the left vertex of
    // the small grid where n is located.
    int y = n % 9 / 3 * 3;
    // Judge whether the small nine grid in which N is legal.
    for (int i = x; i < x + 3; i++) {
        for (int j = y; j < y + 3; j++) {
            if (Array[i][j] == key) return false;
        }
    }
    // All valid, return correct.
    return true;
}


int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));
    int c_num, n_num, m_num;
    char*  s_path = NULL;
    SudokuGame sudoku_game;
    int i = 0;
    if (argc == 1) {
        return 0;
    }
    for (i = 1; i < argc; i++) {
        // The number of sudoku endpoints required.
        if (strcmp(argv[i], "-c") == 0) {
            c_num = atoi(argv[++i]);
            if (c_num <= 0 || c_num > 1000000) {
                cout << "Parameter setting error!" << endl;
                return 0;
            }
            sudoku_game.generate_endgame(c_num);
        } else if (strcmp(argv[i], "-s") == 0) {
            s_path = argv[++i];
            sudoku_game.solute_sudoku(s_path);
        } else if (strcmp(argv[i], "-n") == 0) {
            n_num = atoi(argv[++i]);
            if (n_num <= 0 || n_num > 10000) {
                cout << "Parameter setting error!" << endl;
                return 0;
            }
            if (argc == 3) {
                sudoku_game.generate_game(n_num, 10, 65);
                return 0;
            }
            // Generate difficulty of the game.
            if (strcmp(argv[++i], "-m") == 0) {
                if (argc != 5) {
                    cout << "Parameter setting error!" << endl;
                    return 0;
                }
                m_num = atoi(argv[++i]);
                if (m_num == 1) {
                    sudoku_game.generate_game(n_num, 10, 30);
                } else if (m_num == 2) {
                    sudoku_game.generate_game(n_num, 31, 50);
                } else if (m_num == 3) {
                    sudoku_game.generate_game(n_num, 51, 65);
                } else {
                    cout << "Level setting error!" << endl;
                    return 0;
                }
                return 0;
            } else if (strcmp(argv[i], "-r") == 0) {
                if (argc != 5) {
                    cout << "Parameter setting error!" << endl;
                    return 0;
                }
                // Converts the range string to two numbers.
                int range1 = 0, range2 = 0;
                string ran_str1 = "";
                string ran_str2 = "";
                unsigned int j = 0;
                i++;
                for (; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == '~') {
                        break;
                    }
                }
                for (unsigned int k = 0; k < j; k++) {
                    ran_str1.push_back(argv[i][k]);
                }
                range1 = atoi(ran_str1.c_str());
                // There's only one number, no ~.
                if (j != strlen(argv[i])) {
                    for (unsigned int k = j + 1; k < strlen(argv[i]); k++) {
                        ran_str2.push_back(argv[i][k]);
                    }
                    range2 = atoi(ran_str2.c_str());
                } else {
                    range2 = range1;
                }
                if (range1 <= 0 || range2 <= 0) {
                    cout << "Parameter setting error!" << endl;
                    return 0;
                }
                sudoku_game.generate_game(n_num, range1, range2);
                return 0;
            } else if (strcmp(argv[i], "-u") == 0) {
                if (argc != 4) {
                    cout << "Parameter setting error!" << endl;
                    return 0;
                }
                sudoku_game.generate_only_game(n_num);
                return 0;
            } else {
                cout << "Parameter setting error!" << endl;
                return 0;
            }
        } else {
            cout << "There is no matching parameter." << endl;
            return 0;
        }
    }
    return 0;
}
