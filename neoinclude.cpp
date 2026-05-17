/*
 * Copyright (C) 2026 Nicholas Hope
 * 
 * This program is free software: you can redistribute it and/or modify...
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
 };
 
 } // namespace neo