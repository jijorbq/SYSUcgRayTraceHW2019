#include <QVector3D>
#include <map>
class AbstractRay{

public:
	virtual QVector3D getOrigin()=0;
	virtual QVector3D getDirection()=0;
	virtual std::pair<double, double> getRange()=0;
	virtual QVector3D raytracing()=0;
private:
	QVector3D origin, direction;
	pair<double,double> range;
};