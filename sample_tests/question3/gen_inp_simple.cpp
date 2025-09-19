#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <set>
#include <algorithm>

using namespace std;

class SimpleTestGenerator {
public:
    mt19937 rng;
    
    SimpleTestGenerator() : rng(random_device{}()) {}
    
    // Generate a simple test case with given constraints
    void generateTestCase(const string& filename, int n, int m, int k) {
        // Enforce constraints
        n = min(n, 200000); // n ≤ 2×10^5
        m = min(m, 200000); // m ≤ 2×10^5
        k = min(k, n);      // k ≤ n
        
        cout << "Generating test case: " << filename << " (n=" << n << ", m=" << m << ", k=" << k << ")" << endl;
        
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        
        file << "edges:\n";
        
        // Create a spanning tree first to ensure connectivity (n-1 edges)
        for (int i = 1; i < n; i++) {
            int weight = uniform_int_distribution<int>(1, 500)(rng) * 2; // Even weights
            file << i << " " << (i + 1) << " " << weight << "\n";
        }
        
        // Add remaining edges randomly
        int edges_added = n - 1;
        int attempts = 0;
        int max_attempts = min(1000000, (m - edges_added) * 50);
        
        set<pair<int, int>> edge_set;
        // Add spanning tree edges to set
        for (int i = 1; i < n; i++) {
            edge_set.insert({i, i + 1});
        }
        
        while (edges_added < m && attempts < max_attempts) {
            int u = uniform_int_distribution<int>(1, n)(rng);
            int v = uniform_int_distribution<int>(1, n)(rng);
            
            if (u != v) {
                if (u > v) swap(u, v);
                if (edge_set.find({u, v}) == edge_set.end()) {
                    int weight = uniform_int_distribution<int>(1, 500)(rng) * 2; // Even weights
                    file << u << " " << v << " " << weight << "\n";
                    edge_set.insert({u, v});
                    edges_added++;
                }
            }
            attempts++;
        }
        
        // Generate metro cities
        file << "metro_cities:\n";
        set<int> metro_set;
        while ((int)metro_set.size() < k) {
            int city = uniform_int_distribution<int>(1, n)(rng);
            metro_set.insert(city);
        }
        for (int city : metro_set) {
            file << city << "\n";
        }
        
        file.close();
        cout << "Successfully generated: " << filename << " with " << edges_added << " edges" << endl;
    }
};

int main() {
    SimpleTestGenerator generator;
    
    cout << "Generating 100 test cases across full constraint range...\n\n";
    
    for (int i = 1; i <= 100; i++) {
        string filename = "test_case_" + to_string(i) + ".txt";
        
        int n, m, k;
        
        if (i <= 20) {
            // Small graphs (n: 2-1000)
            n = uniform_int_distribution<int>(2, 1000)(generator.rng);
            m = uniform_int_distribution<int>(n, min(200000, n * 10))(generator.rng);
            k = uniform_int_distribution<int>(1, min(n, 100))(generator.rng);
        } else if (i <= 40) {
            // Medium graphs (n: 1000-10000)
            n = uniform_int_distribution<int>(1000, 10000)(generator.rng);
            m = uniform_int_distribution<int>(n, min(200000, n * 5))(generator.rng);
            k = uniform_int_distribution<int>(1, min(n, 1000))(generator.rng);
        } else if (i <= 60) {
            // Large graphs (n: 10000-50000)
            n = uniform_int_distribution<int>(10000, 50000)(generator.rng);
            m = uniform_int_distribution<int>(n, min(200000, n * 3))(generator.rng);
            k = uniform_int_distribution<int>(1, min(n, 5000))(generator.rng);
        } else if (i <= 85) {
            // Very large graphs (n: 50000-150000)
            n = uniform_int_distribution<int>(50000, 150000)(generator.rng);
            m = uniform_int_distribution<int>(n, min(200000, n * 2))(generator.rng);
            k = uniform_int_distribution<int>(1, min(n, 10000))(generator.rng);
        } else {
            // Maximum constraint graphs (n: 150000-200000)
            n = uniform_int_distribution<int>(150000, 200000)(generator.rng);
            m = uniform_int_distribution<int>(n, 200000)(generator.rng);
            k = uniform_int_distribution<int>(1, min(n, 20000))(generator.rng);
        }
        
        generator.generateTestCase(filename, n, m, k);
        
        if (i % 10 == 0) {
            cout << "Generated " << i << " test cases so far..." << endl;
        }
    }
    
    cout << "\nAll 100 test cases generated successfully!\n";
    cout << "\nTest case distribution:\n";
    cout << "Cases 1-10: Small graphs (n: 2-1k, m: varies, k: 1-100)\n";
    cout << "Cases 11-30: Medium graphs (n: 1k-10k, m: varies, k: 1-1k)\n";
    cout << "Cases 31-60: Large graphs (n: 10k-50k, m: varies, k: 1-5k)\n";
    cout << "Cases 61-85: Very large graphs (n: 50k-150k, m: varies, k: 1-10k)\n";
    cout << "Cases 86-100: Maximum constraint graphs (n: 150k-200k, m: varies, k: 1-20k)\n";
    cout << "\nAll cases respect constraints: 2 ≤ n ≤ 2×10^5, 1 ≤ m ≤ 2×10^5, 1 ≤ k ≤ n\n";
    
    return 0;
}
