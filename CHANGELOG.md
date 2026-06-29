# Changelog

## [0.2.0] - 2026-06-29

### Changes
- Updated the dependency versions

## [0.1.0] - 2026-06-11

- First release. See [0.0.0] section in CHANGELOG.md for full changelog.

## [0.0.0] - 2026-01-16

### Added
- CPU vendor identification
- Instruction set extension detection (SSE, SSE2, SSE3, SSSE3, SSE4.1, SSE4.2, AVX, AVX2, FMA, POPCNT, AES-NI, MOVBE, BMI1, BMI2, SHA, RDRAND, RDTSCP, NEON)
- CPU cycle counter (`sn_platform_cpu_cycle_counter`)
- Cycle counter frequency and invariant detection
- Page size, cache line size, logical/physical core count
- amd64 x86-64 implementation (CPUID, RDTSC, RDTSCP, LFENCE/MFENCE/SFENCE)
- ARM64 implementation (cntvct_el0 cycle counter, per-platform feature detection via sysctlbyname / `/proc/cpuinfo`)
- SnCore dependency
- CI workflows (Linux, macOS, Windows, formatting)
