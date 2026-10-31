#include <cpptrace/from_current.hpp>
#include <iostream>

import Birdle;

int main(int argc, char* argv[])
{
    // debug builds should have a debugger attached, so ignore the try/catch in debug builds
#ifdef NDEBUG
    CPPTRACE_TRY
#endif
    {
        bdl::BirdleGame game;
        game.Run();
    }
#ifdef NDEBUG
    CPPTRACE_CATCH(const std::exception& e)
    {
        std::cout << "Exception thrown: " << e.what() << std::endl;
        cpptrace::from_current_exception().print(std::cout);
    }
#endif
}
