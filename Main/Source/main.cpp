#include "Framework/GameApp.h"
const unsigned int SCR_WIDTH = 1960;
const unsigned int SCR_HEIGHT = 1080;
int main()
{
	GameApp app("Project Dawn", SCR_WIDTH, SCR_HEIGHT);
	if (!app.Init())
	{
		return -1;
	}

	app.Run();
}