#include "TFTScreenLib.h"


GlyphRenderer::GR_UpArrow(MCUFRIEND_kbv* tft, Rect bounds, int strokeColor, bool fill = false)
  {
      //Draw an up arrow. 
      int point0x = bounds.rectWidth / 2 + bounds.x;
      int point0y = bounds.y + 5;

      int point1x = bounds.x + 5;
      int point1y = (bounds.rectHeight - 10) + point0y;

      int point2x = (bounds.rectWidth - 5) + bounds.x;
      int point2y = point1y;

      if(fill == true)
      {
        tft->fillTriangle(point0x, point0y, point1x, point1y,  point2x, point2y, strokeColor);
      }
      else
      {
        tft->drawTriangle(point0x, point0y, point1x, point1y,  point2x, point2y, strokeColor);
      }
  }

  GlyphRenderer::GR_DownArrow(MCUFRIEND_kbv* tft, Rect bounds, int strokeColor, bool fill = false)
  {
      int point0x = bounds.rectWidth / 2 + bounds.x;
      int point0y = (bounds.rectHeight - 5) + bounds.y;

      int point1x = bounds.x + 5;
      int point1y = bounds.y + 5;

      int point2x = (bounds.rectWidth - 5) + bounds.x;
      int point2y = point1y;
      
      if(fill == true)
      {
        tft->fillTriangle(point0x, point0y, point1x, point1y,  point2x, point2y, strokeColor);
      }
      else
      {
        tft->drawTriangle(point0x, point0y, point1x, point1y,  point2x, point2y, strokeColor);
      }
  }

UIControl::UIControl() {};

UIControl::UIControl(char* id, MCUFRIEND_kbv* tftScreenRef)
    {
      this->tft = tftScreenRef;
      this->enabled = true;
      this->visible = true;
      this->isDirty = false;
	  Id = id;
    }

    UIControl* UIControl::GetParent()
    {
      return this->parent;
    }

    UIControl* UIControl::SetParent(UIControl* control)
    {
      this->parent = control;
    }

    bool UIControl::GetIsDirty()
    {
      return this->isDirty;
    }
    
    void UIControl::Enable()
    {
      this->enabled = true;
      this->isDirty = true;
    }

    void UIControl::Disable()
    {
      this->enabled = false;
      this->isDirty = true;
    }
    
    char* UIControl::GetId()
    {
		return Id;
    }

    Rect UIControl::GetBounds()
    {
      return this->Bounds;
    }
    
    UIControl* UIControl::HitTest(Point point)
    {
      if(enabled == true && visible == true)
      {
       if (((Bounds.x < point.x) && (point.x < (Bounds.x + Bounds.rectWidth))) && ((Bounds.y < point.y) && (point.y < Bounds.y + Bounds.rectHeight)))
       {
        return this;
       }
      }

      return NULL;
    }
    
    void UIControl::Hide() {}
    void UIControl::Draw(){}
    void UIControl::OnHit(){}

	char* UIControl::GetType()
	{
		return "UIControl";
	}

UIControlContainer::UIControlContainer() {};
UIControlContainer::UIControlContainer(char* id, int backgroundColor, UIControl **controls, int numControls, MCUFRIEND_kbv* tftScreenRef):UIControl(id, tftScreenRef)
    {
       this->controls = controls;
       this->backgroundColor = backgroundColor;
       this->numControls = numControls;

       for(int i=0;i<numControls;i++)
       {
          controls[i]->SetParent(this);
       }
    }

    int UIControlContainer::GetBackgroundColor()
    {
      return backgroundColor;
    }

    void UIControlContainer::SetBackgroundColor(int color)
    {
      backgroundColor = color;
      isDirty = true;
    }

    UIControl* UIControlContainer::HitTest(Point point)
    {
     for(int i=0;i<numControls;i++)
      {
         if(controls[i]->HitTest(point) != NULL)
         {
            return controls[i];
         }
      }

      return NULL;
    }

	char* UIControlContainer::GetType()
	{
		return "UIControlContainer";
	}

    void UIControlContainer::Draw()
    {
      if(visible == true)
      {
        tft->fillScreen(backgroundColor);
        
        for(int i=0;i<numControls;i++)
        {
          controls[i]->Draw();
        }
      }
    }
    
    Screen::Screen() {};
    Screen::Screen(char* id, int backgroundColor, UIControl **controls, int numControls, MCUFRIEND_kbv* tftScreenRef):UIControlContainer(id, backgroundColor, controls, numControls, tftScreenRef)
    {
		Serial.print ("Call parent GetIId "); Serial.println(this->GetId());
	}

	char* Screen::GetType()
	{
		return "Screen";
	}

    UILabel::UILabel() {};
    UILabel::UILabel(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, Point origin, MCUFRIEND_kbv* tftScreenRef):UIControl(id, tftScreenRef)
    {
      this->origin = origin; 
      this->textColor = textColor;
      this->backgroundColor = backgroundColor;

	  this->visible = false;
	  this->SetText(text);
	  this->visible = true;
	  
    }

    UILabel::UILabel(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, Rect bounds, MCUFRIEND_kbv* tftScreenRef):UIControl(id, tftScreenRef)
    {
      this->origin.x = bounds.x;
      this->origin.y = bounds.y;
      this->Bounds = bounds; 
      this->textColor = textColor;
      this->backgroundColor = backgroundColor;

	  this->visible = false;
	  this->SetText(text);
	  this->visible = true;
    }

	char* UILabel::GetType()
	{
		return "UILabel";
	}

	void UILabel::SetText(char* inputText)
	{
		if(this->visible == true && this->text != NULL)
		{
			tft->fillRect(this->origin.x, this->origin.y, this->Bounds.rectWidth, this->Bounds.rectHeight, this->backgroundColor);
		}

		char buff[strlen(inputText) + 1];
		memset(buff, '\0', strlen(inputText) + 1);
		strcpy(buff, inputText);

		text = new String(inputText);
		
		int16_t textExtentX,textExtentY;
		uint16_t textExtentWidth, textExtentHeight;
		
		tft->getTextBounds((const char*)buff, origin.x, origin.y, &textExtentX, &textExtentY, &textExtentWidth, &textExtentHeight);

		Bounds.x = origin.x;
		Bounds.y = origin.y;
		Bounds.rectWidth = textExtentWidth + BUTTON_HORIZONTAL_PADDING;
		Bounds.rectHeight = textExtentHeight + BUTTON_VERTICAL_PADDING;

		this->isDirty = true;

		this->Draw();
	}

	char* UILabel::GetText()
	{
		return text->c_str();
	}

    void UILabel::Hide()
    {
      int16_t textExtentX,textExtentY;
      uint16_t textExtentWidth, textExtentHeight;

      tft->getTextBounds(text->c_str(), origin.x, origin.y, &textExtentX, &textExtentY, &textExtentWidth, &textExtentHeight);
      tft->fillRect(origin.x, origin.y, textExtentWidth, textExtentHeight, backgroundColor);
    }
    
    void UILabel::Draw()
    {
      if(visible == true)
      {  
		Serial.print(this->Bounds.rectWidth); Serial.print(" "); Serial.print(this->Bounds.rectHeight);
		tft->fillRect(this->origin.x, this->origin.y, this->Bounds.rectWidth, this->Bounds.rectHeight, this->backgroundColor);
        tft->setCursor(origin.x, origin.y);
        tft->setTextColor(this->textColor);

        tft->print(text->c_str());
      }
    }
    
    UIButton::UIButton() {};
    
	char* UIButton::GetType()
	{
		return "UIButton";
	}

    UIButton::UIButton(char* id, char* text, Point origin, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR):UIControl(id, tftScreenRef)
    {
      this->text = new String(text);
      this->origin = origin;
	  this->backgroundColor = backgroundColor;
    }

    UIButton::UIButton(char* id, char* text, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR):UIControl(id, tftScreenRef)
    {
      this->text = new String(text);
      this->origin = origin;
      this->Bounds.rectWidth = buttonWidth;
      this->Bounds.rectHeight = buttonHeight;
	  this->backgroundColor = backgroundColor;
    }

	//Sets the background color, then calls Draw() to redraw the button.
	void UIButton::SetBackgroundColor(unsigned int color)
	{
		this->backgroundColor = color;
		this->Draw();
	}

	//Renders the current text.
	void UIButton::DrawText()
	{
		if(this->text != NULL)
		{
			tft->setCursor(origin.x + (BUTTON_HORIZONTAL_PADDING / 2), origin.y + (BUTTON_VERTICAL_PADDING / 2));
			tft->setTextColor(BUTTON_TEXT_COLOR);

			char buff[text->length() + 1];
			tft->setTextWrap(false);
			text->toCharArray(buff, text->length());
			tft->print(buff);
		}
	}

	//Reset the control boundaries, taking the text extent boundaries into account.
	void UIButton::ResetBounds(int textExtentWidth, int textExtentHeight)
	{
	    Bounds.x = origin.x;
        Bounds.y = origin.y;
        Bounds.rectWidth = textExtentWidth + BUTTON_HORIZONTAL_PADDING;
        Bounds.rectHeight = textExtentHeight + BUTTON_VERTICAL_PADDING;
	}

	//Get the correct color to draw the background.
	unsigned int UIButton::GetBackgroundColor()
	{
		return (this->enabled == true? this->backgroundColor:DISABLED_GRAY);
	}

    void UIButton::Draw()
    {
      if(visible == true)
      {
        int16_t textExtentX,textExtentY;
        uint16_t textExtentWidth, textExtentHeight;

		char buff[text->length() + 1];
		tft->setTextWrap(false);
		text->toCharArray(buff, text->length());

        tft->getTextBounds(buff, origin.x, origin.y, &textExtentX, &textExtentY, &textExtentWidth, &textExtentHeight);

        tft->fillRect(origin.x, origin.y, textExtentWidth + BUTTON_HORIZONTAL_PADDING, textExtentHeight + BUTTON_VERTICAL_PADDING, this->GetBackgroundColor());
        tft->drawRect(origin.x, origin.y, textExtentWidth + BUTTON_HORIZONTAL_PADDING, textExtentHeight + BUTTON_VERTICAL_PADDING, BUTTON_BORDER_COLOR);
        tft->drawRect(origin.x+1, origin.y+1, textExtentWidth + (BUTTON_HORIZONTAL_PADDING - 2), textExtentHeight + (BUTTON_VERTICAL_PADDING - 2), BUTTON_BORDER_COLOR);
        
		if(this->text != NULL)
		{
			DrawText();
		}

		ResetBounds(textExtentWidth, textExtentHeight);
      }
    };

	UIRoundCornerButton::UIRoundCornerButton() {};
    UIRoundCornerButton::UIRoundCornerButton(char* id, char* text, Point origin, MCUFRIEND_kbv* tftScreenRef, unsigned int backgroundColor = BUTTON_BACKGROUND_COLOR):UIButton(id, text, origin, tftScreenRef, backgroundColor)
    { }
    
	char* UIRoundCornerButton::GetType()
	{
		return "UIRoundCornerButton";
	}
    void UIRoundCornerButton::Draw()
    {
      if(visible == true)
      {
        int16_t textExtentX,textExtentY;
        uint16_t textExtentWidth, textExtentHeight;

		char buff[text->length() + 1];
		tft->setTextWrap(false);
		text->toCharArray(buff, text->length());

        tft->getTextBounds(buff, origin.x, origin.y, &textExtentX, &textExtentY, &textExtentWidth, &textExtentHeight);
        
        tft->fillRoundRect(origin.x, origin.y, textExtentWidth + BUTTON_HORIZONTAL_PADDING, textExtentHeight + BUTTON_VERTICAL_PADDING, BUTTON_OUTER_BORDER_RADIUS, this->GetBackgroundColor());
        tft->drawRoundRect(origin.x, origin.y, textExtentWidth + BUTTON_HORIZONTAL_PADDING, textExtentHeight + BUTTON_VERTICAL_PADDING, BUTTON_OUTER_BORDER_RADIUS, BUTTON_BORDER_COLOR);
        tft->drawRoundRect(origin.x+1, origin.y+1, textExtentWidth + (BUTTON_HORIZONTAL_PADDING - 2), textExtentHeight + (BUTTON_VERTICAL_PADDING - 2), BUTTON_INNER_BORDER_RADIUS, BUTTON_BORDER_COLOR);
        
		DrawText();
  
		ResetBounds(textExtentWidth, textExtentHeight);
      }
    }


    UIDownScrollButton::UIDownScrollButton() {}

    UIDownScrollButton::UIDownScrollButton(char* id, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef):UIButton(id, "", origin, buttonWidth, buttonHeight, tftScreenRef)
    {}

    void UIDownScrollButton::Draw()
    {
      UIButton::Draw();

      this->GR_DownArrow(tft, this->Bounds, BLACK);
    }


    UIUpScrollButton::UIUpScrollButton() {}

    UIUpScrollButton::UIUpScrollButton(char* id, Point origin, int buttonWidth, int buttonHeight, MCUFRIEND_kbv* tftScreenRef):UIButton(id, "", origin, buttonWidth, buttonHeight, tftScreenRef)
    {}

    void UIUpScrollButton::Draw()
    {
      UIButton::Draw();
      this->GR_UpArrow(tft, this->Bounds, RED);
    }


    UIListContainer::UIListContainer(){};
    UIListContainer::UIListContainer(char* id, int backgroundColor, UIControl **listItems, int itemHeight, int numItems, int controlHeight, int controlWidth, int selectedItemIndex, Point origin, MCUFRIEND_kbv* tftScreenRef):UIControlContainer(id, backgroundColor, listItems, numItems, tftScreenRef)
    {
      this->Bounds.x = origin.x;
      this->Bounds.y = origin.y;
      this->Bounds.rectHeight = controlHeight;
      this->Bounds.rectWidth = controlWidth;
      this->numItems = numItems;
      this->itemHeight = itemHeight;

      char upScrollButtonIdString[strlen(id) + 9];
      strcat(upScrollButtonIdString, id);
      strcat(upScrollButtonIdString, "_scrollup");

      char downScrollButtonIdString[strlen(id) + 9];
      strcat(downScrollButtonIdString, id);
      strcat(downScrollButtonIdString, "_scrolldn");

      char scrollHandleButtonIdString[strlen(id) + 9];
      strcat(scrollHandleButtonIdString, id);
      strcat(scrollHandleButtonIdString, "_scrollhn");
      
      Point upScrollButtonOrigin;
      upScrollButtonOrigin.x = this->Bounds.x + this->Bounds.rectWidth - 30;
      upScrollButtonOrigin.y = this->Bounds.y + 1;

      Point downScrollButtonOrigin;
      downScrollButtonOrigin.x = upScrollButtonOrigin.x;
      downScrollButtonOrigin.y = (this->Bounds.y + this->Bounds.rectHeight) - 20;

      Point scrollHandleButtonOrigin;
      scrollHandleButtonOrigin.x = upScrollButtonOrigin.x;
      scrollHandleButtonOrigin.y = upScrollButtonOrigin.y + 30;
      
      this->upScrollButton = new UIUpScrollButton(upScrollButtonIdString, upScrollButtonOrigin, 30, 100, tftScreenRef);
      this->downScrollButton = new UIDownScrollButton(downScrollButtonIdString, downScrollButtonOrigin, 30, 100, tftScreenRef);
      this->scrollHandleButton = new UIButton(scrollHandleButtonIdString, "", scrollHandleButtonOrigin, 30, 100, tftScreenRef);
    };

    int UIListContainer::GetItemHeight() {return itemHeight;}
    int UIListContainer::GetItemCount() {return numItems;}
    int UIListContainer::GetSelectedItemIndex() {return selectedItemIndex;}
    int UIListContainer::GetFirstVisibleItemIndex() {return 0;}
    Rect UIListContainer::GetScrollArea () {

    }
    
    void UIListContainer::Draw()
    {
      if(visible == true)
      {
        //Draw the background first. 
        tft->fillRect(this->Bounds.x, this->Bounds.y, this->Bounds.rectWidth, this->Bounds.rectHeight, this->backgroundColor);

        //Draw the outside border. For now, this is always BLACK.
        tft->drawRect(this->Bounds.x, this->Bounds.y, this->Bounds.rectWidth, this->Bounds.rectHeight, BLACK);

        //Draw the inner list border. This is always BLACK.
        tft->drawRect(this->Bounds.x + 1, this->Bounds.y + 1, this->Bounds.rectWidth - this->scrollWidth, (this->Bounds.rectHeight-2), BLACK);
        
        //Determine which list item(s) are visible.
        int numItemsVisible = min((this->Bounds.rectHeight - 3) / this->itemHeight, numItems);

        //Draw bounds for each list item.
        
        for(int i=firstVisibleItemIndex;i<numItemsVisible;i++)
        {
          tft->drawRect(this->Bounds.x + 2, this->Bounds.y + 2 + (i * this->itemHeight), this->Bounds.rectWidth - (this->scrollWidth + 2), this->itemHeight, BLACK);

          //Draw each list item.
        }

        //Draw buttons.
        this->upScrollButton->Draw();
        this->downScrollButton->Draw();

        if(this->scrollThumbArea == NULL)
        {
            this->scrollThumbArea = new Rect{
              this->upScrollButton->GetBounds().x, this->upScrollButton->GetBounds().y + this->upScrollButton->GetBounds().rectHeight,
              this->downScrollButton->GetBounds().x, this->downScrollButton->GetBounds().rectWidth
            };
        }

        this->scrollHandleButton->Draw();

      }
    }

    UIControl* UIListContainer::HitTest(Point point)
    {
		UIControl* control = NULL;
		int i = 0;
		while(i < 3 && control == NULL)
		{
			control = this->scrollControls[i++]->HitTest(point);
		}
		
		if(control != NULL)
		{
			//Delegate to scrolling functionality
		}

		return this;
    }

    int UIListContainer::GetScrollThumbHeight()
    {
      int scrollAreaHeight = this->Bounds.rectHeight - (buttonHeight * 2);
      return min(scrollAreaHeight / numItems, 30);
    }
    
    int UIListContainer::GetScrollThumbYValue()
    {
      int scrollAreaHeight = this->Bounds.rectHeight - (buttonHeight * 2);
      
    }

    UIScrollPositionButton::UIScrollPositionButton() {}

    UIScrollPositionButton::UIScrollPositionButton(char* id, Point origin, int buttonWidth, int buttonMinHeight, int scrollAreaHeight, UIListContainer* parentList, MCUFRIEND_kbv* tftScreenRef):UIButton(id, "", origin, buttonWidth, buttonMinHeight, tftScreenRef)
    {
      this->parentList = parentList;  
      this->scrollAreaHeight = scrollAreaHeight;
      this->buttonMinHeight = buttonMinHeight;
    }

    void UIScrollPositionButton::Draw()
    {
      //Get the number of items in the parent list.
      int numItems = this->parentList->GetItemCount();
      int listHeight = this->parentList->GetBounds().rectHeight;
      int listItemHeight = this->parentList->GetItemHeight();
      int listPosition = this->parentList->GetFirstVisibleItemIndex();
      int listSelectedItemIndex = this->parentList->GetSelectedItemIndex();
      
      //Set height to number of items / listItemHeight
      int myHeight = min(scrollAreaHeight / listItemHeight, buttonMinHeight);
      UIButton::Draw();
    }
    

   




    UIListItem::UIListItem() {};
    UIListItem::UIListItem(char* id, char* text, unsigned int textColor, unsigned int backgroundColor, UIControl* container, Point origin, MCUFRIEND_kbv* tftScreenRef):UILabel(id, text, textColor, backgroundColor, origin, tftScreenRef)
    {

    }

    void UIListItem::Draw()
    {
      UILabel::Draw();
      Rect containerBounds = container->GetBounds();
      Serial.print(containerBounds.x); Serial.print(" "); Serial.println(containerBounds.y);
      
      Bounds.rectWidth = containerBounds.rectWidth;
    }

