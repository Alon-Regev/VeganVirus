#include <Windows.h>
#include <math.h>

#define PI 3.1415926
#define R 200

// function moves mouse
void moveMouse(int dx, int dy)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dx = dx;
	input.mi.dy = dy;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	// send keyboard inputs
	SendInput(1, &input, sizeof(INPUT));
}

void main()
{
	for (int i = 0; i < 120; i++)
	{
		float prevAngle = i / 40. * 2 * PI;
		float prevDX = R * sin(prevAngle);
		float prevDY = -R * cos(prevAngle) * prevDX / R;
		float angle = (i + 1) / 40. * 2 * PI;
		float DX = R * sin(angle);
		float DY = -R * cos(angle) * DX / R;
		moveMouse(DX - prevDX, DY - prevDY);
		Sleep(15);
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0));
}