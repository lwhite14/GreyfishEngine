#ifndef COMPONENT_H
#define COMPONENT_H

class Component 
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void DrawUI() = 0;
};

#endif //COMPONENT_H
