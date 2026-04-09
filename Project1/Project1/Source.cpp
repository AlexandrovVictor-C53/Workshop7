#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// Структура для хранения пары (x, y)
struct Point {
    double x, y;
};

// Функция вычисления параметров прямой методом парных точек
// Вход: вектор точек (x, y) – должен иметь чётное число элементов
// Выход: a (угловой коэффициент), da (погрешность a), b (свободный член), db (погрешность b)
void pairedPoints(const vector<Point>& points, double& a, double& da, double& b, double& db) {
    int n = points.size();
    if (n % 2 != 0 || n < 2) {
        cerr << "Ошибка: число точек должно быть чётным и не менее 2" << endl;
        exit(1);
    }
    int m = n / 2; // количество пар
    vector<double> a_i(m);
    // 1) Вычисляем a_i для каждой пары (первая половина с последней)
    for (int i = 0; i < m; ++i) {
        double x1 = points[i].x;
        double y1 = points[i].y;
        double x2 = points[n - 1 - i].x;
        double y2 = points[n - 1 - i].y;
        a_i[i] = (y2 - y1) / (x2 - x1);
    }
    // 2) Среднее a_i
    double sum_a = 0;
    for (double ai : a_i) sum_a += ai;
    a = sum_a / m;
    // 3) Погрешность a (стандартное отклонение среднего)
    double sum_sq = 0;
    for (double ai : a_i) sum_sq += (ai - a) * (ai - a);
    double sigma_a = sqrt(sum_sq / (m - 1));
    double t_student = 2.0; // для малого m (можно взять 2.0 для приближения)
    da = t_student * sigma_a;
    // 4) Вычисляем b = ȳ - a·x̄
    double sum_x = 0, sum_y = 0;
    for (const auto& p : points) {
        sum_x += p.x;
        sum_y += p.y;
    }
    double x_mean = sum_x / n;
    double y_mean = sum_y / n;
    b = y_mean - a * x_mean;
    // 5) Погрешность b (упрощённо)
    double sigma_b = da * x_mean;
    db = sigma_b;
}

int main() {
    // Параметры установки
    const double R3 = 12.0;      // Ом
    const double T = 298.15;     // K
    const double delta_T = 0.5;  // K

    // Чтение данных из файла: два столбца (Uэб, Uкб)
    ifstream fin("data.txt");
    if (!fin) {
        cerr << "Ошибка: не найден файл data.txt" << endl;
        return 1;
    }
    vector<Point> points;
    double Ueb, Ukb;
    while (fin >> Ueb >> Ukb) {
        points.push_back({ Ueb, Ukb });
    }
    fin.close();
    int n = points.size();
    if (n == 0) {
        cerr << "Файл пуст" << endl;
        return 1;
    }

    // Преобразуем: x = Uэб, y = ln(Ik), где Ik = Ukb / R3 (переводим в мА)
    vector<Point> logPoints;
    for (const auto& p : points) {
        double Ik = p.y / R3;           // в мА (если Ukb в В, то Ik в А, но для логарифма без разницы)
        double lnIk = log(Ik);          // натуральный логарифм
        logPoints.push_back({ p.x, lnIk });
    }

    // Вычисление методом парных точек
    double a, da, b, db;
    pairedPoints(logPoints, a, da, b, db);

    // Результаты
    cout << fixed << setprecision(6);
    cout << "Лабораторная работа №7" << endl;
    cout << "T = " << T << " K, ΔT = " << delta_T << " K" << endl;
    cout << "R3 = " << R3 << " Ом" << endl;
    cout << "Метод парных точек (n = " << n << " точек, " << n / 2 << " пар)" << endl;
    cout << "Угловой коэффициент tgα = a = " << a << " ± " << da << endl;
    cout << "Свободный член b = " << b << " ± " << db << endl;

    // Вычисление e/k
    double e_over_k = T * a;
    double delta_e_over_k = sqrt(a * a * (delta_T * delta_T / 9.0) + T * T * da * da);
    cout << "e/k = " << e_over_k << " ± " << delta_e_over_k << " Кл/К" << endl;

    // Дополнительно: вывод таблицы 2 (парные точки) для отчёта
    cout << "\nТаблица парных точек:" << endl;
    int m = n / 2;
    cout << "№\tx1\tx2\ty1\ty2\ta_i" << endl;
    for (int i = 0; i < m; ++i) {
        double x1 = logPoints[i].x;
        double y1 = logPoints[i].y;
        double x2 = logPoints[n - 1 - i].x;
        double y2 = logPoints[n - 1 - i].y;
        double ai = (y2 - y1) / (x2 - x1);
        cout << i + 1 << "\t" << x1 << "\t" << x2 << "\t" << y1 << "\t" << y2 << "\t" << ai << endl;
    }
    cout << "Среднее a = " << a << ",  Δa = " << da << endl;

    return 0;
}
