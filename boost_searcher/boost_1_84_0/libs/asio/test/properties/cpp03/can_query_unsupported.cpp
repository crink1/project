//
// cpp03/can_query_unsupported.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio/query.hpp>
#include <cassert>

struct prop
{
};

struct object
{
};

namespace boost {
namespace asio {

template <>
struct is_applicable_property<object, prop>
{
  static const bool value = true;
};

} // namespace asio
} // namespace boost

int main()
{
  assert((!boost::asio::can_query<object, prop>::value));
  assert((!boost::asio::can_query<const object, prop>::value));
}
