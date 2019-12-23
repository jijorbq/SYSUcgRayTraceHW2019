
class AbstractCamera{
public:
	virtual QVector3D getPixel(AbstactRay &r)=0;
private:
};