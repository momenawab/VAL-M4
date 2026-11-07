// d3d_probe: minimal D3D11 + D3D12 device-creation probe (no rendering).
#include <windows.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi.h>
#include <cstdio>

static const char* fl(D3D_FEATURE_LEVEL l) {
    switch (l) {
    case D3D_FEATURE_LEVEL_12_2: return "12_2"; case D3D_FEATURE_LEVEL_12_1: return "12_1";
    case D3D_FEATURE_LEVEL_12_0: return "12_0"; case D3D_FEATURE_LEVEL_11_1: return "11_1";
    case D3D_FEATURE_LEVEL_11_0: return "11_0"; default: return "other";
    }
}

int main() {
    // DXGI adapter enumeration
    IDXGIFactory* f = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&f);
    printf("CreateDXGIFactory: 0x%08lx\n", hr);
    if (SUCCEEDED(hr)) {
        IDXGIAdapter* a = nullptr;
        for (UINT i = 0; f->EnumAdapters(i, &a) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC d; a->GetDesc(&d);
            printf("Adapter %u: %ls | VRAM %llu MB | Vendor 0x%04x\n", i, d.Description,
                   (unsigned long long)(d.DedicatedVideoMemory >> 20), d.VendorId);
            a->Release();
        }
        f->Release();
    }
    // D3D11
    ID3D11Device* dev = nullptr; ID3D11DeviceContext* ctx = nullptr; D3D_FEATURE_LEVEL got;
    D3D_FEATURE_LEVEL req[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, req, 2,
                           D3D11_SDK_VERSION, &dev, &got, &ctx);
    printf("D3D11CreateDevice: 0x%08lx (feature level %s)\n", hr, SUCCEEDED(hr) ? fl(got) : "n/a");
    if (dev) { ctx->Release(); dev->Release(); }
    // D3D12
    D3D_FEATURE_LEVEL tests[] = {D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1,
                                 D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_0};
    for (auto l : tests) {
        ID3D12Device* d12 = nullptr;
        hr = D3D12CreateDevice(nullptr, l, __uuidof(ID3D12Device), (void**)&d12);
        printf("D3D12CreateDevice FL %s: 0x%08lx\n", fl(l), hr);
        if (d12) d12->Release();
    }
    return 0;
}
