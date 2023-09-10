#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

const string MSG_STARTED = "Поиск решения игры в условиях неопределенности\n";
const string MSG_COMPLETED = "Результаты успешно сохранены в файл\n";

const string MSG_READ_FILE = "Считывание данных из файла\n";

const string ERR_INPUT_NOT_FOUND = "Файл input.txt не найден\n";
const string ERR_OUTPUT_NOT_FOUND = "Файл output.txt не может быть открыт\n";

/**
 * Find math expectation
 *
 * @param &row
 *
 * @return math expectation
 */
float findMathExpectation(vector<int> &row) {

    float sum = 0;
    for (int i: row) {
        sum += float(i);
    }
    sum /= float(row.size());

    return sum;
}

/**
 * Find risk matrix
 *
 * @param maxColumn
 * @param &matrix_risk
 *
 * @return risk matrix
 */
void findMatrixRisk(vector<int> maxColumn, vector<vector<int>> &matrixRisk) {

    for (int i = 0; i < matrixRisk[0].size(); i++) {
        for (auto &j: matrixRisk) {
            j[i] = maxColumn[i] - j[i];
        }
    }
}

/**
 * Find all maximum
 *
 * @param vec
 * @param &result
 * @param maxValue
 *
 * @return all maximum
 */
template<typename T1, typename T2>
void findAllMaximum(vector<T1> vec, vector<pair<int, T2>> &result, bool maxValue = true) {

    T2 minmaxValue = vec[0];
    result.push_back(make_pair(1, minmaxValue));

    for (int i = 1; i < vec.size(); i++) {
        if ((minmaxValue < vec[i] && maxValue) || (minmaxValue > vec[i] && !maxValue)) {
            minmaxValue = vec[i];
            result.clear();
            result.push_back(make_pair(i + 1, minmaxValue));
        } else if (minmaxValue == vec[i]) {
            result.push_back(make_pair(i + 1, minmaxValue));
        }
    }
}

/**
 * Find min or max value from row
 *
 * @param row
 * @param maxValue
 *
 * @return min or max value from row
 */
template<typename T>
int findMinMax(vector<T> row, bool maxValue = true) {
    sort(row.begin(), row.end());
    if (!maxValue) return row.front();
    else return row.back();
}

int main() {

    /** Init start value */
    setlocale(LC_ALL, "ru");

    const float lambda = 0.5, mu = 0.5;
    const int n = 8, m = 10;

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


    /** Search for the optimal strategy using the Wald criterion */
    vector<pair<int, int>> resWald;
    vector<int> WaldW;

    for (const auto &i: A) {
        WaldW.push_back(findMinMax(i, false));
    }

    findAllMaximum(WaldW, resWald, true);

    /** Search for the optimal strategy using the Savage criterion */
    vector<pair<int, int>> resSavage;
    vector<int> D, DColumn;
    vector<vector<int>> m_risk = A;

    for (int i = 0; i < A[0].size(); i++) {
        for (auto &j: A) {
            DColumn.push_back(j[i]);
        }
        D.push_back(findMinMax(DColumn, true));
        DColumn.clear();
    }

    findMatrixRisk(D, std::ref(m_risk));
    D.clear();

    for (const auto &i: m_risk) {
        D.push_back(findMinMax(i, true));
    }

    findAllMaximum(D, resSavage, false);

    /** Search for an optimal strategy using the Hurwitz criterion */
    vector<pair<int, float>> resHurwitz;
    vector<float> HurwitzW;

    for (int i = 0; i < A.size(); i++) {
        HurwitzW.push_back(findMinMax(A[i], true) * lambda + findMinMax(A[i], false) * (1 - lambda));
    }

    findAllMaximum(HurwitzW, resHurwitz, true);

    /** Search for the optimal strategy using the Bayes-Laplace criterion */
    vector<pair<int, float>> resBayeslaplace;
    vector<float> math_exp;

    for (int i = 0; i < A.size(); i++) {
        math_exp.push_back(findMathExpectation(A[i]));
    }

    findAllMaximum(math_exp, resBayeslaplace, true);


    /** Search for the optimal strategy using the Hodge-Lehman criterion */

    vector<pair<int, float>> resHodgeLehman;
    vector<float> HLW;

    for (int i = 0; i < A.size(); i++) {
        HLW.push_back(findMathExpectation(A[i]) * mu + findMinMax(A[i], false) * (1 - mu));
    }

    findAllMaximum(HLW, resHodgeLehman, true);

    /** Output to the response file */
    ofstream $out;
    $out.open("output.txt");

    if (!$out.is_open()) {
        cout << ERR_OUTPUT_NOT_FOUND;

        return 1;
    }

    $out << "Ответ:" << endl << "по критерию Вальда: ";
    if (!resWald.empty())
        $out << to_string(resWald[0].second) + " при стратегии " +
                to_string(resWald[0].first) << endl;

    $out << "по критерию Сэвиджа: ";
    if (!resSavage.empty())
        $out << to_string(resSavage[0].second) + " при стратегии " +
                to_string(resSavage[0].first) << endl;

    $out << "по критерию Гурвица: ";
    if (!resHurwitz.empty()) {
        $out << resHurwitz[0].second;
        $out << " при стратегии ";
        $out << to_string(resHurwitz[0].first) << endl;
    }

    $out << "по критерию Байеса Лапласа: ";
    if (!resBayeslaplace.empty()) {
        $out << resBayeslaplace[0].second;
        $out << " при стратегии ";
        $out << resBayeslaplace[0].first << endl;
    }

    $out << "по критерию Ходжа Лемана: ";
    if (!resHodgeLehman.empty()) {
        $out << resHodgeLehman[0].second;
        $out << " при стратегии ";
        $out << resHodgeLehman[0].first << endl;
    }
    $out.close();

    cout << MSG_COMPLETED;

    return 0;
}