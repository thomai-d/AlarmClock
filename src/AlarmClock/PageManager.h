#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#define PAGE_RQ_NONE	0xff
#define UI_PAGES		3

#include "Page.h"
#include "Debug.h"
#include "InputHardware.h"

class PageManagerClass
{
public:
	
	void SetPage(uint8_t index, Page* page) { this->pages[index] = page; }

	void Home();
	void NextPage();

	void Service();

	void OnUserInactivity();

private:

	uint8_t currentPageId = PAGE_RQ_NONE;
	uint8_t pageRequestId = PAGE_RQ_NONE;

	Page* currentPage;

	Page* pages[UI_PAGES];

};

extern PageManagerClass PageManager;

#endif