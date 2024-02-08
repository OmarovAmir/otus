#pragma once

#include <boost/asio.hpp>
#include <netinet/in.h>

using ip_transparent = boost::asio::detail::socket_option::boolean<SOL_IP, IP_TRANSPARENT>;