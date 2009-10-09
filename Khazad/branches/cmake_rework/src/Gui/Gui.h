#ifndef UI__HEADER
#define UI__HEADER

#include <stdafx.h>
#include <Singleton.h>


class Ui
{
DECLARE_SINGLETON_CLASS(Ui)

public:
	~Ui();
	bool Init();
	bool Run();

    gcn::Gui* GuiChanMainObject;
    gcn::SDLInput* Input;
    gcn::OpenGLGraphics* GraphicsImplementation;

    gcn::ImageFont* font;
    gcn::Label* label;
    gcn::Container* TopWidget;

    bool ProcessEvent(SDL_Event event, Sint32 RelativeX, Sint32 RelativeY);
    bool isWidgetCollision(gcn::Widget* TestWidget, Uint16 RealX, Uint16 RealY);
    bool Draw();
    void updateSizing();

    bool isDone()                   { return done; }
    void setDone(bool NewState)     { done = NewState; }

    void GetConfirmation(const char* Question, gcn::ActionListener* Listener);
    void HideConfirmationWindow();
    void setMapViewState();
    void setZSliders(int16_t A, int16_t B);
    void setZSliderRange (int16_t Z);

    gcn::ActionListener* getExitListener()      { return ExitListener; }

    gcn::Image* HiddenOn;
    gcn::Image* HiddenOff;
    gcn::ImageButton* HiddenToggleButton;

protected:
    void InitMainMenu();
    void InitCameraControlMenu();
    void InitConfirmationWindow();
    void InitDepthSlider();

    gcn::Window* CameraControlWindow;
    gcn::Window* MainMenuWindow;
    gcn::Window* ConfirmationWindow;

    gcn::ImageButton* YesButton;
    gcn::ImageButton* NoButton;
    gcn::ActionListener* ConfirmationAction;
    gcn::ActionListener* ExitListener;

    gcn::KhazSlider* DepthSlider;
    bool done;  // quit program command recived by UI
};


#define UI (Ui::GetInstance())
#define GRAPHICS (Ui::GetInstance()->GraphicsImplementation)

#endif // UI__HEADER