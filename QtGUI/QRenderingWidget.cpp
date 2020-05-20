#include "QRenderingWidget.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <ctime>
#include <memory>
#include "BasicRenderer\PrimitiveTypes.h"
#include "BasicRenderer\ImageFormats.h"
#include "BasicRenderer\Vector3.h"
#include "BasicRenderer\ObjLoader.h"
#include "BasicRenderer\ImageExporter.h"
#include "BasicRenderer\Material.h"
#include "BasicRenderer\TestScene.h"

QRenderingWidget::QRenderingWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	bRenderer = std::make_unique<Renderer>();
	setMouseTracking(false);
	SetScene("");
}

QRenderingWidget::~QRenderingWidget()
{
}

void QRenderingWidget::SetScene(const char* filename)
{
	scene.reset(TestScene());
}

void QRenderingWidget::SaveFrame(const char* path)
{
	ImageExporter::ExportToBMP(std::string(path), std::string("saved_render"), frame);
}

void QRenderingWidget::SetRenderingMode(int index)
{
	switch (index)
	{
	case 1:
		renderingMode = Renderer::RenderingMode::RAYTRACER;
		break;
	default:
		renderingMode = Renderer::RenderingMode::RASTERIZER;
		break;
	}
}

void QRenderingWidget::SetShadingMode(int index)
{
	switch (index)
	{
	case 1:
		shadingMode = Renderer::ShadingMode::NORMAL;
		break;
	default:
		shadingMode = Renderer::ShadingMode::LIT;
		break;
	}
}

void QRenderingWidget::initializeGL()
{
	img = std::make_unique<QImage>(width(), height(), QImage::Format_ARGB32);
	img->fill(QColor(211, 211, 211));

	renderingTime = clock();

	connect(this, SIGNAL(RenderingCompleted(double)), this, SLOT(RenderFrame()));
}

void QRenderingWidget::resizeGL(int w, int h)
{
	img = std::make_unique<QImage>(w, h, QImage::Format_ARGB32);
	emit RendererResized();
}

void QRenderingWidget::paintGL()
{
	
}

void QRenderingWidget::RenderFrame()
{
	float deltaMs = (clock() - renderingTime) * 0.001f;
	renderingTime = clock();

	Camera& camera = bRenderer->GetCamera();

	camera.GetTransform().Rotate(cameraRot.y * deltaMs, cameraRot.x * deltaMs, 0.0f);

	Vector3 forward = camera.GetTransform().right * cameraPos.z;
	Vector3 right = camera.GetTransform().forward * cameraPos.x;
	Vector3 up = camera.GetTransform().up * cameraPos.y;
	camera.GetTransform().Translate((forward + right + up) * deltaMs);

	
	cameraPos = Vector3::Zero();
	cameraRot = Vector2::Zero();

	//Calls paintEvent()
	update();
}

void QRenderingWidget::paintEvent(QPaintEvent * e)
{
	QPainter painter(this) ;
	
	double beginClock = clock();
	
	frame = bRenderer->Render(width(), height(), *scene, renderingMode, shadingMode, 1, 1);

	QRgb* rgb = reinterpret_cast<QRgb*>(img->bits());
	int size = width() * height();

	const Color* c = frame->GetColorBuffer();

	for (int i = 0; i < size; ++i)
	{
		unsigned int ua = 255 << 24;
		ua |= static_cast<unsigned int>(c[i].x * 255) << 16;
		ua |= static_cast<unsigned int>(c[i].y * 255) << 8;
		ua |= static_cast<unsigned int>(c[i].z * 255);

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
			cameraPos.z += cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_S)
		{
			cameraPos.z -= cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_Q)
		{
			cameraPos.y -= cameraSpeed;
		}
		if (event->key() == Qt::Key::Key_E)
		{
			cameraPos.y += cameraSpeed;
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

void QRenderingWidget::mousePressEvent(QMouseEvent * event)
{
	lastMousePos = Vector2(event->x(), event->y());
}

void QRenderingWidget::mouseMoveEvent(QMouseEvent * event)
{
	Vector2 currentPos(event->x(), event->y());
	float ratio = 1.0 / height();
	cameraRot = cameraRot + Vector2((currentPos.x - lastMousePos.x) * ratio * cameraRotationSpeed, (currentPos.y - lastMousePos.y) * ratio * cameraRotationSpeed);
	lastMousePos = currentPos;
}


