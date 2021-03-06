#include <GUI.h>

#include <Singleton.h>

#include <SplashScreen.h>
#include <GameSetup.h>
#include <MainGameScreen.h>

#include <Renderer.h>

#include <Game.h>

DECLARE_SINGLETON(GUIManager)

GUIManager::GUIManager()
{
	ContinueRunning = true;

    ActiveScreen = NO_SCREEN;
}

bool GUIManager::Init()
{
    new CEGUI::DefaultLogger();
    CEGUI::Logger::getSingleton().setLogFilename("CEGUI.log");

    GUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/GUI/schemes", "FileSystem", "schemes");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/GUI/imagesets", "FileSystem", "imagesets");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/GUI/fonts", "FileSystem", "fonts");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/GUI/layouts", "FileSystem", "layouts");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/GUI/looknfeel", "FileSystem", "looknfeels");


    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");

    CEGUI::SchemeManager::getSingleton().create("ArkanaLook.scheme");

    CEGUISystem = &CEGUI::System::getSingleton();

    CEGUISystem->setDefaultMouseCursor("Vanilla-Images", "MouseArrow");
    CEGUISystem->setMouseClickEventGenerationEnabled(true);
    //CEGUISystem->setDefaultFont("Blackletter-10");


    CEGUIWindowManager = &CEGUI::WindowManager::getSingleton();

	// Initialize Screens here matching them to their Enum

	ScreenList[SCREEN_SPLASH] = (ScreenBase*) new SplashScreen();
	ScreenList[SCREEN_SPLASH]->Init();

	ScreenList[SCREEN_GAME_SETUP] = (ScreenBase*) new GameSetupScreen();
	ScreenList[SCREEN_GAME_SETUP]->Init();

	ScreenList[SCREEN_MAIN_GAME] = (ScreenBase*) new MainGameScreen();
	ScreenList[SCREEN_MAIN_GAME]->Init();



    ShowScreen(SCREEN_SPLASH);

	return true;
}

GUIManager::~GUIManager()
{

}

void GUIManager::ShowScreen(ScreenType TargetScreen)
{
    if (TargetScreen != ActiveScreen)
    {
        ActiveScreen = TargetScreen;
        CEGUISystem->setGUISheet(ScreenList[TargetScreen]->getRootWindow());
    }
}

void GUIManager::DirtyActiveScreen()
{
    if (ActiveScreen != NO_SCREEN)
    {
        ScreenList[ActiveScreen]->SetDirty();
    }
}

//-----------------MOUSE INPUT INJECTION--------------------//

bool GUIManager::injectMouseMove(int X, int Y, int Z)
{
	return CEGUISystem->injectMouseMove(X, Y) & CEGUISystem->injectMouseWheelChange(Z);
}

bool GUIManager::injectMousePress(int X, int Y, OIS::MouseButtonID ID)
{
    return CEGUISystem->injectMouseButtonDown((CEGUI::MouseButton)ID);
}

bool GUIManager::injectMouseRelease(int X, int Y, OIS::MouseButtonID ID)
{
    return CEGUISystem->injectMouseButtonUp((CEGUI::MouseButton)ID);
}

//-----------------KEYBOARD INPUT INJECTION--------------------//

bool GUIManager::injectKeyPress(OIS::KeyEvent Event)
{
    CEGUISystem->injectChar(Event.text);

    ScreenList[ActiveScreen]->ProcessKeyPress(Event);

    return CEGUISystem->injectKeyDown(Event.key);
}

bool GUIManager::injectKeyRelease(OIS::KeyEvent Event)
{
    CEGUISystem->injectChar(Event.text);

    ScreenList[ActiveScreen]->ProcessKeyRelease(Event);

    return CEGUISystem->injectKeyUp(Event.key);
}

bool GUIManager::injectTimePulse(float TimeElapsed)
{
    CEGUISystem->injectTimePulse(TimeElapsed);
}
