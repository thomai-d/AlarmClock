#include "PageManager.h"

PageManagerClass PageManager;

void PageManagerClass::Home()
{ 
	if (this->currentPageId != 0)
	{
		// Block input until page is loaded.
		InputHardware.BlockInput(true);
	}

	this->pageRequestId = 0;
}

void PageManagerClass::NextPage()
{
	// Block input until page is loaded.
	InputHardware.BlockInput(true); 

	// Special: Enter page 2 (Settings) only if button 1 is held down.
	if (this->currentPageId == 1 && !InputHardware.IsButton1Down())
	{
		this->pageRequestId = 0;
		return;
	}

	this->pageRequestId = (this->currentPageId + 1) % UI_PAGES;
}

void PageManagerClass::Service()
{
	if (this->pageRequestId == PAGE_RQ_NONE)
	{
		// Sleeping...
		return;
	}

	if (this->currentPageId == this->pageRequestId)
	{
		// Special case: No page set, yet.
		if (this->currentPage != NULL)
			this->currentPage->Refresh();

		// Special case: Wakeup.
		if (!Display2.IsBacklightOn())
		{
			Display2.ActivateBacklight();
			InputHardware.ActivateInput();
		}

		return;
	}

	if (this->currentPage != NULL)
	{
		Display2.DeactivateBacklight();
		this->currentPage->OnViewUnLoaded();
	}

	this->currentPage = this->pages[this->pageRequestId];
	this->currentPageId = this->pageRequestId;
	this->currentPage->OnViewLoaded();
	InputHardware.Focus = this->currentPage;
	Display2.ActivateBacklight();
	InputHardware.ActivateInput();
}

void PageManagerClass::OnUserInactivity()
{
	Display2.DeactivateBacklight();
	InputHardware.BlockInput(false);

	if (this->currentPage != NULL)
	{
		// Unload if not main page.
		if (this->currentPageId != 0)
			this->currentPage->OnViewUnLoaded();

		this->currentPage->OnUserInactivity();
	}

	this->pageRequestId = PAGE_RQ_NONE;
}
