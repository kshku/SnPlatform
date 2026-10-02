# Changelog

## [0.3.3] - 2026-10-02

### Changed
- Take sncore v0.3.2 rather than v0.3.1

## [0.3.2] - 2026-09-28

### Fixed
- sn_platform_page_size() narrowed the sysconf result with SN_MAX(0, ps), which
  turned the documented -1 error return into a page size of zero instead of
  reporting it, and SN_ASSERT is compiled out in release so nothing caught it.
  The result is now checked before it is narrowed

### Changed
- -Wconversion and -Wsign-conversion are on for gcc and clang. The code was
  already clean of both

## [0.3.1] - 2026-09-28

### Changed
- Take sncore v0.3.1 rather than v0.2.0

## [0.3.0] - 2026-09-28

### Fixed
- The cpu instruction wrappers sn_cpuid, sn_xgetbv, sn_rdtsc, sn_rdtscp and
  sn_cntvct_el0 are internal helpers that no public header declares, but they
  were compiled with external linkage, so a static build published them as
  global symbols where they could collide with a consumer's own. They are now
  static. On MSVC the definitions come from platform.asm instead, so the
  declarations there stay externally visible and the file still binds to them.
  The public headers are unchanged, so this narrows the ABI without changing
  the API

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
