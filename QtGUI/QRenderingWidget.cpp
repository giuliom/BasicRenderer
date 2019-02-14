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


QRenderingWidget::QRenderingWidget(QWidget* parent)
{
	QOpenGLWidget::QOpenGLWidget(parent);
	bRenderer = std::make_unique<BasicRenderer>();
	setMouseTracking(false);
}


QRenderingWidget::~QRenderingWidget()
{
}

void QRenderingWidget::SetScene(const char* filename)
{
	scene = std::make_unique<World>();

	scene->sun.SetDirection({ 0.0f, -1.0f, 0.0f });
	scene->sun.intensity = 1.0f;

	Material* red = new Material({ 1.0f, 0.01f, 0.01f });
	Material* green = new Material({ 0.01f, 1.0f, 0.01f });
	Material* blue = new Material({ 0.01f, 0.01f, 1.0f });

	Material* silver = new Material({ 0.972f, 0.960f, 0.915f });//, Material::Type::METALLIC);
	Material* copper = new Material({ 0.955f, 0.637f, 0.538f });
	Material* gold = new Material({ 1.0f, 0.766f, 0.336f });
	Material* chromium = new Material({ .550f, 0.556f, 0.554f });//, Material::Type::DIELECTRIC);

	//TODO resource manager needed
	std::shared_ptr<Mesh> bunnyMesh(ObjLoader::Load(filename)); 

	SceneObject* bunny = new SceneObject(bunnyMesh, red);
	bunny->transform.SetScale(10.f, 10.f, 10.f);
	bunny->transform.SetPosition(1.0f, -1.0f, -5.0f);
	//bunny->transform.Rotate(0.0f, 0.01f, 0.0f);
	scene.get()->hierarchy.push_back(bunny);

	SceneObject* bunny2 = new SceneObject(bunnyMesh);
	bunny2->transform.SetScale(10.f, 10.f, 10.f);
	bunny2->transform.SetPosition(-0.5f, -1.0f, -5.0f);
	//bunny2->transform.Rotate(0.0f, 0.01f, 0.0f);
	scene.get()->hierarchy.push_back(bunny2);

	Sphere* sp = new Sphere({ -0.8f, 0.0f, -1.5f }, 0.4f, silver);
	Sphere* sp2 = new Sphere({ 0.0f, 0.0f, -1.5f }, 0.4f, gold);
	Sphere* sp3 = new Sphere({ 0.8f, 0.0f, -1.5f }, 0.4f, chromium);
	Sphere* sp4 = new Sphere({ 0.6f, 0.0f, -3.5f }, 1.0f, copper);

	Plane* pl = new Plane({ 0.f, -0.5f, 0.0f }, { 0.f, 1.f, 0.f }, copper);

	Cube* cube = new Cube(gold);
	cube->transform.SetPosition({ 0.25f, -0.3f, -2.0f });
	cube->transform.Scale(0.2f, 0.2f, 0.2f);

	//scene.get()->hierarchy.push_back(sp);
	//scene.get()->hierarchy.push_back(sp2);
	//scene.get()->hierarchy.push_back(sp3);
	//scene.get()->hierarchy.push_back(sp4);

	scene.get()->hierarchy.push_back(pl);

	scene.get()->hierarchy.push_back(cube);
}

void QRenderingWidget::SaveFrame(const char* path)
{
	ImageExporter::ExportToBMP(path, frame);
}

void QRenderingWidget::SetRenderingMode(int index)
{
	switch (index)
	{
	case 1:
		renderingMode = BasicRenderer::RenderingMode::RAYTRACER;
		break;
	default:
		renderingMode = BasicRenderer::RenderingMode::RASTERIZER;
		break;
	}
}

void QRenderingWidget::SetShadingMode(int index)
{
	switch (index)
	{
	case 1:
		shadingMode = BasicRenderer::ShadingMode::NORMAL;
		break;
	default:
		shadingMode = BasicRenderer::ShadingMode::LIT;
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

	bRenderer->camera.transform.Rotate(cameraRot.y * deltaMs, cameraRot.x * deltaMs, 0.0f);

	Vector3 forward = bRenderer->camera.transform.right * cameraPos.z;
	Vector3 right = bRenderer->camera.transform.forward * cameraPos.x;
	Vector3 up = bRenderer->camera.transform.up * cameraPos.y;
	bRenderer->camera.transform.Translate((forward + right + up) * deltaMs);

	
	cameraPos = Vector3::Zero();
	cameraRot = Vector2::Zero();

	//Calls paintEvent()
	update();
}

void QRenderingWidget::paintEvent(QPaintEvent * e)
{
	QPainter painter(this) ;
	
	double beginClock = clock();
	
	frame = bRenderer->Render(width(), height(), *scene, renderingMode, shadingMode);

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


