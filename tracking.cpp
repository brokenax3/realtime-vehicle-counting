/* Functions which enable object tracking */
#include <vector>
#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>

#include "tracking.h"
#include "helper.cpp"

void CentroidTracker::registerObject(Centroid centroid)
{
    Object nObject;
    nObject.id = rand() % 300 + 1;
    nObject.centroid.x = centroid.x; 
    nObject.centroid.y = centroid.y; 

    this->objects.push_back(nObject);

    /* std::cout << "Object with id=" << nObject.id << " registered. The centroid is " << nObject.centroid.x << ", " << nObject.centroid.y << std::endl; */
}

void CentroidTracker::unregisterObject(int id)
{
    int rIndex;
    // Check object list for id to unregister
    for(int index = 0; index < objects.size(); index++)
    {
        if(objects[index].id == id)
        {
            rIndex = index;
            break;
        }
    }
    std::cout << "An object left" << std::endl;
    objects.erase(objects.begin() + rIndex);
}

void CentroidTracker::updateObjects(std::vector<cv::Rect> rectangles)
{
    // Register all rectangles as there are no objects
    if(this->objects.size() == 0)
    {
        for(cv::Rect rectangle : rectangles)
        {
            Centroid centroid;

            centroid.x = (rectangle.x + rectangle.width) / 2;
            centroid.y = (rectangle.y + rectangle.height) / 2;

            this->registerObject(centroid);
            /* std::cout << "work here"; */
        }
        return;
    }
    // Increment disappear counter for all objects
    if(rectangles.size() == 0)
    {
        for(Object object : this->objects)
        {
            object.disappear_count++;

            if(object.disappear_count > max_disappear)
            {
                this->unregisterObject(object.id);
            }
        }

        return;
    }
    else
    {
        // Create a list of centroids to find centroid-object pairs
        std::vector<Centroid> nCentroids;

        for(cv::Rect rectangle : rectangles)
        {
            Centroid nCentroid;
            std::cout << boost::format("[[TRACKING]] x, y : %1%, %2%") % rectangle.x % rectangle.y << std::endl;
            nCentroid.x = rectangle.x + (rectangle.width / 2);
            nCentroid.y = rectangle.y + (rectangle.height / 2);
            std::cout << boost::format("New Centroids (%1%, %2%)") % nCentroid.x % nCentroid.y << std::endl;
            nCentroids.push_back(nCentroid);
        }

        // Unpack old centroid values
        std::vector<Centroid> oCentroids;
        for(Object object : this->objects)
        {
            std::cout << boost::format("Old Centroids %1%") % object.getCentroid() << std::endl;
            oCentroids.push_back(object.centroid);
        }

        // Find the closest distance between each centroid pairs
        std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> expTuple;
        std::vector<std::tuple<int, Centroid>> foundPairs;
        std::vector<Centroid> newCentroid;
        std::vector<int> updatedIdx;
        /* std::tie(foundPairs, newCentroid) = find_centroid_pairs(oCentroids, nCentroids); */

        expTuple = find_centroid_pairs(oCentroids, nCentroids);

        // Update centroid location
        for(int i = 0; i < foundPairs.size(); i++)
        {
            this->objects.at(i).centroid = std::get<1>(foundPairs.at(i));
            updatedIdx.push_back(i);
        }

        // Increment Lost Counter
        std::vector<int> lostIdx;
        std::vector<int> oldObjectsRange(this->objects.size());
        std::iota(oldObjectsRange.begin(),oldObjectsRange.end(), 0);
        std::set_difference(
            oldObjectsRange.begin(), 
            oldObjectsRange.end(), 
            updatedIdx.begin(), 
            updatedIdx.end(), 
            std::back_inserter(lostIdx)
        );

        std::vector<int> unregisterIdx;
        for(int idx : lostIdx)
        {
            this->objects.at(idx).disappear_count++;

            if(this->objects.at(idx).disappear_count > 10000)
            {
                unregisterIdx.push_back(idx);
            }
        }

        for(int idx : unregisterIdx)
        {
            this->unregisterObject(idx);
        }

        for(Centroid centroid : newCentroid)
        {
            this->registerObject(centroid);
        }
    }
}


