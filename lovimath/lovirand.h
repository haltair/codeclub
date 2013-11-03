#pragma once

namespace dlovi {
    void seedRand();
    int randInt(int min, int max);
    double uniformRand();
    double uniformRand(double min, double max);
    double gaussianRand();
    double gaussianRand(double mu, double sigma);
}
