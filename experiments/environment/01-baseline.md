# Experiment 01: Baseline Wine + D3DMetal environment

Date: 2026-11-07. Host: Mac mini M4 (Mac16,10), 16 GB, macOS 27.0.1, Xcode 27.2.

## Setup
- Apple "Evaluation environment for Windows games 3.0" DMG inspected (redist/lib only; no Wine).
- Installed Gcenx cask `game-porting-toolkit` 3.0-3 (Wine 7.7, "Game Porting Toolkit 1.1", 806 MB).
- Bundled D3DMetal version.plist == DMG's (3.0, source 32047000000000); library swap not needed.
- Prefix: `~/val-m4-prefix` (reports Windows 10.0.19043).
- Toolchain: Homebrew mingw-w64 (GCC 16.2.0).

## Test: d3d_probe.exe (experiments/directx/d3d_probe.cpp)
Result:
- DXGI adapter: "AMD Compatibility Mode", 12124 MB reported VRAM, vendor 0x1002 (D3DMetal presents Apple GPU as AMD).
- D3D11CreateDevice: S_OK, feature level 11_1.
- D3D12CreateDevice: S_OK for FL 12_2, 12_1, 12_0, 11_0.

## Conclusion
Windows executable compatibility and DX11/DX12 device creation work on this machine.
This says nothing about rendering correctness, performance, or anti-cheat (Vanguard) compatibility.

## Test 2: d3d11_render_test.exe (experiments/directx/d3d11_render_test.cpp)
Runtime HLSL compile (vs_5_0/ps_5_0 via D3DCompile), offscreen 256x256 RGBA8 render, staging readback.
- Shader compile: OK. Pixel readback at 3 sample points matched expected UV gradient within 1/255: PASS.
- First run failed (all black) due to a bug in the test, not the stack: the full-screen triangle was
  counter-clockwise and default back-face culling removed it. Fixed by disabling culling.
- Timing (5000 trivial draws + readback sync): ~4 ms. Not a meaningful GPU benchmark (tiny workload,
  translation layer may defer work); recorded only as a smoke-test number.

## Updated conclusion
Shader compilation, translation to Metal, and correct rasterization work for a basic D3D11 pipeline.
Still untested: D3D12 rendering, complex shaders, real game workloads, anti-cheat.
