#include "bfs/emerson.hpp"

namespace emptiness_check::bfs::emerson
{

bool is_empty(const automates::inv_buchi &automat) noexcept
{
    return automat.is_generalized();
}

} // namespace emptiness_check::dfs::nested
