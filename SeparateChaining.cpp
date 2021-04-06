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

void tester(){
    ADS_set<int> test{ 12 };
    ADS_set<int> test3(test);
    ADS_set<int> test4{ 12 };

    ADS_set<int> test_comp{ 12 };

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
    for (size_t i{ 0 }; i < 1000; i++) {
        int number2 = rand() % 10000;
        test.insert({ number2 });
        test_comp.insert({ number2 });

    }
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
    if (test.find(1)==test.end() && test.find(2)==test.end() && test.find(3)==test.end()) std::cout << "\nYES\n}";


    ADS_set<int> test6{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,1 };
    for (auto v : test6) {
        std::cout << v << "\n";
    }
    test6.dump();}

int main(int argc, const char* argv[]) {
tester();
  
}
        
    
