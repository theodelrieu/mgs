#pragma once

#include <stdexcept>

namespace mgs
{
namespace exceptions
{
class exception : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};
}
}
