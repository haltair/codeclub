#include "lovirand.h"
#include "lovimath.h"
#include <cstdlib>
#include <ctime>

using namespace std;

namespace dlovi {
    void seedRand() {
        srand((unsigned) time(NULL));
    }

    int randInt(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    double uniformRand() {
        return double(rand()) / double(RAND_MAX);
    }

    double uniformRand(double min, double max) {
        return (double(rand()) / double(RAND_MAX) * (max - min)) + min;
    }

    double gaussianRand() {
        // Implemented according to the polar Box-Muller transform method in: http://www.taygeta.com/random/gaussian.html
        double x1, x2, w;
        do {
            x1 = 2.0 * uniformRand() - 1.0;
            x2 = 2.0 * uniformRand() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        return x1 * w;
    }

    double gaussianRand(double mu, double sigma) {
        // Implemented according to the polar Box-Muller transform method in: http://www.taygeta.com/random/gaussian.html
        double x1, x2, w, z;
        do {
            x1 = 2.0 * uniformRand() - 1.0;
            x2 = 2.0 * uniformRand() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        z = x1 * w;

        return z * sigma + mu;
    }
}
