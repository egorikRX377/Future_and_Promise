#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>

void myTerminate()
{
    std::cout << "My terminate has been called!" << std::endl;
    std::exit(100);
}

void foo1(int a, int b, std::promise<int>& pr)
{
    std::this_thread::sleep_for(std::chrono::seconds(4));
    if (a + b > 0)
    {
        pr.set_value(a + b);
    }
    else
    {
        pr.set_exception(std::make_exception_ptr(std::runtime_error("Negative answer!")));
    }
}

void foo2(std::future<int>& result, std::promise<int>& except)
{
    try
    {
        int result1 = result.get();
        std::cout << "Result is: " << result1 << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception in foo2: " << ex.what() << std::endl;
        except.set_exception(std::make_exception_ptr(std::runtime_error("Negative answer!")));
    }
}

int main()
{
    set_terminate(myTerminate);

    std::promise<int> pr1;
    std::promise<int> except;

    std::future<int> result = pr1.get_future();
    std::future<int> futExcept = except.get_future();

    std::thread th1(foo1, -10, 5, std::ref(pr1));
    std::thread th2(foo2, std::ref(result), std::ref(except));

    th1.join();
    th2.join();

    try
    {
        int anyResult = futExcept.get();
    }
    catch (const std::exception& ex1)
    {
        terminate();
    }

    return 0;
}