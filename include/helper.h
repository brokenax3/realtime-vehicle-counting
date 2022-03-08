#ifndef __HELPER_H__
#define __HELPER_H__

#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <iostream>

namespace po = boost::program_options;

struct Roi {
   public:
    int width;
    int height;
    int x;
    int y;
    friend std::istream& operator>>(std::istream& istream, Roi& roi) {
        return istream >> std::skipws >> roi.width >> roi.height >> roi.x >>
               roi.y;
    }
    friend std::ostream& operator<<(std::ostream& ostream, const Roi roi) {
        return ostream << "{" << roi.width << "," << roi.height << "," << roi.x
                       << "," << roi.y << "}" << std::endl;
    }
};

po::variables_map parse_cmd(int argc, char* argv[]);
#endif
