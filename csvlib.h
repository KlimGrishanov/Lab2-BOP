#ifndef CSVLIB_H
#define CSVLIB_H

#include <string>
#include <fstream>
#include <iostream>

#define SEPARATOR ','
#define ERR_FILE_NOT_EXIST "Can't open file"
#define ERR_END_OF_FILE "EOF!"
#define ONE_PAGE 100

struct csv_node {
    std::string word;
    csv_node* next_csv_node = nullptr;
};

struct csv_data {
    int node_quantity;
    csv_node* head_csv_node;
};

struct metrics {
    double high;
    double low;
    double medium;
};

struct metrics_node{
    std::string value;
    metrics_node *next_node;
};

struct metrics_data{
    int quantity_records;
    metrics_node *head_node = nullptr;
    metrics_node *tail_node = nullptr;
};

std::ifstream openfile(std::string filename);
std::string read_record(int num_of_line, std::string filename);
csv_data* split_record_to_word(std::string temp);
void print_arr_of_word(csv_data data);
void free_csv_data(csv_data data);
bool is_required_region(csv_data* data, std::string region);

#endif // CSVLIB_H
