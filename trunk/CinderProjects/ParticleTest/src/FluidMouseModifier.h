#include "cinder/app/AppBasic.h"
#include "FluidModifier.h"

class FluidMouseModifier : public FluidModifier 
{
public:
	FluidMouseModifier (app::AppBasic *app, Emitter *emitter, size_t resolution, const Vec3f& position, const float width, const float height);
	
	virtual ~FluidMouseModifier ();

private:

private:

};