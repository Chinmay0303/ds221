#ifndef USER_CODE_H
#define USER_CODE_H

// Feel free to include more library functions
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


// 1- Complete this function
// Write a function based on the below approach and algorithm:
// Approach:
// 1. Hash Map (Unordered Map)
// Algorithm:
// Traverse the list, for each parcel ID, store the count and minimum weight in a hash map.
// After traversal, collect IDs with count > 1 and their minimum weights, then sort by ID.

vector<vector<int>> question_one(const vector<vector<int>>& parcels) {
    unordered_map<int, pair<int, int>> parcel_map;  // ID -> (count, min_weight)

    for (const auto& parcel : parcels) {
        int id = parcel[0];
        int weight = parcel[1];

        if (parcel_map.find(id) == parcel_map.end()) {
            parcel_map[id] = {1, weight};  // First occurrence
        } else {
            parcel_map[id].first++;  // Increment count
            parcel_map[id].second = min(parcel_map[id].second, weight);  // Update min weight
        }
    }

    vector<vector<int>> result;
    for (const auto& entry : parcel_map) {
        if (entry.second.first > 1) {  // Count > 1
            result.push_back({entry.first, entry.second.second});
        }
    }

    // Sort by ID
    sort(result.begin(), result.end());

    return result;
    // return parcels;
}

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
Complete this function
*/

// vector<int> question_two(
//     const vector<int>& preorder,
//     const vector<int>& inorder,
//     const vector<vector<int>>& leafParcels,
//     const vector<vector<int>>& query
// ) {
//     // TODO: Implement function
// }




/* 
Complete this function
*/
// long long question_three(
//     const vector<vector<int>>& edges,
//     const vector<int>& metro_cities
// ) {
//     // TODO: Implement function
// }




#endif // USER_CODE_H