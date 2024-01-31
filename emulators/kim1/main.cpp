#include "kim1.hpp"

#include <iostream>
#include <thread>
#include <memory>

int main()
{
    std::unique_ptr<KIM1> kim1 = std::make_unique<KIM1>();
    while (true) {
        std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
        kim1->clock();
    }

    return 0;
}
