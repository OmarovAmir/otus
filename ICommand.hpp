#ifndef A8159899_07B5_4D41_AC7D_5826BDCA5394
#define A8159899_07B5_4D41_AC7D_5826BDCA5394

#include <memory>
#include <string>

#include <CommandType.hpp>

class ICommand
{
    CommandType _type;

  public:
    explicit ICommand(CommandType type)
        : _type(type)
    {}

    CommandType getType() const { return _type; }
    virtual std::string execute() const { return std::string(); };
};

using CommandPtr = std::shared_ptr<ICommand>;

#endif /* A8159899_07B5_4D41_AC7D_5826BDCA5394 */
