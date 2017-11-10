#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <qopenglwidget.h>
#include <QTimer>
#include "BasicRenderer\BasicRenderer.h"

class QRenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

protected:
	std::unique_ptr<BasicRenderer> bRenderer;
	std::unique_ptr<QImage> img;
	std::unique_ptr<QTimer> timer;
	std::unique_ptr<SceneObject> scene;

	std::string rTime = "";

	float cameraSpeed = 3.0f;
	float cameraRotationSpeed = 20.0f;
	Vector3 cameraPos;
	Vector2 cameraRot;
	Vector2 lastMousePos;

	double renderingTime = 0.0;

public:
	QRenderingWidget(QWidget* parent);
	~QRenderingWidget();

	void SetScene(const char* filename);

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
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

};

#endif

