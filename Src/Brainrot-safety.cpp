#include <windows.h>
#include <cstdlib>
#include <ctime>

#pragma comment(lib,"winmm.lib")
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")

// ================= BYTEBEAT =================
void PlayRandomBytebeat(int seconds)
{
    HWAVEOUT h;
    WAVEFORMATEX w = { WAVE_FORMAT_PCM,1,8000,8000,1,8,0 };
    waveOutOpen(&h, WAVE_MAPPER, &w, 0, 0, CALLBACK_NULL);

    static char buf[8000 * 20];
    int mode = rand() % 10;

    for (int t = 0; t < sizeof(buf); t++)
    {
        switch (mode)
        {
        case 0: buf[t] = t * (t >> 5 | t >> 8); break;
        case 1: buf[t] = (t * t >> 6) | t >> 4; break;
        case 2: buf[t] = (t >> 3 | t * 7) & (t >> 7); break;
        case 3: buf[t] = t * ((t >> 9 | t >> 13) & 25); break;
        case 4: buf[t] = (t >> 4) | (t * 9); break;
        case 5: buf[t] = t ^ (t >> 6); break;
        case 6: buf[t] = (t * 3) & (t >> 8); break;
        case 7: buf[t] = (t >> 4) | (t * 5); break;
        case 8: buf[t] = ((t >> 6 | t >> 9) * 10) + (t & t >> 8); break;
        case 9: buf[t] = (t * 7) ^ (t >> 5); break;
        }
    }

    WAVEHDR hdr = { buf,sizeof(buf),0,0,0,0,0,0 };
    waveOutPrepareHeader(h, &hdr, sizeof(hdr));
    waveOutWrite(h, &hdr, sizeof(hdr));
    Sleep(seconds * 1000);
    waveOutClose(h);
}

// ================= GDI EFFECT =================
DWORD WINAPI GdiEffect(LPVOID param)
{
    int type = (int)param;
    HDC hdc = GetWindowDC(GetDesktopWindow());
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    double a = 0;

    while (true)
    {
        switch (type)
        {
        case 0: // PatBlt effect
        {
            SelectObject(hdc, CreateSolidBrush(RGB(rand() % 225, rand() % 225, rand() % 225)));
            PatBlt(hdc, 0, 0, w, h, PATINVERT);
        }
        break;
        case 1: for (int i = 0; i < 20; i++) { RECT r = { rand() % w,rand() % h,rand() % w,rand() % h }; HBRUSH b = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)); FillRect(hdc, &r, b); DeleteObject(b); } break;
        case 2: for (int i = 0; i < 15; i++) Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h); break;
        case 3: for (int i = 0; i < 5; i++) { POINT pts[4]; for (int j = 0; j < 4; j++) { pts[j].x = rand() % w; pts[j].y = rand() % h; }PolyBezier(hdc, pts, 4); } break;
        case 4: for (int y = 0; y < h; y++) { int x = (int)(sin(a + y * 0.05) * 50); BitBlt(hdc, 0, y, w, 1, hdc, x, y, SRCCOPY); } a += 0.1; break;
        case 5: for (int x = 0; x < w; x++) { int y = (int)(cos(a + x * 0.05) * 50); BitBlt(hdc, x, 0, 1, h, hdc, x, y, SRCCOPY); } a += 0.1; break;
        case 6: for (int i = 0; i < 30; i++) Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h); break;
        case 7: for (int i = 0; i < 20; i++) { POINT pts[3] = { {rand() % w,rand() % h},{rand() % w,rand() % h},{rand() % w,rand() % h} }; Polygon(hdc, pts, 3); } break;
        case 8: BitBlt(hdc, 0, 0, w, h, hdc, rand() % 20 - 10, rand() % 20 - 10, SRCCOPY); break;
        case 9: for (int i = 0; i < 10; i++) PatBlt(hdc, rand() % w, rand() % h, 50, 50, PATINVERT); break;
        case 10: for (int i = 0; i < 200; i++) SetPixel(hdc, rand() % w, rand() % h, RGB(rand() % 256, rand() % 256, rand() % 256)); break;
        case 11: StretchBlt(hdc, 10, 10, w - 20, h - 20, hdc, 0, 0, w, h, SRCCOPY); break;
        case 12: for (int y = 0; y < h; y++) { int x = (int)(sin(a + y * 0.03) * 60); BitBlt(hdc, 0, y, w, 1, hdc, x, y, SRCCOPY); } a += 0.2; break;
        case 13: for (int i = 0; i < 50; i++) MoveToEx(hdc, rand() % w, rand() % h, NULL), LineTo(hdc, rand() % w, rand() % h); break;
        case 14: for (int i = 0; i < 200; i++) SetPixel(hdc, rand() % w, rand() % h, RGB(rand() % 256, rand() % 256, rand() % 256)); break;
        case 15: for (int i = 0; i < 10; i++) { POINT pts[4]; for (int j = 0; j < 4; j++) pts[j] = { rand() % w,rand() % h }; PolyBezier(hdc, pts, 4); } break;
        case 16: for (int y = 0; y < h; y++) { int x = (int)(sin(a + y * 0.05) * 50); BitBlt(hdc, 0, y, w, 1, hdc, x, y, SRCCOPY); } for (int i = 0; i < 10; i++) Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h); a += 0.1; break;
        case 17: for (int i = 0; i < 20; i++) Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h); for (int i = 0; i < 100; i++) SetPixel(hdc, rand() % w, rand() % h, RGB(rand() % 256, rand() % 256, rand() % 256)); break;
        case 18: PatBlt(hdc, 0, 0, w, h, PATINVERT); for (int y = 0; y < h; y++) { int x = (int)(cos(a + y * 0.04) * 40); BitBlt(hdc, 0, y, w, 1, hdc, x, y, SRCCOPY); } a += 0.1; break;
        case 19: for (int i = 0; i < 15; i++) { int s = rand() % 3; if (s == 0) Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h); if (s == 1) Ellipse(hdc, rand() % w, rand() % h, rand() % w, rand() % h); if (s == 2) { POINT pts[3] = { {rand() % w,rand() % h},{rand() % w,rand() % h},{rand() % w,rand() % h} }; Polygon(hdc, pts, 3); } } break;
        }

        Sleep(20);
    }
}

// ================= RUNNER =================
void RunEffect(int type)
{
    HANDLE t = CreateThread(0, 0, GdiEffect, (LPVOID)type, 0, 0);
    PlayRandomBytebeat(20); // 20 mp minden effekt
    TerminateThread(t, 0);
    CloseHandle(t);
}

// ================= ENTRY =================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    FreeConsole();
    srand((unsigned int)time(0));

    // 20 különbözõ effekt
    for (int i = 0; i < 20; i++)
        RunEffect(i);

    return 0;
}
