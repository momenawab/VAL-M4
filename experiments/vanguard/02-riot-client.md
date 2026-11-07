# Experiment 02: Riot Client under Wine / CrossOver (no bypass attempted)

Date: 2026-11-07. Goal: see how far the unmodified, official Riot Client gets on macOS and whether
Vanguard is ever reached. Installer: official Riot CDN (`live.live.eu.exe`, SHA-256 e0d8bb74...3b65).
No Vanguard, VM-detection, or file patching was attempted. A throwaway account was required; no login happened.

| # | Environment | Result |
|---|---|---|
| 1 | GPTK Wine 7.7 (Gcenx cask 3.0-3) | Installer and background services OK (~880 MB). UI process crashes at startup: breakpoint exception 0x80000003 at one fixed address inside Riot Client.exe (Chromium abort). Chromium flags (`--disable-gpu --no-sandbox --in-process-gpu`) did not change it. |
| 2 | Wine Staging 11.21 (Gcenx build, own prefix) | Installer OK. UI process starts and stays alive, window opens but paints white. |
| 3 | Wine 11.21 + renderer=vulkan / gl | Still white. |
| 4 | Wine 11.21 + d3d11/dxgi disabled for the UI exe | UI exits silently, no window. |
| 5 | Wine 11.21 + ELECTRON_EXTRA_LAUNCH_ARGS | No effect. |
| 6 | Wine 11.21 + virtual desktop | Not supported by the macOS driver ("Failed to set primary display settings"); no UI. |
| 7 | CrossOver 26.3.0 trial, Win10 64-bit bottle, backends d3dmetal / dxvk / dxmt / wined3d | Installer OK. UI window opens, responds to the mouse (cursor changes over fields and buttons) but is always white. Resize, minimize/restore and maximize did not force a repaint. |

## Side findings
- Wrapping the UI exe to force `--disable-gpu` made the window draw, but the Riot Client verifies its install files on every start
  (`File is 2831569 bytes but 239243432 bytes was expected`) and the self-repair then fails on a Wine path lookup
  (`File path could not be found in a mapped drive`). Modifying Riot's files is therefore not viable, and it was reverted.
- Riot's updater metadata (`Riot Client.ok`) is deleted by a failed repair; re-running the official installer restores it.
- The Wine log noise (COM class not registered, SIO_IDEAL_SEND_BACKLOG_QUERY, timezone fixmes) is not what blocks the UI.

## Conclusion
The Riot Client UI is alive under Wine 11.21 and CrossOver but cannot present its content on macOS here, so login and
VALORANT launch were never reached. Vanguard was never started, so this experiment says nothing new about Vanguard itself;
see `docs/vanguard-analysis.md`. This is a rendering/presentation problem, not a Riot block.

## Reproduce
`experiments/vanguard/run_riot.sh <label>` (Wine 11.21 prefix) and `experiments/vanguard/cx_try.sh <backend>` (CrossOver bottle `RiotVAL`).
