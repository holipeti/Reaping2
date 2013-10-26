#include "i_core.h"

void RandomController::Update( double Seconds )
{
	if(!mActor) return;
	mActor->SetHeading(mActor->GetHeading()+Seconds*(rand()%10-5));
	mActor->SetOrientation(mActor->GetHeading());
	mActor->SetSpeed(rand()%10-5);
}
