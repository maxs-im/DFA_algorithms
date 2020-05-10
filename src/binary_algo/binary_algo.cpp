#include "binary_algo.hpp"

#include <set>
#include <queue>

namespace binary_algo
{
    using namespace dfa;
    using pr = std::pair<uint32_t, uint32_t>;

    automaton do_act(
        const automaton& a, const automaton& b,
        OPERATOR op, 
        bool print
    )
    {
        const auto alphabet = a.get_alphabet_num();
        assert(alphabet == b.get_alphabet_num() && "Impossible alphabet");

        std::vector<pr> w_q, c_states, f_states, h_table;

        pr c0 {a.start, b.start};
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
        
        minimizate(c_states, h_table);

        if (print) 
            print_pairs(c_states, c0, f_states, h_table);

        return automaton(
            static_cast<uint32_t>(c_states.size()), 
            convert_fn({c0}).back(), 
            convert_fn(f_states), 
            convert_fn(h_table)
        );
    }

    namespace
    {
        void minimizate(
            std::vector<pr> &states,
            std::vector<pr> &table
        )
        {
            const auto alph_num = table.size() / states.size();
            std::set<pr> uniq_states;
            for (int i = 0; i < states.size(); ++i)
            {
                auto res = uniq_states.insert(states[i]);
                if (!res.second)
                {
                    states.erase(states.begin() + i);
                    table.erase(
                        table.begin() + alph_num*i, 
                        table.begin() + alph_num*(i + 1)
                    );
                    --i;
                }
            }
        }

        void print_pairs(
            const std::vector<pr> &states,
            const pr& start, 
            const std::vector<pr> &final_states,
            const std::vector<pr> &table
        )
        {
            std::cout << "\tPaired DFA result\n";

            const auto alph_num = table.size() / states.size();
            for (uint32_t st = 0; st < states.size(); ++st, std::cout << "\n")
            {
                if (states[st] == start) 
                    std::cout << "->";
                std::cout << "[" << states[st].first << "," 
                    << states[st].second << "]";

                const auto &fs = final_states;
                if (std::find(fs.begin(), fs.end(), states[st]) != fs.end())
                    std::cout << "*";
                std::cout << "\t|";
                
                for (uint32_t i = 0; i < alph_num; ++i, std::cout << "\t") 
                {
                    const auto indx = st * alph_num + i;
                    std::cout << "[" << table[indx].first << "," 
                        << table[indx].second << "]";
                }
            }
        }

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
                assert(!"Bad operation");
                return false;
            }
        }
    } // namespace anonymous
} // namespace binary_algo
