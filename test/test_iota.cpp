#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// This shows how to find the values which are not in `a` given that `a` would
// have a range of 0 to 8
int main() {
    std::vector<int> a = {0, 4, 20};
    std::vector<int> result;
    std::vector<int> b(a.back());
    std::iota(b.begin(), b.end(), 0);
    std::set_difference(b.begin(), b.end(), a.begin(), a.end(),
                        std::back_inserter(result));

    std::cout << "This is a :" << std::endl;
    std::cout << "Size of a :" << a.size() << std::endl;

    for (int item : a) {
        std::cout << item << std::endl;
    }

    std::cout << "This is b :" << std::endl;
    std::cout << "Size of b :" << b.size() << std::endl;

    for (int itemb : b) {
        std::cout << itemb << std::endl;
    }

    std::cout << "This is result :" << std::endl;
    std::cout << "Size of result :" << result.size() << std::endl;

    for (int itemr : result) {
        std::cout << itemr << std::endl;
    }
}
