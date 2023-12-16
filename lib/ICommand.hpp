#pragma once

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
