#include "ScreenRecorder.h"
#include <wrl/client.h>
#include <wrl/def.h>
#include <d3d9.h>
#define HRCHECK(__expr) {hr = __expr; if (FAILED(hr)) return hr;}
#define RELEASE(__p) {if(__p!=nullptr){__p->Release();__p=nullptr;}}

ScreenRecorder::ScreenRecorder(End *outputDevice, EncodingProfile &profile, QObject *parent): QObject(parent)
{

}

HRESULT dumpScreenBuffer(UINT adapter, UINT count, LPBYTE output) {
    HRESULT hr = S_OK;
    IDirect3D9 *d3d = nullptr;
    IDirect3DDevice9 *device = nullptr;
    IDirect3DSurface9 *surface = nullptr;
    D3DPRESENT_PARAMETERS parameters{};
    parameters.BackBufferFormat = D3DFMT_A8B8G8R8;
    D3DDISPLAYMODE mode;
    D3DLOCKED_RECT rc;
    UINT pitch;
    SYSTEMTIME st;

    // init D3D and get screen size
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    HRCHECK(d3d->GetAdapterDisplayMode(adapter, &mode));

    parameters.Windowed = TRUE;
    parameters.BackBufferCount = 1;
    parameters.BackBufferHeight = mode.Height;
    parameters.BackBufferWidth = mode.Width;
    parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parameters.hDeviceWindow = NULL;

    // create device & capture surface
    HRCHECK(d3d->CreateDevice(adapter, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device));
    HRCHECK(device->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, nullptr));

    // compute the required buffer size
    HRCHECK(surface->LockRect(&rc, NULL, 0));
    pitch = rc.Pitch;
    HRCHECK(surface->UnlockRect());

    GetSystemTime(&st); // measure the time we spend doing <count> captures
    wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    // get the data
    HRCHECK(device->GetFrontBufferData(0, surface));

    // copy it into our buffers
    HRCHECK(surface->LockRect(&rc, NULL, 0));
    CopyMemory(output, rc.pBits, rc.Pitch * mode.Height);
    HRCHECK(surface->UnlockRect());
    GetSystemTime(&st);
    wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    RELEASE(surface);
    RELEASE(device);
    RELEASE(d3d);
    QImage* img = new QImage(mode.Width, mode.Height, QImage::Format_ARGB32);
    for (int y = 0; y < img->height(); y++)
    {
        memcpy(img->scanLine(y), (void*) output[y], img->bytesPerLine());
    }
    img->save("output.png", "PNG");
    return hr;
}
