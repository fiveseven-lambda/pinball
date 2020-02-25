#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <sys/time.h>

#include "ball.h"

const int Width = 500, Height = 700;

int main(void){
	Display *display = XOpenDisplay(NULL);
	if(display == NULL){
		fputs("error: XOpenDisplay failed\n", stderr);
		return -1;
	}
	int defaultScreen = XDefaultScreen(display);

	int windowWidth = Width;
	int windowHeight = Height;
	Window rootWindow = XRootWindow(display, defaultScreen);

	int defaultDepth = XDefaultDepth(display, defaultScreen);
	GC defaultGC = XDefaultGC(display, defaultScreen);

	unsigned long black = XBlackPixel(display, defaultScreen);
	unsigned long white = XWhitePixel(display, defaultScreen);

	Window window = XCreateSimpleWindow(
		display,
		rootWindow,
		0, 0,
		windowWidth, windowHeight,
		0, black,
		white
	);

	XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

	Pixmap buffer = XCreatePixmap(display, window, Width, Height, defaultDepth);

	XGCValues xgc;
	xgc.foreground = black;
	GC bufBlackGC = XCreateGC(display, buffer, GCForeground, &xgc);
	GC winBlackGC = XCreateGC(display, window, GCForeground, &xgc);
	xgc.foreground = white;
	GC bufWhiteGC = XCreateGC(display, buffer, GCForeground, &xgc);
	GC winWhiteGC = XCreateGC(display, window, GCForeground, &xgc);

	XFillRectangle(display, buffer, bufWhiteGC, 0, 0, Width, Height);

	Ball ball;

	XMapWindow(display, window);
	XEvent event;

	struct{
		int x, y;
	} area;

	auto copyarea = [&](int x, int y, int width, int height){
		bool flag = false;
		int x0, y0, x1, y1;
		if(windowWidth > Width) area.x = (windowWidth - Width) / 2;
		else area.x = 0;
		x0 = area.x;
		x1 = area.x + Width;
		if(x0 < x) x0 = x;
		else flag = true;
		if(x1 > x + width) x1 = x + width;
		else flag = true;
		if(windowHeight > Height) area.y = (windowHeight - Height) / 2;
		else area.y = 0;
		y0 = area.y;
		y1 = area.y + Height;
		if(y0 < y) y0 = y;
		else flag = true;
		if(y1 > y + height) y1 = y + height;
		else flag = true;
		if(flag) XFillRectangle(display, window, winWhiteGC, 0, 0, windowWidth, windowHeight);
		XCopyArea(
			display, buffer, window, defaultGC,
			x0 - area.x, y0 - area.y,
			x1 - x0, y1 - y0,
			x0, y0
		);
	};

	for(;;){
		if(XPending(display)){
			XNextEvent(display, &event);
			switch(event.type){
				case Expose:
					copyarea(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
					break;
				case DestroyNotify:
					XFreeGC(display, bufBlackGC);
					XFreeGC(display, bufWhiteGC);
					XFreePixmap(display, buffer);
					XCloseDisplay(display);
					return 0;
				case ConfigureNotify:
					windowWidth = event.xconfigure.width;
					windowHeight = event.xconfigure.height;
					break;
			}
		}else{
			struct timeval currentTime;
			gettimeofday(&currentTime, NULL);
			double t = currentTime.tv_sec + currentTime.tv_usec * .000001 - ball.t;

			XDrawArc(display, buffer, bufWhiteGC, ball.x, ball.y, ball.r * 2, ball.r * 2, 0, 360 * 64);
			copyarea(area.x + ball.x, area.y + ball.y, ball.r * 2 + 1, ball.r * 2 + 1);

			ball.x = ball.p.x + ball.v.x * t;
			ball.y = ball.p.y + ball.v.y * t + ball.g * t * t;

			XDrawArc(display, buffer, bufBlackGC, ball.x, ball.y, ball.r * 2, ball.r * 2, 0, 360 * 64);

			copyarea(area.x + ball.x, area.y + ball.y, ball.r * 2 + 1, ball.r * 2 + 1);
			usleep(20000);
		}
	}
}
