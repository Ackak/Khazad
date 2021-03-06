#include <Renderer.h>

#include <Singleton.h>

#include <TerrainRender.h>
#include <Camera.h>
#include <Timer.h>
#include <Game.h>
#include <Map.h>


DECLARE_SINGLETON(Renderer)

Renderer::Renderer()
{
    OgreDebugOverlay = NULL;
    RenderTimer = new Timer(50);
}

bool Renderer::InitializeRenderer()
{
    // TODO replace with entirely manual plugin loading
    #ifdef LINUX_BUILD
        OgreRoot = new Ogre::Root("plugins.linux.cfg");
    #else
        OgreRoot = new Ogre::Root("plugins.windows.cfg");
    #endif

    setupRenderSystem();
    setupScene();
    createSunLight();
    createCamera();

    defineResources();
    initializeResourceGroups();

    RenderTimer->Start();

    return true;
}

Renderer::~Renderer()
{

}

void Renderer::setSliceLevels(int32_t Top, int32_t Bottom)
{
	for (std::vector<TerrainRendering*>::iterator it = TerrainRenders.begin(); it != TerrainRenders.end(); ++it)
	{
		TerrainRendering* TargetRender = *it;
		int Z = TargetRender->getCellSceneNode()->getPosition().z;
		TargetRender->setVisible(Z > Bottom && Z <= Top);
	}
}

void Renderer::RenderFrame()
{
    RenderTimer->Unpause();

    if (OgreDebugOverlay != NULL && OgreDebugOverlay->isVisible())
    {
        UpdateOverlay();
    }

    RefreshTerrainGeometry();

    OgreRoot->renderOneFrame();

    OgreRoot->_fireFrameStarted();

    RenderTimer->Pause();
}

void Renderer::RefreshTerrainGeometry()
{
	for (std::vector<TerrainRendering*>::iterator it = TerrainRenders.begin(); it != TerrainRenders.end(); ++it)
	{
		TerrainRendering* TargetRender = *it;
		if (TargetRender->isDirty())
		{
			TargetRender->BuildFaceGeometry();
		}
	}
}

void Renderer::registerTerrainRender(TerrainRendering* NewTerrain)
{
	TerrainRenders.push_back(NewTerrain);
}

void Renderer::defineResources()
{
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

void Renderer::setupRenderSystem()
{
    if (!OgreRoot->restoreConfig() && !OgreRoot->showConfigDialog())
    {
        throw Ogre::Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");
    }

    OgreRoot->initialise(true, "Khazad");
}

void Renderer::initializeResourceGroups()
{
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Renderer::setupScene()
{
    OgreSceneManager = OgreRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
    OgreRenderWindow = OgreRoot->getAutoCreatedWindow();

    OgreSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

void Renderer::createSunLight()
{
    OgreSceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

    Ogre::Light* SunLight = OgreSceneManager->createLight();
    SunLight->setType(Ogre::Light::LT_DIRECTIONAL);

    SunLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
    SunLight->setSpecularColour(1.0f, 1.0f, 1.0f);

    SunLight->setDirection(Ogre::Vector3(0, -1, -1));  // Down and slightly off at an angle

    Ogre::SceneNode* SunLightSceneNode = getRootNode()->createChildSceneNode();
    SunLightSceneNode->attachObject(SunLight);
}

void Renderer::createCamera()
{
    Camera* newCamera = new Camera();
    Cameras.push_back(newCamera);
    ActiveCameraIndex = Cameras.size() - 1;

    newCamera->Init();
}

void Renderer::TakeScreenShoot()
{
    OgreRenderWindow->writeContentsToFile("ScreenShoot.png");
}

void Renderer::FocusActiveCameraAt(Ogre::Vector3 Point)
{
    getActiveCamera()->FocusAt(Point);
}

void Renderer::ToggleOverlay()
{
    OgreDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

    if (OgreDebugOverlay->isVisible())
    {
        OgreDebugOverlay->hide();
    }
    else
    {
        OgreDebugOverlay->show();
    }
}

void Renderer::UpdateOverlay()
{
    static Ogre::String currFps = "Current FPS: ";
    static Ogre::String avgFps = "Average FPS: ";
    static Ogre::String bestFps = "Best FPS: ";
    static Ogre::String worstFps = "Worst FPS: ";
    static Ogre::String tris = "Triangle Count: ";
    static Ogre::String batches = "Batch Count: ";

    Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
    Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
    Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
    Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

    const Ogre::RenderTarget::FrameStats& stats = RENDERER->getWindow()->getStatistics();

    guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
    guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
    guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS) + " " + Ogre::StringConverter::toString(stats.bestFrameTime) + " ms");
    guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS) + " " + Ogre::StringConverter::toString(stats.worstFrameTime) + " ms");

    Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
    guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

    Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
    guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));

    double GameAgerage = GAME->getGameTimer()->getAverage();
    double RenderAgerage = RenderTimer->getAverage();
    int TickCount = GAME->getCurrentTick();
    double Total = GameAgerage + RenderAgerage;

    char buffer[32];
    //sprintf(buffer, "Tick Number %i", TickCount);
    sprintf(buffer, "GameAvg %.2f%          RenderAvg %.2f%", (GameAgerage / Total) * 100.0f, (RenderAgerage / Total) * 100.0f);

	Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
	guiDbg->setCaption(buffer);
}
