#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <qimage.h>
#include "QRenderingWidget.h"
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>

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
	void SaveImage();
	void OpenFile();
	void UpdateFpsLabel(double time);
	void UpdateResolutionLabels();


private:
    Ui::MainWindow *ui;
	QRenderingWidget* renderView;
	QLineEdit* widthLineEdit;
	QLineEdit* heightLineEdit;
	QPushButton* renderButton;
	QPushButton* saveButton;
	QComboBox* renderingCombo;
	QComboBox* shadingCombo;

};

#endif // MAINWINDOW_H
