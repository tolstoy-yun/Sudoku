#include<iostream>
#include <fstream>
#include<string>
#include<string.h>
#include <algorithm>
#include <ctime>
using namespace std;

const int N = 9;
long get_file_size(const char* str_file_name) {
	FILE * fp = fopen(str_file_name, "r");
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fclose(fp);
	cout << size << endl;
	return size;
}

class SudokuGame {
public:
	char model[N][N];//存放数独模型
	char sudoku_endgame[2 * N * N + 1];//存放生成的终局
	char first_row[10];//第一行，也是第一个全排列
	int c = 0;//生成终局的数量
	int offset[N] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };//偏移值
	bool sign_O = false;
	bool sign_D = false;
	bool OnlyOneA = true;
	int flag = 0;


	//生成第一个模板
	void first_model() {
		//数字
		char number[N] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		//随机确定最开始的排列
		int ran = 0 + rand() % 362880;
		for (int i = 0; i < ran; i++) {
			next_permutation(number, number + N);//全排列
		}
		for (int i = 0; i < N; i++) {
			first_row[i] = number[i];
		}
		first_row[N] = '\0';
		//生成数独模板
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int k = (j + offset[i]) % N;//计算偏移坐标
				model[i][j] = first_row[k];
			}
		}
	}

	//更新模板
	void refresh_model() {
		next_permutation(first_row, first_row + N);
		//生成数独模板
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int k = (j + offset[i]) % N;//计算偏移坐标
				model[i][j] = first_row[k];
			}
		}
	}

	//在原始模型基础上进行变换并保存36个排列，option为1则保存终盘到文件夹，为2则随机挖空
	void conversion_model(int option, int p = 0, int q = 0) {
		int A[3] = { 4, 5, 6 };//456为一组
		int B[3] = { 7, 8, 9 };//456为一组
		//前三行不变
		char first_three_row[6 * N];// 保存123行
		int index = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < N - 1; j++) {
				first_three_row[index++] = (model[i][j]);
				first_three_row[index++] = ' ';
			}
			first_three_row[index++] = (model[i][N - 1]);
			first_three_row[index++] = '\n';//换行
		}
		for (int a = 0; a < 6; a++) {
			//确定A组456,的一个排列
			char mid_three_row[6 * N];//保存456行
			int index = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < N - 1; j++) {
					//每生成一个数字，直接保存。注意空格和换行。
					mid_three_row[index++] = (model[A[i] - 1][j]);
					mid_three_row[index++] = ' ';
				}
				mid_three_row[index++] = (model[A[i] - 1][N - 1]);
				mid_three_row[index++] = '\n';//换行
			}
			//同一个A的排列可以有6种不同的排列
			for (int b = 0; b < 6; b++) {
				//确定B组789，的一个排列
				char last_three_row[6 * N];
				int index = 0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < N - 1; j++) {
						//每生成一个数字，直接保存。注意空格和换行。
						last_three_row[index++] = (model[B[i] - 1][j]);
						last_three_row[index++] = ' ';
					}
					last_three_row[index++] = (model[B[i] - 1][N - 1]);
					last_three_row[index++] = '\n';//换行
				}
				//剩余需求量变化-1
				c--;
				cout << "c为：" << c << endl;
				//保存到数组中
				save_array(first_three_row, mid_three_row, last_three_row);
				//保存到文件中
				if (option == 1) {
					write_file("endgame.txt", sudoku_endgame);
				}
				//随机挖空
				if (option == 2) {
					dig_hole(p, q);
					write_file("game.txt", sudoku_endgame);
				}
				//唯一解
				if (option == 3) {
					dig_hole(p, q);
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
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							cout << only_sudoku[i][j] << " ";
						}
						cout << endl;
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
					if (OnlyOneA == true) {
						char result[2 * N * N + 1];
						int k = 0;
						for (int i = 0; i < 9; i++) {
							for (int j = 0; j < 8; j++) {
								result[k++] = only_sudoku[i][j];
								result[k++] = ' ';
							}
							result[k++] = only_sudoku[i][N - 1];
							result[k++] = '\n';
						}
						result[2 * N * N] = '\0';
						write_file("game.txt", result);
					}
					else {
						c++;
					}
				}
				if (c == 0) return;//为零则，结束生成
				//B的下一个排列
				next_permutation(B, B + 3);
			}
			//A的下一个排列
			next_permutation(A, A + 3);
		}
	}

	//将生成的前中后三行数独保存到一个大数组中
	void save_array(char a[], char b[], char c[]) {
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

	void write_file(string file_name, char* game) {
		//写入文件
		fstream f;
		f.open(file_name, ios::out | ios::app);
		for (int i = 0; i < strlen(game); i++) {
			cout << game[i];
			f << game[i];
		}
		f << endl;
		f.close();
	}

	//生成终局
	void generate_endgame(int num) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(1);
			refresh_model();
		}
	}

	//随机挖空，随机挖洞的个数为p-q个
	void dig_hole(int p, int q) {
		//范围为0，则不挖洞，返回
		if (p == 0 && q == 0) {
			return;
		}
		//生成p-q范围内的个数，即挖洞的个数
		int hole_num = (rand() % (q - p + 1)) + p;
		cout << "挖洞个数为：" << hole_num << endl;
		//生成随机挖洞的位置
		int *ran_pos = new int[hole_num];
		for (int i = 0; i < hole_num; i++) {
			ran_pos[i] = rand() % 161;
			//如果生成随机洞的位置等于单数则重新生成
			if (ran_pos[i] % 2 != 0) {
				i--;
			}
			//如果产生了重复随机数，则重新生成
			for (int j = 0; j < i; j++) {
				if (ran_pos[j] == ran_pos[i]) {
					i--;
				}
			}
		}
		//将生成的随机洞位置排序
		sort(ran_pos, ran_pos + hole_num);
		//根据生成随机挖洞的位置挖洞
		int j = 0;
		for (int i = 0; i < sizeof(sudoku_endgame); i++) {
			if (j == hole_num) break;
			if (i == ran_pos[j]) {
				sudoku_endgame[i] = '$';
				j++;
			}
		}
		delete[] ran_pos;
	}

	//生成数独游戏
	void generate_game(int num, int p, int q) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(2, p, q);
			refresh_model();
		}
	}

	//生成唯一解数独
	void generate_only_game(int num) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(3, 10, 65);
			refresh_model();
		}
	}

	/* 判断值在填入位置n时是否满足条件 */
	bool check(int n, char key, char **Array) {
		/* 判断n所在横列是否合法 */
		for (int i = 0; i < 9; i++)
		{
			/* j为n竖坐标 */
			int j = n / 9;
			if (Array[j][i] == key) return false;
		}
		/* 判断n所在竖列是否合法 */
		for (int i = 0; i < 9; i++)
		{
			/* j为n横坐标 */
			int j = n % 9;
			if (Array[i][j] == key) return false;
		}
		/* x为n所在的小九宫格左顶点竖坐标 */
		int x = n / 9 / 3 * 3;
		/* y为n所在的小九宫格左顶点横坐标 */
		int y = n % 9 / 3 * 3;
		/* 判断n所在的小九宫格是否合法 */
		for (int i = x; i < x + 3; i++)
		{
			for (int j = y; j < y + 3; j++)
			{
				if (Array[i][j] == key) return false;
			}
		}
		/* 全部合法，返回正确 */
		return true;
	}

	/* 深搜求解数独*/
	int solve_dfs(int n, char **Array) {
		/* 数独填满则退出递归 */
		if (n > 80) {
			sign_D = true;
			return 0;
		}
		/* 当前位不为$时跳过 */
		if (Array[n / 9][n % 9] != '$') {
			solve_dfs(n + 1, Array);
		}
		else {
			for (int i = 1; i <= 9; i++) {
				/* 满足条件时填入数字 */
				if (check(n, (i + '0'), Array) == true) {
					Array[n / 9][n % 9] = i + '0';
					/* 继续搜索 */
					solve_dfs(n + 1, Array);
					/* 返回时如果构造成功，则直接退出 */
					if (sign_D == true) return 0;
					/* 如果构造不成功，还原当前位 */
					Array[n / 9][n % 9] = '$';
				}
			}
		}
	}

	/*  筛选唯一解情况*/
	int select_only(int n, char** Array) {
		if (flag > 1) {
			OnlyOneA = false;
			return 0;
		}
		/* 数独填满则退出递归 */
		if (n > 80) {
			sign_O = true;
			flag += 1;
			return 0;
		}
		/* 当前位不为$时跳过 */
		if (Array[n / 9][n % 9] != '$') {
			select_only(n + 1, Array);
		}
		else {
			for (int i = 1; i <= 9; i++) {
				/* 满足条件时填入数字 */
				if (check(n, (i + '0'), Array) == true) {
					Array[n / 9][n % 9] = i + '0';
					/* 继续搜索 */
					select_only(n + 1, Array);
					/* 如果构造不成功，还原当前位 */
					Array[n / 9][n % 9] = '$';
				}
			}
			/* 返回时如果构造成功，则直接退出 */
			if (sign_O == true) return 0;
		}
	}

	char** read_file(string file_name, int n) {
		char **sudoku_array = new char*[9];
		for (int i = 0; i < 9; i++) {
			sudoku_array[i] = new char[9];
			memset(sudoku_array[i], '$', sizeof(char)*(9));
		}
		fstream f;
		f.open(file_name);
		f.seekg(173 * n, ios::cur);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 18; j++) {
				if (j % 2 == 0) {
					f >> sudoku_array[i][j / 2];
				}
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				cout << sudoku_array[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		return sudoku_array;
	}

	void solute_sudoku(const char* file_path) {
		int sudoku_num = get_file_size(file_path) / 173;
		char **sudoku_array = new char*[9];
		for (int i = 0; i < 9; i++) {
			sudoku_array[i] = new char[9];
			memset(sudoku_array[i], '$', sizeof(char)*(9));
		}
		//读出每一个数独，求解并写入文件
		for (int i = 0; i < sudoku_num; i++) {
			sudoku_array = read_file(file_path, i);
			sign_D = false;
			solve_dfs(0, sudoku_array);
			char result[2 * N * N + 1];
			int k = 0;
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 8; j++) {
					result[k++] = sudoku_array[i][j];
					result[k++] = ' ';
				}
				result[k++] = sudoku_array[i][N - 1];
				result[k++] = '\n';
			}
			result[2 * N * N] = '\0';
			/*for (int i = 0; i < sizeof(result); i++) {
				cout << result[i];
			}*/
			write_file("sudoku.txt", result);
		}
	}
};

bool check_num(char* num) {
	for (int i = 0; i < strlen(num); i++) {
		if (num[i]<'0' || num[i]>'9') {
			return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL));
	int c_num, n_num, m_num, r_num;
	char*  s_path = NULL;
	SudokuGame sudoku_game;
	//get_file_size("game.txt");
	//sudoku_game.generate_only_game(5);
	int i = 0;
	if (argc == 1) {
		return 0;
	}
	for (i = 1; i < argc; i++) {
		//需要的数独终盘数量
		if (strcmp(argv[i], "-c") == 0) {
			c_num = (int)atoi(argv[++i]);
			if (c_num <= 0) {
				cout << "输入的终盘数量应大于0" << endl;
				return 0;
			}
			sudoku_game.generate_endgame(c_num);
		}
		//求解数独
		else if (strcmp(argv[i], "-s") == 0) {
			s_path = argv[++i];
			sudoku_game.solute_sudoku(s_path);
		}
		//需要的游戏数量
		else if (strcmp(argv[i], "-n") == 0) {
			n_num = (int)atoi(argv[++i]);
			cout << "生成数量为：" << n_num << endl;
			if (n_num <= 0) {
				cout << "输入的游戏数量应大于0" << endl;
				return 0;
			}
			//生成游戏的难度
			if (strcmp(argv[++i], "-m") == 0) {
				m_num = (int)atoi(argv[++i]);
				if (m_num == 1) {
					sudoku_game.generate_game(n_num, 10, 30);
				}
				else if (m_num == 2) {
					sudoku_game.generate_game(n_num, 31, 50);
				}
				else if (m_num == 3) {
					sudoku_game.generate_game(n_num, 51, 65);
				}
				else {
					cout << "Level setting error!" << endl;
				}
				return 0;
			}
			//生成游戏中挖空的数量范围
			else if (strcmp(argv[i], "-r") == 0) {
				//将范围字符串转换为两个数字
				int range1 = 0, range2 = 0;
				string ran_str1 = "";
				string ran_str2 = "";
				int j = 0;
				i++;
				for (; j < strlen(argv[i]); j++) {
					if (argv[i][j] == '~') {
						break;
					}
				}
				for (int k = 0; k < j; k++) {
					ran_str1.push_back(argv[i][k]);
				}
				range1 = atoi(ran_str1.c_str());
				//只有一个数，没有~
				if (j != strlen(argv[i])) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						ran_str2.push_back(argv[i][k]);
					}
					range2 = atoi(ran_str2.c_str());
				}
				//有~
				else {
					range2 = range1;
				}
				//cout << range1 << " " << range2 << endl;
				if (range1 <= 0 || range2 <= 0) {
					cout << "输入的范围应大于0" << endl;
					return 0;
				}
				sudoku_game.generate_game(n_num, range1, range2);
				return 0;
			}
			else if (strcmp(argv[i], "-u") == 0) {
				sudoku_game.generate_only_game(n_num);
				return 0;
			}
			sudoku_game.generate_game(n_num, 10, 65);
		}
		else {
			cout << "There is no matching parameter." << endl;
			return 0;
		}
	}
	return 0;
}
