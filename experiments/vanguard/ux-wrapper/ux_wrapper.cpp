// Starts the real Riot Client UX with extra Chromium rendering flags. Rendering workaround only.
#include <windows.h>
#include <string>
int main() {
    std::wstring cmd = GetCommandLineW();
    // strip our own exe name (first token, possibly quoted)
    size_t i = 0;
    if (cmd[0] == L'"') { i = cmd.find(L'"', 1); i = (i == std::wstring::npos) ? cmd.size() : i + 1; }
    else { i = cmd.find(L' '); if (i == std::wstring::npos) i = cmd.size(); }
    std::wstring rest = cmd.substr(i);
    std::wstring real = L"C:\\Riot Games\\Riot Client\\RiotClientElectron\\Riot Client.real.exe";
    std::wstring full = L"\"" + real + L"\" --disable-gpu --disable-gpu-compositing --disable-software-rasterizer" + rest;
    STARTUPINFOW si = {sizeof(si)}; PROCESS_INFORMATION pi;
    if (!CreateProcessW(real.c_str(), &full[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) return 1;
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0; GetExitCodeProcess(pi.hProcess, &code); return (int)code;
}
