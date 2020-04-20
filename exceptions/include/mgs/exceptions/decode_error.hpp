#pragma once

#include <string>

#include <mgs/exceptions/exception.hpp>

namespace mgs
{
namespace exceptions
{
class decode_error : public exception
{
public:
  using exception::exception;
};
}
}
