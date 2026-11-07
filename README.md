# VAL-M4

Open research notes on whether VALORANT can run locally on an Apple Mac mini M4 (no cloud gaming, no remote PC).

**Status: not feasible with supported methods.** The graphics stack works; Riot Vanguard is the blocking constraint.
This project does not attempt to bypass, disable, patch or hide Vanguard or virtual-machine detection.

- `docs/feasibility.md` - one-page summary
- `docs/vanguard-analysis.md` - what Riot documents about Vanguard
- `experiments/environment/` - Wine / Game Porting Toolkit baseline and D3D tests
- `experiments/directx/` - small D3D11/D3D12 test programs (build with mingw-w64)
- `experiments/vanguard/` - Riot Client attempts under Wine and CrossOver

Hardware: Mac16,10, Apple M4, 10-core GPU, 16 GB. macOS 27.0.1, Xcode 27.2.
Third-party software used: Apple Game Porting Toolkit / D3DMetal 3.0, Gcenx Wine builds, CrossOver 26.3 (trial).
Logs are not committed (they can contain machine identifiers).
