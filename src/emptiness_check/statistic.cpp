#include "statistic.hpp"

#include <chrono>
#include <functional>
#include <vector>

using namespace emptiness_check::statistic;

/// \namespace Anonymous namespace. Hidden templated statistic gathering implementation
namespace
{

/// \struct Store statistic for ONE automaton
struct one_call
{
    /// \brief Time wasted on generation
    call_durration generation;
    /// \brief Time wasted on conversion. If this was a case
    std::optional<call_durration> conversion;
    /// \brief Time wasted and result of each NBA algorithm
    std::vector<std::pair<call_durration, bool>> nba;
    /// \brief Time wasted and result of each NGA algorithm
    std::vector<std::pair<call_durration, bool>> nga;
};

/// \brief Track time for passed function during its operation
/// \param T: result of the function call
/// \param fn: callback that we be tracked
/// \param wasted time on calculation and its result
template<typename T>
typename std::enable_if<not std::is_void<T>::value, std::pair<call_durration, T>>::type
time_call(std::function<T()> fn) noexcept
{
    // starting timepoint
    auto start = std::chrono::high_resolution_clock::now();

    auto result = fn();

    // ending timepoint
    auto stop = std::chrono::high_resolution_clock::now();

    return {duration_cast<std::chrono::microseconds>(stop - start), std::move(result)};
}

/// \brief Running all available operations with provided authomaton
/// \param T: automaton type
/// \param callbacks: callbacks that will help to proceed automaton
/// \return return gathered statistic for ONE automaton
template<typename T>
one_call calculation(const callbacks_handler<T> &callbacks) noexcept
{

    // run generation
    auto[gen_durr, automaton] = time_call<T>(callbacks.generation_fn);
    // run conversion
    auto[conv_durr, nba_automaton] = time_call<std::optional<T>>(
            [&conv_fn = callbacks.conv_fn, &at = automaton]() { return conv_fn(at); }
    );

    // To prevent copying NBA->NBA
    auto get_worker = [&automaton = automaton, &opt_nba = nba_automaton]() -> const T &
    {
        return opt_nba ? *opt_nba : automaton;
    };

    std::vector<std::pair<call_durration, bool>> nba_results{};
    nba_results.reserve(callbacks.nba_algorithms.size());
    // remember NBA results
    for (const auto &fn : callbacks.nba_algorithms)
        nba_results.push_back(time_call<bool>([&fn, &nba = get_worker()]() { return fn(nba); }));

    std::vector<std::pair<call_durration, bool>> nga_results{};
    nga_results.reserve(callbacks.nga_algorithms.size());
    // means @automaton is generalized
    if (nba_automaton)
    {
        // remember NGA results
        for (const auto &fn : callbacks.nga_algorithms)
            nga_results.push_back(time_call<bool>([&fn, &nga = automaton]() { return fn(nga); }));
    }

    return {.generation = gen_durr, .conversion = nba_automaton ? std::make_optional(conv_durr) : std::nullopt,
            .nba = std::move(nba_results), .nga = std::move(nga_results)};
}

/// \brief Check algorithms output. All are assumed to be the same. Otherwise, we will report information
/// \param common [out]: general statistic that we will update during the check
/// \param common_counter [out]: remember number of invocation. (For example NGA algorithms may not be called at all
///                                 due to only NBA generation)
/// \param one_run: invariable result during the calculation (local statistic)
/// \return true if answers are not equal -> need to report this information
bool one_run_algorithms_gather(std::vector<std::pair<uint32_t, call_durration>> &common, uint32_t &common_counter,
                               const std::vector<std::pair<call_durration, bool>> &one_run
) noexcept
{
    if (one_run.empty())
        return false;

    ++common_counter;
    bool report = false;
    bool nba_emptiness = one_run[0].second;
    for (size_t j = 0; j < one_run.size(); ++j)
    {
        auto&[durr, val] = one_run[j];
        if (j == common.size())
            common.emplace_back(std::make_pair(0, call_durration::zero()));
        assert(j < common.size() && "Unexpected behaviour: Vector size less than expected!");

        // update general statistic
        if (val)
            common[j].first++;
        common[j].second += durr;

        // Report if not all results equal. Need further deep investigation
        if (val != nba_emptiness)
        {
            report = true;
        }
    }

    return report;
}

/// \brief Gather statistic for each repetition and then average it
/// \param T: automaton type
/// \param repetition: number of re-running procedure for the automaton
/// \param callbacks: callbacks to proceed automaton
/// \return averaged statistic
template<typename T>
one_step one_step_generation(const uint32_t repetition, const callbacks_handler<T> &callbacks) noexcept
{
    one_step result{};
    result.repetition = repetition;

    uint32_t nba_calls_counter = 0,
            nga_calls_counter = 0;
    for (uint32_t i = 0; i < result.repetition; ++i)
    {
        // calculation call
        one_call run_result = calculation(callbacks);

        // start gathering info
        result.average_generation += run_result.generation;
        if (run_result.conversion)
        {
            ++result.average_conversion.first;
            result.average_conversion.second += *run_result.conversion;
        }

        // Collect NBA and then NGA stat from one shot.
        // If at least one reports, then return every result (verify results for the identity)
        if (one_run_algorithms_gather(result.average_nba, nba_calls_counter, run_result.nba)
            || one_run_algorithms_gather(result.average_nga, nga_calls_counter, run_result.nga))
        {
            auto collect = [](const auto &data)
            {
                std::vector<bool> answers;
                answers.reserve(data.size());
                for (auto&[_, val] : data)
                    answers.push_back(val);

                return std::move(answers);
            };

            result.different_results.emplace_back(std::make_pair(collect(run_result.nba), collect(run_result.nga)));
        }
    }

    // get average from the total
    result.average_generation /= result.repetition;
    for (auto&[_, durr] : result.average_nba)
        durr /= nba_calls_counter;
    for (auto&[_, durr] : result.average_nga)
        durr /= nga_calls_counter;

    if (result.average_conversion.first)
        result.average_conversion.second /= result.average_conversion.first;

    return std::move(result);
}

} // namespace anonymous

/// \note: due to need to hide template implementation
one_step emptiness_check::statistic::one_step_generation_dfs(const uint32_t repetition,
                            const callbacks_handler<automates::buchi> &callbacks) noexcept
{
    return one_step_generation(repetition, callbacks);
}
