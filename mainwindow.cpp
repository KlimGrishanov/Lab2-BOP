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

// Getter
int MainWindow::get_current_page() {
    return cur_page;
}

int MainWindow::get_region_col() {
    return region_col;
}


//Setter
void MainWindow::set_current_page(int new_page) {
    cur_page = new_page;
}

void MainWindow::set_region_col(int new_region_col) {
    region_col = new_region_col;
}

void MainWindow::getpath() {
    QString path = QFileDialog::getOpenFileName();
    ui->path_lbl->setText(path);
}

// Headers
void MainWindow::init_header() {
    QString path = ui->path_lbl->text();

    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    ui->table->setColumnCount(arr_of_word->node_quantity);
    set_region_col(find_region_col(arr_of_word));
    QStringList QColumns = convert_row_to_qt_format(arr_of_word);
    ui->table->setHorizontalHeaderLabels(QColumns);
    free_csv_data(*arr_of_word);
}

void MainWindow::set_header() {
    QString path = ui->path_lbl->text();

    std::string header = read_record(0, path.toStdString());
    csv_data* arr_of_word = split_record_to_word(header);
    QStringList QColumns = convert_row_to_qt_format(arr_of_word);
    free_csv_data(*arr_of_word);
    ui->table->setHorizontalHeaderLabels(QColumns);
}


// DataLoader
void MainWindow::load_rows(int i, int index) {
    QString path = ui->path_lbl->text();
    QString region = ui->region_line_edit->text();

    ui->table->setRowCount(0);
    while(i < 100){
        std::string line = read_record(index, path.toStdString());
        if(line == ERR_END_OF_FILE || line == ERR_FILE_NOT_EXIST){
            break;
        }
        csv_data* arr_of_word = split_record_to_word(line);
        if(arr_of_word->node_quantity != 0 && (is_required_region(arr_of_word, region.toStdString(), get_region_col()) || region == "")){
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

// QT Utilitiy
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

// SLOTS
void MainWindow::next_page(){
    set_header();

    set_current_page(get_current_page() + 1);
    ui->lbl_pages->setText(QString::number(get_current_page()-1));
    if(get_current_page() > 1) {
        ui->prev_page_btn->setEnabled(1);
    }

    int i = 0;
    int index = (get_current_page()-1)*ONE_PAGE-ONE_PAGE+1;
    load_rows(i, index);
}

void MainWindow::prev_page(){
    set_header();

    set_current_page(get_current_page() - 1);
    ui->lbl_pages->setText(QString::number(get_current_page()-1));
    if(get_current_page() == 2) {
        ui->prev_page_btn->setDisabled(1);
    }

    int i = 0;
    int index = (get_current_page()-1)*ONE_PAGE-ONE_PAGE+1;
    load_rows(i, index);
}

void MainWindow::load_data() {
    init_header();

    ui->table->setRowCount(0);

    set_current_page(1);
    ui->lbl_pages->setText(QString::number(get_current_page()));
    ui->next_page_btn->setEnabled(1);
    set_current_page(get_current_page() + 1);

    int i = 0;
    int index = 1;
    load_rows(i, index);
}

void MainWindow::calculate_data() {
    int col = ui->metrics_line_edit->text().toInt();
    metrics_data* data = get_data_from_table(col);
    metrics metrics_value = calc_metrics(data);

    ui->high_lbl->setText(QString::number(metrics_value.high));
    ui->low_lbl->setText(QString::number(metrics_value.low));
    ui->medium_lbl->setText(QString::number(metrics_value.medium));
}
