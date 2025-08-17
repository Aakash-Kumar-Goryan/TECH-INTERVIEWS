#include <vector>
class Solution {
public:
    int smallestChair(vector<vector<int>>& times, int targetFriend) {
        priority_queue<pair<int, int>, vector<pair<int, int>>,
                       greater<pair<int, int>>>
            orderQueue;
        for (int i = 0; i < times.size(); i++) {
            orderQueue.emplace(times[i][1], i);
            orderQueue.emplace(times[i][0], i);
        }
        vector<int> chairMap(times.size(), -1);
        priority_queue<int, vector<int>, greater<int>> chair;
        for (int i = 0; i < times.size(); i++) {
            chair.push(i);
        }
        while (!orderQueue.empty()) {
            auto [time, person] = orderQueue.top();
            orderQueue.pop();
            if (chairMap[person] == -1) {
                chairMap[person] = chair.top();
                chair.pop();
                if (person == targetFriend)
                    return chairMap[person];
            } else {
                chair.push(chairMap[person]);
                chairMap[person] = -1;
            }
        }
        return -1;
    }
};