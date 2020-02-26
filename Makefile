a.out: main.cpp ball.h ball.cpp pin.h pin.cpp
	g++ main.cpp ball.cpp pin.cpp -lX11
