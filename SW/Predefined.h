#pragma once
#include <memory>

enum class PostCommandType : long
{
	ReceivedSlideMotorPosition,
	ReceivedSlideMotorSpeed,
	ReceivedSlideMotorAcceleration,
	ReceivedSlideMotorDeceleration,
	ReceivedSlideMotorJerk,
	ReceivedSlideMotorHome,
	ReceivedSlideMotorMove,
	ReceivedSlideMotorStop,
	ReceivedSlideMotorReset,
	
	ReceivedSpindleMotorPosition,
	ReceivedSpindleMotorSpeed,
	ReceivedSpindleMotorAcceleration,
	ReceivedSpindleMotorDeceleration,
	ReceivedSpindleMotorJerk,
	ReceivedSpindleMotorHome,
	ReceivedSpindleMotorMove,
	ReceivedSpindleMotorStop,
	ReceivedSpindleMotorReset,
};

class PostObj;

void PostCommand(PostCommandType type, std::shared_ptr<PostObj> obj);
