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
	char model[N][N];//�������ģ��
	char sudoku_endgame[2 * N * N + 1];//������ɵ��վ�
	char first_row[10];//��һ�У�Ҳ�ǵ�һ��ȫ����
	int c = 0;//�����վֵ�����
	int offset[N] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };//ƫ��ֵ
	bool sign_O = false;
	bool sign_D = false;
	bool OnlyOneA = true;
	int flag = 0;


	//���ɵ�һ��ģ��
	void first_model() {
		//����
		char number[N] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		//���ȷ���ʼ������
		int ran = 0 + rand() % 362880;
		for (int i = 0; i < ran; i++) {
			next_permutation(number, number + N);//ȫ����
		}
		for (int i = 0; i < N; i++) {
			first_row[i] = number[i];
		}
		first_row[N] = '\0';
		//��������ģ��
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int k = (j + offset[i]) % N;//����ƫ������
				model[i][j] = first_row[k];
			}
		}
	}

	//����ģ��
	void refresh_model() {
		next_permutation(first_row, first_row + N);
		//��������ģ��
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int k = (j + offset[i]) % N;//����ƫ������
				model[i][j] = first_row[k];
			}
		}
	}

	//��ԭʼģ�ͻ����Ͻ��б任������36�����У�optionΪ1�򱣴����̵��ļ��У�Ϊ2������ڿ�
	void conversion_model(int option, int p = 0, int q = 0) {
		int A[3] = { 4, 5, 6 };//456Ϊһ��
		int B[3] = { 7, 8, 9 };//456Ϊһ��
		//ǰ���в���
		char first_three_row[6 * N];// ����123��
		int index = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < N - 1; j++) {
				first_three_row[index++] = (model[i][j]);
				first_three_row[index++] = ' ';
			}
			first_three_row[index++] = (model[i][N - 1]);
			first_three_row[index++] = '\n';//����
		}
		for (int a = 0; a < 6; a++) {
			//ȷ��A��456,��һ������
			char mid_three_row[6 * N];//����456��
			int index = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < N - 1; j++) {
					//ÿ����һ�����֣�ֱ�ӱ��档ע��ո�ͻ��С�
					mid_three_row[index++] = (model[A[i] - 1][j]);
					mid_three_row[index++] = ' ';
				}
				mid_three_row[index++] = (model[A[i] - 1][N - 1]);
				mid_three_row[index++] = '\n';//����
			}
			//ͬһ��A�����п�����6�ֲ�ͬ������
			for (int b = 0; b < 6; b++) {
				//ȷ��B��789����һ������
				char last_three_row[6 * N];
				int index = 0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < N - 1; j++) {
						//ÿ����һ�����֣�ֱ�ӱ��档ע��ո�ͻ��С�
						last_three_row[index++] = (model[B[i] - 1][j]);
						last_three_row[index++] = ' ';
					}
					last_three_row[index++] = (model[B[i] - 1][N - 1]);
					last_three_row[index++] = '\n';//����
				}
				//ʣ���������仯-1
				c--;
				cout << "cΪ��" << c << endl;
				//���浽������
				save_array(first_three_row, mid_three_row, last_three_row);
				//���浽�ļ���
				if (option == 1) {
					write_file("endgame.txt", sudoku_endgame);
				}
				//����ڿ�
				if (option == 2) {
					dig_hole(p, q);
					write_file("game.txt", sudoku_endgame);
				}
				//Ψһ��
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
				if (c == 0) return;//Ϊ���򣬽�������
				//B����һ������
				next_permutation(B, B + 3);
			}
			//A����һ������
			next_permutation(A, A + 3);
		}
	}

	//�����ɵ�ǰ�к������������浽һ����������
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
		//д���ļ�
		fstream f;
		f.open(file_name, ios::out | ios::app);
		for (int i = 0; i < strlen(game); i++) {
			cout << game[i];
			f << game[i];
		}
		f << endl;
		f.close();
	}

	//�����վ�
	void generate_endgame(int num) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(1);
			refresh_model();
		}
	}

	//����ڿգ�����ڶ��ĸ���Ϊp-q��
	void dig_hole(int p, int q) {
		//��ΧΪ0�����ڶ�������
		if (p == 0 && q == 0) {
			return;
		}
		//����p-q��Χ�ڵĸ��������ڶ��ĸ���
		int hole_num = (rand() % (q - p + 1)) + p;
		cout << "�ڶ�����Ϊ��" << hole_num << endl;
		//��������ڶ���λ��
		int *ran_pos = new int[hole_num];
		for (int i = 0; i < hole_num; i++) {
			ran_pos[i] = rand() % 161;
			//��������������λ�õ��ڵ�������������
			if (ran_pos[i] % 2 != 0) {
				i--;
			}
			//����������ظ������������������
			for (int j = 0; j < i; j++) {
				if (ran_pos[j] == ran_pos[i]) {
					i--;
				}
			}
		}
		//�����ɵ������λ������
		sort(ran_pos, ran_pos + hole_num);
		//������������ڶ���λ���ڶ�
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

	//����������Ϸ
	void generate_game(int num, int p, int q) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(2, p, q);
			refresh_model();
		}
	}

	//����Ψһ������
	void generate_only_game(int num) {
		c = num;
		first_model();
		while (c > 0) {
			conversion_model(3, 10, 65);
			refresh_model();
		}
	}

	/* �ж�ֵ������λ��nʱ�Ƿ��������� */
	bool check(int n, char key, char **Array) {
		/* �ж�n���ں����Ƿ�Ϸ� */
		for (int i = 0; i < 9; i++)
		{
			/* jΪn������ */
			int j = n / 9;
			if (Array[j][i] == key) return false;
		}
		/* �ж�n���������Ƿ�Ϸ� */
		for (int i = 0; i < 9; i++)
		{
			/* jΪn������ */
			int j = n % 9;
			if (Array[i][j] == key) return false;
		}
		/* xΪn���ڵ�С�Ź����󶥵������� */
		int x = n / 9 / 3 * 3;
		/* yΪn���ڵ�С�Ź����󶥵������ */
		int y = n % 9 / 3 * 3;
		/* �ж�n���ڵ�С�Ź����Ƿ�Ϸ� */
		for (int i = x; i < x + 3; i++)
		{
			for (int j = y; j < y + 3; j++)
			{
				if (Array[i][j] == key) return false;
			}
		}
		/* ȫ���Ϸ���������ȷ */
		return true;
	}

	/* �����������*/
	int solve_dfs(int n, char **Array) {
		/* �����������˳��ݹ� */
		if (n > 80) {
			sign_D = true;
			return 0;
		}
		/* ��ǰλ��Ϊ$ʱ���� */
		if (Array[n / 9][n % 9] != '$') {
			solve_dfs(n + 1, Array);
		}
		else {
			for (int i = 1; i <= 9; i++) {
				/* ��������ʱ�������� */
				if (check(n, (i + '0'), Array) == true) {
					Array[n / 9][n % 9] = i + '0';
					/* �������� */
					solve_dfs(n + 1, Array);
					/* ����ʱ�������ɹ�����ֱ���˳� */
					if (sign_D == true) return 0;
					/* ������첻�ɹ�����ԭ��ǰλ */
					Array[n / 9][n % 9] = '$';
				}
			}
		}
	}

	/*  ɸѡΨһ�����*/
	int select_only(int n, char** Array) {
		if (flag > 1) {
			OnlyOneA = false;
			return 0;
		}
		/* �����������˳��ݹ� */
		if (n > 80) {
			sign_O = true;
			flag += 1;
			return 0;
		}
		/* ��ǰλ��Ϊ$ʱ���� */
		if (Array[n / 9][n % 9] != '$') {
			select_only(n + 1, Array);
		}
		else {
			for (int i = 1; i <= 9; i++) {
				/* ��������ʱ�������� */
				if (check(n, (i + '0'), Array) == true) {
					Array[n / 9][n % 9] = i + '0';
					/* �������� */
					select_only(n + 1, Array);
					/* ������첻�ɹ�����ԭ��ǰλ */
					Array[n / 9][n % 9] = '$';
				}
			}
			/* ����ʱ�������ɹ�����ֱ���˳� */
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
		//����ÿһ����������Ⲣд���ļ�
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
		//��Ҫ��������������
		if (strcmp(argv[i], "-c") == 0) {
			c_num = (int)atoi(argv[++i]);
			if (c_num <= 0) {
				cout << "�������������Ӧ����0" << endl;
				return 0;
			}
			sudoku_game.generate_endgame(c_num);
		}
		//�������
		else if (strcmp(argv[i], "-s") == 0) {
			s_path = argv[++i];
			sudoku_game.solute_sudoku(s_path);
		}
		//��Ҫ����Ϸ����
		else if (strcmp(argv[i], "-n") == 0) {
			n_num = (int)atoi(argv[++i]);
			cout << "��������Ϊ��" << n_num << endl;
			if (n_num <= 0) {
				cout << "�������Ϸ����Ӧ����0" << endl;
				return 0;
			}
			//������Ϸ���Ѷ�
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
			//������Ϸ���ڿյ�������Χ
			else if (strcmp(argv[i], "-r") == 0) {
				//����Χ�ַ���ת��Ϊ��������
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
				//ֻ��һ������û��~
				if (j != strlen(argv[i])) {
					for (int k = j + 1; k < strlen(argv[i]); k++) {
						ran_str2.push_back(argv[i][k]);
					}
					range2 = atoi(ran_str2.c_str());
				}
				//��~
				else {
					range2 = range1;
				}
				//cout << range1 << " " << range2 << endl;
				if (range1 <= 0 || range2 <= 0) {
					cout << "����ķ�ΧӦ����0" << endl;
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
