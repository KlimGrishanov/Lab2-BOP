#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "csvlib.h"

// Выгрузка первой строчки
// Ограничение страниц

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openfile_btn, &QPushButton::clicked, this, &MainWindow::getpath);
    connect(ui->load_data_btn, &QPushButton::clicked, this, &MainWindow::load_data);
    connect(ui->calculate_btn, &QPushButton::clicked, this, &MainWindow::calculate_data);
    connect(ui->next_page_btn, &QPushButton::clicked, this, &MainWindow::next_page);
    connect(ui->prev_page_btn, &QPushButton::clicked, this, &MainWindow::prev_page);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getpath() {
    QString path = QFileDialog::getOpenFileName();
    ui->path_lbl->setText(path);
}

void MainWindow::next_page(){
    QString path = ui->path_lbl->text();
    QString region = ui->region_line_edit->text();
    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    QStringList QColumns = convert_row_to_qt_format(arr_of_word);
    free_csv_data(*arr_of_word);
    ui->table->setHorizontalHeaderLabels(QColumns);
    ui->table->setRowCount(0);
    cur_page += 1;
    ui->lbl_pages->setText(QString::number(cur_page-1));
    int i = 0;
    int index = (cur_page-1)*ONE_PAGE-ONE_PAGE+1;
    while(i < 100){ //Читает одну и ту же строчку
        std::string line = read_record(index, path.toStdString());
        if(line == ERR_END_OF_FILE || line == ERR_FILE_NOT_EXIST){
            break;
        }
        csv_data* arr_of_word = split_record_to_word(line);
        print_arr_of_word(*arr_of_word);
        std::cout << region.toStdString() << std::endl;
        if(is_required_region(arr_of_word, region.toStdString()) || region == ""){
            QStringList row = convert_row_to_qt_format(arr_of_word);
            free_csv_data(*arr_of_word);
            ui->table->setRowCount(i+1);
            for(size_t k = 0; k < (size_t)ui->table->columnCount(); k++){
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setData(Qt::EditRole, row.at(k).toDouble());
                item->setText(row.at(k));
                ui->table->setItem(i, k, item);
            }
            i++;
        }
        index++;
    }
}

void MainWindow::prev_page(){
    QString path = ui->path_lbl->text();
    QString region = ui->region_line_edit->text();
    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    QStringList QColumns = convert_row_to_qt_format(arr_of_word);
    free_csv_data(*arr_of_word);
    ui->table->setHorizontalHeaderLabels(QColumns);
    ui->table->setRowCount(0);
    cur_page -= 1;
    ui->lbl_pages->setText(QString::number(cur_page-1));
    int i = 0;
    int index = (cur_page-1)*ONE_PAGE-ONE_PAGE+1;
    while(i < 100){
        std::string line = read_record(index, path.toStdString());
        if(line == ERR_END_OF_FILE || line == ERR_FILE_NOT_EXIST){
            break;
        }
        csv_data* arr_of_word = split_record_to_word(line);
        print_arr_of_word(*arr_of_word);
        std::cout << region.toStdString() << std::endl;
        if(is_required_region(arr_of_word, region.toStdString()) || region == ""){
            QStringList row = convert_row_to_qt_format(arr_of_word);
            free_csv_data(*arr_of_word);
            ui->table->setRowCount(i+1);
            for(size_t k = 0; k < (size_t)ui->table->columnCount(); k++){
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setData(Qt::EditRole, row.at(k).toDouble());
                item->setText(row.at(k));
                ui->table->setItem(i, k, item);
            }
            i++;
        }
        index++;
    }
}

bool isDouble(const std::string& str)
{
    bool result = true;
    try
    {
        std::stod(str);
    }
    catch (const std::invalid_argument& e)
    {
        result = false;
    }
    catch (const std::out_of_range& e)
    {
        result = false;
    }

    return result;
}


void MainWindow::load_data() {
    QString path = ui->path_lbl->text();
    QString region = ui->region_line_edit->text();
    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    ui->table->setColumnCount(arr_of_word->node_quantity);
    QStringList QColumns = convert_row_to_qt_format(arr_of_word);
    ui->table->setHorizontalHeaderLabels(QColumns);
    free_csv_data(*arr_of_word);
    ui->table->setRowCount(0);
    cur_page = 1;
    ui->lbl_pages->setText(QString::number(cur_page));
    cur_page++;
    int i = 0;
    int index = 1;
    while(i < 100){
        std::string line = read_record(index, path.toStdString());
        if(line == ERR_END_OF_FILE || line == ERR_FILE_NOT_EXIST){
            break;
        }
        csv_data* arr_of_word = split_record_to_word(line);
        std::cout << region.toStdString() << std::endl;
        if(is_required_region(arr_of_word, region.toStdString()) || region == ""){
            QStringList row = convert_row_to_qt_format(arr_of_word);
            free_csv_data(*arr_of_word);
            ui->table->setRowCount(i+1);
            for(size_t k = 0; k < (size_t)ui->table->columnCount(); k++){
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setData(Qt::EditRole, row.at(k).toDouble());
                item->setText(row.at(k));
                ui->table->setItem(i, k, item);
            }
            i++;
        }
        index++;
    }
}

void MainWindow::calculate_data() {
    calc_metrics(ui->metrics_line_edit->text().toInt());
}

metrics MainWindow::calc_metrics(int col) {
    metrics_data* data = get_data_from_table(col);
    print_data_from_table(data);
    std::cout << "hi\n";
    sort_metrics_array(data);
    print_data_from_table(data);
    metrics data_metrics;
    ui->high_lbl->setText(QString::number(isDouble(data->tail_node->value) ? std::stod(data->tail_node->value) : 0));
    ui->low_lbl->setText(QString::number(isDouble(data->head_node->value) ? std::stod(data->head_node->value) : 0));

    if(data->quantity_records % 2 == 0) {
        std::cout << "%2 == 0\n";
        metrics_node *node = data->head_node;
        double avg_medium = 0;
        for(int i = 0; i <= data->quantity_records/2; i++){
            if((i == data->quantity_records/2-1 || i == data->quantity_records/2) && isDouble(node->value)) {
                std::cout << "+ " << node->value << '\n';
                avg_medium += std::stod(node->value);
            }
            node = node->next_node;
            std::cout << "MEDIUM I: " << i << "; MEDIUM VALUE: " << data_metrics.medium << '\n';
        }
        avg_medium /= 2;
        data_metrics.medium = avg_medium;
    } else {
        std::cout << "%2 != 0\n";
        metrics_node *node = data->head_node;
        for(int i = 0; i <= data->quantity_records/2; i++){
            if(isDouble(node->value)){
                ui->medium_lbl->setText(QString::number(std::stod(node->value)));
            } else {
                ui->medium_lbl->setText("0");
            }
            node = node->next_node;
            std::cout << "MEDIUM I: " << i << "; MEDIUM VALUE: " << data_metrics.medium << '\n';
        }
    }

    std::cout << data_metrics.high << '\n' << data_metrics.low << '\n' << data_metrics.medium << '\n';

    return data_metrics;
}

metrics_data *MainWindow::sort_metrics_array(metrics_data* metrics_arr) {
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

void MainWindow::swap_metric_elem(metrics_node *a, metrics_node *b){
    std::string temp_value = a->value;
    a->value = b->value;
    b->value = temp_value;
}

metrics_data *MainWindow::get_data_from_table(size_t col) {
    metrics_data* metrics = (metrics_data*)malloc(sizeof(metrics_data));
    metrics->quantity_records = 1;
    metrics_node* metric_elem = (metrics_node*)malloc(sizeof(metrics_node));
    metrics->head_node = metric_elem;
    metrics->tail_node = metric_elem;
    for (size_t i = 0; i < (size_t)ui->table->rowCount(); i++){
        QTableWidgetItem *item = ui->table->item(i, col);
        if(isDouble(item->text().toStdString())) {
            metric_elem->value = item->text().toStdString();
            if(i != (size_t)ui->table->rowCount()-1){
                metrics_node* next_elem = (metrics_node*)malloc(sizeof(metrics_node));
                metric_elem->next_node = next_elem;
                metric_elem = next_elem;
                metrics->tail_node = metric_elem;
                metrics->quantity_records++;
            }
        }
    }
    return metrics;
}

void MainWindow::print_data_from_table(metrics_data* metrics_arr) {
    metrics_node *metric_elem = metrics_arr->head_node;
    for(int i = 0; i < metrics_arr->quantity_records; i++) {
        std::cout << metric_elem->value << '\n';
        metric_elem = metric_elem->next_node;
    }
}

//metrics_data *MainWindow::metrics_sort(metrics_data* not_sorted_metrics_arr){

//}

QStringList MainWindow::convert_row_to_qt_format(csv_data *arr_of_word)  {
    QStringList temp = {};
    csv_node* node = arr_of_word->head_csv_node;
    for(size_t i = 0; i < (size_t)ui->table->columnCount(); i++){
        if(node == nullptr){
            temp.append("");
        } else {
            temp.append(QString::fromStdString(node->word));
            node = node->next_csv_node;
        }
    }
    return temp;
}
