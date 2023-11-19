#ifndef D462B73C_978B_4C2C_B334_054CF9BB74F3
#define D462B73C_978B_4C2C_B334_054CF9BB74F3

#include <string>

class Command
{
    std::string _cmd;

  public:
    explicit Command(std::string cmd)
        : _cmd{cmd}
    {}

    std::string execute() const { return _cmd; }
};

#endif /* D462B73C_978B_4C2C_B334_054CF9BB74F3 */
