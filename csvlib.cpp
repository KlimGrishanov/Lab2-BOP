#include "csvlib.h"

std::ifstream openfile(std::string filename) {
    std::ifstream inFile(filename);
    return inFile;
}

bool is_required_region(csv_data* data, std::string region){
    bool ans = false;
    csv_node *first_node = data->head_csv_node;
    for(int i = 0; i < data->node_quantity; i++){
        if(first_node->word == region) {
            ans = true;
            break;
        }
        first_node = first_node->next_csv_node;
    }
    return ans;
}

std::string read_record(int num_of_line, std::string filename) {
    std::string temp = "";
    std::ifstream file = openfile(filename);
    if(file.is_open()){
        for(int i = 0; i <= num_of_line; i++){
            if(file.eof()){
                temp = ERR_END_OF_FILE;
                break;
            }
            std::getline(file, temp);
        }
        file.close();
    } else {
        temp = ERR_FILE_NOT_EXIST;
    }
    return temp;
}

csv_data* split_record_to_word(std::string temp) {
    csv_node* first_node = (csv_node*)malloc(sizeof(csv_node));
    csv_data* data = (csv_data*)malloc(sizeof(csv_data));
    data->head_csv_node = first_node;
    data->node_quantity = 0;
    int last_delim = 0;
    if(first_node != NULL) {
        for(size_t i = 0; i < temp.length(); i++) {
            if(temp[i] == SEPARATOR || i == temp.length()-1) {
                csv_node *node = (csv_node*)malloc(sizeof(csv_node));
                node->word = "";
                if(i != temp.length()-1){
                    for(size_t k = last_delim; k < i; k++) {
                        first_node->word += temp[k];
                    }
                } else {
                    int diff = i - last_delim;
                    for(size_t k = last_delim; k <= i && diff != 0; k++) {
                        first_node->word += temp[k];
                    }
                }
                first_node->next_csv_node = node;
                first_node = first_node->next_csv_node;
                data->node_quantity++;
                last_delim = i+1;
            }
            if(temp[i] == SEPARATOR && i == temp.length()-1){
                csv_node *node = (csv_node*)malloc(sizeof(csv_node));
                node->word = "";
                first_node->next_csv_node = node;
                first_node = first_node->next_csv_node;
                data->node_quantity++;
            }
        }
    }
    std::cout << "Node Q: " << data->node_quantity << '\n';
    std::cout << "Region: " << data->head_csv_node->next_csv_node->word << '\n';
    return data;
}

void print_arr_of_word(csv_data data) {
    csv_node *first_node = data.head_csv_node;
    for(int i = 0; i < data.node_quantity; i++){
        std::cout << first_node->word << '\n';
        first_node = first_node->next_csv_node;
    }
}

void free_csv_data(csv_data data) {
    csv_node *first_node = data.head_csv_node;
    for(int i = 0; i < data.node_quantity; i++) {
        csv_node *next_node = first_node->next_csv_node;
        free(first_node);
        first_node = next_node;
    }
}
