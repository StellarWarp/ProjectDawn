#include "Material.h"
#include "Light.h"


RenderLayer Material::GetRenderLayer()
{
	return forwardPass->GetRenderLayer();
}
