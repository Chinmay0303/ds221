#ifndef USER_CODE_H
#define USER_CODE_H

// Feel free to include more library functions
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <functional>
#include <climits>
#include <tuple>

using namespace std;

// 1
// 1- Complete this function
// Abstract this question into math:
// Now give the optimised approach and algorithm to solve this problem in steps
// Write a function based on the below approach and algorithm:
// Approach:
// 1. Hash Map (Unordered Map)
// Algorithm:
// Traverse the list, for each parcel ID, store the count and minimum weight in a hash map.
// After traversal, collect IDs with count > 1 and their minimum weights, then sort by ID.

// vector<vector<int>> question_one(const vector<vector<int>>& parcels) {
//     unordered_map<int, pair<int, int>> parcel_map;  // ID -> (count, min_weight)

//     for (const auto& parcel : parcels) {
//         int id = parcel[0];
//         int weight = parcel[1];

//         if (parcel_map.find(id) == parcel_map.end()) {
//             parcel_map[id] = {1, weight};  // First occurrence
//         } else {
//             parcel_map[id].first++;  // Increment count
//             parcel_map[id].second = min(parcel_map[id].second, weight);  // Update min weight
//         }
//     }

//     vector<vector<int>> result;
//     for (const auto& entry : parcel_map) {
//         if (entry.second.first > 1) {  // Count > 1
//             result.push_back({entry.first, entry.second.second});
//         }
//     }

//     // Sort by ID
//     sort(result.begin(), result.end());

//     return result;
//     // return parcels;
// }

// Now Write a function based on the below approach and algorithm:
// Approach:
// 2. Sorting + Linear Scan
// Algorithm:
// Sort the parcels by ID.
// Scan the sorted list, track duplicates and minimum weights.

// vector<vector<int>> question_one(const vector<vector<int>>& parcels) {
//     // Step 1: Sort the parcels by ID
//     vector<vector<int>> sorted_parcels = parcels;
//     sort(sorted_parcels.begin(), sorted_parcels.end());

//     vector<vector<int>> result;
//     int n = sorted_parcels.size();

//     // Step 2: Scan the sorted list
//     for (int i = 0; i < n; i++) {
//         int id = sorted_parcels[i][0];
//         int min_weight = sorted_parcels[i][1];
//         int count = 1;

//         // Count duplicates
//         while (i + 1 < n && sorted_parcels[i + 1][0] == id) {
//             count++;
//             min_weight = min(min_weight, sorted_parcels[i + 1][1]);
//             i++;
//         }

//         // If duplicates found, add to result
//         if (count > 1) {
//             result.push_back({id, min_weight});
//         }
//     }

//     return result;
// }


/* 
2-Complete this function
*/
// Abstract this question into math:
// Now give the optimised approach and algorithm to solve this problem in steps
// Okay now complete this function and give the best code according to the above discussion:

vector<int> question_two(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
) {
    int n = preorder.size();
    if (n == 0) return {};
    
    // Step 1: Build tree structure and create level-order mapping
    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    };
    
    // Helper function to build tree from preorder and inorder
    unordered_map<int, int> inorder_map;
    for (int i = 0; i < inorder.size(); i++) {
        inorder_map[inorder[i]] = i;
    }
    
    function<TreeNode*(int, int, int)> buildTree = [&](int pre_idx, int in_start, int in_end) -> TreeNode* {
        if (in_start > in_end) return nullptr;
        
        TreeNode* root = new TreeNode(preorder[pre_idx]);
        int in_idx = inorder_map[preorder[pre_idx]];
        
        root->left = buildTree(pre_idx + 1, in_start, in_idx - 1);
        root->right = buildTree(pre_idx + 1 + (in_idx - in_start), in_idx + 1, in_end);
        
        return root;
    };
    
    TreeNode* root = buildTree(0, 0, n - 1);
    
    // Step 2: Create level-order mapping (node value -> level-order index)
    unordered_map<int, int> levelOrderIndex;
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});
    
    while (!q.empty()) {
        auto [node, idx] = q.front();
        q.pop();
        
        levelOrderIndex[node->val] = idx;
        
        if (node->left) q.push({node->left, 2 * idx + 1});
        if (node->right) q.push({node->right, 2 * idx + 2});
    }
    
    // Step 3: Identify leaf nodes and their level-order indices
    vector<int> leafNodes;
    function<void(TreeNode*)> findLeaves = [&](TreeNode* node) {
        if (!node) return;
        if (!node->left && !node->right) {
            leafNodes.push_back(node->val);
        }
        findLeaves(node->left);
        findLeaves(node->right);
    };
    findLeaves(root);
    
    // Step 4: Build parcel sets for each node (bottom-up propagation)
    unordered_map<int, unordered_set<int>> nodeParcels;
    
    // Initialize leaf nodes with their parcels
    for (int i = 0; i < leafNodes.size() && i < leafParcels.size(); i++) {
        for (int parcel : leafParcels[i]) {
            nodeParcels[leafNodes[i]].insert(parcel);
        }
    }
    
    // Post-order traversal to propagate parcels upward
    function<void(TreeNode*)> propagateParcels = [&](TreeNode* node) {
        if (!node) return;
        
        propagateParcels(node->left);
        propagateParcels(node->right);
        
        // If it's an internal node, merge parcels from children
        if (node->left || node->right) {
            if (node->left) {
                for (int parcel : nodeParcels[node->left->val]) {
                    nodeParcels[node->val].insert(parcel);
                }
            }
            if (node->right) {
                for (int parcel : nodeParcels[node->right->val]) {
                    nodeParcels[node->val].insert(parcel);
                }
            }
        }
    };
    propagateParcels(root);
    
    // Step 5: Process queries
    vector<int> results;
    
    for (const auto& queryList : query) {
        int bestNode = -1;
        int bestIndex = -1;
        
        // DFS to find the highest-indexed node containing all query parcels
        function<void(TreeNode*)> findBestNode = [&](TreeNode* node) {
            if (!node) return;
            
            // Check if current node contains all query parcels
            bool containsAll = true;
            for (int parcel : queryList) {
                if (nodeParcels[node->val].find(parcel) == nodeParcels[node->val].end()) {
                    containsAll = false;
                    break;
                }
            }
            
            if (containsAll) {
                int currentIndex = levelOrderIndex[node->val];
                if (currentIndex > bestIndex) {
                    bestIndex = currentIndex;
                    bestNode = node->val;
                }
                
                // Continue searching in children for potentially higher-indexed nodes
                findBestNode(node->left);
                findBestNode(node->right);
            }
            // If current node doesn't contain all parcels, no need to check children
        };
        
        findBestNode(root);
        results.push_back(bestIndex);
    }
    
    return results;
}




/* 
Complete this function
*/
// Abstract this question into math:
// Now give the optimised approach and algorithm to solve this problem in steps
// Okay now complete this function and give the best code according to the above discussion:

// long long question_three(
//     const vector<vector<int>>& edges,
//     const vector<int>& metro_cities
// ) {
//     // Build adjacency list
//     unordered_map<int, vector<pair<int, int>>> graph;
//     int n = 0;
    
//     for (const auto& edge : edges) {
//         int u = edge[0], v = edge[1], w = edge[2];
//         graph[u].push_back({v, w});
//         graph[v].push_back({u, w});
//         n = max(n, max(u, v));
//     }
    
//     // Handle edge cases
//     if (n < 1) return -1;
//     if (n == 1) return 0; // Both trucks start at the same city
    
//     // Check if nodes 1 and n exist
//     if (graph.find(1) == graph.end() || graph.find(n) == graph.end()) {
//         return -1;
//     }
    
//     // Create set of metro cities for O(1) lookup
//     unordered_set<int> metro_set(metro_cities.begin(), metro_cities.end());
    
//     // Dijkstra with states: state 0 = normal fuel, state 1 = boosted fuel
//     // dist[node][state] = minimum time to reach node with given fuel state
//     auto dijkstra = [&](int start) -> vector<vector<long long>> {
//         vector<vector<long long>> dist(n + 1, vector<long long>(2, LLONG_MAX));
//         priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, greater<>> pq;
        
//         dist[start][0] = 0;
//         pq.push({0, start, 0}); // {distance, node, fuel_state}
        
//         while (!pq.empty()) {
//             auto [d, u, fuel_state] = pq.top();
//             pq.pop();
            
//             if (d > dist[u][fuel_state]) continue;
            
//             // Explore neighbors
//             for (auto [v, w] : graph[u]) {
//                 // Travel with current fuel state
//                 long long travel_time = (fuel_state == 1) ? w / 2 : w;
//                 long long new_dist = d + travel_time;
                
//                 if (new_dist < dist[v][fuel_state]) {
//                     dist[v][fuel_state] = new_dist;
//                     pq.push({new_dist, v, fuel_state});
//                 }
//             }
            
//             // If current node is a metro city and we haven't refueled yet, try refueling
//             if (fuel_state == 0 && metro_set.count(u)) {
//                 if (d < dist[u][1]) {
//                     dist[u][1] = d; // Refuel takes 0 time
//                     pq.push({d, u, 1});
//                 }
//             }
//         }
        
//         return dist;
//     };
    
//     // Run Dijkstra from both starting points
//     auto dist1 = dijkstra(1);     // From Kargil
//     auto distn = dijkstra(n);     // From Kanyakumari
    
//     // Check if the two cities are connected (either truck can reach the other's starting city)
//     if (min(dist1[n][0], dist1[n][1]) == LLONG_MAX && min(distn[1][0], distn[1][1]) == LLONG_MAX) {
//         return -1; // Cities are not connected
//     }
    
//     long long min_meeting_time = LLONG_MAX;
    
//     // Check all possible meeting points
//     for (int city = 1; city <= n; city++) {
//         if (graph.find(city) == graph.end()) continue;
        
//         // Find minimum time for truck 1 to reach this city
//         long long time1 = min(dist1[city][0], dist1[city][1]);
        
//         // Find minimum time for truck n to reach this city
//         long long timen = min(distn[city][0], distn[city][1]);
        
//         // Both trucks must be able to reach this city
//         if (time1 != LLONG_MAX && timen != LLONG_MAX) {
//             // Meeting time is when the slower truck arrives
//             long long meeting_time = max(time1, timen);
//             min_meeting_time = min(min_meeting_time, meeting_time);
//         }
//     }
    
//     return (min_meeting_time == LLONG_MAX) ? -1 : min_meeting_time;
// }




#endif // USER_CODE_H