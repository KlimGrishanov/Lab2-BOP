#include "metrics.h"

// Calculating Metrics
metrics calc_metrics(metrics_data* data) {
    sort_metrics_array(data);
    metrics data_metrics;

    data_metrics.high = isDouble(data->tail_node->value) ? std::stod(data->tail_node->value) : 0;
    data_metrics.low = isDouble(data->head_node->value) ? std::stod(data->head_node->value) : 0;
    data_metrics.medium = 0;

    if(data->quantity_records % 2 == 0) {
        metrics_node *node = data->head_node;
        double avg_medium = 0;
        for(int i = 0; i <= data->quantity_records/2; i++){
            if((i == data->quantity_records/2-1 || i == data->quantity_records/2) && isDouble(node->value)) {
                avg_medium += std::stod(node->value);
            }
            node = node->next_node;
        }
        avg_medium /= 2;
        data_metrics.medium = avg_medium;
    } else {
        metrics_node *node = data->head_node;
        for(int i = 0; i <= data->quantity_records/2; i++){
            if(isDouble(node->value)){
                data_metrics.medium = std::stod(node->value);
            }
            node = node->next_node;
        }
    }

    return data_metrics;
}


// Sort Metrics Array
metrics_data *sort_metrics_array(metrics_data* metrics_arr) {
    metrics_node *node = metrics_arr->head_node;
    for(int i = 0; i < metrics_arr->quantity_records-1; i++) {
        metrics_node *temp_node = node->next_node;
        for(int k = i+1; k < metrics_arr->quantity_records; k++){
            if(isDouble(node->value) && isDouble(temp_node->value) && std::stod(node->value) > std::stod(temp_node->value)){
                swap_metric_elem(node, temp_node);
            }
            temp_node = temp_node->next_node;
        }
        node = node->next_node;
    }
    return metrics_arr;
}

void swap_metric_elem(metrics_node *a, metrics_node *b){
    std::string temp_value = a->value;
    a->value = b->value;
    b->value = temp_value;
}

