/* Functions which enable object tracking */
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#include "objects.h"

class CentroidTracker
{
    private:
        std::vector<Object> objects;

    public:
        int max_disappear = 50;

        void registerObject(Centroid centroid)
        {
            Object nObject;
            nObject.id = objects.size() + 1;
            nObject.centroid.x = centroid.x; 

            objects.push_back(nObject);

            std::cout << "Object with id=" << nObject.id << " registered. The centroid is " << nObject.getCentroid() << std::endl;
        }

        void unregisterObject(int id)
        {
            int rindex;
            // Check object list for id to unregister
            for(int index = 0; index < objects.size(); index++)
            {
                if(objects[index].id == id)
                {
                    rindex = index;
                    break;
                }
            }
            objects.erase(objects.begin() + rindex);
        }

        void updateObjects(std::vector<cv::Rect> rectangles)
        {
            // Register all rectangles as there are no objects
            if(objects.size() == 0)
            {
                for(cv::Rect rectangle : rectangles)
                {
                    Centroid centroid;

                    centroid.x = (rectangle.x + rectangle.width) / 2;
                    centroid.y = (rectangle.y + rectangle.height) / 2;

                    registerObject(centroid);
                }

                return;
            }
            // Increment disappear counter for all objects
            if(rectangles.size() == 0)
            {
                for(Object object : objects)
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
                std::vector<Centroid> centroids;

                for(cv::Rect rectangle : rectangles)
                {
                    Centroid nCentroid;
                    nCentroid.x = (rectangle.x + rectangle.width) / 2;
                    nCentroid.y = (rectangle.y + rectangle.height) / 2;

                    centroids.push_back(nCentroid);
                }
            }
            
        }



};


