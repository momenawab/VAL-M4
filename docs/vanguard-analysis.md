# Vanguard Analysis

Status: research notes, 2026-11-07. All claims below are from Riot's own pages unless marked as inference.
This project does not attempt to bypass, patch, disable, or hide Vanguard or VM detection.

## Confirmed from Riot documentation

| Claim | Source |
|---|---|
| "VALORANT does not support cloud gaming or virtual machines." | [Minimum & Recommended PC Specs](https://support.riotgames.com/en-us/valorant/support-tools/minimum-recommended-pc-specs) |
| Every CPU tier lists "ARM: Not supported". | same |
| Supported OS: Windows 10 (Build 19041+) 64-bit, Windows 11 64-bit. The page does not mention macOS, Linux, or Wine. (The page is internally inconsistent about the Windows 10 build number: 19041+ vs 10904+.) | same |
| Windows 11 additionally requires TPM 2.0 and UEFI Secure Boot. | same |
| VAN 9100: "Vanguard is not supported in virtual machine environments"; fix is "Use a regular Windows installation." | [VAN 9100](https://support.riotgames.com/en-us/riot/client/error-van-9100) |
| Vanguard = user-mode client + kernel-mode driver; driver loads early at boot; relies on IOMMU/Secure Boot-class protections. | [Riot security post](https://www.riotgames.com/en/news/a-message-about-vanguard-from-our-security-privacy-teams), [pre-boot gap post](https://www.riotgames.com/en/news/vanguard-security-update-motherboard) |
| Stricter "Vanguard Pre-Check" tier lists Secure Boot, TPM 2.0, IOMMU, VBS, HVCI. | [Vanguard Pre-Check](https://support.riotgames.com/en-us/riot/performance/vanguard-pre-check) |

## Implications for this project (inference)

- Wine/D3DMetal on macOS provides no Windows kernel, so a kernel-mode anti-cheat driver has nothing to load into.
- A Windows ARM VM on Apple Silicon would hit both stated blockers: virtual machine and ARM.
- Riot's documentation does not say anything specific about Wine or macOS; the conclusion that they are unsupported follows from the Windows-only OS list and the VM/ARM rules, not from an explicit statement.

## Conclusion

No supported path to run VALORANT on a Mac mini M4 was found in Riot's documentation.
The graphics side (Experiments 01) is a separate, solved-in-principle problem; Vanguard is the blocking constraint.

## Open items
- Check whether Riot publishes any statement on macOS/Wine/Linux support or a roadmap (none found so far).
- Re-verify before publishing; Riot's pages change.
