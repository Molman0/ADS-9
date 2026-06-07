// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <vector>

using namespace std;
using namespace chrono;

int main() {
    const int MAX_N = 20;
    const int K = 10;

    random_device rd;
    mt19937 gen(rd());

    cout << "n\tgetAllPerms\tgetPerm1\tgetPerm2\n";

    for (int n = 1; n <= MAX_N; ++n) {
        vector<char> in;
        for (int i = 0; i < n; ++i) in.push_back('1' + i);

        PMTree tree(in);
        long long total = tree.getTotalPerms();
        if (total == 0) continue;

        auto start = steady_clock::now();
        auto all = getAllPerms(tree);
        auto end = steady_clock::now();
        long long time_all = duration_cast<microseconds>(end - start).count();

        vector<int> rand_nums(K);
        uniform_int_distribution<int> dist(1, total);
        for (int i = 0; i < K; ++i) rand_nums[i] = dist(gen);

        long long time1_sum = 0;
        for (int num : rand_nums) {
            start = steady_clock::now();
            auto perm = getPerm1(tree, num);
            end = steady_clock::now();
            time1_sum += duration_cast<microseconds>(end - start).count();
        }
        double avg1 = time1_sum / (double)K;

        long long time2_sum = 0;
        for (int num : rand_nums) {
            start = steady_clock::now();
            auto perm = getPerm2(tree, num);
            end = steady_clock::now();
            time2_sum += duration_cast<microseconds>(end - start).count();
        }
        double avg2 = time2_sum / (double)K;

        cout << n << "\t" << time_all << "\t\t" << fixed << setprecision(2) << avg1 << "\t\t\t" << avg2 << "\n";
    }
    return 0;
}
