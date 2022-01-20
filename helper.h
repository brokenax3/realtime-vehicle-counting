#include <string>
#include <vector>

#include "objects.h"

float find_distance(int x1, int x2, int y1, int y2);

std::tuple<std::vector<std::tuple<int, Centroid>>, std::vector<Centroid>> find_centroid_pairs(std::vector<Centroid> centroidsOld, std::vector<Centroid> centroidsInput);
