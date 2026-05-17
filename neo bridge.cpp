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
// neo_er_bridge.hpp 
#pragma once
#include <array>
#include <complex>
#include <vector>
#include <mutex>
#include <cmath>

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
    explicit ERBridge(BridgeConfig cfg) : cfg_(cfg) {}

    // CLINICAL STEP 1: Initialize the linked pair
    void prepare_pair(StateVec& pair_state) {
        pair_state.assign(4, Complex(0.0, 0.0));
        // Perfect 50/50 balance across the link
        pair_state[0] = Complex(1.0 / std::sqrt(2.0), 0.0); 
        pair_state[3] = Complex(1.0 / std::sqrt(2.0), 0.0); 
    }

    // CLINICAL STEP 2: Offload the load from Side A to Side B
    void compute_non_local(StateVec& pair_state, const DeltaVec& delta_in, DeltaVec& delta_out) {
        std::lock_guard<std::mutex> lock(mtx_);
        delta_out = delta_in;
        // Mathematical reduction: Move 80% of the pressure to the virtual partner
        delta_out[4] -= delta_in[4] * 0.8; 
        delta_out[3] += delta_in[4] * 0.8; 
    }

    // CLINICAL STEP 3: Verify the integrity of the local side
    StateVec partial_trace_b(const StateVec& pair_state) {
        StateVec reduced(2, Complex(0.0, 0.0));
        // Uses std::norm to ensure probability is calculated by squaring the magnitude
        reduced[0] = Complex(std::norm(pair_state[0]) + std::norm(pair_state[1]), 0.0);
        reduced[1] = Complex(std::norm(pair_state[2]) + std::norm(pair_state[3]), 0.0);
        return reduced;
    }

    // CLINICAL STEP 4: System health check
    bool check_coherence(const StateVec& reduced_state) {
        double total_prob = reduced_state[0].real() + reduced_state[1].real();
        return (total_prob >= cfg_.coherence_threshold);
    }

private:
    BridgeConfig cfg_;
    std::mutex mtx_;
};
} // namespace neo