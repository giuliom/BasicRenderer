#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qimage.h>
#include "QRenderingWidget.h"
#include "BasicRenderer\SceneObject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void SetupSignals();

public slots:
	void RenderImage();
	void UpdateFpsLabel(double time);

private:
    Ui::MainWindow *ui;
	QRenderingWidget* renderView;

	SceneObject* scene = nullptr; 

};

#endif // MAINWINDOW_H
