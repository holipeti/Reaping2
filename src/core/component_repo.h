#ifndef INCLUDED_COMPONENT_REPO_H
#define INCLUDED_COMPONENT_REPO_H

class Component;
class ComponentRepo : public Factory<Component>, public Singleton<ComponentRepo>
{
    friend class Singleton<ComponentRepo>;
    ComponentRepo();
protected:
	template<typename Elem_T>
	static std::auto_ptr<Component> CreateComponent( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<Component> ComponentRepo::CreateComponent( int32_t Id )
{
	return std::auto_ptr<Elem_T>( new Elem_T() );
}

#endif//INCLUDED_COMPONENT_REPO_H