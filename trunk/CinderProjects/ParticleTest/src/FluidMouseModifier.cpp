#include "FluidMouseModifier.h"

FluidMouseModifier::FluidMouseModifier (app::AppBasic *app, Emitter *emitter, size_t resolution, const Vec3f& position, const float width, const float height)
: FluidModifier (app, emitter, resolution, position, width, height)
{

}
	
FluidMouseModifier::~FluidMouseModifier ()
{
}
