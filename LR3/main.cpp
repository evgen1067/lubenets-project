#include <functional>
#include "iostream"
#include "sstream"
#include "fstream"
#include "cmath"
#include "string"
#include "vector"
#include "cstdio"

using namespace std;

const string MSG_STARTED = "Определение оптимальных альтернатив с учетом всех нечетких отношений предпочтения и их важности\n\n";
const string MSG_READ_FILE = "Считывание данных из файла\n";
const string MSG_COMPLETED = "Результаты успешно сохранены в файл\n";

const string ERR_INPUT_NOT_FOUND = "Файл input.txt не найден\n";
const string ERR_OUTPUT_NOT_FOUND = "Файл output.txt не может быть открыт\n";

int main()
{
    setlocale(LC_ALL, "ru");

    int $rCount = 7, $rSize = 6, $i = 0, $j = 0, $k = 0;

    vector<float> $lambdas;
    vector<vector<vector<float>>> $rMatrices;
    vector<float> $mu;

    cout << MSG_STARTED;
    cout << MSG_READ_FILE;

    string $str;
    vector<vector<float>> $rMatrix($rSize, vector<float> ($rSize));
    ifstream $in("input.txt");
    if (!$in.is_open()) {
        cout << ERR_INPUT_NOT_FOUND;

        return 1;
    }

    $i = 0, $k = -1;

    // считывание весовых коэффициентов
    getline($in, $str);
    istringstream $ist($str);
    string $row;
    while ($ist >> $row) {
        $lambdas.push_back(stod($row));
    }

    // считывание нечетких отношений предпочтения
    while (getline($in, $str)) {
        if ($str.empty()) {
            continue;
        }

        string $row;
        istringstream $ist($str);
        $j = 0;
        if ($i % $rSize == 0) {
            $k++;
            if($k >= 1) {
                $rMatrices.push_back($rMatrix);
            }
        }

        while ($ist >> $row) {
            if ($k >= $rCount) {
                break;
            }
            $rMatrix[$i % $rSize][$j] = stod($row);
            $j++;
        }
        $i++;
    }
    $rMatrices.push_back($rMatrix);
    $in.close();

    vector<vector<float>> $qFirst($rSize, vector<float>($rSize)),
                          $qFirstS($rSize, vector<float>($rSize)),
                          $qSecond($rSize, vector<float>($rSize)),
                          $qSecondS($rSize, vector<float>($rSize));
    vector<float> $qFirstMu, $qSecondMu;

    /** Q1 & Q2 */
    float $minValue;
    for ($i = 0; $i < $rSize; $i++) {
        for ($j = 0; $j < $rSize; $j++) {
            $minValue = $rMatrices[0][$i][$j];
            for ($k = 0; $k < $rMatrices.size(); $k++) {
                if ($rMatrices[$k][$i][$j] < $minValue) {
                    $minValue = $rMatrices[$k][$i][$j];
                }
                $qSecond[$i][$j] += $lambdas[$k] * $rMatrices[$k][$i][$j];
            }
            $qFirst[$i][$j] = $minValue;
        }
    }

    /** Q1-s & Q2-s */
    for ($i = 0; $i < $rSize; $i++) {
        for ($j = 0; $j < $rSize; $j++) {
            $qFirstS[$i][$j] = $qFirst[$i][$j] - $qFirst[$j][$i] < 0 ? 0 : $qFirst[$i][$j] - $qFirst[$j][$i];
            $qSecondS[$i][$j] = $qSecond[$i][$j] - $qSecond[$j][$i] < 0 ? 0 : $qSecond[$i][$j] - $qSecond[$j][$i];
        }
    }

    /** Q1-mu & Q2-mu */
    float $maxValue1, $maxValue2;
    for ($i = 0; $i < $rSize; $i++) {
        $maxValue1 = $qFirstS[0][$i], $maxValue2 = $qSecondS[0][$i];
        for ($j = 0; $j < $rSize; $j++) {
            if ($qFirstS[$j][$i] > $maxValue1) {
                $maxValue1 = $qFirstS[$j][$i];
            }
            if ($qSecondS[$j][$i] > $maxValue2) {
                $maxValue2 = $qSecondS[$j][$i];
            }
        }
        $qFirstMu.push_back(1 - $maxValue1);
        $qSecondMu.push_back(1 - $maxValue2);
    }

    $minValue = 0.0;
    pair<int, float> $result;
    for ($i = 0; $i < $qFirstMu.size(); $i++) {
        $minValue = $qFirstMu[$i] < $qSecondMu[$i] ? $qFirstMu[$i] : $qSecondMu[$i];
        $mu.push_back($minValue);
        if ($i == 0) {
            $result = make_pair(1, $minValue);
        }
        if ($minValue > $result.second) {
            $result = make_pair($i + 1, $minValue);
        }
    }

    ofstream $out;
    $out.open("output.txt");
    if (!$out.is_open()) {
        cout << ERR_OUTPUT_NOT_FOUND;
    }

    $out << "Ответ:\n";
    for ($i = 0; $i < $mu.size(); $i++) {
        $out << "phi_н.д.(x"
             << $i + 1
             << ") = "
             << $mu[$i]
             << ";\n"
        ;
    }
    $out << "\nОптимальная альтернатива - x"
         << $result.first
         << ", phi_н.д.(x"
         << $result.first
         << ") = "
         << $result.second
    ;
    $out.close();

    cout << MSG_COMPLETED;

    return 0;
}
