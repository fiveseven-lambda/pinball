#include <X11/Xlib.h>
#include <cmath>
#include <cfloat>
#include "ball.h"
#include "pin.h"

void Floor::draw(Display *display, Drawable drawable, GC gc, int, int width){
	XDrawLine(display, drawable, gc, 0, height, width, height);
}
double Floor::next_hit(const Ball &ball){
	double tmp0 = ball.v.y * ball.v.y - 4 * ball.g * (ball.p.y - height + ball.r);
	if(tmp0 < 0){
		return DBL_MAX;
	}else{
		double t0 = (-ball.v.y - std::sqrt(tmp0)) / (2 * ball.g);
		double t1 = (-ball.v.y + std::sqrt(tmp0)) / (2 * ball.g);
		if(t0 > 0){
			return t0;
		}else if(t1 > 0){
			return t1;
		}else{
			return DBL_MAX;
		}
	}
}

double Floor::next_hit_hit(const Ball &ball, double t){
	double ret = - ball.v.y / ball.g - t;
	if(ret < 0) ret = DBL_MAX;
	return ret;
}

void Floor::hit(Ball &ball, double t){
	ball.p.x = ball.p.x + ball.v.x * t;
	ball.p.y = ball.p.y + ball.v.y * t + ball.g * t * t;
	ball.t += t;
	ball.v.y = -ball.v.y - 2 * ball.g * t;
}
