#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <set>

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

class Model;
class MapScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPushButton * demo_btn, * genetic_btn;
    QComboBox * relief_combo;
    QGraphicsView * view;
    QLineEdit * coverageCount_edit, * baseRadius_edit,
              * population_edit, * crossing_edit, * generations_edit,
              * genom_edit;
    MapScene *scene;

    Model * model;

    void loadStations();

public:
    MainWindow(QWidget *parent = 0);
    void run();
    ~MainWindow();    

private slots:
    void slotShowDemo();
    void slotStartAlgorihtm();
    void slotGenerateRelief(int mode = 0);
};

#endif // MAINWINDOW_HPP
