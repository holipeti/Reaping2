#include "i_ui.h"

void Button::Init( Json::Value& Descriptor )
{
	BaseClass::Init(Descriptor);
	Json::Value& TriggerAction=Descriptor["action"];
	if(!TriggerAction.isArray())return;
	size_t const NumActions=TriggerAction.size();
	if(!NumActions)return;
	mActions.reserve(NumActions);
	for(Json::Value::iterator i=TriggerAction.begin(),e=TriggerAction.end();i!=e;++i)
		AddAction(*i);
}

void Button::AddAction( Json::Value& TriggerAction )
{
	if(!TriggerAction.isArray())return;
	size_t const NumActions=TriggerAction.size();
	if(!NumActions)return;
	ActionDesc Desc;
	if(!Json::GetStr(TriggerAction[0],Desc.mAction))return;
	if(NumActions==2)
	{
		Json::Value& Arg=TriggerAction[1];
		if(Arg.isInt())
			Desc.mArg=(int32_t)Arg.asInt();
		else if(Arg.isDouble())
			Desc.mArg=Arg.asDouble();
		else if(Arg.isString())
			Desc.mArg=Arg.asString();
	}
	mActions.push_back(Desc);
}

bool Button::Trigger()
{
	for(Actions_t::const_iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
	{
		ActionDesc const& Act=*i;
		switch(Act.mArg.GetType())
		{
		case Prop::T_Null:
			RootModel::Get()[Act.mAction]();
			break;
		case Prop::T_Int:
			RootModel::Get()[Act.mAction]((int32_t)Act.mArg);
			break;
		case Prop::T_Double:
			RootModel::Get()[Act.mAction]((double)Act.mArg);
			break;
		case Prop::T_Str:
			RootModel::Get()[Act.mAction]((std::string)Act.mArg);
			break;
		}
	}
	return !mActions.empty();
}

Button::Button( int32_t Id )
:Widget(Id)
{

}

void Button::OnMouseEnter()
{
	operator()(PT_Highlight)=1;
}

void Button::OnMouseLeave()
{
	operator()(PT_Highlight)=0;
}