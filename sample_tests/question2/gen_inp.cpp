#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <fstream>

using namespace std;

class TestCaseGenerator {
private:
    mt19937 rng;
    
    // Generate a random binary tree with n nodes and return preorder/inorder traversals
    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    };
    
    // Generate a complete binary tree structure
    TreeNode* generateTree(int n) {
        if (n == 0) return nullptr;
        
        vector<TreeNode*> nodes;
        for (int i = 0; i < n; i++) {
            nodes.push_back(new TreeNode(i));
        }
        
        // Build complete binary tree structure
        for (int i = 0; i < n; i++) {
            if (2 * i + 1 < n) nodes[i]->left = nodes[2 * i + 1];
            if (2 * i + 2 < n) nodes[i]->right = nodes[2 * i + 2];
        }
        
        return nodes[0];
    }
    
    void preorderTraversal(TreeNode* root, vector<int>& preorder) {
        if (!root) return;
        preorder.push_back(root->val);
        preorderTraversal(root->left, preorder);
        preorderTraversal(root->right, preorder);
    }
    
    void inorderTraversal(TreeNode* root, vector<int>& inorder) {
        if (!root) return;
        inorderTraversal(root->left, inorder);
        inorder.push_back(root->val);
        inorderTraversal(root->right, inorder);
    }
    
    void getLeafNodes(TreeNode* root, vector<int>& leaves) {
        if (!root) return;
        if (!root->left && !root->right) {
            leaves.push_back(root->val);
            return;
        }
        getLeafNodes(root->left, leaves);
        getLeafNodes(root->right, leaves);
    }
    
    // Generate random parcels for leaf nodes
    vector<vector<int>> generateLeafParcels(const vector<int>& leaves, int maxParcelsPerLeaf) {
        vector<vector<int>> leafParcels(leaves.size());
        unordered_set<int> usedParcelIds;
        int nextParcelId = 1;
        
        for (int i = 0; i < leaves.size(); i++) {
            int numParcels = uniform_int_distribution<int>(1, maxParcelsPerLeaf)(rng);
            
            for (int j = 0; j < numParcels; j++) {
                leafParcels[i].push_back(nextParcelId++);
            }
        }
        
        return leafParcels;
    }
    
    // Generate queries ensuring some overlap and some disjoint cases
    vector<vector<int>> generateQueries(const vector<vector<int>>& leafParcels, int numQueries) {
        vector<vector<int>> queries;
        vector<int> allParcels;
        
        // Collect all parcels
        for (const auto& leaf : leafParcels) {
            for (int parcel : leaf) {
                allParcels.push_back(parcel);
            }
        }
        
        for (int i = 0; i < numQueries; i++) {
            vector<int> query;
            int querySize = uniform_int_distribution<int>(1, min(10, (int)allParcels.size()))(rng);
            
            if (i % 4 == 0) {
                // Single leaf case - all parcels from one leaf
                int leafIdx = uniform_int_distribution<int>(0, leafParcels.size() - 1)(rng);
                int numFromLeaf = min(querySize, (int)leafParcels[leafIdx].size());
                for (int j = 0; j < numFromLeaf; j++) {
                    query.push_back(leafParcels[leafIdx][j]);
                }
            } else if (i % 4 == 1) {
                // Cross-leaf case - parcels from multiple leaves
                set<int> querySet;
                while (querySet.size() < querySize && querySet.size() < allParcels.size()) {
                    int parcel = allParcels[uniform_int_distribution<int>(0, allParcels.size() - 1)(rng)];
                    querySet.insert(parcel);
                }
                query.assign(querySet.begin(), querySet.end());
            } else if (i % 4 == 2) {
                // Mixed case - some existing, some non-existing
                set<int> querySet;
                int existingCount = querySize / 2;
                int nonExistingCount = querySize - existingCount;
                
                // Add existing parcels
                for (int j = 0; j < existingCount && !allParcels.empty(); j++) {
                    int parcel = allParcels[uniform_int_distribution<int>(0, allParcels.size() - 1)(rng)];
                    querySet.insert(parcel);
                }
                
                // Add non-existing parcels
                int maxParcel = *max_element(allParcels.begin(), allParcels.end());
                for (int j = 0; j < nonExistingCount; j++) {
                    querySet.insert(maxParcel + j + 1);
                }
                
                query.assign(querySet.begin(), querySet.end());
            } else {
                // Random case
                set<int> querySet;
                for (int j = 0; j < querySize; j++) {
                    int parcel = uniform_int_distribution<int>(1, allParcels.size() + 10)(rng);
                    querySet.insert(parcel);
                }
                query.assign(querySet.begin(), querySet.end());
            }
            
            queries.push_back(query);
        }
        
        return queries;
    }
    
public:
    TestCaseGenerator() : rng(random_device{}()) {}
    
    void generateTestCase(int n, int maxParcelsPerLeaf, int numQueries, const string& filename) {
        // Generate tree
        TreeNode* root = generateTree(n);
        
        // Get traversals
        vector<int> preorder, inorder;
        preorderTraversal(root, preorder);
        inorderTraversal(root, inorder);
        
        // Get leaf nodes
        vector<int> leaves;
        getLeafNodes(root, leaves);
        
        // Generate parcels for leaves
        vector<vector<int>> leafParcels = generateLeafParcels(leaves, maxParcelsPerLeaf);
        
        // Generate queries
        vector<vector<int>> queries = generateQueries(leafParcels, numQueries);
        
        // Write to file using ofstream
        ofstream fout(filename);
        fout << "preorder:";
        for (int val : preorder) fout << " " << val;
        fout << "\n";
        
        fout << "inorder:";
        for (int val : inorder) fout << " " << val;
        fout << "\n";
        
        fout << "parcel_on_leafs:\n";
        for (const auto& leaf : leafParcels) {
            for (int i = 0; i < leaf.size(); i++) {
                if (i > 0) fout << " ";
                fout << leaf[i];
            }
            fout << "\n";
        }
        
        fout << "queries:\n";
        for (const auto& query : queries) {
            for (int i = 0; i < query.size(); i++) {
                if (i > 0) fout << " ";
                fout << query[i];
            }
            fout << "\n";
        }
        fout.close();
    }
    
    void generateEdgeCases() {
        // Edge Case 1: Single node tree
        ofstream fout("test_case_1.txt");
        fout << "preorder: 0\n";
        fout << "inorder: 0\n";
        fout << "parcel_on_leafs:\n";
        fout << "1 2 3\n";
        fout << "queries:\n";
        fout << "1\n";
        fout << "1 2\n";
        fout << "4\n";
        fout.close();
        
        // Edge Case 2: Linear tree (only left children)
        fout.open("test_case_2.txt");
        fout << "preorder: 0 1 2 3\n";
        fout << "inorder: 3 2 1 0\n";
        fout << "parcel_on_leafs:\n";
        fout << "1 2 3\n";
        fout << "queries:\n";
        fout << "1\n";
        fout << "1 2 3\n";
        fout << "4\n";
        fout.close();
        
        // Edge Case 3: Complete binary tree
        fout.open("test_case_3.txt");
        fout << "preorder: 0 1 3 4 2 5 6\n";
        fout << "inorder: 3 1 4 0 5 2 6\n";
        fout << "parcel_on_leafs:\n";
        fout << "1 2 9\n";
        fout << "3 6\n";
        fout << "4 7\n";
        fout << "8 10 11\n";
        fout << "queries:\n";
        fout << "1 2\n";
        fout << "2 9 6\n";
        fout << "3 7 11\n";
        fout.close();
        
        // Edge Case 4: Large query size
        fout.open("test_case_4.txt");
        fout << "preorder: 0 1 2 3 4\n";
        fout << "inorder: 1 0 3 2 4\n";
        fout << "parcel_on_leafs:\n";
        fout << "1 2 3 4 5\n";
        fout << "6 7 8 9 10\n";
        fout << "11 12 13 14 15\n";
        fout << "queries:\n";
        fout << "1 2 3 4 5 6 7 8 9 10\n";
        fout << "1 6 11\n";
        fout << "100\n";
        fout.close();
    }
    
    void generateRandomTestCases(int numCases) {
        cout << "Generating " << numCases << " random test cases...\n";
        
        for (int i = 5; i <= numCases + 4; i++) {
            // Generate random parameters within constraints
            int n, maxParcelsPerLeaf, numQueries;
            
            if (i <= 14) {
                // Small cases (10-100 nodes)
                n = uniform_int_distribution<int>(10, 100)(rng);
                maxParcelsPerLeaf = uniform_int_distribution<int>(1, 10)(rng);
                numQueries = uniform_int_distribution<int>(1, 50)(rng);
            } else if (i <= 34) {
                // Medium cases (100-1000 nodes)
                n = uniform_int_distribution<int>(100, 1000)(rng);
                maxParcelsPerLeaf = uniform_int_distribution<int>(5, 25)(rng);
                numQueries = uniform_int_distribution<int>(10, 200)(rng);
            } else if (i <= 64) {
                // Large cases (1000-10000 nodes)
                n = uniform_int_distribution<int>(1000, 10000)(rng);
                maxParcelsPerLeaf = uniform_int_distribution<int>(10, 50)(rng);
                numQueries = uniform_int_distribution<int>(50, 1000)(rng);
            } else if (i < 104) {
                // Very large cases (10000-100000 nodes)
                n = uniform_int_distribution<int>(10000, 100000)(rng);
                maxParcelsPerLeaf = uniform_int_distribution<int>(20, 80)(rng);
                numQueries = uniform_int_distribution<int>(100, 5000)(rng);
            } else {
                // Maximum constraint cases (100000+ nodes)
                n = uniform_int_distribution<int>(100000, 1000000)(rng);
                maxParcelsPerLeaf = uniform_int_distribution<int>(30, 100)(rng);
                numQueries = uniform_int_distribution<int>(1000, 10000)(rng);
            }
            
            string filename = "test_case_" + to_string(i) + ".txt";
            generateTestCase(n, maxParcelsPerLeaf, numQueries, filename);
            
            if (i % 10 == 0) {
                cout << "Generated " << (i) << " test cases...\n";
            }
            if(i>100){
                cout << "Generated " << (i) << " test cases...\n";
            }
        }
    }
};

int main(int argc, char* argv[]) {
    TestCaseGenerator generator;
    
    if (argc > 1 && string(argv[1]) == "edge") {
        cout << "Generating edge cases...\n";
        generator.generateEdgeCases();
        cout << "Edge cases generated: test_case_1.txt to test_case_4.txt\n";
        return 0;
    }
    
    if (argc > 1 && string(argv[1]) == "random") {
        generator.generateRandomTestCases(100);
        cout << "Random test cases generated: test_case_5.txt to test_case_104.txt\n";
        return 0;
    }
    
    if (argc > 1 && string(argv[1]) == "all") {
        cout << "Generating all test cases...\n";
        generator.generateEdgeCases();
        generator.generateRandomTestCases(100);
        cout << "All test cases generated successfully!\n";
        cout << "Total files created: test_case_1.txt to test_case_104.txt\n";
        cout << "- Edge cases: test_case_1.txt to test_case_4.txt\n";
        cout << "- Random cases: test_case_5.txt to test_case_104.txt\n";
        return 0;
    }
    
    // Default: Generate small sample test cases
    cout << "Generating sample test cases...\n";
    cout << "Usage options:\n";
    cout << "- ./gen_inp edge      : Generate edge cases only\n";
    cout << "- ./gen_inp random    : Generate 100 random cases only\n";
    cout << "- ./gen_inp all       : Generate all test cases\n";
    cout << "\nGenerating edge cases by default...\n";
    
    generator.generateEdgeCases();
    cout << "Edge cases generated: test_case_1.txt to test_case_4.txt\n";
    
    return 0;
}
