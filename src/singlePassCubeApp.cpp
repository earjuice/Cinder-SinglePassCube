#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/CameraUi.h"
#include "cinder/Log.h"
#include "cinder/Utilities.h"

#include "sattelites.h""

#define GRIDSIZE  1024
#define FBOWIDTH GRIDSIZE*3
#define FBOHEIGHT GRIDSIZE*2
#define EQUIWIDTH  4096
#define EQUIHEIGHT 2048

#define pX GL_VIEWPORT_SWIZZLE_POSITIVE_X_NV
#define nX GL_VIEWPORT_SWIZZLE_NEGATIVE_X_NV
#define pY GL_VIEWPORT_SWIZZLE_POSITIVE_Y_NV
#define nY GL_VIEWPORT_SWIZZLE_NEGATIVE_Y_NV
#define pZ GL_VIEWPORT_SWIZZLE_POSITIVE_Z_NV
#define nZ GL_VIEWPORT_SWIZZLE_NEGATIVE_Z_NV
#define pW GL_VIEWPORT_SWIZZLE_POSITIVE_W_NV


using namespace ci;
using namespace ci::app;
using namespace std;


class singlePassCubeApp : public App {
public:
	singlePassCubeApp();
	~singlePassCubeApp();
	void mouseDown(MouseEvent event) override;
	void mouseDrag(MouseEvent event) override;
	void mouseUp(MouseEvent event) override;
	void mouseWheel(MouseEvent event) override;
	void update() override;
	void draw() override;
	void drawText();
	void check_gl_extensions();
	void setupFbo();
	void setupCamera();
	void drawCube();

	gl::TextureCubeMapRef	 textureCubeMap, textureDepthCubeMap;
	gl::FboRef mFboCube;
	gl::FboCubeMapRef	mDynamicCubeMapFbo;
	CameraPersp				mCam;
	CameraUi				mCameraUi;
	Sattelites sats;

};

singlePassCubeApp::singlePassCubeApp() :
	mCameraUi(&mCam)
{
	check_gl_extensions();
	setupFbo();
	setupCamera();
	gl::enableVerticalSync(false);

}
singlePassCubeApp::~singlePassCubeApp()
{
}

void singlePassCubeApp::setupFbo()
{
	textureCubeMap = gl::TextureCubeMap::create(GRIDSIZE, GRIDSIZE, gl::TextureCubeMap::Format().internalFormat(GL_RGBA).mipmap(false));
	textureDepthCubeMap = gl::TextureCubeMap::create(GRIDSIZE, GRIDSIZE, gl::TextureCubeMap::Format().internalFormat(GL_DEPTH_COMPONENT32F).mipmap(false));
	mFboCube = gl::Fbo::create(GRIDSIZE, GRIDSIZE, gl::Fbo::Format().attachment(GL_COLOR_ATTACHMENT0, textureCubeMap).attachment(GL_DEPTH_ATTACHMENT, textureDepthCubeMap));
	mDynamicCubeMapFbo = gl::FboCubeMap::create(GRIDSIZE, GRIDSIZE, gl::FboCubeMap::Format().textureCubeMapFormat(gl::TextureCubeMap::Format().internalFormat(GL_RGBA)));
	mDynamicCubeMapFbo->getFormat().setDepthBufferInternalFormat(GL_DEPTH_COMPONENT32F) ;
}

void singlePassCubeApp::setupCamera()
{
	mCam = CameraPersp(GRIDSIZE, GRIDSIZE, 90, 0.1f, 100.0f);
	mCam.setWorldUp(vec3(0., 1., 0.));
	mCam.lookAt(vec3(0.), vec3(0.0f, 0.0f, 10.0f));
	mCam.setInfiniteFarClip(true);
	gl::enableDepthReversed();
}

void singlePassCubeApp::check_gl_extensions()
{
	console() << "GL_RENDERER " << glGetString(GL_RENDERER) << endl;
	console() << "GL_VERSION " << glGetString(GL_VERSION) << endl;

	int NumberOfExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	bool hasAllRequiredExt = false;
	int accReqExt = 0;
	for (int i = 0; i < NumberOfExtensions; i++) {
		const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);
		//	console() << "GL_EXTENSIONS " << ccc << endl;
		if (strcmp((const char *)ccc, (const char *)"GL_NV_viewport_swizzle") == 0) {
			accReqExt++;
			console() << "Has GL_NV_viewport_swizzle" << endl;
		}
		else if (strcmp((const char *)ccc, (const char *)"GL_NV_viewport_array2") == 0) {
			accReqExt++;
			console() << "Has GL_NV_viewport_array2" << endl;
		}
	}
}

void singlePassCubeApp::mouseDown(MouseEvent event)
{
	mCameraUi.mouseDown(event);
}

void singlePassCubeApp::mouseDrag(MouseEvent event)
{
	mCameraUi.mouseDrag(event);
}
void singlePassCubeApp::mouseUp(MouseEvent event)
{
	mCameraUi.mouseUp(event);
}
void singlePassCubeApp::mouseWheel(MouseEvent event)
{
	mCameraUi.mouseWheel(event);
}

void singlePassCubeApp::update()
{
	sats.camPos = mCam.getEyePoint();
	sats.update();
	//renderCubemap
	drawCube();
}

void singlePassCubeApp::drawCube()
{
	// bind the fbo and enable depth testing
	gl::context()->pushFramebuffer();
	mDynamicCubeMapFbo->bindFramebuffer();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureCubeMap->getId(), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureDepthCubeMap->getId(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);
		for (int face = 0; face < 6; face++) {
			glViewportIndexedf(face, 0, 0, GRIDSIZE, GRIDSIZE);
		}
		glViewportSwizzleNV(0, nZ, nY, pW, pX); // positive X face
		glViewportSwizzleNV(1, pZ, nY, pW, nX); // negative X face
		glViewportSwizzleNV(2, pX, pZ, pW, pY); // positive Y face
		glViewportSwizzleNV(3, pX, nZ, pW, nY); // negative Y face
		glViewportSwizzleNV(4, pX, nY, pW, pZ); // positive Z face
		glViewportSwizzleNV(5, nX, nY, pW, nZ); // negative Z face

		gl::ScopedDepth scopedDepthTest(true);
		gl::ScopedMatrices dMat;
		gl::ScopedViewMatrix scVM();
		gl::setViewMatrix(mCam.getViewMatrix());
		sats.draw();
	
	gl::context()->popFramebuffer();

	//reset viewports
	for (int face = 0; face < 6; face++) {
		glViewportSwizzleNV(face, pX, nY, pZ, pW); // positive Z face
	}

}

void singlePassCubeApp::draw()
{

	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	gl::ScopedMatrices mMainMats;
	gl::ScopedDepth d(false);
	gl::clear(Color(0, 1, 0));
	gl::color(Color(1., 1., 1.));

	gl::drawEquirectangular(textureCubeMap, Rectf(0, getWindowWidth()*.5, getWindowWidth(), 0), 0); // try this alternative
	drawText();

}
void singlePassCubeApp::drawText()
{
		gl::setMatricesWindow(getWindowSize(),false);
		gl::ScopedBlendAlpha blend_;
		TextLayout layoutL, layoutR;

		layoutR.clear(ColorA::gray(0.2f, 0.5f));
		layoutR.setFont(Font("Arial", 18));
		layoutR.setColor(Color::white());
		layoutR.setLeadingOffset(3);
		layoutR.addRightLine(string("FPS: " + to_string(int(getAverageFps()))));

		auto texL = gl::Texture::create(layoutL.render(true));
		gl::draw(texL, vec2(16, 10));

		auto texR = gl::Texture::create(layoutR.render(true));
		gl::draw(texR, vec2(getWindowWidth() - texR->getWidth() - 16, 10));
}

CINDER_APP(singlePassCubeApp, RendererGl(RendererGl::Options().version(4, 5).coreProfile()), [](App::Settings* settings)
{
	settings->prepareWindow(Window::Format().size(vec2(1920, 960)).title("singlePassCubeApp"));
	//settings->setFrameRate(60.0f);
	settings->disableFrameRate();

})

