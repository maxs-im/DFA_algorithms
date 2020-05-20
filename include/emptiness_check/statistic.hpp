#pragma once

#include "automates/inv_buchi.hpp"

#include <chrono>
#include <functional>

namespace emptiness_check::statistic
{

/// \typedef common time measure
using call_durration = std::chrono::duration<double, std::milli>;

/// \struct Handle all time-wasting callbacks (actions)
/// \param T: selected authomaton class
template<typename T>
struct callbacks_handler
{
    /// \brief generation function
    std::function<T()> generation_fn;
    /// \brief conversion function
    std::function<std::optional<T>(const T &)> conv_fn;
    /// \brief container of a serial NBA algorithms functions
    std::vector<std::function<bool(const T &)>> nba_algorithms;
    /// \brief container of a serial NGA algorithms functions
    std::vector<std::function<bool(const T &)>> nga_algorithms;
};

/// \struct Statistic report for the one similar data entry
struct one_step
{
    /// \brief Future automaton states number
    automates::buchi::atm_size states = 0;
    /// \brief Collect average time for the called conversions
    call_durration average_conversion = {};
    /// \brief Average one automaton generation time
    call_durration average_generation = call_durration::zero();

    /// \brief Collect algorithms answers positime (is_empty = true) with their average time calculation
    std::vector<std::pair<automates::buchi::atm_size, call_durration>> average_nba = {};
    /// \brief Collect algorithms answers positime (is_empty = true) with their average time calculation
    std::vector<std::pair<automates::buchi::atm_size, call_durration>> average_nga = {};

    /// \brief Increases when NGA != NBA on emptiness
    automates::buchi::atm_size different_results = 0;
};

/// \brief Generate particular automatons and collect data. DFS approach
/// \param repetition: number of re-creation and collecting data from almost similar. To get average stats
/// \param callbacks: callbacks that will be tracked on each call
/// \return statistic for particular configured automaton
one_step one_step_generation(automates::buchi::atm_size repetition,
                                 const callbacks_handler<automates::buchi> &callbacks) noexcept;

/// \note: due to need to hide template implementation
one_step one_step_generation(automates::buchi::atm_size repetition,
                                 const callbacks_handler<automates::inv_buchi> &callbacks) noexcept;

} // namespace emptiness_check::statistic
