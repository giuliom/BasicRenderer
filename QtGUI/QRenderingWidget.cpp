#include "QRenderingWidget.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <ctime>
#include <memory>
#include "BasicRenderer\PrimitiveTypes.h"
#include "BasicRenderer\ImageFormats.h"
#include "BasicRenderer\Vector3.h"
#include "BasicRenderer\ObjLoader.h"


QRenderingWidget::QRenderingWidget(QWidget* parent)
{
	QOpenGLWidget::QOpenGLWidget(parent);
	bRenderer = std::make_unique<BasicRenderer>();
}


QRenderingWidget::~QRenderingWidget()
{
}

void QRenderingWidget::SetScene(const char* filename)
{
	scene = std::make_unique<SceneObject>(ObjLoader::Load(filename));
}

void QRenderingWidget::initializeGL()
{
	img = std::make_unique<QImage>(width(), height(), QImage::Format_ARGB32);
	img->fill(QColor(211, 211, 211));

	renderingTime = clock();

	timer = std::make_unique<QTimer>(this);
	timer->setInterval(15);
	timer->start();
	connect(timer.get(), SIGNAL(timeout()), this, SLOT(RenderFrame()));
	
}

void QRenderingWidget::resizeGL(int w, int h)
{
	//delete img;
	img = std::make_unique<QImage>(w, h, QImage::Format_ARGB32);
}

void QRenderingWidget::paintGL()
{
	
}

void QRenderingWidget::RenderFrame()
{
	float deltaMs = (clock() - renderingTime) * 0.001;
	renderingTime = clock();
	bRenderer->camera.transform.Translate(cameraPos * deltaMs);
	cameraPos = Vector3::Zero();

	update();
}

void QRenderingWidget::paintEvent(QPaintEvent * e)
{
	QPainter painter(this) ;
	
	double beginClock = clock();
	
	const auto buf = bRenderer->Render(width(), height(), *scene);

	QRgb* rgb = (QRgb*)img->bits();
	int size = width() * height();

	const Color* c = buf->GetColorBuffer();

	for (int i = 0; i < size; ++i)
	{
		unsigned int ua = 255 << 24;
		ua |= (unsigned int) (c[i].x * 255) << 16;
		ua |= (unsigned int) (c[i].y * 255) << 8;
		ua |= (unsigned int) (c[i].z * 255);

		rgb[i] = ua;
	}
	
	painter.drawImage(0, 0, *img);
	
	double endClock = clock() - beginClock;
	
	emit RenderingCompleted(endClock);
	
}

void QRenderingWidget::keyPressEvent(QKeyEvent * event)
{
	
	if (event->type() == QKeyEvent::KeyPress)
	{
		if (event->key() == Qt::Key::Key_A)
		{
			cameraPos.x -= cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_D)
		{
			cameraPos.x += cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_W)
		{
			cameraPos.y += cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_S)
		{
			cameraPos.y -= cameraSpeed;
		}
	}
	
	if (event->type() == QKeyEvent::KeyRelease)
	{
		if (event->key() == Qt::Key::Key_Escape)
		{
			//TODO
		}
	}
}


