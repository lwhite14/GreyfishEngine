
#include "Runner.h"
#include "Scenes/Scene.h"
#include "Scenes/BasicScene.h"

int main()
{
	Runner runner = Runner("Greyfish Engine", 1200, 600);
	Scene* base;
	BasicScene derived;
	base = &derived;
	return runner.Run(*base);
}