#include "Game.h"

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

#include <stdexcept>
#include <format>

int main()
{
    CPPTRACE_TRY
    {
        birdle::Game game;
        game.run();
    }
    CPPTRACE_CATCH (const std::exception& e)
    {
        const auto& stacktrace = cpptrace::from_current_exception();
        
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR, "birdle",
            std::format("Oops! birdle crashed.\n{}\n{}", e.what(), stacktrace.to_string()).c_str(), nullptr);
        
        stacktrace.print();
        cpptrace::rethrow();
    }
    return 0;
}
