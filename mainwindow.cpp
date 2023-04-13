#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "csvlib.h"
#include "metrics.h"
#include "utility.h"

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
    while(i < 100){
        std::string line = read_record(index, path.toStdString());
        if(line == ERR_END_OF_FILE || line == ERR_FILE_NOT_EXIST){
            break;
        }
        csv_data* arr_of_word = split_record_to_word(line);
        print_arr_of_word(*arr_of_word);
        std::cout << region.toStdString() << std::endl;
        if(is_required_region(arr_of_word, region.toStdString(), region_col) || region == ""){
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
        if(is_required_region(arr_of_word, region.toStdString(), region_col) || region == ""){
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

void MainWindow::load_data() {
    QString path = ui->path_lbl->text();
    QString region = ui->region_line_edit->text();
    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    ui->table->setColumnCount(arr_of_word->node_quantity);
    region_col = find_region_col(arr_of_word);
    std::cout << region_col << '\n';
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
        if(line != ""){
            csv_data* arr_of_word = split_record_to_word(line);
            std::cout << region.toStdString() << std::endl;
            if(is_required_region(arr_of_word, region.toStdString(), region_col) || region == ""){
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
        }
        index++;
    }
}

void MainWindow::calculate_data() {
    int col = ui->metrics_line_edit->text().toInt();
    metrics_data* data = get_data_from_table(col);
    metrics metrics_value = calc_metrics(data);

    ui->high_lbl->setText(QString::number(metrics_value.high));
    ui->low_lbl->setText(QString::number(metrics_value.low));
    ui->medium_lbl->setText(QString::number(metrics_value.medium));
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

QStringList MainWindow::convert_row_to_qt_format(csv_data *arr_of_word)  {
    QStringList temp = {};
    csv_node* node = arr_of_word->head_csv_node;
    for(size_t i = 0; i < (size_t)ui->table->columnCount() && arr_of_word->node_quantity != 0; i++){
        if(node == nullptr){
            temp.append("");
        } else {
            temp.append(QString::fromStdString(node->word));
            node = node->next_csv_node;
        }
    }
    return temp;
}
