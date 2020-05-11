#include "dfs/nested.hpp"

namespace emptiness_check::dfs::nested
{

bool is_empty(const automates::buchi::nba &automat)
{
    return automat.is_final(0);
}

} // namespace emptiness_check::dfs::nested
