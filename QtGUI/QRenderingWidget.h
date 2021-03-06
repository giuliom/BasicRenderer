#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <qopenglwidget.h>
#include <QTimer>
#include <mutex>
#include "BasicRenderer.h"

using namespace BasicRenderer;

class QRenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

protected:
	std::unique_ptr<Model> m_model;
	std::unique_ptr<RenderState> m_renderStates[2];
	std::unique_ptr<Renderer> m_renderer;
	uint m_rendering_state_index = 0;
	std::atomic<uint> m_available_state_index = 1;

	std::unique_ptr<QImage> imgDisplay;
	std::unique_ptr<QTimer> timer;
	std::atomic<double> m_renderingTimeMs;
	TimePoint m_lastUpdateTime;

	std::atomic<bool> m_loop = true;
	std::atomic<Renderer::RenderingMode> m_renderingMode = Renderer::RenderingMode::RASTERIZER;
	std::atomic<Renderer::ShadingMode> m_shadingMode = Renderer::ShadingMode::LIT;
	std::atomic<unsigned int> m_pixelSamples = 2;
	std::atomic<unsigned int> m_maxBounces = 2;
	std::atomic<int> m_width;
	std::atomic<int> m_height;

	std::mutex m_modelSwapMtx;
	std::mutex m_renderMtx;
	std::thread m_modelThread;
	std::thread m_renderThread;

	std::string rTime = "";

	std::mutex m_inputMtx;
	std::vector<std::unique_ptr<InputEvent>> m_inputEvents;

public:
	QRenderingWidget(QWidget* parent);
	~QRenderingWidget();

	void SetScene(const char* filename);
	void SaveFrame(const char* path);

signals:
	void RenderingCompleted(double time);
	void RendererResized();

public slots:
	void RenderFrame();
	void SetRenderingMode(int index);
	void SetShadingMode(int index);

protected:

	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;
	virtual void paintEvent(QPaintEvent *e) override;

	void FixedUpdateLoopThread();
	void RenderLoopThread();
	
	void CopyFrameBufferToQImage(QImage& img, const FrameBuffer& frame);
	void CopyQImageToFrameBuffer(FrameBuffer& frame, const QImage& img);

	inline std::unique_ptr<RenderState>& GetAvailableState()	{ return m_renderStates[m_available_state_index]; }
	inline const RenderState& GetRenderingState() const			{ return *m_renderStates[m_rendering_state_index]; }

	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
};

#endif

