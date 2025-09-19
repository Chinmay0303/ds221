#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <cmath>

using namespace std;

class TestCaseGenerator {
private:
    mt19937 rng;
    
public:
    TestCaseGenerator() : rng(random_device{}()) {}
    
    // Generate a random even number in range [min_val, max_val]
    int randomEven(int min_val, int max_val) {
        int val = uniform_int_distribution<int>(min_val/2, max_val/2)(rng) * 2;
        return val;
    }
    
    // Check if graph is connected using BFS
    bool isConnected(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> adj(n + 1);
        for (const auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        
        vector<bool> visited(n + 1, false);
        queue<int> q;
        q.push(1);
        visited[1] = true;
        int count = 1;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                    count++;
                }
            }
        }
        
        return count == n;
    }
    
    // Generate a connected graph
    vector<vector<int>> generateConnectedGraph(int n, int m) {
        vector<vector<int>> edges;
        
        // Ensure m is not too large to avoid infinite loops
        long long max_possible_edges = (long long)n * (n - 1) / 2;
        m = min(m, (int)min(max_possible_edges, 200000LL));
        
        // First, create a spanning tree to ensure connectivity
        vector<int> nodes;
        for (int i = 1; i <= n; i++) {
            nodes.push_back(i);
        }
        shuffle(nodes.begin(), nodes.end(), rng);
        
        // Create spanning tree
        for (int i = 1; i < n; i++) {
            int weight = randomEven(2, 1000);
            edges.push_back({nodes[i-1], nodes[i], weight});
        }
        
        // For very large graphs, use a more memory-efficient approach
        if (n > 5000) {
            // For large graphs, use unordered_set for better performance
            unordered_set<long long> edge_set;
            for (const auto& edge : edges) {
                int u = min(edge[0], edge[1]);
                int v = max(edge[0], edge[1]);
                edge_set.insert((long long)u * 1000000LL + v);
            }
            
            int attempts = 0;
            int remaining_edges = max(0, m - (int)edges.size());
            int max_attempts = min(remaining_edges * 100, 2000000);
            
            while (edges.size() < m && attempts < max_attempts) {
                int u = uniform_int_distribution<int>(1, n)(rng);
                int v = uniform_int_distribution<int>(1, n)(rng);
                
                if (u != v) {
                    int min_uv = min(u, v);
                    int max_uv = max(u, v);
                    long long edge_key = (long long)min_uv * 1000000LL + max_uv;
                    
                    if (edge_set.find(edge_key) == edge_set.end()) {
                        int weight = randomEven(2, 1000);
                        edges.push_back({u, v, weight});
                        edge_set.insert(edge_key);
                    }
                }
                attempts++;
            }
        } else {
            // For smaller graphs, use the original approach with set
            set<pair<int, int>> edge_set;
            for (const auto& edge : edges) {
                edge_set.insert({min(edge[0], edge[1]), max(edge[0], edge[1])});
            }
            
            int attempts = 0;
            int remaining_edges = max(0, m - (int)edges.size());
            int max_attempts = min(1000000, remaining_edges * 10);

            while (edges.size() < m && attempts < max_attempts) {
                int u = uniform_int_distribution<int>(1, n)(rng);
                int v = uniform_int_distribution<int>(1, n)(rng);
                
                if (u != v) {
                    pair<int, int> edge_pair = {min(u, v), max(u, v)};
                    if (edge_set.find(edge_pair) == edge_set.end()) {
                        int weight = randomEven(2, 1000);
                        edges.push_back({u, v, weight});
                        edge_set.insert(edge_pair);
                    }
                }
                attempts++;
            }
        }
        
        return edges;
    }
    
    // Generate test case 1: Simple connected graph
    void generateTestCase1(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        file << "1 2 4\n";
        file << "2 3 6\n";
        file << "metro_cities:\n";
        file << "2\n";
        file.close();
        cout << "Generated test case 1 (simple): " << filename << endl;
    }
    
    // Generate test case 2: Disconnected graph (should return -1)
    void generateTestCase2(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        file << "1 2 4\n";
        file << "3 4 6\n";
        file << "metro_cities:\n";
        file << "2\n";
        file.close();
        cout << "Generated test case 2 (disconnected): " << filename << endl;
    }
    
    // Generate test case 3: Linear path with metro cities
    void generateTestCase3(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        for (int i = 1; i < 6; i++) {
            file << i << " " << (i + 1) << " " << randomEven(4, 12) << "\n";
        }
        file << "metro_cities:\n";
        file << "3\n";
        file.close();
        cout << "Generated test case 3 (linear): " << filename << endl;
    }
    
    // Generate test case 4: Star graph with center as metro city
    void generateTestCase4(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        int center = 3;
        for (int i = 1; i <= 6; i++) {
            if (i != center) {
                file << center << " " << i << " " << randomEven(4, 10) << "\n";
            }
        }
        file << "metro_cities:\n";
        file << center << "\n";
        file.close();
        cout << "Generated test case 4 (star): " << filename << endl;
    }
    
    // Generate test case 5: Complete small graph
    void generateTestCase5(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        int n = 4;
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                file << i << " " << j << " " << randomEven(2, 8) << "\n";
            }
        }
        file << "metro_cities:\n";
        file << "2\n3\n";
        file.close();
        cout << "Generated test case 5 (complete): " << filename << endl;
    }
    
    // Generate test case 8: Large random connected graph
    void generateLargeCase(const string& filename) {
        int n = uniform_int_distribution<int>(50, 100)(rng);
        int m = uniform_int_distribution<int>(n, min(200, n * (n - 1) / 2))(rng);
        int k = uniform_int_distribution<int>(1, min(10, n))(rng);
        
        vector<vector<int>> edges = generateConnectedGraph(n, m);
        
        // Select random metro cities
        set<int> metro_set;
        while (metro_set.size() < k) {
            int city = uniform_int_distribution<int>(1, n)(rng);
            metro_set.insert(city);
        }
        
        ofstream file(filename);
        file << "edges:\n";
        for (const auto& edge : edges) {
            file << edge[0] << " " << edge[1] << " " << edge[2] << "\n";
        }
        file << "metro_cities:\n";
        for (int city : metro_set) {
            file << city << "\n";
        }
        file.close();
        cout << "Generated large test case: " << filename 
             << " (n=" << n << ", m=" << m << ", k=" << k << ")" << endl;
    }
    
    // Generate random test case with given constraints
    void generateRandomCase(const string& filename, int min_n, int max_n, int min_m_factor, int max_m_factor) {
        int n = uniform_int_distribution<int>(min_n, max_n)(rng);
        
        // Calculate m based on factors of n to control density
        int min_m = max(n - 1, (int)(n * min_m_factor / 10.0)); // Ensure connectivity
        
        // Be more conservative with edge count for large graphs
        long long max_theoretical_m = (long long)n * (n - 1) / 2;
        int max_practical_m;
        
        if (n > 10000) {
            // For very large graphs, keep it very sparse
            max_practical_m = min((int)(n * 3), 100000);
        } else if (n > 1000) {
            // For large graphs, moderate density
            max_practical_m = min((int)(n * max_m_factor / 10.0), 200000);
        } else {
            // For smaller graphs, can afford higher density
            max_practical_m = min((int)(n * max_m_factor / 10.0), min(200000, (int)max_theoretical_m));
        }
        
        int m = uniform_int_distribution<int>(min_m, max_practical_m)(rng);
        
        // k varies from 1 to reasonable limit based on graph size
        int max_k = min(n, max(1, min((int)sqrt(n), n/10)));
        int k = uniform_int_distribution<int>(1, max_k)(rng);
        
        cout << "Generating graph with n=" << n << ", target m=" << m << ", k=" << k << "..." << endl;
        
        try {
            vector<vector<int>> edges = generateConnectedGraph(n, m);
            
            // Select random metro cities
            set<int> metro_set;
            
            // Add some random cities first
            while (metro_set.size() < k) {
                int city = uniform_int_distribution<int>(1, n)(rng);
                metro_set.insert(city);
            }
            
            ofstream file(filename);
            if (!file.is_open()) {
                cout << "Error: Could not create file " << filename << endl;
                return;
            }
            
            file << "edges:\n";
            for (const auto& edge : edges) {
                file << edge[0] << " " << edge[1] << " " << edge[2] << "\n";
            }
            file << "metro_cities:\n";
            for (int city : metro_set) {
                file << city << "\n";
            }
            file.close();
            
            cout << "Successfully generated: " << filename 
                 << " (n=" << n << ", m=" << edges.size() << ", k=" << k << ")" << endl;
        } catch (const exception& e) {
            cout << "Error generating " << filename << ": " << e.what() << endl;
            // Create a minimal fallback test case
            ofstream file(filename);
            file << "edges:\n1 " << n << " 2\n";
            file << "metro_cities:\n1\n";
            file.close();
            cout << "Created fallback test case for " << filename << endl;
        }
    }
    
    // Generate test case 6: Bottleneck graph (tests if algorithm finds optimal meeting point)
    void generateTestCase6(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        // Create a bottleneck graph: 1-2-3-4-5 where 3 is the bottleneck
        file << "1 2 2\n";
        file << "2 3 2\n";
        file << "3 4 2\n";
        file << "4 5 2\n";
        // Add some branches to make it more interesting
        file << "2 6 4\n";
        file << "4 7 4\n";
        file << "metro_cities:\n";
        file << "3\n6\n";
        file.close();
        cout << "Generated test case 6 (bottleneck): " << filename << endl;
    }
    
    // Generate test case 7: Multiple metro cities on optimal paths
    void generateTestCase7(const string& filename) {
        ofstream file(filename);
        file << "edges:\n";
        // Create a graph where multiple metro cities affect the optimal solution
        file << "1 2 10\n";
        file << "2 3 10\n";
        file << "3 4 10\n";
        file << "4 5 10\n";
        file << "1 6 8\n";
        file << "6 7 8\n";
        file << "7 5 8\n";
        file << "metro_cities:\n";
        file << "2\n3\n6\n7\n";
        file.close();
        cout << "Generated test case 7 (multi-metro): " << filename << endl;
    }
};

int main() {
    TestCaseGenerator generator;
    
    cout << "Generating test cases for Question 3 (Logistics Problem)...\n\n";
    
    // Generate edge case test cases (1-7)
    generator.generateTestCase1("test_case_1.txt");
    generator.generateTestCase2("test_case_2.txt");
    generator.generateTestCase3("test_case_3.txt");
    generator.generateTestCase4("test_case_4.txt");
    generator.generateTestCase5("test_case_5.txt");
    generator.generateTestCase6("test_case_6.txt");
    generator.generateTestCase7("test_case_7.txt");
    
    // Generate a few medium-sized test cases (8-10)
    for (int i = 8; i <= 10; i++) {
        generator.generateLargeCase("test_case_" + to_string(i) + ".txt");
    }
    
    cout << "\nGenerating 100 random test cases for empirical analysis...\n";
    
    // Generate 100 random test cases with varying sizes and densities
    for (int i = 11; i <= 110; i++) {
        string filename = "test_case_" + to_string(i) + ".txt";
        
        if (i <= 30) {
            // Small graphs (n: 2-100, sparse to medium density)
            generator.generateRandomCase(filename, 2, 100, 10, 30);
        } else if (i <= 60) {
            // Medium graphs (n: 100-1000, various densities)
            generator.generateRandomCase(filename, 100, 1000, 10, 50);
        } else if (i <= 85) {
            // Large graphs (n: 1000-5000, sparse to medium density)
            generator.generateRandomCase(filename, 1000, 5000, 5, 15);
        } else if (i <= 100) {
            // Very large graphs (n: 5000-15000, sparse)
            generator.generateRandomCase(filename, 5000, 15000, 2, 8);
        } else {
            // Extreme cases (n: 15000-30000, very sparse)
            generator.generateRandomCase(filename, 15000, 30000, 1, 3);
        }
        
        if (i % 10 == 0) {
            cout << "Generated " << (i - 10) << " random test cases so far..." << endl;
        }
    }
    
    cout << "\nAll test cases generated successfully!\n";
    cout << "\nTest case descriptions:\n";
    cout << "1-7: Edge cases (simple, disconnected, linear, star, complete, bottleneck, multi-metro)\n";
    cout << "8-10: Medium-sized random connected graphs\n";
    cout << "11-30: Small graphs (n: 2-100) for basic testing\n";
    cout << "31-60: Medium graphs (n: 100-1000) for performance analysis\n";
    cout << "61-85: Large graphs (n: 1000-5000) for scalability testing\n";
    cout << "86-100: Very large graphs (n: 5000-15000) for stress testing\n";
    cout << "101-110: Extreme cases (n: 15000-30000) for maximum constraint testing\n";
    
    return 0;
}
