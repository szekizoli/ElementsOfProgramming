#include<algorithm>
#include<iterator>
#include<list>
#include<numeric>
#include<vector>

#include "benchmark/benchmark.h"
#include "eop.h"

static void BM_rotate_forward_nontrivial_list(benchmark::State& state) {
  std::list<int> input(state.range(0));
  std::iota(input.begin(), input.end(), 0);
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_forward_nontrivial(input.begin(), m, input.end()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_forward_nontrivial_list)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_forward_nontrivial(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::iota(input.begin(), input.end(), 0);
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_forward_nontrivial(input.begin(), m, input.end()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_forward_nontrivial)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_bidirectional_nontrivial(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::iota(input.begin(), input.end(), 0);
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_bidirectional_nontrivial(input.begin(), m, input.end());
  } 
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_bidirectional_nontrivial)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_random_access_nontrivial(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::iota(input.begin(), input.end(), 0);
   while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_random_access_nontrivial(input.begin(), m, input.end()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_random_access_nontrivial)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_std(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::iota(input.begin(), input.end(), 0);
   while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    std::rotate(input.begin(), m, input.end()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_std)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_with_buffer_nontrivial(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::vector<int> buffer(state.range(0));
  std::iota(input.begin(), input.end(), 0);
   while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_with_buffer_nontrivial(input.begin(), m, input.end(), buffer.begin()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_with_buffer_nontrivial)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);

static void BM_rotate_forward_annotated(benchmark::State& state) {
  std::vector<int> input(state.range(0));
  std::vector<int> buffer(state.range(0));
  std::iota(input.begin(), input.end(), 0);
   while (state.KeepRunning()) {
    state.PauseTiming();
    auto m = input.begin();
    std::advance(m, state.range(0) >> 1);
    state.ResumeTiming();
    eop::rotate_forward_annotated(input.begin(), m, input.end()); 
  }
}
// Register the function as a benchmark
BENCHMARK(BM_rotate_forward_annotated)->Arg(8)->Arg(64)->Arg(1<<10)->Arg(8<<10);
