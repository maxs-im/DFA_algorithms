#pragma once

#include "automates/buchi.hpp"

namespace emptiness_check::bfs::emerson
{

bool is_empty(const automates::buchi &automat) noexcept;

}