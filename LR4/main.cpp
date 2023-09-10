#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

const string MSG_STARTED = "Поиск коэффициента конкордации для 7 экспертов и 10 альтернатив для каждой из групп.\n"
                           "Определение группы 6 экспертов с наиболее согласованным мнением и коэффициент конкордации для неё.\n";
const string MSG_COMPLETED = "Результаты успешно сохранены в файл\n";

const string MSG_READ_FILE = "Считывание данных из файла\n";

const string ERR_INPUT_NOT_FOUND = "Файл input.txt не найден\n";
const string ERR_OUTPUT_NOT_FOUND = "Файл output.txt не может быть открыт\n";

/**
 * Find coefficient Concordance
 *
 * @param &matrix
 * @param &R
 *
 * @return coefficient Concordance
 */
void findCoeffConcordance(
        vector<vector<int>> &matrix,
        float &R) {

    int n = int(matrix.size());
    int m = int(matrix[0].size());
    vector<float> summary;
    float sum_summary = 0.0;

    for (int i = 0; i < m; i++) {
        float sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += float(matrix[j][i]);
        }
        summary.push_back(sum);
        sum_summary += sum;
    }

    float S = 0.0;
    for (int i = 0; i < summary.size(); i++) {
        S +=  float(pow(summary[i] - (sum_summary / float(summary.size())), 2));
    }

    R = float((12 * S) / (n * n * (pow(m, 3) - m)));
}


int main() {

    /** Init start value */
    setlocale(LC_ALL, "ru");

    int n = 7, m = 10;

    cout << MSG_STARTED;
    cout << MSG_READ_FILE;

    /** Reading data from a file */
    string str;
    vector<vector<int>> A(n, vector<int>(m));
    ifstream $in("input.txt");

    if (!$in.is_open()) {
        cout << ERR_INPUT_NOT_FOUND;

        return 1;
    }

    int column = 0;
    while (getline($in, str)) {

        string row;
        istringstream ist(str);
        int j = 0;

        while (ist >> row) {
            A[column][j] = stoi(row);
            j++;
        }
        column++;
    }
    $in.close();

    /** Find coefficient concordance for all group */

    vector<pair<float, int>> matrix_result;
    pair<float, int> result;
    float full_result;

    findCoeffConcordance(A, ref(full_result));

    for (auto i = 0; i < A.size(); i++) {
        vector<vector<int>> copy_start_matrix(A.size(), vector<int>(A.front().size()));
        copy(A.begin(), A.end(), copy_start_matrix.begin());

        copy_start_matrix.erase(copy_start_matrix.cbegin() + i);

        float R;

        findCoeffConcordance(copy_start_matrix, ref(R));
        matrix_result.emplace_back(R, i + 1);
    }

    sort(matrix_result.begin(), matrix_result.end());
    reverse(matrix_result.begin(), matrix_result.end());
    result = matrix_result.front();


    /** Output to the response file */
    ofstream $out;
    $out.open("output.txt");

    if (!$out.is_open()) {
        cout << ERR_OUTPUT_NOT_FOUND;

        return 1;
    }

    sort(matrix_result.begin(), matrix_result.end(),
         [](const auto &x, const auto &y) { return x.second < y.second; });

    if ($out.is_open()) {
        $out << "Ответ:";
        for (auto &i: matrix_result) {
            $out << "\nбез эксперта e" << i.second << ", R" << i.second << " = "
                 << round(i.first * 1000) / 1000 << ";";
        }
        $out << "\n\nкоэффициент конкордации всей группы R = " << round(full_result * 1000) / 1000
             << ", наиболее согласованная группа – без эксперта e" << result.second << ", R" << result.second << " = "
             << round(result.first * 1000) / 1000;
    }

    $out.close();

    cout << MSG_COMPLETED;

    return 0;
}