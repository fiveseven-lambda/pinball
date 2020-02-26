#include <sys/time.h>
#include <cstddef>
#include "ball.h"

Ball::Ball(){
	struct timeval start_time;
	gettimeofday(&start_time, NULL);
	p.x = 50;
	p.y = 100;
	v.x = 10;
	v.y = 0;
	t = start_time.tv_sec + start_time.tv_usec * .000001;
}
