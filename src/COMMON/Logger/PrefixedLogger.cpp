#include "PrefixedLogger.hpp"

namespace common {

namespace detail {
static std::ostream &operator<<(std::ostream &os, Prefix const &prefix) {
  prefix(os);
  return os;
}

} // namespace detail

PrefixedLogger::PrefixedLogger(ILogger &adaptee, Prefix prefix)
    : adaptee(adaptee), prefix(prefix) {}

PrefixedLogger::PrefixedLogger(ILogger &adaptee, const std::string &prefix)
    : PrefixedLogger(adaptee, [prefix](std::ostream &os) { os << prefix; }) {}

void PrefixedLogger::log(Level level, const std::string &message) {
  adaptee.log(level, prefix, message);
}

} // namespace common
