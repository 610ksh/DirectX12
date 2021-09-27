#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	// sealed : 봉인. 이걸 상속한 대상들은 사용못하도록 막음.
	virtual void FinalUpdate() sealed {}

};

