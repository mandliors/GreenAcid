#include "GreenAcid.h"
#include <iostream>

int main(void)
{
	GreenAcid::AppDescriptor desc;
	desc.Title = "GreenAcid Engine";
	desc.Width = 1280;
	desc.Height = 720;
	desc.IconPath = "assets/textures/logo.png";
	GreenAcid::Application::Init(desc);
	GreenAcid::Application::Run();

	return 0;
}