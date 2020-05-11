#include "dfs/two_stack.hpp"

namespace emptiness_check::dfs::two_stack
{

namespace
{

}

bool is_empty(const automates::buchi &automat)
{
    std::unordered_map<uint32_t, uint32_t> S;
    std::unordered_set<uint32_t> P;


    return automat.is_final(0);
}

} // namespace emptiness_check::dfs::two_stack
