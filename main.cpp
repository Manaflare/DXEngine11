#include "PreCompiled.h"
#include "Framework.h"
#include "GameScene.h"
int main()
{
	Framework* framework = new Framework();

	if (framework->Initialize(new GameScene()))
	{
		framework->Run();
	}

	delete framework;
	return 0;
}