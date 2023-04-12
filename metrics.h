#ifndef METRICS_H
#define METRICS_H

#include "mainwindow.h"
#include "csvlib.h"

struct metrics{
    double high;
    double minimum;
    double medium;
};

metrics *calc_metric () {
    metrics* temp = (metrics*)malloc(sizeof(metrics));

    return temp;
};

#endif // METRICS_H
