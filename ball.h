struct Vector{
	double x, y;
};

struct Ball{
	double t;
	Vector p, v;
	static double r;
	int x, y;
	static double g;
	Ball();
};
