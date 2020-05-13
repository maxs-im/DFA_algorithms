#pragma once

#include "automates/inv_buchi.hpp"

namespace emptiness_check::bfs::emerson
{

bool is_empty(const automates::inv_buchi &automat) noexcept;

}