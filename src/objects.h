#include <string>

#ifndef __OBJECT_H__
#define __OBJECT_H__
class Centroid
{
    public:
        float x;
        float y;
};

class Object
{
    public:
        long id;
        Centroid centroid;
        float x_coord;
        float y_coord;
        float width;
        float height;

        int disappear_count = 0;
        std::string getCentroid();
};
#endif
