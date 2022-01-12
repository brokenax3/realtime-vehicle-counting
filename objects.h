#include <string>

class Centroid
{
    public:
        int x;
        int y;
};

class Object
{
    public:
        int id;
        Centroid centroid;
        int disappear_count;
        std::string getCentroid();
};
