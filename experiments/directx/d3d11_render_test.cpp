// d3d11_render_test: offscreen triangle render, pixel readback verification, simple timing.
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cstdio>
#include <cstdint>

static const char* kShader = R"(
struct VSOut { float4 pos : SV_Position; float2 uv : TEXCOORD0; };
VSOut VS(uint id : SV_VertexID) {
    // full-screen triangle (winding is CCW, so the host disables culling)
    float2 p = float2((id << 1) & 2, id & 2);
    VSOut o; o.pos = float4(p * 2.0 - 1.0, 0, 1); o.uv = p; return o;
}
float4 PS(VSOut i) : SV_Target { return float4(i.uv.x, i.uv.y, 0.5, 1.0); }
)";

#define CHK(x) do { HRESULT _h = (x); if (FAILED(_h)) { printf("FAIL %s -> 0x%08lx\n", #x, _h); return 1; } } while (0)

int main() {
    const UINT W = 256, H = 256;
    ID3D11Device* dev; ID3D11DeviceContext* ctx; D3D_FEATURE_LEVEL fl;
    CHK(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &dev, &fl, &ctx));

    ID3DBlob *vsb, *psb, *err = nullptr;
    HRESULT h = D3DCompile(kShader, strlen(kShader), "t", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vsb, &err);
    if (FAILED(h)) { printf("VS compile fail 0x%08lx: %s\n", h, err ? (char*)err->GetBufferPointer() : ""); return 1; }
    h = D3DCompile(kShader, strlen(kShader), "t", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psb, &err);
    if (FAILED(h)) { printf("PS compile fail 0x%08lx: %s\n", h, err ? (char*)err->GetBufferPointer() : ""); return 1; }
    printf("Shader compile: OK (VS %zu B, PS %zu B)\n", vsb->GetBufferSize(), psb->GetBufferSize());

    ID3D11VertexShader* vs; ID3D11PixelShader* ps;
    CHK(dev->CreateVertexShader(vsb->GetBufferPointer(), vsb->GetBufferSize(), nullptr, &vs));
    CHK(dev->CreatePixelShader(psb->GetBufferPointer(), psb->GetBufferSize(), nullptr, &ps));

    D3D11_TEXTURE2D_DESC td = {}; td.Width = W; td.Height = H; td.MipLevels = 1; td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM; td.SampleDesc.Count = 1; td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_RENDER_TARGET;
    ID3D11Texture2D* rt; CHK(dev->CreateTexture2D(&td, nullptr, &rt));
    ID3D11RenderTargetView* rtv; CHK(dev->CreateRenderTargetView(rt, nullptr, &rtv));
    td.Usage = D3D11_USAGE_STAGING; td.BindFlags = 0; td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    ID3D11Texture2D* st; CHK(dev->CreateTexture2D(&td, nullptr, &st));

    D3D11_VIEWPORT vp = {0, 0, (float)W, (float)H, 0, 1};
    ctx->RSSetViewports(1, &vp);
    ctx->OMSetRenderTargets(1, &rtv, nullptr);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->VSSetShader(vs, nullptr, 0); ctx->PSSetShader(ps, nullptr, 0);
    D3D11_RASTERIZER_DESC rd = {}; rd.FillMode = D3D11_FILL_SOLID; rd.CullMode = D3D11_CULL_NONE; rd.DepthClipEnable = TRUE;
    ID3D11RasterizerState* rs; CHK(dev->CreateRasterizerState(&rd, &rs)); ctx->RSSetState(rs);
    float clear[4] = {0, 0, 0, 1};
    ctx->ClearRenderTargetView(rtv, clear);
    ctx->Draw(3, 0);
    ctx->CopyResource(st, rt);
    D3D11_MAPPED_SUBRESOURCE m; CHK(ctx->Map(st, 0, D3D11_MAP_READ, 0, &m));
    auto px = [&](UINT x, UINT y) { return (const uint8_t*)m.pData + y * m.RowPitch + x * 4; };
    // Expected: uv = (x+0.5)/W, with D3D y flipped: uv.y = 1 - (y+0.5)/H
    bool ok = true;
    UINT pts[3][2] = {{10, 10}, {128, 128}, {240, 200}};
    for (auto& p : pts) {
        const uint8_t* c = px(p[0], p[1]);
        float eu = (p[0] + 0.5f) / W, ev = 1.0f - (p[1] + 0.5f) / H;
        int er = (int)(eu * 255 + 0.5f), eg = (int)(ev * 255 + 0.5f);
        bool pass = abs(c[0] - er) <= 2 && abs(c[1] - eg) <= 2 && abs(c[2] - 128) <= 2 && c[3] == 255;
        printf("px(%u,%u) = %3u,%3u,%3u,%3u  expected ~%3d,%3d,128,255  %s\n", p[0], p[1], c[0], c[1], c[2], c[3], er, eg, pass ? "PASS" : "FAIL");
        ok &= pass;
    }
    ctx->Unmap(st, 0);
    printf("Render correctness: %s\n", ok ? "PASS" : "FAIL");

    // Timing: N full-target draws, then sync via readback.
    const int N = 5000;
    LARGE_INTEGER f, t0, t1; QueryPerformanceFrequency(&f); QueryPerformanceCounter(&t0);
    for (int i = 0; i < N; ++i) ctx->Draw(3, 0);
    ctx->CopyResource(st, rt);
    ctx->Map(st, 0, D3D11_MAP_READ, 0, &m); ctx->Unmap(st, 0);
    QueryPerformanceCounter(&t1);
    double s = (double)(t1.QuadPart - t0.QuadPart) / f.QuadPart;
    printf("Timing: %d draws (%ux%u) in %.1f ms = %.0f draws/s\n", N, W, H, s * 1000, N / s);
    return ok ? 0 : 2;
}
