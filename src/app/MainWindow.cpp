#include "MainWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("WOH mod creator");
    resize(1000, 700);

    setCentralWidget(new QWidget(this));
    createMenus();
}

void MainWindow::createMenus() {
    auto* fileMenu = menuBar()->addMenu("&File");

    auto* newAction = fileMenu->addAction("&New");
    auto* openAction = fileMenu->addAction("&Open");
    auto* saveAction = fileMenu->addAction("&Save");
    auto* saveAsAction = fileMenu->addAction("Save &As");
    fileMenu->addSeparator();
    auto* exitAction = fileMenu->addAction("E&xit");

    Q_UNUSED(newAction);
    Q_UNUSED(openAction);
    Q_UNUSED(saveAction);
    Q_UNUSED(saveAsAction);

    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}