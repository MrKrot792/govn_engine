#include "tve_app.hpp"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    tve::App app{};

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
