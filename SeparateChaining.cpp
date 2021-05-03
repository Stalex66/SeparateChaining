//
//  main.cpp
//  SeperateChaining
//
//  Created by Alexander Moser on 30.03.21.
//

#include <iostream>
#include "ADS_set.h"
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;


class Person {
    std::string vn;
    std::string nn;
public:
    Person() = default;
    Person(const std::string& vn, const std::string& nn) : vn{ vn }, nn{ nn } { }
    friend struct std::hash<Person>;
    friend struct std::equal_to<Person>;
    friend struct std::less<Person>;
    friend std::ostream& operator<<(std::ostream& o, const Person& p) { return o << '[' << p.nn << ", " << p.vn << ']'; }
    friend std::istream& operator>>(std::istream& i, Person& p) { return i >> p.vn >> p.nn; }
};

namespace std {
    template <> struct hash<Person> {
        size_t operator()(const Person& p) const {
            return std::hash<std::string>{}(p.vn) ^ std::hash<std::string>{}(p.nn) << 1;
        }
    };
    template <> struct equal_to<Person> {
        bool operator()(const Person& lhs, const Person& rhs) const {
            return lhs.vn == rhs.vn && lhs.nn == rhs.nn;
        }
    };
    template <> struct less<Person> {
        bool operator()(const Person& lhs, const Person& rhs) const {
            return lhs.nn < rhs.nn || (lhs.nn == rhs.nn && lhs.vn < rhs.vn);
        }
    };
};



void tester() {
    ADS_set<int, 100> test{ 12 };
    ADS_set<int, 100> test3(test);
    ADS_set<int, 100> test4{ 12 };

    ADS_set<int, 100> test_comp{ 12 };

    //std::cout << test.count(45);
    test.find(12);
    test_comp.clear();
    test.dump();
    int test2 = 0;
    for (auto v : test) {
        std::cout << static_cast<int>(v) << "\n";
        test2++;
    }
    test.clear();
    test.dump();
    std::vector<int> test10;
    for (size_t i{ 0 }; i < 150000; i++) {
        int number2 = rand() % 30000000;

        test10.push_back(i);


    }
    //for (size_t i{ 0 }; i < 100000; i++) {

        //test10.push_back(i);
   // }



    for (auto v : test) {
        std::cout << static_cast<int>(v) << "\n";
        test2++;
    }
    for (auto v : test) {
        std::cout << static_cast<int>(v) << "\n";
        test2++;
    }
    for (auto v : test) {
        std::cout << static_cast<int>(v) << "\n";
        test2++;
    }

#define WRK(cond) std::cout << (cond ? "\n worked \n" : "\n ERROR \n");
    WRK(test3 == test4);
    WRK(!(test3 != test4));
    WRK(test == test_comp);
    WRK(!(test != test_comp));
    test3.erase({ 12 });
    WRK(!(test3 == test4));
    WRK(test3 != test4);
    //test3.dump();
    test.insert({ 1 });
    test.erase(1);
    test.erase(2);
    test.erase(3);
    //test.dump();
    if (test.find(1) == test.end() && test.find(2) == test.end() && test.find(3) == test.end()) std::cout << "\nYES\n}";


    ADS_set<int> test6{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,1 };
    for (auto v : test6) {
        std::cout << v << "\n";
    }
    test6.dump();
    cout << "\n" << "NEU " << "\n";

    ADS_set<int, 9> neu;
    neu.insert(test10.begin(), test10.end());

}

int main(int argc, const char* argv[]) {
    while(true){
    
    vector<int> test;
    ADS_set<int, 13> ads;
    for (size_t i{ 0 }; i < 10; i++) {

        test.push_back(i);


    }
    ads.insert(test.begin(), test.end());
    ADS_set<int, 13>hey{4, 5, 6};
    swap(ads,hey);
      hey.dump();
    ads.dump();
    if (hey == ads) cout << "YES";
    for (size_t i{ 0 }; i < 1000; i++) {

        ads.erase(i);


    }
    hey.dump();
    ads.dump();
    cout << "hello";
}
}
