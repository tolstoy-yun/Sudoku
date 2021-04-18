#pragma once
// Copyright 2021 ÷Ï‘œ°¢”‡∫∆”Ó
// Author: ÷Ï‘œ°¢”‡∫∆”Ó

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H
#include<string>

const int N = 9;
long get_file_size(const char* str_file_name);

class SudokuGame {
public:
	char model[N][N];  // Store the Sudoku model.
	char sudoku_endgame[2 * N * N + 1];  // Stores the generated endgame.
	char first_row[10] = { '$' };  // The first row, the first full row.
	int c = 0;  // The number of endpoints generated.
	int offset[N] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };  // Offset value
	bool sign_O = false;
	bool sign_D = false;
	bool OnlyOneA = true;
	int flag = 0;

public:
	// Write sudoku to a file.
	void write_game(std::string file_name, char* game);
	// Read Sudoku from the document.
	char** read_game(std::string file_name, long int n);
	// Generate the first template.
	void first_model();
	// Update the template.
	void refresh_model();
	/*
	*  Transformation is made on the basis of the original model and 36 permutations are saved.
	*  For option 1, the end disk is saved to the folder;
	*  for option 2, the end disk is randomly holed out
	*/
	void conversion_model(int option, int p = 0, int q = 0);
	// Save the first, middle and last three lines of Sudoku in a large array.
	void save_array(char a[], char b[], char c[]);
	// To generate the final.
	void generate_endgame(int num);
	// Random holing, the number of random holing is p~q.
	bool dig_hole(int p, int q);
	// Generate a Sudoku game.
	void generate_game(int num, int p, int q);
	// Deep search to solve Sudoku.
	int solve_dfs(int n, char **Array);
	// solve sudoku
	void solute_sudoku(const char* file_path);
	// Filter unique solutions.
	int select_only(int n, char** Array);
	// Generate a unique solution to a Sudoku.
	void generate_only_game(int num);
	// Determine whether the value satisfies the condition when filling in position n.
	bool check(int n, char key, char **Array);
};
#endif // !SUDOKU_GAME_H
