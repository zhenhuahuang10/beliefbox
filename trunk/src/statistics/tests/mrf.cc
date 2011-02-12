#include "Matrix.h"
#include "Random.h"
#include "MersenneTwister.h"
#include "Distribution.h"
#include <vector>

class MarkovRandomField
{
public:
	Matrix K; ///< kernel
	real beta; ///< beta
	int H; ///< height
	int W; ///< width
	int n_classes; ///< classes
	Matrix S; ///< states
	Matrix V; ///< potentials
	MersenneTwisterRNG rng; ///< rng

	struct Point
	{
		int x, y;
		Point() : x(0), y(0) {}
		Point(int x_, int y_) : x(x_), y(y_) {}
	};

	MarkovRandomField(Matrix& kernel, real inverse_temperature, int height, int width, int n_classes_)
		: K(kernel),
		  beta(inverse_temperature),
		  H(height),
		  W(width),
		  n_classes(n_classes_),
		  S(width, height),
		  V(width, height)
	{
		for (int x=0; x<W; ++x) {
			for (int y=0; y<H; ++y) {
				S(x, y) = rng.discrete_uniform(n_classes);
			}
		}
	}
	void SweepGibbs()
	{
		int n = 4;
		std::vector<Point> neighbours(n);
		for (int x=0; x<W; ++x) {
			for (int y=0; y<H; ++y) {
				Point p(x, y);
				neighbours[0] = Point((x-1) % W, y);
				neighbours[1] = Point((x+1) % W, y);
				neighbours[2] = Point(x, (y - 1) % H);
				neighbours[3] = Point(x, (y+1) % H);
				Vector v(n_classes);
				std::vector<int> c(n);
				for (int i=0; i<n; ++i) {
					c[i] = (int) S(neighbours[i].x, neighbours[i].y);
				}
				for (int z=0; z<n_classes; ++z) {
					v(z) = 0;
					for (int i=0; i<n; ++i) {
						v(z) += K(z, c[i]);
					}
				}
				Vector P = exp(v*beta);
				P /= P.Sum();
				S(x, y) = DiscreteDistribution::generate(P);
			}
		}
	}

	void SweepMH()
	{
		int n = 4;
		std::vector<Point> neighbours(n);
		for (int x=0; x<W; ++x) {
			for (int y=0; y<H; ++y) {
				Point p(x, y);
				neighbours[0] = Point((x-1) % W, y);
				neighbours[1] = Point((x+1) % W, y);
				neighbours[2] = Point(x, (y - 1) % H);
				neighbours[3] = Point(x, (y+1) % H);
				Vector v(n_classes);
				std::vector<int> c(n);
				for (int i=0; i<n; ++i) {
					c[i] = (int) S(neighbours[i].x, neighbours[i].y);
				}
				for (int z=0; z<n_classes; ++z) {
					v(z) = 0;
					for (int i=0; i<n; ++i) {
						v(z) += K(z, c[i]);
					}
				}
				// use simple trick to generate uniformly a value other than current
				int s_old = S(x, y);				
				int s_new = rng.discrete_uniform(n_classes - 1);
				if (s_new == s_old) {
					s_new = n_classes - 1;
				}
				real v_old = v(s_old);
				real v_new = v(s_new);
				real accept = exp(beta * (v_new - v_old)); 
				if (urandom() < accept) { // no need to take min
					S(x, y) = s_new;
				}
			}
		}
	}
};


int main(void)
{

	
	real beta = 0.85;
	int h = 128;
	int w = 128;
	int n_classes = 4;

	Matrix K(Matrix::Unity(n_classes, n_classes));
	K = K * 2 - 1;
	//K.print(stdout);
	
	MarkovRandomField mrf(K,
						  beta,
						  h, w,
						  n_classes);
	//mrf.S.print(stdout);
	for (int i=0; i<1000; ++i) {
		mrf.SweepGibbs();
	}
	mrf.S.print(stdout);
}
