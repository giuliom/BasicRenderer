#include "QRenderingWidget.h"
#include "BasicRenderer\PrimitiveTypes.h"
#include "BasicRenderer\ImageFormats.h"
#include <qpainter.h>
#include <ctime>


QRenderingWidget::QRenderingWidget(QWidget* parent)
{
	QOpenGLWidget::QOpenGLWidget(parent);
	bRenderer = new BasicRenderer();
}


QRenderingWidget::~QRenderingWidget()
{

}

void QRenderingWidget::SetScene(SceneObject* scene)
{
	this->scene = scene; 
}

void QRenderingWidget::initializeGL()
{
	img = new QImage(width(), height(), QImage::Format_ARGB32);
	img->fill(QColor(211, 211, 211));
}

void QRenderingWidget::resizeGL(int w, int h)
{
	img = new QImage(w, h, QImage::Format_ARGB32);
}

void QRenderingWidget::paintGL()
{
}

void QRenderingWidget::paintEvent(QPaintEvent * e)
{
	double beginClock = clock();

	QPainter painter(this) ;

	const FrameBuffer* buf = bRenderer->Render(width(), height(), *scene);

	QRgb* rgb = (QRgb*)img->bits();

	//TODO optimize. Using bit shift?
	for (int i = 0; i < width() * height(); ++i)
	{
		Color c = buf->GetColor(i);
		int r = (int)(c.x * 255);
		int g = (int)(c.y * 255);
		int b = (int)(c.z * 255);

		*rgb = QColor(r, g, b).rgba();
		++rgb;
	}
	
	painter.drawImage(0, 0, *img);

	double endClock = beginClock - clock();
	
	//emit RenderingCompleted(endClock);
}


