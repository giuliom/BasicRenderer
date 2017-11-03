#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <qopenglwidget.h>
#include "BasicRenderer\BasicRenderer.h"

class QRenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

protected:
	BasicRenderer* bRenderer = nullptr;
	QImage* img = nullptr;
	SceneObject* scene = nullptr;

	std::string rTime = "";

public:
	QRenderingWidget(QWidget* parent);
	~QRenderingWidget();

	void SetScene(SceneObject* scene);

//signals:
	void RenderingCompleted(double time);

protected:

	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	virtual void paintEvent(QPaintEvent *e) override;



};

#endif

