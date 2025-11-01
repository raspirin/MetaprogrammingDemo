#include <iostream>

#include "Query.h"
#include "World.h"
#include "TypeList.h"
#include "TypeHash.h"

void sys_no_param() {}

void sys_bad_params(int i) {}

// void sys_bad_params2(Query<int> q) {};

void sys_good_params(Query<std::tuple<int>>) { return; }

void sys_good_params2(Query<std::tuple<float>, With<int>> q1, Query<std::tuple<int, float>> q2)
{
    // for (auto iter: q1)
    // {
    //
    // }
}

void sys1(Query<std::tuple<int, float>> q) {}

int main(int argc, char* argv[])
{
    constexpr auto id = TypeHash::index_of<int>();
    std::cout << id;
    return 0;
}
