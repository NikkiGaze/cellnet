#include "mainwindow.hpp"

#include <QLayout>
#include <QLabel>

#include "mapscene.hpp"
#include "Model/model.hpp"
#include "Model/genetic.hpp"
#include "Model/coveragecalculator.hpp"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *window = new QWidget();

    QVBoxLayout * main_lay = new QVBoxLayout();
    window->setLayout(main_lay);

    scene = new MapScene();
    view = new QGraphicsView();
    view->setScene(scene);

    QHBoxLayout * base_lay = new QHBoxLayout();
    QLabel * baseRadius_lbl = new QLabel("Base radius: ");
    baseRadius_edit = new QLineEdit("20.0");
    baseRadius_edit->setReadOnly(true);
    QLabel * relief_lbl = new QLabel("Relief");
    relief_combo = new QComboBox();
    relief_combo->addItems(QStringList() << "DEFAULT" << "SINUS" << "LINEAR" << "MODULE" << "BLOCK");

    base_lay->addWidget(baseRadius_lbl);
    base_lay->addWidget(baseRadius_edit);
    base_lay->addStretch();
    base_lay->addWidget(relief_lbl);
    base_lay->addWidget(relief_combo);

    QHBoxLayout * demo_lay = new QHBoxLayout();
    QLabel * coverageCount_lbl = new QLabel("Coverage: ");
    coverageCount_edit = new QLineEdit();
    coverageCount_edit->setReadOnly(true);
    QLabel * genom_lbl = new QLabel("Genom: ");
    genom_edit = new QLineEdit();
    genom_edit->setReadOnly(true);
    demo_btn = new QPushButton("Start demo");

    demo_lay->addWidget(coverageCount_lbl);
    demo_lay->addWidget(coverageCount_edit);
    demo_lay->addWidget(genom_lbl);
    demo_lay->addWidget(genom_edit);
    demo_lay->addStretch();
    demo_lay->addWidget(demo_btn);

    QHBoxLayout * genetic_lay = new QHBoxLayout();

    QLabel * population_lbl = new QLabel("Population: ");
    population_edit = new QLineEdit("10");
    QLabel * crossing_lbl = new QLabel("Crossing: ");
    crossing_edit = new QLineEdit("10");
    QLabel * generations_lbl = new QLabel("Generations: ");
    generations_edit = new QLineEdit("10");
    genetic_btn = new QPushButton("Start genetic");

    genetic_lay->addWidget(population_lbl);
    genetic_lay->addWidget(population_edit);
    genetic_lay->addWidget(crossing_lbl);
    genetic_lay->addWidget(crossing_edit);
    genetic_lay->addWidget(generations_lbl);
    genetic_lay->addWidget(generations_edit);
    genetic_lay->addWidget(genetic_btn);

    main_lay->addWidget(view);
    main_lay->addLayout(base_lay);
    main_lay->addLayout(demo_lay);
    main_lay->addLayout(genetic_lay);

    setCentralWidget(window);

    connect(relief_combo, SIGNAL(currentIndexChanged(int)), SLOT(slotGenerateRelief(int)));
    connect(genetic_btn, SIGNAL(pressed()), SLOT(slotStartAlgorihtm()));
    connect(demo_btn, SIGNAL(pressed()), SLOT(slotShowDemo()));

    float radius = baseRadius_edit->text().toFloat();
    model = new Model(radius);
}

void MainWindow::run()
{
    setMaximumSize(700, 700);
    loadStations();
    slotGenerateRelief();
    show();
}

void MainWindow::loadStations()
{
    model->loadStations();
    for(size_t i = 0; i < model->getStations().size(); i++)
    {
        Station * station = model->getStations().at(i);
        int x = station->x;
        int y = station->y;
        scene->addEmptyStation(x, y);
    }
}

MainWindow::~MainWindow()
{
    delete model;
}

void MainWindow::slotShowDemo()
{
    model->clear();

    Genom gen(model->getStations().size());
    gen.randomGenerate();

    for(size_t i = 0; i < model->getStations().size(); i++)
    {
        Station * station = model->getStations().at(i);
        int x = station->x;
        int y = station->y;
        if(gen.contains(i))
        {
            scene->addRealStation(x, y);
        }
        else
        {
            scene->addEmptyStation(x, y);
        }
    }

    CoverageMask mask = model->calcCoverage(gen);

    scene->addMask(mask);
    coverageCount_edit->setText(QString::number(mask.size()));
    genom_edit->setText(QString::fromStdString(gen.toString(true)));
}

void MainWindow::slotStartAlgorihtm()
{
    blockSignals(true);
    int population_count = population_edit->text().toInt();
    int crossing = crossing_edit->text().toInt();
    int generations_count = generations_edit->text().toInt();

    model->startAlgorihtm(population_count, crossing, generations_count);
    blockSignals(false);
}

void MainWindow::slotGenerateRelief(int mode)
{
    model->clear();
    Matrix map = model->generateRelief(mode);
    scene->fillMap(map);
    for(size_t i = 0; i < model->getStations().size(); i++)
    {
        Station * station = model->getStations().at(i);
        int x = station->x;
        int y = station->y;
        scene->addEmptyStation(x, y);
    }
}