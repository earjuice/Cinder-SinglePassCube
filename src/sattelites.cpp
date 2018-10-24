#include "sattelites.h""

Sattelites::Sattelites()
{
	bSky = true;
	bBalls = true;
	bColored = false;
	setupSatellites();
	setupSkyBox();
}
void Sattelites::setupSatellites()
{
	// setup satellites (orbiting spheres )
	for (int i = 0; i < 33; ++i) {
		mSatellites.push_back(Satellite());
		float angle = i / 33.0f;
		mSatellites.back().mColor = Colorf(CM_HSV, angle, 1.0f, 1.0f);
		mSatellites.back().mPos = vec3(cos(angle * 2 * M_PI) * 7, 0, sin(angle * 2 * M_PI) * 7);
	}
	auto satForm = gl::GlslProg::Format().vertex(loadAsset("shader.vert")).geometry(loadAsset("shader.geom")).fragment(loadAsset("shader.frag")).define("TRIANGLES");
	auto satBoxGlsl = gl::GlslProg::create(satForm);

	mSatelliteBatch = gl::Batch::create(geom::Sphere(), satBoxGlsl);
}


void Sattelites::setupSkyBox()
{

	mSkyBoxCubeMap = gl::TextureCubeMap::create(loadImage(loadAsset("env_map.jpg")), gl::TextureCubeMap::Format().mipmap(false));
	mSkyBoxCubeMap2 = gl::TextureCubeMap::create(loadImage(loadAsset("env_map_color.jpg")), gl::TextureCubeMap::Format().mipmap(false));

	auto skyForm = gl::GlslProg::Format().vertex(loadAsset("shader.vert")).geometry(loadAsset("shader.geom")).fragment(loadAsset("shader.frag")).define("TRIANGLES").define("TEXTUREDCUBE");
	auto skyBoxGlsl = gl::GlslProg::create(skyForm);

	mSkyBoxBatch = gl::Batch::create(geom::Cube().size(vec3(100.)), skyBoxGlsl);
}
void Sattelites::update()
{
	// move the satellites
	if (bBalls) {
		for (int i = 0; i < 33; ++i) {
			float angle = i / 33.0f;
			mSatellites[i].mPos = vec3(cos(angle * 2 * M_PI) * 7, 6 * sin(getElapsedSeconds() * 2 + angle * 4 * M_PI), sin(angle * 2 * M_PI) * 7);
		}
	}
}
void Sattelites::draw()
{
	if (bSky)
	{
		gl::ScopedModelMatrix mmMat;
		drawSkyBox();
	}
	if (bBalls)
	{
		drawSatellites();
	}
}
void Sattelites::drawSatellites()
{
	for (const auto &satellite : mSatellites) {
		gl::pushModelMatrix();
		gl::translate(satellite.mPos);
		gl::color(satellite.mColor);
		mSatelliteBatch->draw();
		gl::popModelMatrix();
	}
	gl::color(ColorA(1., 1., 1., 1.));
}

void Sattelites::drawSkyBox()
{
	(bColored ? mSkyBoxCubeMap2 : mSkyBoxCubeMap)->bind();
	mSkyBoxBatch->getGlslProg()->uniform("uCubeMapTex", 0);
	gl::pushMatrices();
	gl::translate(camPos);
	gl::scale(TEXSIZE*2, TEXSIZE*2, TEXSIZE*2);
	mSkyBoxBatch->draw();
	gl::popMatrices();
}