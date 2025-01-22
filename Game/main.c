#include "draw.h"

int main(int argc, char* argv[])
{
	menu_state state = showMenu();
	if (state == Game)
	{
		draw();
	}
	return 0;
}
