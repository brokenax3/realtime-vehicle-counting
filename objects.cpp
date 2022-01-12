#include <string>
#include "helper.cpp"
#include "objects.h"

std::string Object::getCentroid()
{
    return string_format("%s %s", centroid.x, centroid.y); 
}
