#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvlib.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int cur_page;

private:
    Ui::MainWindow *ui;
    QStringList convert_row_to_qt_format(csv_data *arr_of_word);
    metrics_data *get_data_from_table(size_t col);
    void print_data_from_table(metrics_data* metrics_arr);
    void copy_node(metrics_node* a, metrics_node* b);
    void swap_metric_elem(metrics_node *a, metrics_node *b);
    metrics_data *sort_metrics_array(metrics_data* metrics_arr);
    metrics calc_metrics(int col);

private slots:
    void next_page();
    void prev_page();
    void getpath();
    void load_data();
    void calculate_data();
};
#endif // MAINWINDOW_H
