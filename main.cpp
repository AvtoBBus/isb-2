#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <random>
const double PI = 3.141592653589793238463;
const double PI_chi[4] = {0.2148, 0.3672, 0.2305, 0.1875};

using namespace std;
// HELP FUNCTION
void add_more_one(string bin_str, double &gamma)
{
    for (int i = (int)gamma; i < (86 + rand() % 3); i++)
    {
        int index = rand() % 128;
        while (bin_str[index] != '0')
            index = rand() % 128;
        bin_str[index] = '1';
        gamma = i + 1;
    }
}

void calc_v(string str_to_check, int *v)
{
    int counter = 1;
    for (int i = 0; i < 7; i++)
    {
        if (str_to_check[i] == '1')
        {
            int new_counter = 1, j = i + 1;
            while (str_to_check[j] == '1' && j != 8)
            {
                new_counter++;
                j++;
            }
            if (new_counter > counter)
                counter = new_counter;
        }
    }
    if (counter <= 1)
        v[0]++;
    else if (counter == 2)
        v[1]++;
    else if (counter == 3)
        v[2]++;
    else
        v[3]++;
}
// HELP FUNCTION

// ALL TESTS
void check_the_largest_seq(string bin_str)
{
    string str_to_check = "";
    int v[4] = {0, 0, 0, 0};
    for (int i = 0; i < 128; i += 8)
    {
        for (int j = i; j < i + 8; j++)
            str_to_check += bin_str[j];
        calc_v(str_to_check, v);
        str_to_check.clear();
    }
    double chi_square = 0.0;
    for (int i = 0; i < 4; i++)
    {
        chi_square += (pow((v[i] - 16 * PI_chi[i]), 2)) / (16 * PI_chi[i]);
    }
    FILE *file = fopen("res_largest_seq_test.txt", "a");
    fprintf(file, "%lf", chi_square);
    fprintf(file, "%c", '\n');
    fclose(file);
}

void check_adjacent_bits(string bin_str, double &gamma)
{
    FILE *file = fopen("res_switch_test.txt", "a");
    gamma /= 128;
    if (abs(gamma - 0.5) < (2 / sqrt(128)))
    {
        fprintf(file, "%c", '0');
        fprintf(file, "%c", '\n');
        fclose(file);
        return;
    }
    int count_bin_switch = 0;
    for (int i = 0; i < 127; i++)
    {
        if (bin_str[i + 1] != bin_str[i])
            count_bin_switch += 1;
    }
    double result = erfc(abs(count_bin_switch - 256 * gamma * (1 - gamma)) / (2 * sqrt(256) * gamma * (1 - gamma)));
    fprintf(file, "%lf", result);
    fprintf(file, "%c", '\n');
    fclose(file);
}

void frequence_test(string bin_str)
{
    double result_sum = 0.0;
    for (int i = 0; i < 128; i++)
    {
        const char *help_str;
        if (bin_str[i] == '0')
            result_sum -= 1.0;
        else
            result_sum += 1.0;
    }
    result_sum /= sqrt(128);
    result_sum = erfc(result_sum / sqrt(2));
    FILE *file = fopen("res_frequency_test.txt", "a");
    fprintf(file, "%lf", result_sum);
    fprintf(file, "%c", '\n');
    fclose(file);
}
// ALL TESTS

// FUNCTION TO GENERATE RANDOM NUMBER
char generate_num(int input_i, int start_value, int *num_mas)
{
    int num = 0;
    if (input_i == 1)
        num = start_value * 73129 + 95121;
    else
        num = num_mas[input_i - 2] * 73129 + 95121;
    num = abs(num) % 100000;
    num *= pow(-1, rand());
    num_mas[input_i - 1] = num;
    if (num < 0)
        return '1';
    else
        return '0';
}

int main()
{
    for (int j = 0; j < 3; j++)
    {
        this_thread::sleep_for(chrono::milliseconds(5000));
        time_t t = time(nullptr);
        tm *now = localtime(&t);
        int num_mas[128];

        cout << "Start generate" << endl;

        string bin_str = "";
        double gamma = 0.0;

        for (int i = 0; i < 128; i++)
        {
            bin_str += generate_num(i + 1, (now->tm_hour + now->tm_min + now->tm_sec) / 3, num_mas);
        }
        for (int i = 0; i < 128; i++)
        {
            if (bin_str[i] == '1')
                gamma += 1.0;
        }

        if (gamma < 70)
            add_more_one(bin_str, gamma);

        bin_str += '\n';
        FILE *file = fopen("random_bin_sequence.txt", "a");
        const char *str_to_write = bin_str.c_str();
        fprintf(file, "%s", str_to_write);
        fclose(file);

        cout << bin_str << endl;
        frequence_test(bin_str);
        check_adjacent_bits(bin_str, gamma);
        check_the_largest_seq(bin_str);
    }
}