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

std::vector<Centroid> filter_centroids(std::vector<Centroid> centroidsInput);

float find_distance(float x1, float y1, float x2, float y2);

std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> find_centroid_pairs(std::vector<Object> objectInput, std::vector<Centroid> centroidsInput);
