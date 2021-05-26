#include "mainwindow.h"

MainWindow::MainWindow() {
    scene = new Scene(this);
    scene->setSceneRect(0,0,200,200);
    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing);
    setCentralWidget(view);

    createActions();
    createConnections();
    createToolBar();
}

void MainWindow::createActions(){
    selectAction = new QAction("Select object", this);
    selectAction->setData(int(Scene::SelectObject));
    selectAction->setIcon(QIcon(":/icons/select.png"));
    selectAction->setCheckable(true);

    routerAction = new QAction("Draw Router", this);
    routerAction->setData(int(Scene::DrawRouter));
    routerAction->setIcon(QIcon(":/icons/router.png"));
    routerAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(routerAction);;
}

void MainWindow::createConnections(){
    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));
}

void MainWindow::actionGroupClicked(QAction *action){
    scene->setMode(Scene::Mode(action->data().toInt()));
}

void MainWindow::createToolBar(){
    drawingToolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);
    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(routerAction);
}
