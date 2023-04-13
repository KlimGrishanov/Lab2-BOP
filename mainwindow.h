#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvlib.h"
#include "metrics.h"

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
    int region_col = -1;

private:
    Ui::MainWindow *ui;
    QStringList convert_row_to_qt_format(csv_data *arr_of_word);
    metrics_data *get_data_from_table(size_t col);
    void set_header();
    void load_rows(int i, int index);
    void init_header();

    int get_current_page();
    void set_current_page(int new_page);
    int get_region_col();
    void set_region_col(int new_region_col);

private slots:
    void next_page();
    void prev_page();
    void getpath();
    void load_data();
    void calculate_data();
};
#endif // MAINWINDOW_H
