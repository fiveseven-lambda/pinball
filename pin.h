class Pin{
public:
	double hit_time;
	virtual void draw(Display *, Drawable, GC, int, int) = 0;
	virtual double next_hit(const Ball &) = 0;
	virtual double next_hit_hit(const Ball &, double) = 0;
	virtual void hit(Ball &, double) = 0;
};

class Floor : public Pin{
public:
	double height;
	Floor(double height) : height(height) {}
	void draw(Display *, Drawable, GC, int, int);
	double next_hit(const Ball &);
	double next_hit_hit(const Ball &, double);
	void hit(Ball &, double);
};
