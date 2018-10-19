#ifndef TFTScreenLib_h
#define TFTScreenLib_h

#include <Arduino.h>
#include <stddef.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

// Assign human-readable names to some common 16-bit color values:
#define BLACK  0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define BACKGROUND_COLOR 0x33F3
#define BUTTON_BACKGROUND_COLOR 0xDF3D
#define BUTTON_BORDER_COLOR 0x31C7
#define BUTTON_OUTER_BORDER_RADIUS 5
#define BUTTON_INNER_BORDER_RADIUS 3
#define BUTTON_HORIZONTAL_PADDING 20
#define BUTTON_VERTICAL_PADDING 20
#define BUTTON_TEXT_COLOR BLACK 
#define DISABLED_GRAY 0x8410

typedef struct  
{
  int x,y,rectWidth,rectHeight;
} Rect;

typedef struct 
{
  int x,y;
} Point;


class GlyphRenderer
{
  public:
  GR_UpArrow(MCUFRIEND_kbv* tft, Rect bounds, int strokeColor, bool fill = false);
  GR_DownArrow(MCUFRIEND_kbv* tft, Rect bounds, int strokeColor, bool fill = false);
};

class UIControl
{
  private:
    UIControl* parent;
    char* Id;

  protected:
    Rect Bounds;
    MCUFRIEND_kbv* tft;
    bool enabled;
    bool visible;
    bool isDirty;
   
  public:
    UIControl();
    UIControl(char* id, MCUFRIEND_kbv* tftScreenRef);
    UIControl* GetParent();
    UIControl* SetParent(UIControl* control);
    bool GetIsDirty();
    void Enable();
    void Disable();
    char* GetId();
    Rect GetBounds();
 
    virtual UIControl* HitTest(Point point);
    
    virtual void Hide();
    virtual void Draw();
    virtual void OnHit();
	virtual char* GetType();
};

class UIControlContainer:public UIControl
{
  public:
    UIControlContainer();
    UIControlContainer(char* id, int backgroundColor, UIControl **controls, int numControls, MCUFRIEND_kbv* tftScreenRef);

    int GetBackgroundColor();
    void SetBackgroundColor(int color);
    virtual UIControl* HitTest(Point point);
    virtual void Draw();
	virtual char* GetType();

  protected:
    int backgroundColor;
    int numControls;
    UIControl** controls;
};

class Screen: public UIControlContainer
{
   public:
    Screen();
    Screen(char* id, int backgroundColor, UIControl **controls, int numControls, MCUFRIEND_kbv* tftScreenRef);
	virtual char* GetType();
};

class UILabel: public UIControl 
{
  public:
    UILabel();
    UILabel(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, Point origin, MCUFRIEND_kbv* tftScreenRef);

    UILabel(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, Rect bounds, MCUFRIEND_kbv* tftScreenRef);

    virtual void Hide();
    
    virtual void Draw();
    
	virtual void SetText(char* text);
	virtual char* GetText();
	virtual char* GetType();
  protected:
    int textColor;
    int backgroundColor;
    String* text = NULL;
    Point origin;
};

class UIButton:public UIControl, public GlyphRenderer
{
  public:
    UIButton();
    
    UIButton(char* id, char* text, Point origin, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR);

    UIButton(char* id, char* text, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR);
    virtual void Draw();
	virtual char* GetType();
	void SetBackgroundColor(unsigned int color);
  protected:
      String* text;
      Point origin;
	  unsigned int backgroundColor;
	  void DrawText();
	  unsigned int GetBackgroundColor();
	  void ResetBounds(int textExtentWidth, int textExtentHeight);
};

class UIDownScrollButton:public UIButton 
{
  public:
    UIDownScrollButton();

    UIDownScrollButton(char* id, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef);

    virtual void Draw();
};

class UIUpScrollButton:public UIButton
{
  public:
    UIUpScrollButton();
    UIUpScrollButton(char* id, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef);
    virtual void Draw();
};

class UIListContainer: public UIControlContainer 
{
  public:
    UIListContainer();
    UIListContainer(char* id, int backgroundColor, UIControl **listItems, int itemHeight, int numItems, int controlHeight, int controlWidth, int selectedItemIndex, Point origin, MCUFRIEND_kbv* tftScreenRef);

    int GetItemHeight();
    int GetItemCount();
    int GetSelectedItemIndex();
    int GetFirstVisibleItemIndex();
    Rect GetScrollArea ();
    
    virtual void Draw();

    virtual UIControl* HitTest(Point point);

  protected:
    int scrollWidth = 30;
    int itemHeight;
    int numItems;
    int selectedItemIndex = -1;
    int firstVisibleItemIndex = 0;
    int buttonHeight = 30;
    Rect* scrollThumbArea;
    
    UIButton* upScrollButton;
    UIButton* downScrollButton;
    UIButton* scrollHandleButton;

	UIButton* scrollControls[3] = {upScrollButton, downScrollButton, scrollHandleButton};
		
    int GetScrollThumbHeight();
    int GetScrollThumbYValue();
};

class UIScrollPositionButton:public UIButton
{ 
  public:
    UIScrollPositionButton();

    UIScrollPositionButton(char* id, Point origin, int buttonWidth, int buttonMinHeight, int scrollAreaHeight, UIListContainer* parentList, MCUFRIEND_kbv* tftScreenRef);

    virtual void Draw();
    
  protected:
    int scrollAreaHeight;
    int buttonMinHeight;
    UIListContainer* parentList;
    
};

class UIRoundCornerButton: public UIButton
{
  public:
    UIRoundCornerButton();
    UIRoundCornerButton(char* id, char* text, Point origin, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR);
    virtual char* GetType();
    virtual void Draw();
};

class UIListItem:public UILabel 
{
  public:
    UIListItem();
    UIListItem(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, UIControl* container, Point origin, MCUFRIEND_kbv* tftScreenRef);

    virtual void Draw();

  private:
    UIControl* container;
};
#endif