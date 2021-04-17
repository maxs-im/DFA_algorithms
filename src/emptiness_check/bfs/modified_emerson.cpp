#include "bfs/modified_emerson.hpp"
#include<iostream>

namespace emptiness_check::bfs::modified_emerson
{

/// \namespace Anonymous namespace. Helpers with Modified Emerson-Lei algorithm
namespace
{

std::vector<uint32_t> pre(const automates::inv_buchi &automat, std::vector<uint32_t> states) noexcept
{
    std::vector<uint32_t> result{};
    for(auto state : states)
    {
        const auto accepted_transitions = automat.acceptable_inv_transitions(state);
        if(accepted_transitions)
            result.insert(result.end(), accepted_transitions.value()->second.begin(), accepted_transitions.value()->second.end());
    }

    std::sort(result.begin(), result.end() );
    result.erase(unique(result.begin(), result.end()), result.end());

    return result;
}

std::vector<uint32_t> backward_bfs(const automates::inv_buchi &automat, std::vector<uint32_t> states) noexcept
{
    std::vector<uint32_t> S = states, B = states, temp_value{};

    do {
        B = pre(automat, B);
        std::set_difference(B.begin(), B.end(), S.begin(), S.end(), std::back_inserter(temp_value));
        B = temp_value;
        temp_value.clear();
        std::set_union(B.begin(), B.end(), S.begin(), S.end(), std::back_inserter(temp_value));
        S = temp_value;
        temp_value.clear();
    }while(!B.empty());

    return S;
}

std::vector<uint32_t> inf(const automates::inv_buchi &automat, std::vector<uint32_t> S) noexcept
{
    std::vector<uint32_t> OldS{}, temp_value{}, pre_s{};

    do {
        OldS = S;
        pre_s = pre(automat, S);
        std::set_intersection(S.begin(), S.end(), pre_s.begin(), pre_s.end(), std::back_inserter(temp_value));
        S = temp_value;
        temp_value.clear();
    }while(S != OldS);

    return S;
}

}

bool is_empty(const automates::inv_buchi &automat) noexcept
{
    std::vector<std::unordered_set<uint32_t>> final_states = automat.get_final_sets();
    std::vector<std::vector<uint32_t>> F;
    std::vector<uint32_t> OldL{}, temp_value{};
    std::vector<uint32_t> L(automat.m_set_of_states);
    std::sort(L.begin(), L.end());

    for(auto i = 0; i < final_states.size(); i++){
        std::vector<uint32_t> temp_value;
        temp_value.assign(final_states[i].begin(), final_states[i].end());
        std::sort(temp_value.begin(), temp_value.end());
        F.push_back(temp_value);
    }

    do {
        OldL = L;
        for(auto Fi : F)
        {
            L = inf(automat, OldL);
            std::set_intersection(OldL.begin(), OldL.end(), Fi.begin(), Fi.end(), std::back_inserter(temp_value));
            L = pre(automat, temp_value);
            temp_value.clear();
            L = backward_bfs(automat, L);
        }
    }while( OldL != L);

    if (std::find(L.begin(), L.end(), automates::inv_buchi::INITIAL_STATE) != L.end())
        return false;
    else
        return true;
}

} // namespace emptiness_check::dfs::modified_emerson
