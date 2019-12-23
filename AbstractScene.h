#include "AbstractRay.h"

class AbstractScene{
public:
	virtual void hit(AbstractRay &r) = 0;
private:
};