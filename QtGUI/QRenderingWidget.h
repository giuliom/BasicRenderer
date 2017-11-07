#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <qopenglwidget.h>
#include <QTimer>
#include "BasicRenderer\BasicRenderer.h"

class QRenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

protected:
	BasicRenderer* bRenderer = nullptr;
	QImage* img = nullptr;
	QTimer* timer = nullptr;
	SceneObject* scene = nullptr;

	std::string rTime = "";

	float cameraSpeed = 1.5f;
	Vector3 cameraPos;

	double renderingTime = 0.0;

public:
	QRenderingWidget(QWidget* parent);
	~QRenderingWidget();

	void SetScene(SceneObject* scene);

signals:
	void RenderingCompleted(double time);

public slots:
	void RenderFrame();

protected:

	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	virtual void paintEvent(QPaintEvent *e) override;

	virtual void keyPressEvent(QKeyEvent *event) override;

};

#endif

