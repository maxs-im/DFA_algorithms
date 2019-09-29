#include "binary_algo.hpp"

#include <set>
#include <queue>
#include <cassert>

namespace binary_algo
{
    using namespace dfa;

    automaton do_act(const automaton& a, const automaton& b, OPERATOR op)
    {
        const auto alphabet = a.get_alphabet_num();
        assert(alphabet == b.get_alphabet_num() && "Impossible alphabet");

        using pr = std::pair<uint32_t, uint32_t>;
        std::vector<pr> w_q, c_states, f_states, h_table;

        pr c0 = {a.start, b.start};
        w_q.push_back(c0);
        while (!w_q.empty())
        {
            auto st = w_q.back();
            w_q.pop_back();
            c_states.push_back(st);

            if (act_condition(a.is_final(st.first), b.is_final(st.second), op)) 
                f_states.push_back(st);

            for (uint32_t x = 0; x < alphabet; ++x)
            {
                pr new_st = { a.action(st.first, x), b.action(st.second, x) };

                const auto &cs = c_states;
                if (std::find(cs.begin(), cs.end(), new_st) == cs.end())
                    w_q.push_back(new_st);

                h_table.push_back(new_st);
            }
        }
        
        auto convert_fn = [&](const std::vector<pr> &p_vec) {
            std::vector<uint32_t> vec;
            vec.reserve(p_vec.size());

            for (const auto &el : p_vec)
                for (uint32_t i = 0; i < c_states.size(); ++i)
                    if (c_states[i] == el)
                    {
                        vec.push_back(i);
                        break;
                    }

            assert(vec.size() == p_vec.size() && "Smth goes wrong in convert");
            return vec;
        };

        return automaton(
            static_cast<uint32_t>(c_states.size()), 
            convert_fn({c0}).back(), 
            convert_fn(f_states), 
            convert_fn(h_table)
        );
    }

    namespace
    {
        bool act_condition(bool a, bool b, OPERATOR op)
        {
            switch (op)
            {
            case ASSOCIATION:
                return a || b;
            case INTERSECTION:
                return a && b;
            case DIFFERENCE:
                return a || !b;
            default:
                assert(false && "Bad operation");
                return false;
            }
        }
    }
}