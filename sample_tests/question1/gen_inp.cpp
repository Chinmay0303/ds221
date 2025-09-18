#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <set>

using namespace std;

// Generates a single test case and writes to filename
void generate_test_case(const string& filename, int n, int max_id, int max_weight, int duplicate_count) {
    ofstream fout(filename);
    fout << "id,weight\n";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> id_dist(0, max_id);
    uniform_int_distribution<> weight_dist(0, max_weight);

    vector<pair<int, int>> parcels;
    set<int> used_ids;

    // Generate some duplicates
    for (int i = 0; i < duplicate_count; ++i) {
        int id = id_dist(gen);
        int w1 = weight_dist(gen);
        int w2 = weight_dist(gen);
        parcels.push_back({id, w1});
        parcels.push_back({id, w2});
        used_ids.insert(id);
    }

    // Generate remaining unique parcels
    while (parcels.size() < n) {
        int id = id_dist(gen);
        if (used_ids.count(id)) continue; // avoid more duplicates
        int w = weight_dist(gen);
        parcels.push_back({id, w});
        used_ids.insert(id);
    }

    // Write parcels to file
    for (auto& p : parcels) {
        fout << p.first << "," << p.second << "\n";
    }
    fout.close();
}

int main() {
    // Test case parameters
    // give 100 sizes
    vector<int> sizes = {10,100,1000}; // You can add larger sizes for stress testing
    for (int i = 10000; i < 1010000; i=i+10000) {
        sizes.push_back(i); // from 100k to 10M
    }
    int max_id = 1000000;
    int max_weight = 1000000;

    for (size_t i = 0; i < sizes.size(); ++i) {
        //change filename to store in sample_tests/question1

        string filename = "./sample_tests/question1/test_case_" + to_string(i+1) + ".txt";

        int duplicate_count;

        // generate some edge cases
        // if (i == 0) duplicate_count = sizes[i]/2 + 1; // 50% duplicates for smallest
        // if (i == sizes.size() - 1) duplicate_count = 0; // no duplicates for largest

        // else randomly generate from 0% to 50%
        duplicate_count = rand() % (sizes[i]/2 + 1);

        generate_test_case(filename, sizes[i], max_id, max_weight, duplicate_count);
        cout << "Generated " << filename << " with " << sizes[i] << " parcels.\n";
    }
    return 0;
}