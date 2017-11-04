#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BasicRenderer\ObjLoader.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	renderView = ui->renderWidget;
	scene = new SceneObject(ObjLoader::Load("C:/BasicRenderer/Assets/bunny.obj")); //"../../Assets/bunny.obj")
	renderView->SetScene(scene);
	renderView->setFocus();

	SetupSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
	delete scene;
}

void MainWindow::SetupSignals()
{
	connect(ui->actionRender, SIGNAL(triggered()), this, SLOT(RenderImage()));
    connect(renderView, SIGNAL(RenderingCompleted(double)), this, SLOT(UpdateFpsLabel(double)));
}

void MainWindow::UpdateFpsLabel(double time)
{
	std::string rTime= std::to_string(time);
	rTime = rTime.substr(0, rTime.length() - 6).append("ms ");
	ui->fpsLabel->setText(rTime.c_str());
}

//TODO later for offline rendering
void MainWindow::RenderImage()
{
	renderView->update();

	
}
