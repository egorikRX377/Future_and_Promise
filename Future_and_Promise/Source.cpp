#include <iostream>
#include <future>

void calculateSquare(std::promise<int>&& prom, int x) 
{
    prom.set_value(x * x);
}

int main() 
{
    std::promise<int> prom;
    std::future<int> result = prom.get_future();

    std::thread t(calculateSquare, std::move(prom), 5);
    std::cout << "Waiting for result..." << std::endl;
    int square = result.get(); 
    t.join();
    std::cout << "Result: " << square << std::endl;
    return 0;
}