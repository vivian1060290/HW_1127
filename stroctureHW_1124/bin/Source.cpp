#include <iostream>
#include <string>
#include <map>
#include <cmath> // 為了使用 pow()
using namespace std;

// 單項式類別
class Term {
public:
    int coef; // 係數
    int exp;  // 次方

    // 預設構造函數
    Term(int c = 0, int e = 0) : coef(c), exp(e) {}

    // 重載比較運算符（排序用）
    bool operator<(const Term& other) const {
        return exp < other.exp; // 按次方遞增排序
    }

    // 重載加法（合併同次方的項）
    Term operator+(const Term& other) const {
        if (exp == other.exp) {
            return Term(coef + other.coef, exp);
        }
        throw invalid_argument("Exponents do not match!");
    }

    // 重載乘法
    Term operator*(const Term& other) const {
        return Term(coef * other.coef, exp + other.exp);
    }

    // 判斷是否為 0 項
    bool isZero() const {
        return coef == 0;
    }
};

// 多項式類別
class Polynomial {
private:
    map<int, Term> terms; // map 的 key 表示次方，value 表示 Term 物件

    // 將字串解析為多項式
    void parse(const string& poly) {
        int i = 0;
        while (i < poly.length()) {
            int coef = 0, exp = 0;
            bool hasX = false, isNegative = false;

            // 處理正負號
            if (poly[i] == '-') {
                isNegative = true;
                i++;
            } else if (poly[i] == '+') {
                i++;
            }

            // 取得係數
            while (i < poly.length() && isdigit(poly[i])) {
                coef = coef * 10 + (poly[i] - '0');
                i++;
            }
            if (coef == 0) coef = 1; // 若係數省略，默認為 1
            if (isNegative) coef = -coef;

            // 檢查是否有 x
            if (i < poly.length() && poly[i] == 'x') {
                hasX = true;
                i++;
            }

            // 處理次方
            if (hasX && i < poly.length() && poly[i] == '^') {
                i++;
                while (i < poly.length() && isdigit(poly[i])) {
                    exp = exp * 10 + (poly[i] - '0');
                    i++;
                }
            } else if (hasX) {
                exp = 1; // 若無次方符號，默認次方為 1
            }

            // 若無 x，次方為 0
            if (!hasX) exp = 0;

            // 更新 map
            if (terms.count(exp)) {
                terms[exp].coef += coef; // 合併同次方項
            } else {
                terms[exp] = Term(coef, exp);
            }
        }
    }

public:
    // 默認構造函數
    Polynomial() {}

    // 從字串構造多項式
    Polynomial(const string& poly) {
        parse(poly);
    }

    // 加法
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        for (auto it : other.terms) {
            if (result.terms.count(it.first)) {
                result.terms[it.first] = result.terms[it.first] + it.second;
            } else {
                result.terms[it.first] = it.second;
            }
        }
        return result;
    }

    // 乘法
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (auto it1 : terms) {
            for (auto it2 : other.terms) {
                Term product = it1.second * it2.second;
                if (result.terms.count(product.exp)) {
                    result.terms[product.exp].coef += product.coef;
                } else {
                    result.terms[product.exp] = product;
                }
            }
        }
        return result;
    }

    // 將多項式轉為字串格式
    string toString() const {
        string result = "";
        for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
            int coef = it->second.coef;
            int exp = it->second.exp;

            if (coef == 0) continue; // 忽略係數為 0 的項

            // 處理正負號
            if (!result.empty()) {
                if (coef > 0) result += " + ";
                else result += " - ";
            } else {
                if (coef < 0) result += "-";
            }

            // 處理係數
            coef = abs(coef);
            if (coef != 1 || exp == 0) result += to_string(coef);

            // 處理次方
            if (exp > 0) result += "x";
            if (exp > 1) result += "^" + to_string(exp);
        }

        return result.empty() ? "0" : result;
    }

    // Eval 函式：計算多項式在某一 x 值下的值
    int Eval(int x) const {
        int result = 0;
        for (auto it : terms) {
            result += it.second.coef * pow(x, it.second.exp);
        }
        return result;
    }

    // 運算子 >> 輸入多項式
    friend istream& operator>>(istream& is, Polynomial& p) {
        string poly;
        getline(is, poly);    // 讀入字串
        p = Polynomial(poly); // 呼叫解析建構函式
        return is;
    }

    // 運算子 << 輸出多項式
    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        os << p.toString(); // 使用 toString 格式化
        return os;
    }
};

int main() {
    Polynomial p1, p2;

    cout << "輸入第一個多項式: ";
    cin >> p1;
    cout << "輸入第二個多項式: ";
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;

    cout << "加法結果: " << sum << endl;
    cout << "乘法結果: " << product << endl;

    int x;
    cout << "輸入 x 的值來計算多項式 p1 的結果: ";
    cin >> x;
    cout << "p1 在 x = " << x << " 時的值為: " << p1.Eval(x) << endl;

    return 0;
}
