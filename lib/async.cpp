#include <DataExtractorManager.hpp>
#include <async.hpp>

static auto manager = DataExtractorManager();

std::size_t connect(std::function<void(const std::string&)> transmitCallback)
{
    return manager.connect(transmitCallback);
}

void receive(const std::size_t handle, const std::string& cmd) { manager.receive(handle, cmd); }

void disconnect(const std::size_t handle) { manager.disconnect(handle); }
