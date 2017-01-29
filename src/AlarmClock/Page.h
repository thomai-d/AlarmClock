#ifndef PAGE_H
#define PAGE_H

#include "IInputFocus.h"

class Page : public IInputFocus
{
public:
	virtual void OnViewLoaded() { }
	virtual void OnViewUnLoaded() { }
	virtual void OnUserInactivity() { }

	virtual void Refresh(bool force = false) { }
};

#endif