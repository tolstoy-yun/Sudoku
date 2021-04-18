#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>
#include "../sudoku_game/include/sudoku_game.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        // 测试first_model，判断生成的第一行是否随机
        TEST_METHOD(TestMethod1)
        {
            char temp_first_row[10] = { '$' };
            SudokuGame sudoku_game;
            sudoku_game.first_model();
            for (int i = 0; i < 10; i++) {
                temp_first_row[i] = sudoku_game.first_row[i];
            }
            sudoku_game.first_model();
            // 比较两次生成的是否不等，不等则证明随机
            Assert::AreNotEqual(sudoku_game.first_row, temp_first_row);
        }
        // 测试refresh_model，判断第一行是否已进行排列变换，model是否按照规则移位
        TEST_METHOD(TestMethod2)
        {
            char result_first_row[10] = { '1','2','3','4','5','6','7','9','8' };
            char result_model[9][9] = {
                {'1','2','3','4','5','6','7','9','8'},
                {'4','5','6','7','9','8','1','2','3'},
                {'7','9','8','1','2','3','4','5','6'},
                {'2','3','4','5','6','7','9','8','1'},
                {'5','6','7','9','8','1','2','3','4'},
                {'9','8','1','2','3','4','5','6','7'},
                {'3','4','5','6','7','9','8','1','2'},
                {'6','7','9','8','1','2','3','4','5'},
                {'8','1','2','3','4','5','6','7','9'}

            };
            // 初始化first_row为1,2,3,4,5,6,7,8,9
            SudokuGame sudoku_game;
            for (int i = 1; i < 10; i++) {
                sudoku_game.first_row[i - 1] = (char)('0' + i);
            }
            sudoku_game.first_row[9] = '\0';
            sudoku_game.refresh_model();
            // 测试第一行排列变换后的结果
            Assert::AreEqual(sudoku_game.first_row, result_first_row);
            // 测试由第一行偏移变换得到的model的结果
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    Assert::AreEqual(sudoku_game.model[i][j], result_model[i][j]);
                }
            }

        }
        // 测试dig_hole
        TEST_METHOD(TestMethod3) {
            SudokuGame sudoku_game;
            char end_model[163] = { "1 2 3 4 5 6 7 9 8\n4 5 6 7 9 8 1 2 3\n7 9 8 1 2 3 4 5 6\n2 3 4 5 6 7 9 8 1\n5 6 7 9 8 1 2 3 4\n9 8 1 2 3 4 5 6 7\n3 4 5 6 7 9 8 1 2\n6 7 9 8 1 2 3 4 5\n8 1 2 3 4 5 6 7 9\n" };
            for (int i = 0; i < 163; i++) {
                sudoku_game.sudoku_endgame[i] = end_model[i];
            }
            // 测试p=q=0
            Assert::IsFalse(sudoku_game.dig_hole(0, 0));
            // 测试p<q
            Assert::IsFalse(sudoku_game.dig_hole(5, 1));
            // 测试q>80
            Assert::IsFalse(sudoku_game.dig_hole(30, 97));
            // 测试生成挖洞数是否在范围内
            Assert::IsTrue(sudoku_game.dig_hole(10, 65));
            int count = 0;
            for (int i = 0; i < 163; i++) {
                if (sudoku_game.sudoku_endgame[i] == '$') {
                    count++;
                }
            }
            Assert::IsTrue(count >= 10);
            Assert::IsTrue(count <= 65);
        }
        //测试get_file_size()
        TEST_METHOD(TestMethod4) {
            int a = get_file_size("F:\\sudoku_game\\sudoku_game\\sudoku_game\\test_file\\t_get_file_size.txt");
            Assert::AreEqual(a, 14);
        }
        //测试write_game()
        TEST_METHOD(TestMethod5) {
            char test[6] = { 'a','b','c','d','e' };
            test[5] = '\0';
            SudokuGame test_5;
            test_5.write_game("F:\\sudoku_game\\sudoku_game\\sudoku_game\\test_file\\t_write_game.txt", test);
            fstream f;
            f.open("F:\\sudoku_game\\sudoku_game\\sudoku_game\\test_file\\t_write_game.txt");
            char compare[6];
            for (int i = 0; i < 5; i++) {
                f >> compare[i];
            }
            compare[5] = '\0';
            Assert::AreEqual(test, compare);
        }
        //测试read_game()
        TEST_METHOD(TestMethod6) {
            SudokuGame test_6;
            char **sudoku_array = new char*[9];
            for (int i = 0; i < 9; i++) {
                sudoku_array[i] = new char[9];
                memset(sudoku_array[i], '$', sizeof(char)*(9));
            }
            sudoku_array = test_6.read_game("F:\\sudoku_game\\sudoku_game\\sudoku_game\\test_file\\t_read_game.txt", 0);
            char result_model[9][9] = {
                {'1','6','$','$','$','9','$','8','4'},
                {'7','2','$','3','$','$','$','$','$'},
                {'3','8','$','$','$','$','7','2','9'},
                {'$','$','$','2','9','$','$','4','1'},
                {'2','$','3','8','4','1','6','$','$'},
                {'$','4','$','6','5','$','2','$','3'},
                {'$','7','$','$','3','$','4','$','$'},
                {'9','3','$','$','1','6','$','$','$'},
                {'$','1','$','5','$','$','9','$','$'}

            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    Assert::AreEqual(sudoku_array[i][j], result_model[i][j]);
                }
            }
        }
        //测试conversion_model()
        TEST_METHOD(TestMethod7) {
            char result_model[9][9] = {
                {'1','2','3','4','5','6','7','9','8'},
                {'4','5','6','7','9','8','1','2','3'},
                {'7','9','8','1','2','3','4','5','6'},
                {'2','3','4','5','6','7','9','8','1'},
                {'5','6','7','9','8','1','2','3','4'},
                {'9','8','1','2','3','4','5','6','7'},
                {'3','4','5','6','7','9','8','1','2'},
                {'6','7','9','8','1','2','3','4','5'},
                {'8','1','2','3','4','5','6','7','9'}

            };
            char result_array[163] = { "1 2 3 4 5 6 7 9 8\n4 5 6 7 9 8 1 2 3\n7 9 8 1 2 3 4 5 6\n2 3 4 5 6 7 9 8 1\n5 6 7 9 8 1 2 3 4\n9 8 1 2 3 4 5 6 7\n3 4 5 6 7 9 8 1 2\n6 7 9 8 1 2 3 4 5\n8 1 2 3 4 5 6 7 9\n" };
            SudokuGame test_7;
            test_7.c = 1;
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    test_7.model[i][j] = result_model[i][j];
                }
            }
            test_7.conversion_model(1);
            for (int i = 0; i < 163; i++) {
                Assert::AreEqual(result_array[i], test_7.sudoku_endgame[i]);
            }
            test_7.c = 1;
            test_7.conversion_model(2, 20, 50);
            int count = 0;
            for (int i = 0; i < 163; i++) {
                if (test_7.sudoku_endgame[i] == '$') {
                    count++;
                }
            }
            Assert::IsTrue(count >= 20);
            Assert::IsTrue(count <= 50);
            test_7.c = 1;
            test_7.conversion_model(3, 10, 65);
            Assert::IsTrue(test_7.OnlyOneA);
        }
        //测试save_array()
        TEST_METHOD(TestMethod8) {
            char result_array[163] = { "1 2 3 4 5 6 7 9 8\n4 5 6 7 9 8 1 2 3\n7 9 8 1 2 3 4 5 6\n2 3 4 5 6 7 9 8 1\n5 6 7 9 8 1 2 3 4\n9 8 1 2 3 4 5 6 7\n3 4 5 6 7 9 8 1 2\n6 7 9 8 1 2 3 4 5\n8 1 2 3 4 5 6 7 9\n" };
            char a[55] = { "1 2 3 4 5 6 7 9 8\n4 5 6 7 9 8 1 2 3\n7 9 8 1 2 3 4 5 6\n" };
            char b[55] = { "2 3 4 5 6 7 9 8 1\n5 6 7 9 8 1 2 3 4\n9 8 1 2 3 4 5 6 7\n" };
            char c[55] = { "3 4 5 6 7 9 8 1 2\n6 7 9 8 1 2 3 4 5\n8 1 2 3 4 5 6 7 9\n" };
            SudokuGame test_8;
            test_8.save_array(a, b, c);
            Assert::AreEqual(test_8.sudoku_endgame, result_array);
        }
        //测试generate_endgame()
        TEST_METHOD(TestMethod9) {
            SudokuGame test_9;
            test_9.generate_endgame(20);
            Assert::AreEqual(test_9.c, 0);
        }
        //测试generate_game()
        TEST_METHOD(TestMethod10) {
            SudokuGame test_10;
            test_10.generate_game(10, 10, 65);
            Assert::AreEqual(test_10.c, 0);
        }
        //测试sovle_dfs()
        TEST_METHOD(TestMethod11) {
            SudokuGame test_11;
            char **sudoku_array = new char*[9];
            for (int i = 0; i < 9; i++) {
                sudoku_array[i] = new char[9];
                memset(sudoku_array[i], '$', sizeof(char)*(9));
            }
            char test_model[9][9] = {
                {'$','$','4','1','$','7','$','$','$'},
                {'5','7','$','$','2','$','$','9','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','2','4'},
                {'6','$','$','4','$','3','$','$','1'},
                {'$','$','$','$','7','$','$','$','$'},
                {'$','$','8','6','$','5','$','7','3'},
                {'2','$','$','$','1','$','4','5','$'}
            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    sudoku_array[i][j] = test_model[i][j];
                }
            }
            test_11.solve_dfs(0, sudoku_array);
            char compare_model[9][9] = {
                {'8','9','4','1','6','7','5','3','2'},
                {'5','7','3','8','2','4','1','9','6'},
                {'1','2','6','5','3','9','8','4','7'},
                {'7','4','9','2','8','1','3','6','5'},
                {'3','8','1','7','5','6','9','2','4'},
                {'6','5','2','4','9','3','7','8','1'},
                {'4','3','5','9','7','2','6','1','8'},
                {'9','1','8','6','4','5','2','7','3'},
                {'2','6','7','3','1','8','4','5','9'}
            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    Assert::AreEqual(sudoku_array[i][j], compare_model[i][j]);
                }
            }
        }
        //测试select_only()
        TEST_METHOD(TestMethod12) {
            SudokuGame test_12;
            char **sudoku_array = new char*[9];
            for (int i = 0; i < 9; i++) {
                sudoku_array[i] = new char[9];
                memset(sudoku_array[i], '$', sizeof(char)*(9));
            }
            char test_model[9][9] = {
                {'$','$','4','1','$','7','$','$','$'},
                {'5','7','$','$','2','$','$','9','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','2','4'},
                {'6','$','$','4','$','3','$','$','1'},
                {'$','$','$','$','7','$','$','$','$'},
                {'$','$','8','6','$','5','$','7','3'},
                {'2','$','$','$','1','$','4','5','$'}
            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    sudoku_array[i][j] = test_model[i][j];
                }
            }
            test_12.select_only(0, sudoku_array);
            Assert::IsTrue(test_12.OnlyOneA);
            char test1_model[9][9] = {
                {'$','$','$','1','$','7','$','$','$'},
                {'5','7','$','$','2','$','$','9','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','2','4'},
                {'6','$','$','4','$','3','$','$','1'},
                {'$','$','$','$','7','$','$','$','$'},
                {'$','$','8','6','$','5','$','7','3'},
                {'2','$','$','$','1','$','4','5','$'}
            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    sudoku_array[i][j] = test1_model[i][j];
                }
            }
            test_12.select_only(0, sudoku_array);
            Assert::IsFalse(test_12.OnlyOneA);
        }
        //测试check()
        TEST_METHOD(TestMethod13) {
            SudokuGame test_13;
            bool flag = true;
            char **sudoku_array = new char*[9];
            for (int i = 0; i < 9; i++) {
                sudoku_array[i] = new char[9];
                memset(sudoku_array[i], '$', sizeof(char)*(9));
            }
            char test_model[9][9] = {
                {'$','$','4','1','$','7','$','$','$'},
                {'5','7','$','$','2','$','$','9','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','$','$'},
                {'$','$','$','$','$','$','$','2','4'},
                {'6','$','$','4','$','3','$','$','1'},
                {'$','$','$','$','7','$','$','$','$'},
                {'$','$','8','6','$','5','$','7','3'},
                {'2','$','$','$','1','$','4','5','$'}
            };
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    sudoku_array[i][j] = test_model[i][j];
                }
            }
            flag = test_13.check(0, '7', sudoku_array);
            Assert::IsFalse(flag);
            flag = true;
            flag = test_13.check(0, '2', sudoku_array);
            Assert::IsFalse(flag);
            flag = true;
            flag = test_13.check(77, '6', sudoku_array);
            Assert::IsFalse(flag);
        }
        //测试solute_sudoku()
        TEST_METHOD(TestMethod14) {
            SudokuGame test_14;
            test_14.solute_sudoku("F:\\sudoku_game\\sudoku_game\\sudoku_game\\test_file\\t_solute_sudoku.txt");
            Assert::IsTrue(test_14.sign_D);
        }
        //测试generate_only_game()
        TEST_METHOD(TestMethod15) {
            SudokuGame test_15;
            test_15.generate_only_game(15);
            Assert::AreEqual(test_15.c, 0);
        }

    };
}
