#pragma once
#include "IDivider.h"
class NullDivider : public IDivider
{
public:
	NullDivider();
	virtual float GetLeft()const override;
	virtual float GetTop()const override;
	virtual void Update()override;
	virtual void Draw()override;
};

