#include "dfs/two_stack.hpp"

namespace emptiness_check::dfs::two_stack
{

bool is_empty(const automates::buchi &automat)
{
    return automat.is_final(0);
}

} // namespace emptiness_check::dfs::two_stack
