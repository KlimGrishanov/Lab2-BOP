#ifndef METRICS_H
#define METRICS_H

#include <string>
#include "utility.h"

struct metrics_node{
    std::string value;
    metrics_node *next_node;
};

struct metrics_data{
    int quantity_records;
    metrics_node *head_node = nullptr;
    metrics_node *tail_node = nullptr;
};

struct metrics {
    double high;
    double low;
    double medium;
};

metrics calc_metrics(metrics_data* data);
metrics_data *sort_metrics_array(metrics_data* metrics_arr);
void swap_metric_elem(metrics_node *a, metrics_node *b);

#endif // METRICS_H
