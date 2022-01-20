#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <algorithm>
#include <math.h>
#include <boost/format.hpp>

#include "objects.h"

float find_distance(float x1, float y1, float x2, float y2)
{
    float ans = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    std::cout << boost::format("sqrt((%1% - %2%)^2 + (%3% - %4%)^2) = %5%") % x1 % x2 % y1 % y2 % ans << std::endl;
    return ans;
}

std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> find_centroid_pairs(std::vector<Centroid> centroidsOld, std::vector<Centroid> centroidsInput)
{
    std::cout << "Iteration START" << std::endl;
    std::vector<std::tuple<int, Centroid>> distancePair;
    std::vector<int> assignedPairs;
    std::vector<Centroid> newCentroid;

    // Find pairs which satisfy the given condition
    for(int i = 0; i < centroidsOld.size(); i++)
    {
        std::vector<float> tempDist;
        /* Centroid centOld = centroidsOld[i]; */

        for(Centroid centIn : centroidsInput)
        {
            tempDist.push_back(find_distance(centIn.x, centIn.y, centroidsOld[i].x, centroidsOld[i].y));
        }
        float minimum = *min_element(tempDist.begin(), tempDist.end());
        std::cout << minimum << std::endl;

        if(minimum < 100)
        {
            auto it = find(tempDist.begin(), tempDist.end(), minimum);
            if(it != tempDist.end())
            {
                auto index = std::distance(tempDist.begin(), it);
                std::cout << "Index :" << index << std::endl;
                distancePair.push_back({i, centroidsInput.at(index)});
                assignedPairs.push_back(index);
            }
        }

        std::cout << "Index Assigned Pairs:" << std::endl;
        for(int item : assignedPairs)
        {
            std::cout << item << std::endl;
        }
    }

    // Check for duplicates
    std::sort(assignedPairs.begin(), assignedPairs.end());
    assignedPairs.erase( unique( assignedPairs.begin(), assignedPairs.end() ), assignedPairs.end() );
    std::cout << "Input Size :" << centroidsInput.size() << std::endl;
    for(int item : assignedPairs)
    {
        std::cout << item << std::endl;
    }

    // Find unused new centroids
    std::vector<int> newCentroidIdx;
    std::vector<int> inputRange(centroidsInput.size());
    std::iota(inputRange.begin(), inputRange.end(), 0);
    std::set_difference(
        inputRange.begin(), 
        inputRange.end(), 
        assignedPairs.begin(), 
        assignedPairs.end(), 
        std::back_inserter(newCentroidIdx)
    );

    for(int idx : newCentroidIdx)
    {
        std::cout << idx << std::endl;
        newCentroid.push_back(centroidsInput.at(idx));
    }
    std::cout << "Iteration END" << std::endl;

    assignedPairs.clear();

    return {distancePair, newCentroid};
}
