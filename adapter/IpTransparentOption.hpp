#pragma once

#include <netinet/in.h>
#include <boost/asio.hpp>

using ip_transparent = boost::asio::detail::socket_option::boolean<SOL_IP, IP_TRANSPARENT>;