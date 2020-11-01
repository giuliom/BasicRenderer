#include "QRenderingWidget.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <ctime>
#include <memory>
#include "BasicRenderer\Global.h"
#include "BasicRenderer\PrimitiveTypes.h"
#include "BasicRenderer\ImageFormats.h"
#include "BasicRenderer\Vector3.h"
#include "BasicRenderer\ObjLoader.h"
#include "BasicRenderer\ImageExporter.h"
#include "BasicRenderer\Material.h"
#include "BasicRenderer\TestScene.h"

QRenderingWidget::QRenderingWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	bRenderer = std::make_unique<Renderer>();
	setMouseTracking(false);
	SetScene("");
}

QRenderingWidget::~QRenderingWidget()
{
	m_loop = false;
	m_renderThread.join();
}

void QRenderingWidget::SetScene(const char* filename)
{
	scene.reset(TestScene());
}

void QRenderingWidget::SaveFrame(const char* path)
{
	std::unique_ptr<FrameBuffer> frame = std::make_unique<FrameBuffer>(imgDisplay->width(), imgDisplay->height());
	{
		std::scoped_lock<std::mutex> img_lock(m_renderMtx);
		copyQImageToFrameBuffer(*frame.get(), *imgDisplay);
	}
	ImageExporter::ExportToBMP(std::string(path), std::string("saved_render"), *frame);
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
	imgDisplay = std::make_unique<QImage>(width(), height(), QImage::Format_ARGB32);
	imgDisplay->fill(QColor(211, 211, 211));

	connect(this, SIGNAL(RenderingCompleted(double)), this, SLOT(RenderFrame()));

	Raytracer& raytracer = bRenderer->GetRaytracer();
	raytracer.m_pixelSamples = 2;
	raytracer.m_maxBounces = 2;

	m_renderThread = std::thread(&QRenderingWidget::RenderLoopThread, this);
}

void QRenderingWidget::resizeGL(int w, int h)
{
	{
		std::scoped_lock<std::mutex> inputLock(m_inputMtx);
		m_inputEvents.clear();
	}

	{
		std::scoped_lock<std::mutex> renderLock(m_renderMtx);
		imgDisplay = std::make_unique<QImage>(w, h, QImage::Format_ARGB32);
	}

	emit RendererResized();
}

void QRenderingWidget::paintGL()
{
	
}

void QRenderingWidget::RenderFrame()
{
	//Calls paintEvent()
	update();
}

void QRenderingWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	{
		std::scoped_lock<std::mutex> frame_lock(m_renderMtx);

		painter.drawImage(0, 0, *imgDisplay);
	}

	emit RenderingCompleted(m_renderingTimeMs); // atomic
}

void QRenderingWidget::RenderLoopThread()
{
	while (m_loop) // atomic
	{
		double beginClock = clock();

		InputManager& inputMgr = bRenderer->GetInputManager();
		{
			std::scoped_lock<std::mutex> inputLock(m_inputMtx);
			for (auto& event : m_inputEvents)
			{
				inputMgr.AddEvent(event);
			}

			m_inputEvents.clear();
		}

		// TODO Improve memory performance
		const FrameBuffer* frame = bRenderer->Render(width(), height(), *scene, renderingMode, shadingMode, m_renderingTimeMs * 0.001f);

		{
			std::unique_lock<std::mutex> renderLock(m_renderMtx);

			uint imgSize = static_cast<uint>(imgDisplay->width() * imgDisplay->height());

			if (frame->NumPixels() == imgSize)
			{
				copyFrameBufferToQImage(*imgDisplay.get(), *frame);
			}
		}

		m_renderingTimeMs = (clock() - beginClock) / (CLOCKS_PER_SEC * 0.001); // atomic
	}
}

void QRenderingWidget::copyFrameBufferToQImage(QImage& img, const FrameBuffer& frame)
{
	assert(img.width() == frame.GetWidth() && img.height() == frame.GetHeight());

	QRgb* rgb = reinterpret_cast<QRgb*>(img.bits());
	uint size = width() * height();

	const Color* c = frame.GetColorBuffer();

	if (c != nullptr)
	{
		for (uint i = 0; i < size; ++i)
		{
			uint ua = 255u << 24;
			ua |= static_cast<uint>(c[i].x * 255.999f) << 16;
			ua |= static_cast<uint>(c[i].y * 255.999f) << 8;
			ua |= static_cast<uint>(c[i].z * 255.999f);

			rgb[i] = ua;
		}
	}
}

void QRenderingWidget::copyQImageToFrameBuffer(FrameBuffer& frame, const QImage& img)
{
	assert(img.width() == frame.GetWidth() && img.height() == frame.GetHeight());

	const QRgb* rgb = reinterpret_cast<const QRgb*>(img.bits());
	uint size = width() * height();

		for (uint i = 0; i < size; ++i)
		{
			uint ua = rgb[i];
			uint r = 0u, g = 0u, b = 0u;

			r |= (ua >> 16) & 255u;
			g |= (ua >> 8) & 255u;
			b |= ua & 255u;

			Color c(r / 255.999f, g / 255.999f, b / 255.999f);

			frame.WriteToColor(i, c);
		}
}


void QRenderingWidget::keyPressEvent(QKeyEvent* event)
{
	ButtonState state;
	ButtonType type;
	bool matched = false;

	double time = clock();

	if (event->type() == QKeyEvent::KeyPress)
	{
		state = ButtonState::PRESSED;

		switch (event->key())
		{
		case Qt::Key::Key_W:
		{
			type = ButtonType::KEY_W;
			matched = true;
			break;
		}

		case Qt::Key::Key_A:
		{
			type = ButtonType::KEY_A;
			matched = true;
			break;
		}

		case Qt::Key::Key_S:
		{
			type = ButtonType::KEY_S;
			matched = true;
			break;
		}

		case Qt::Key::Key_D:
		{
			type = ButtonType::KEY_D;
			matched = true;
			break;
		}

		case Qt::Key::Key_Q:
		{
			type = ButtonType::KEY_Q;
			matched = true;
			break;
		}

		case Qt::Key::Key_E:
		{
			type = ButtonType::KEY_E;
			matched = true;
			break;
		}

		}
	}
	else if (event->type() == QKeyEvent::KeyRelease)
	{
		state = ButtonState::RELEASED;

		switch (event->key())
		{
		case Qt::Key::Key_Escape:
		{
			type = ButtonType::KEY_ESCAPE;
			matched = true;
			break;
		}
		}
	}

	if (matched)
	{
		std::unique_ptr<ButtonInputEvent> inputEvent = std::make_unique<ButtonInputEvent>(type, state);

		std::scoped_lock<std::mutex> inputLock(m_inputMtx);
		m_inputEvents.push_back(std::move(inputEvent));
	}
}

void QRenderingWidget::mousePressEvent(QMouseEvent* event)
{
	const Vector2 currentMousePos(event->x(), event->y());
	ButtonType type;
	bool matched = false;

	switch (event->button())
	{
	case Qt::MouseButton::LeftButton:
	{
		type = ButtonType::CURSOR_PRIMARY;
		matched = true;
		break;
	}

	case Qt::MouseButton::RightButton:
	{
		type = ButtonType::CURSOR_SECONDARY;
		matched = true;
		break;
	}

	}

	if (matched)
	{
		std::unique_ptr<ButtonInputEvent> inputEvent = std::make_unique<ButtonInputEvent>(type, ButtonState::PRESSED, currentMousePos);

		std::scoped_lock<std::mutex> inputLock(m_inputMtx);
		m_inputEvents.push_back(std::move(inputEvent));
	}
}

void QRenderingWidget::mouseReleaseEvent(QMouseEvent* event)
{
	const Vector2 currentMousePos(event->x(), event->y());
	ButtonType type;
	bool matched = false;

	switch (event->button())
	{
	case Qt::MouseButton::LeftButton:
	{
		type = ButtonType::CURSOR_PRIMARY;
		matched = true;
		break;
	}

	case Qt::MouseButton::RightButton:
	{
		type = ButtonType::CURSOR_SECONDARY;
		matched = true;
		break;
	}

	}

	if (matched)
	{
		std::unique_ptr<ButtonInputEvent> inputEvent = std::make_unique<ButtonInputEvent>(type, ButtonState::RELEASED, currentMousePos);

		std::scoped_lock<std::mutex> inputLock(m_inputMtx);
		m_inputEvents.push_back(std::move(inputEvent));
	}
}

void QRenderingWidget::mouseMoveEvent(QMouseEvent* event)
{
	const Vector2 currentPos(event->x(), event->y());
	std::unique_ptr<CursorInputEvent> inputEvent = std::make_unique<CursorInputEvent>(currentPos);

	std::scoped_lock<std::mutex> inputLock(m_inputMtx);
	m_inputEvents.push_back(std::move(inputEvent));
}


