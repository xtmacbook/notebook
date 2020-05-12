
#include <scene.h>
#include <RenderNode.h>
#include <shader.h>
#include <gLApplication.h>
#include <camera.h>
#include <windowManager.h>
#include <log.h>
#include <Inputmanager.h>
#include <glinter.h>
#include <resource.h>
#include "geoMipmapping.h"
/**
 * 
	this demo is from Willem H.de Boer "Fast Terrain Rendering Using Geometrical MipMapping"
 */

GLApplication * g_app;
class GeoScene :public Scene
{
protected:
	virtual bool					initSceneModels(const SceneInitInfo&);
	virtual bool					initShader(const SceneInitInfo&);
	virtual bool					update();
public:
	virtual void					render(PassInfo&);
	GeoMipMapping_SP				terrain_ = nullptr;
};

bool GeoScene::initSceneModels(const SceneInitInfo&)
{
	terrain_ = new GeoMipMapping;
	bool result =  terrain_->initHeightMap((get_media_BasePath() + "heightmap/ps_height_4k.png").c_str());
	terrain_->initConstant(getCamera(), g_app->heigh());
	if (!result) return false;
	result = terrain_->initHeightTexture(256, 256);
	if (!result) return false;
	result = terrain_->initBlock();
	if (!result) return false;
	result = terrain_->initGometry();
	CHECK_GL_ERROR;
	return result;
}

bool GeoScene::initShader(const SceneInitInfo&)
{
	Shader* shader =  terrain_->initShader();
	if (shader) { shaders_.push_back(shader); return true; }
	return false;
}

bool GeoScene::update()
{
	if (terrain_) terrain_->updateGeometry(getCamera());
	return true;
}

void GeoScene::render(PassInfo&)
{
	shaders_[0]->turnOn();
	initUniformVal(shaders_[0]);
	terrain_->render();
	shaders_[0]->turnOff();
}

int main()
{
	GeoScene * scene = new GeoScene;
	Camera *pCamera = new Camera();
	scene->setMasterCamera(pCamera);
	WindowManager *pWindowManager = new WindowManager();
	GLApplication application(scene);
	g_app = &application;
	application.setWindowManager(pWindowManager);

	WindowConfig wc;
	DeviceConfig dc;
	wc.title_ = "3d terrain Test";
	wc.width_ = 1024;
	wc.height_ = 906;
	wc.pos_x_ = 50;
	wc.pos_y_ = 50;

	application.initialize(&wc, &dc);
	
	glDisable(GL_CULL_FACE);

	application.initScene();
	pCamera->setClipPlane(0.1f, 5000.0f);
	application.start();

	return 0;
}
