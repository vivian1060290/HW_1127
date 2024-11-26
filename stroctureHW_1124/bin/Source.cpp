#include <iostream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

class Polynomial {
private:
    map<int, int> terms; // map 的 key 表示次方，value 表示該次方的係數

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
            }
            else if (poly[i] == '+') {
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
            }
            else if (hasX) {
                exp = 1; // 若無次方符號，默認次方為 1
            }

            // 若無 x，次方為 0
            if (!hasX) exp = 0;

            // 更新 map
            terms[exp] += coef;
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
            result.terms[it.first] += it.second;
        }
        return result;
    }

    // 乘法
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (auto it1 : terms) {
            for (auto it2 : other.terms) {
                int exp = it1.first + it2.first;
                int coef = it1.second * it2.second;
                result.terms[exp] += coef;
            }
        }
        return result;
    }

    // 將多項式轉為字串格式
    string toString() const {
        string result = "";
        for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
            int coef = it->second;
            int exp = it->first;

            if (coef == 0) continue; // 忽略係數為 0 的項

            // 處理正負號
            if (!result.empty()) {
                if (coef > 0) result += " + ";
                else result += " - ";
            }
            else {
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
};

int main() {
    string poly1, poly2;

    cout << "輸入第一個多項式: ";
    getline(cin, poly1);
    cout << "輸入第二個多項式: ";
    getline(cin, poly2);

    Polynomial p1(poly1);
    Polynomial p2(poly2);

    Polynomial sum = p1 + p2;
    Polynomial product = p1 * p2;

    cout << "加法結果: " << sum.toString() << endl;
    cout << "乘法結果: " << product.toString() << endl;

    return 0;
}
