#include <string>
#include <boost/format.hpp>
#include "objects.h"

std::string Object::getCentroid()
{
    std::string output = boost::str(boost::format("(%1%, %2%)") % this->centroid.x % this->centroid.y);

    return output;
}
