/* Functions which enable object tracking */
#include <vector>
#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>

#include "tracking.h"
#include "helper.cpp"

void CentroidTracker::registerObject(Centroid centroid)
{
    long id = rand() % 300 + 1;
    Object nObject;
    nObject.id = id;
    nObject.centroid.x = centroid.x; 
    nObject.centroid.y = centroid.y; 

    this->objects.push_back(nObject);

    /* std::cout << "Object with id=" << nObject.id << " registered. The centroid is " << nObject.centroid.x << ", " << nObject.centroid.y << std::endl; */
}

void CentroidTracker::unregisterObject(int id)
{
    int rIndex;
    // Check object list for id to unregister
    for(int index = 0; index < this->objects.size(); index++)
    {
        if(this->objects[index].id == id)
        {
            rIndex = index;
            break;
        }
    }
    /* std::cout << "An object left" << std::endl; */
    this->objects.erase(this->objects.begin() + rIndex);
}

void CentroidTracker::updateObjects(std::vector<cv::Rect> rectangles)
{
    /* std::cout << "NUMBER OF RECTANGLES : " << rectangles.size() << std::endl; */
    // Register all rectangles as there are no objects
    if(this->objects.size() == 0)
    {
        for(cv::Rect rectangle : rectangles)
        {
            Centroid centroid;

            centroid.x = rectangle.x + (rectangle.width / 2);
            centroid.y = rectangle.y + (rectangle.height / 2);

            this->registerObject(centroid);
            /* std::cout << "work here"; */
        }
        return;
    }
    // Increment disappear counter for all objects
    if(rectangles.size() == 0)
    {
        for(int i = 0; i < this->objects.size(); i++)
        {
            this->objects[i].disappear_count++;

            if(this->objects[i].disappear_count > 20)
            {
                this->unregisterObject(this->objects[i].id);
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
            /* std::cout << boost::format("[[TRACKING]] x, y : %1%, %2%") % rectangle.x % rectangle.y << std::endl; */
            nCentroid.x = rectangle.x + (rectangle.width / 2);
            nCentroid.y = rectangle.y + (rectangle.height / 2);
            /* std::cout << boost::format("New Centroids (%1%, %2%)") % nCentroid.x % nCentroid.y << std::endl; */
            nCentroids.push_back(nCentroid);
        }

        // Unpack old centroid values
        /* std::vector<Centroid> oCentroids; */
        /* for(Object object : this->objects) */
        /* { */
        /*     std::cout << boost::format("Old Centroids %1%") % object.getCentroid() << std::endl; */
        /*     oCentroids.push_back(object.centroid); */
        /* } */

        // Find the closest distance between each centroid pairs
        std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> expTuple;
        std::vector<std::tuple<int, Centroid>> foundPairs;
        std::vector<Centroid> newCentroid;
        std::vector<int> updatedIdx;
        std::tie(foundPairs, newCentroid) = find_centroid_pairs(this->objects, nCentroids);

        /* expTuple = find_centroid_pairs(oCentroids, nCentroids); */

        // Update centroid location and
        // Increment Lost Counter
        std::vector<int> unregisterID;
        int updated = 0;
        for(int i = 0; i < this->objects.size(); i++)
        {
            /* std::cout << "Checking for " << this->objects[i].id << std::endl; */
            for(int j = 0; j < foundPairs.size(); j++)
            {
                /* std::cout << "Checking Result " << std::get<0>(foundPairs[j]) << std::endl; */
                if(std::get<0>(foundPairs[j]) == this->objects[i].id)
                {
                    /* std::cout << "Found Result for " << this->objects[i].id << std::endl; */
                    this->objects[i].centroid = std::get<1>(foundPairs[j]);
                    updated = 1;
                    break;
                }
            }
            if(updated == 0)
            {
                std::cout << this->objects[i].id << " have dissapeared " << this->objects[i].disappear_count << std::endl;
                this->objects[i].disappear_count++;
                if(this->objects[i].disappear_count > 20)
                {
                    unregisterID.push_back(this->objects[i].id);
                }
            }
            updated = 0;
        }
        std::cout << "UNREGISTER ITEMS : " << std::endl;
        for(int id : unregisterID)
        {
            std::cout << id << std::endl;
            this->unregisterObject(id);
        }
        if(newCentroid.empty() == 0)
        {
            for(Centroid centroid : newCentroid)
            {
                this->registerObject(centroid);
            }
        }
    }
}


