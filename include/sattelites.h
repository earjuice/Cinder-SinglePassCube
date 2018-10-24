#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/Vector.h"

using namespace ci;
using namespace ci::app;
//using namespace std;
#define TEXSIZE  1024

struct Satellite {
	vec3    mPos;
	Colorf  mColor;
};

class Sattelites
{
public:
	Sattelites();
	void update();
	void draw();

	gl::BatchRef			mSatelliteBatch, mSkyBoxBatch;
	vec3 camPos;

private:
	void setupSatellites();
	void setupSkyBox();
	void drawSatellites();
	void drawSkyBox();

	std::vector<Satellite>  mSatellites;
	gl::TextureCubeMapRef	mSkyBoxCubeMap, mSkyBoxCubeMap2, textureCubeMap, textureDepthCubeMap;
	bool					bColored, bBalls, bSky;

};

