#include <iostream>
#include<cmath>
#define PI 3.14159265358979
using namespace std;
/// <summary>
/// 坐标
/// </summary>
typedef struct Coordinate
{
	Coordinate();
    float x;
    float y;
};
Coordinate::Coordinate()
{
	this->x = 0;
	this->y = 0;
}
/// <summary>
/// 极片坐标
/// </summary>
typedef struct PieceCor
{
    Coordinate upoint;
    Coordinate downpoint;
};
/// <summary>
/// 推送量
/// </summary>
typedef struct pushvol
{
    float dx1;
	float dx2;
	float dy;
};
/// <summary>
/// 计算两点与x轴的夹角(由b到a)
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
float CalculateAngle(Coordinate a, Coordinate b)
{
	//由于arctan取值始终在-π/2到π/2(一四象限)，与实际想要的角度不同，需要结合象限更改
	int m;
	//在二三象限
	if (a.x < b.x) m = a.y < b.y ? -1 : 1;
	else m = 0;
	return (atan((a.y - b.y) / (a.x - b.x)) + m * PI);
};
/// <summary>
/// 计算两点的长度
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="length"></param>
void CalculateLength(Coordinate a, Coordinate b, float& length)
{
	sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
};
/// <summary>
/// 计算推送量
/// </summary>
/// <param name="measurePiece">检测极片坐标</param>
/// <param name="standarPiece">标准极片坐标</param>
/// <param name="center">旋转中心坐标</param>
/// <param name="p">推送量</param>
/// <param name="distance">旋转中心到旋转轴的距离</param>
void PushVolume(PieceCor measurePiece, PieceCor standarPiece, Coordinate center, pushvol& p, float distance)
{
	//标准片角度
	float alphaStantart = CalculateAngle(standarPiece.upoint, standarPiece.downpoint);
    //检测片角度
	float alphaMeasure = CalculateAngle(measurePiece.upoint, measurePiece.downpoint);
	float dalpha = alphaMeasure - alphaStantart;//需要旋转的角度，+为顺时针
	// 求旋转需要移动的长度(默认上右下左为正)
    //revpush1和revpush2为旋转角度的所需要的两轴推送量
    float revpush1 = dalpha * (distance / 2);//distance为两个电机距离
    float revpush2 = -dalpha * (distance / 2);
	float updx,updy,downdx,downdy,dx,dy;
	// 极片坐标点旋转后需要移动的长度
	auto AfterRevolvemove = [](Coordinate c, Coordinate center, Coordinate standarpoint, float& dx, float& dy, float angle)
	{
		float length = sqrt(pow(c.x - center.x, 2) + pow(c.y - center.y, 2));//旋转点到检测点的距离
		//到旋转中心的角度
		float beta1 = CalculateAngle(c, center);//旋转中心与测量点的原始角度
		float beta2 = beta1 + angle;//增加角度
		//旋转后的坐标
		float xre = length * cos(beta2) + center.x;
		float yre = length * sin(beta2) + center.y;
		//旋转后需要移动的坐标
		dx = standarpoint.x - xre;
		dy = standarpoint.y - xre;
	};
	//将两个点需要移动的值求一个平均
	AfterRevolvemove(measurePiece.upoint, center, standarPiece.upoint, updx, updy, dalpha);
	AfterRevolvemove(measurePiece.downpoint, center, standarPiece.downpoint, downdx, downdy, dalpha);
	dx = (updx+downdx)/2;
	dy = (updy+downdy)/2;
	//推送量等于旋转推送和旋转后推送
	p.dx1 = revpush1 + dx;
	p.dx2 = revpush2 + dx;
	p.dy = dy;
}
void WriteCor(Coordinate& Coordinate, float x, float y)
{
	Coordinate.x = x;
	Coordinate.y = y;
}
void WritePieceCor(PieceCor& PieceCor, float x1, float y1, float x2, float y2)
{
	WriteCor(PieceCor.upoint, x1, y1);
	WriteCor(PieceCor.downpoint, x2, y2);
}
int main()
{
	PieceCor standarPiece;
	PieceCor measurePiece;
	Coordinate center;
	float distance = 475.2;
	pushvol p;
	WritePieceCor(standarPiece, 2, 2, 1.5, 1.5);
	WritePieceCor(measurePiece, 1.9, 2.1, 1.3, 1.4);
	WriteCor(center, 1, 1);
	PushVolume(measurePiece, standarPiece, center, p, distance);
	cout << "推送量为：" << p.dx1 << p.dx2 << p.dy;
}



