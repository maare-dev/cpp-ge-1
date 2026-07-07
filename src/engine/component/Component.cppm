export module engine.component.Component;
import engine.entity.Entity;

export class Component{
    protected:
        bool active = true;
        const Entity &parent;
    public:
        Component(Entity &parent) : parent(parent){}
        void SetActive(bool active){this->active = active; if (active) OnActive(); else OnPassive();}
        bool IsActive(){return active;}
        virtual void OnRemove(){}
        virtual void OnActive(){}
        virtual void OnPassive(){}
        virtual void Update(){}
        virtual ~Component(){}
};
