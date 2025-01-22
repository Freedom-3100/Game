#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "draw.h"

int main()
{

	menu_state state = showMenu();

	while (state != End)
	{
		if (state == Game)
		{
			draw();
			state = Exit;
		}
		else if (state == Exit)
		{
			state = showMenu();
		}
	}
	
	_CrtDumpMemoryLeaks();
	return 0;
}
