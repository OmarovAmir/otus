#include <DataExtractorManager.hpp>
#include <async.hpp>

static auto manager = DataExtractorManager();

std::size_t connect() { return manager.connect(); }

void receive(const std::size_t handle, const void* buffer, const std::size_t size)
{
    manager.receive(handle, buffer, size);
}

void disconnect(const std::size_t handle) { manager.disconnect(handle); }
