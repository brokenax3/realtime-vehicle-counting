#include "objects.h"
#include "helper.h"

bool test(Centroid a, Centroid b){ return (find_distance(a.x, a.y, 0, 0) < find_distance(b.x, b.y, 0, 0));}

float find_distance(float x1, float y1, float x2, float y2)
{
    float ans = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    std::cout << boost::format("sqrt((%1% - %2%)^2 + (%3% - %4%)^2) = %5%") % x1 % x2 % y1 % y2 % ans << std::endl;
    return ans;
}

std::vector<Centroid> filter_centroids(std::vector<Centroid> centroidsInput)
{
    std::cout << "Old Value" << std::endl;
    for(Centroid centroid : centroidsInput)
    {
        std::cout << boost::format("x, y : %1%, %2%") % centroid.x % centroid.y << std::endl;
    }
    std::sort(centroidsInput.begin(), centroidsInput.end(), test);
    std::cout << "New Value" << std::endl;
    for(Centroid centroid : centroidsInput)
    {
        std::cout << boost::format("x, y : %1%, %2%") % centroid.x % centroid.y << std::endl;
    }
    /* for(int i = 0; i < centroidsInput.size() - 1; i++) */
    /* { */
    /*     float difference = find_distance(centroidsInput[i].x, centroidsInput[i].y, centroidsInput[i + 1].x, centroidsInput[i + 1].y); */
    
    return centroidsInput;
}
std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> find_centroid_pairs(std::vector<Object> objectInput, std::vector<Centroid> centroidsInput)
{
    /* std::cout << "Iteration START" << std::endl; */
    std::vector<std::tuple<int, Centroid>> distancePair;
    std::vector<int> assignedInput(centroidsInput.size());
    std::vector<Centroid> newCentroid;

    for(int input : assignedInput)
    {
        input = 0;
    }

    /* centroidsInput = filter_centroids(centroidsInput); */

    // Find pairs which satisfy the given condition
    for(int i = 0; i < objectInput.size(); i++)
    {
        std::vector<float> tempDist;
        /* Centroid centOld = centroidsOld[i]; */

        for(Centroid centIn : centroidsInput)
        {
            tempDist.push_back(find_distance(centIn.x, centIn.y, objectInput[i].centroid.x, objectInput[i].centroid.y));
        }

        float minimum = *min_element(tempDist.begin(), tempDist.end());
        std::vector<float> temptempDist;
        float secminimum = *min_element(tempDist.begin(), tempDist.end());
        /* std::cout << minimum << std::endl; */

        if(minimum < 25)
        {
            auto it = find(tempDist.begin(), tempDist.end(), minimum);
            if(it != tempDist.end())
            {
                auto index = std::distance(tempDist.begin(), it);
                /* std::cout << "Index :" << index << std::endl; */
                distancePair.push_back({objectInput[i].id, centroidsInput.at(index)});
                assignedInput[index]++;
            }
        }
    }

    for(int i; i < assignedInput.size(); i++)
    {
        if(assignedInput[i] == 0)
        {
            newCentroid.push_back(centroidsInput.at(i));
        }
    }
    
    /* newCentroid = filter_centroids(newCentroid); */

    return {distancePair, newCentroid};
}
