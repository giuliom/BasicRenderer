#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	renderView = ui->renderWidget;
	renderView->SetScene("C:/BasicRenderer/Assets/bunny.obj"); //"../../Assets/bunny.obj");
	renderView->setFocus();

	widthLineEdit = ui->widthEdit;
	heightLineEdit = ui->heightEdit;
	renderButton = ui->renderButton;
	saveButton = ui->saveButton;
	renderingCombo = ui->rendererComboBox;
	shadingCombo = ui->shadingComboBox;

	renderingCombo->addItem("Rasterizer");
	renderingCombo->addItem("Raytracer");

	shadingCombo->addItem("Flat");
	shadingCombo->addItem("Blinn-Phong");

	SetupSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupSignals()
{
	connect(ui->actionRender, SIGNAL(triggered()), this, SLOT(RenderImage()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(SaveImage()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
	connect(renderButton, SIGNAL(pressed()), this, SLOT(RenderImage()));
	connect(saveButton, SIGNAL(pressed()), this, SLOT(SaveImage()));
    connect(renderView, SIGNAL(RenderingCompleted(double)), this, SLOT(UpdateFpsLabel(double)));
	connect(renderView, SIGNAL(RendererResized()), this, SLOT(UpdateResolutionLabels()));
}

//TODO later for offline rendering
void MainWindow::RenderImage()
{
	renderView->update();


}

void MainWindow::SaveImage()
{
}

void MainWindow::OpenFile()
{
}



void MainWindow::UpdateFpsLabel(double time)
{
	std::string rTime = std::to_string(time);
	rTime = rTime.substr(0, rTime.length() - 7).append("ms ");
	ui->fpsLabel->setText(rTime.c_str());
}

void MainWindow::UpdateResolutionLabels()
{
	widthLineEdit->setText(QString::number(renderView->width()));
	heightLineEdit->setText(QString::number(renderView->height()));
}