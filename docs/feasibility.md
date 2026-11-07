# Feasibility Summary

Question: can VALORANT run locally on a Mac mini M4 (Mac16,10, 16 GB, macOS 27.0.1)?

Answer so far: **no supported path exists, and none was demonstrated.**

| Layer | Status | Evidence |
|---|---|---|
| Windows executables (Wine/GPTK, CrossOver) | Works | experiments/environment/01-baseline.md |
| DirectX 11 / 12 device creation via D3DMetal | Works | d3d_probe: D3D11 FL 11_1, D3D12 FL 12_2 |
| Shader compile and correct D3D11 rendering | Works (basic) | d3d11_render_test: pixels verified |
| Riot Client install | Works | experiments/vanguard/02-riot-client.md |
| Riot Client UI | Alive but cannot paint (white window) | same |
| Vanguard anti-cheat | Not reached; documented as unsupported on VMs, ARM, non-Windows | docs/vanguard-analysis.md |
| VALORANT gameplay | Not tested | |

Even with a working login screen, Riot's own documentation limits VALORANT to native x86 Windows with TPM 2.0 and Secure Boot.
This project does not attempt to bypass Vanguard.
