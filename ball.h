struct Vector{
	double x, y;
};

struct Ball{
	double t;
	Vector p, v;
	constexpr static double r = 10;
	int x, y;
	constexpr static double g = 100;
	Ball();
};
