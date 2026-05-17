/*
 * Copyright (C) 2026 Nicholas Hope
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */ 
#pragma once
#include <array>
#include <complex>
#include <vector>
#include <mutex>

namespace neo {

using Complex = std::complex<double>;
using StateVec = std::vector<Complex>;
using DeltaVec = std::array<double, 6>;

struct BridgeConfig {
    int pair_id = 0; 
    double coherence_threshold = 0.95;
};

class ERBridge {
public:
    explicit ERBridge(BridgeConfig cfg);

    void prepare_pair(StateVec& pair_state);
    void compute_non_local(StateVec& pair_state, const DeltaVec& delta_in, DeltaVec& delta_out);
    StateVec partial_trace_b(const StateVec& pair_state) const;
    bool check_coherence(const StateVec& reduced_state) const;

private:
    BridgeConfig cfg_;
    mutable std::mutex mtx_; // 'mutable' allows locking inside const functions if needed later
} // namespace neo 