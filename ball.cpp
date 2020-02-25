#include <sys/time.h>
#include <cstddef>
#include "ball.h"

double Ball::r = 10;
double Ball::g = 20;

Ball::Ball(){
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	p.x = 50;
	p.y = 50;
	v.x = 100;
	t = startTime.tv_sec + startTime.tv_usec * .000001;
}
