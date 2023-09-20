#include "iostream"
#include "sstream"
#include "vector"
#include "cmath"
#include "string"
#include "fstream"
#include "functional"

using namespace std;

const string MSG_STARTED = "Finding a solution to the problem in the case of a matrix of paired comparisons on a set of attributes (experts)\n\n";
const string MSG_COMPLETED = "Results successfully saved to file\n";

const string MSG_READ_FILE = "Reading data from a file\n";

const string ERR_OPTIMAL_NOT_FOUND = "No optimal alternative found!\n";
const string ERR_INPUT_NOT_FOUND = "File input.txt not found\n";
const string ERR_OUTPUT_NOT_FOUND = "The file output.txt cannot be opened\n";
const string ERR_ZERO_DENOMINATOR = "Denominator cannot be zero\n";
const string ERR_EMPTY_STRING = "The line is empty\n";

/**
 * Greatest common divisor
 *
 * @param $a
 * @param $b
 *
 * @return greatest common divisor
 */
int gcd(int $a, int $b) {
    if ($a % $b == 0) return $b;
    if ($b % $a == 0) return $a;
    return $a > $b ? gcd($a % $b, $b) : gcd($a, $b % $a);
}

/**
 * Smallest common multiple
 *
 * @param $a
 * @param $b
 *
 * @return smallest common multiple
 */
int lcm(int $a, int $b) {
    return $a * $b / gcd($a, $b);
}

/**
 * Fractional number
 */
class FractionalNumber {
private:
    int numerator;
    int denominator;

    void reduce() {
        int _gcd = gcd(abs(numerator), denominator);
        if (_gcd != 1) {
            numerator = numerator / _gcd;
            denominator = denominator / _gcd;
        }
    }

public:
    explicit FractionalNumber(int _numerator = 1, int _denominator = 1)
            : numerator(_numerator), denominator(_denominator) {
        if (_denominator == 0) {
            cout << ERR_ZERO_DENOMINATOR;
        }
    }

    explicit FractionalNumber(string &string) {
        if (string.empty()) {
            cout << ERR_EMPTY_STRING;
        }

        int pos = string.find("/");
        if (pos == string::npos) {
            numerator = stoi(string);
            denominator = 1;
        } else {
            numerator = stoi(string.substr(0, pos));
            denominator = stoi(string.substr(pos + 1, string.length()));

            if (denominator == 0) {
                cout << ERR_ZERO_DENOMINATOR;
            }
        }
    }

    string toString() const {
        string fraction = "";
        if (numerator == 0) {
            fraction.append("0");
            return fraction;
        }

        fraction.append(to_string(numerator));
        if (denominator != 1) {
            fraction.append("/");
            fraction.append(to_string(denominator));
        }
        return fraction;
    }

    int getNumerator() const { return numerator; }

    int getDenominator() const { return denominator; }

    FractionalNumber &operator+(FractionalNumber &fraction) {
        int unionDenominator = lcm(denominator, fraction.getDenominator());
        int relNumerator = numerator * unionDenominator;
        int mulNumeration = fraction.numerator * unionDenominator;
        numerator = relNumerator * mulNumeration;
        denominator = unionDenominator;
        reduce();
        return *this;
    }

    FractionalNumber &operator-(FractionalNumber &fraction) {
        int relNumerator = numerator * fraction.getDenominator();
        numerator = numerator * fraction.getDenominator() - denominator * fraction.getNumerator();
        denominator = gcd(denominator, fraction.getDenominator());
        reduce();
        return *this;
    }

    FractionalNumber &operator*(FractionalNumber &fraction) {
        numerator = numerator * fraction.getNumerator();
        denominator = denominator * fraction.getDenominator();
        reduce();
        return *this;
    }

    FractionalNumber &operator/(FractionalNumber &fraction) {
        numerator = numerator * fraction.getDenominator();
        denominator = denominator * fraction.getNumerator();
        reduce();
        return *this;
    }
};

/**
 * Search Matrix priorities
 *
 * @param $matrix - original $matrix
 * @param $sizeMatrix - size of the matrix
 *
 * @return matrix priorities
 */
vector<float> priority(
        FractionalNumber** &$matrix,
        int $sizeMatrix
) {
    vector<float> $priorityVector;
    float prioritySum = 0.0;
    for (int i = 0; i < $sizeMatrix; i++) {
        FractionalNumber value;
        for (int j = 0; j < $sizeMatrix; j++) {
            value = value.operator*($matrix[i][j]);
        }
        $priorityVector.push_back(
                pow(float(value.getNumerator()) / float(value.getDenominator()), 1.0 / $sizeMatrix)
        );
        prioritySum += $priorityVector[i];
    }

    for (int i = 0; i < $sizeMatrix; i++) {
        $priorityVector[i] /= float(prioritySum);
    }

    return $priorityVector;
}

/**
 * Finding final priorities
 *
 * @param $ePriorities
 * @param $wPriorities
 *
 * @return final priorities
 */
vector<float> finalPriorities(vector<float> &$ePriorities, vector<vector<float>> &$wPriorities)
{
    vector<float> $finalPriorities($wPriorities[0].size());
    for (int i = 0; i < $finalPriorities.size(); i++) {
        $finalPriorities[i] = 0.0;
        for (int j = 0; j < $ePriorities.size(); j++) {
            $finalPriorities[i] += $ePriorities[j] * $wPriorities[j][i];
        }
    }

    return $finalPriorities;
}

/**
 * Searching optimal alternatives
 *
 * @param $finalPriorities - final priorities
 *
 * @return optimal alternatives
 */
vector<pair<int, float>> optimalAlternative(vector<float> &$finalPriorities)
{
    float $maxValue = $finalPriorities[0];

    vector<pair<int, float>> $optimalAlternative;
    $optimalAlternative.push_back(make_pair(1, $maxValue));
    for (int i = 1; i < $finalPriorities.size(); i++) {
        if ($maxValue <= $finalPriorities[i]) {
            if ($maxValue < $finalPriorities[i]) {
                $maxValue = $finalPriorities[i];
                $optimalAlternative.clear();
            }
            $optimalAlternative.push_back(make_pair(i + 1, $maxValue));
        }
    }

    return $optimalAlternative;
}

int main()
{
    setlocale(LC_ALL, "ru");

    cout << MSG_STARTED;

    ifstream $in("input.txt");
    if (!$in.is_open()) {
        cout << ERR_INPUT_NOT_FOUND;

        return 1;
    }

    /** 1 x $eMatrix 7x7 */
    int $eSize = 7;
    auto **$eMatrix = new FractionalNumber *[$eSize];
    for (int i = 0; i < $eSize; i++) {
        $eMatrix[i] = new FractionalNumber[$eSize];
    }

    /** 7 x $wMatrix 6x6 */
    int $wCount = 7, $wSize = 6;
    vector<FractionalNumber **> $wMatrix;
    for (int i = 0; i < $wCount; i++) {
        $wMatrix.push_back(new FractionalNumber *[$wSize]);
        for (int j = 0; j < $wSize; j++) {
            $wMatrix[i][j] = new FractionalNumber[$wSize];
        }
    }

    cout << MSG_READ_FILE;

    string $str, $row;
    int $i = 0, $j = 0, $k = -1;
    while (getline($in, $str)) {
        if ($str.empty()) {
            continue;
        }

        $row = "", $j = 0;
        istringstream $stream($str);
        if (($i - $eSize) % $wSize == 0 && $i >= $eSize) {
            $k++;
        }

        while ($stream >> $row) {
            if ($i < $eSize) {
                $eMatrix[$i][$j] = FractionalNumber($row);
            } else if ($k < $wCount) {
                $wMatrix[$k][($i - $eSize) % $wSize][$j] = FractionalNumber($row);
            }
            $j++;
        }
        $i++;
    }
    $in.close();

    /** Finding priorities for matrices */
    vector<float> $ePriority = priority($eMatrix, $eSize);
    vector<vector<float>> $wPriorities($wCount);
    for (int i = 0; i < $wCount; i++) {
        $wPriorities[i] = priority($wMatrix[i], $wSize);
    }


    /** Finding the final priorities of alternatives */
    vector<float> $finalPriorities = finalPriorities($ePriority, $wPriorities);

    /** Search for optimal alternatives */
    vector<pair<int, float>> $optimalPriority = optimalAlternative(ref($finalPriorities));

    /** Output to the response file */
    ofstream $out;
    $out.open("output.txt");
    if (!$out.is_open()) {
        cout << ERR_OUTPUT_NOT_FOUND;

        return 1;
    }

    if ($optimalPriority.empty()) {
        $out << ERR_OPTIMAL_NOT_FOUND;
    } else {
        $out << "Окончательные приоритеты альтернатив:\n";
        for ($i = 0; $i < $finalPriorities.size(); $i++) {
            $out << "w"
                + to_string($i + 1)
                + " = "
                + to_string(round($finalPriorities[$i] * pow(10, 6)) / pow(10, 6))
                + ";"
            ;
            if ($finalPriorities.size() - 1 != $i) {
                $out << " ";
            }
        }
        $out << "\nОтвет: оптимальная альтернатива";
        for ($i = 0; $i < $optimalPriority.size(); $i++) {
            if ($i > 0) {
                $out << ", ";
            }
            $out << " x"
                + to_string($optimalPriority[$i].first)
                + " с приоритетом w"
                + to_string($optimalPriority[$i].first)
                + " = "
                + to_string(round($optimalPriority[$i].second * pow(10, 6)) / pow(10, 6))
                + "."
            ;
        }
    }
    $out.close();
    cout << MSG_COMPLETED;

    return 0;
}
