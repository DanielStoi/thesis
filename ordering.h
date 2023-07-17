#include <vector>

#pragma once

using namespace std;


class ordering{
public:
    virtual int get_ordering(vector<int>& output) = 0;
};