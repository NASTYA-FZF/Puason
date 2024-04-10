#pragma once
#include <gdiplus.h>
#include "Work.h"

struct PotPt
{
	//потенциал
	double phi;
	//int i;
	//int j;
	//координата
	double x, y;

	PotPt& operator=(PotPt& other)
	{
		//i = other.i;
		//j = other.j;
		x = other.x;
		y = other.y;

		phi = other.phi;
		return *this;
	}

};

//ячейка
struct Cell
{

	bool upperTri; //верхний треугольник
	bool lowerTri; //нижний треугольник
	PotPt  pt1, pt2, pt3, pt4;
	double nx_upper, ny_upper;
	double nx_lower, ny_lower;

	//проверка, что точка в ячейке
	bool IsPointIn(double x, double y)
	{
		/// Эта функция смотрит где относительно диагональной оси в квадрате находится точка. Диагон ось рассматриваю как прямую y=kx
		//
		//		Y2 - Y1
		//	K = --------	(X2; Y2) - Моя верхняя правая точка квадрата, (X1; Y1) - координаты рассматриваемой точки
		//		X2 - X1
		//	
		// Так как У увлечивается вниз, а Х вправо, если k>= -1, то точка лежит в нижнем треугольнике
		double k;
		if (x >= pt1.x && x <= pt3.x && y >= pt1.y && y <= pt3.y)
		{
			k = (y - pt2.y) / (x - pt2.x);
			if (k >= -1) { upperTri = false; lowerTri = true; }
			else { upperTri = true; lowerTri = false; }

			return true;
		}
		else return false;
	}
	//рассчет нормалей
	void CalculateNormals()
	{

		double length;
		nx_upper = (pt3.y - pt1.y) * (pt2.phi - pt1.phi) - (pt2.y - pt1.y) * (pt3.phi - pt1.phi);
		ny_upper = -(pt3.x - pt1.x) * (pt2.phi - pt1.phi) + (pt2.x - pt1.x) * (pt3.phi - pt1.phi);

		nx_lower = (pt4.y - pt1.y) * (pt3.phi - pt1.phi) - (pt3.y - pt1.y) * (pt4.phi - pt1.phi);
		ny_lower = -(pt4.x - pt1.x) * (pt3.phi - pt1.phi) + (pt3.x - pt1.x) * (pt4.phi - pt1.phi);


		length = sqrt(ny_upper * ny_upper + nx_upper * nx_upper);
		if (length != 0) {
			nx_upper /= length;
			ny_upper /= length;
		}
		length = sqrt(ny_lower * ny_lower + nx_lower * nx_lower);
		if (length != 0)
		{
			nx_lower /= length;
			ny_lower /= length;
		}
	}

	// где окажется в следующий момент времени
	void Propagate(double& x, double& y, double& step)
	{
		if (upperTri)
		{
			x += step * nx_upper;
			y += step * ny_upper;
		}
		else if (lowerTri)
		{
			x += step * nx_lower;
			y += step * ny_lower;
		}
	}
};

// my_puasson

class my_puasson : public CStatic
{
	DECLARE_DYNAMIC(my_puasson)

public:
	my_puasson();
	virtual ~my_puasson();

protected:
	DECLARE_MESSAGE_MAP()

public:
	std::vector<std::vector<Cell>> cells; // ячейки
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void Convert();
	UINT_PTR token;
	std::vector<std::vector<cell>> setka;
	double xScale, yScale;
	bool draw;
	std::vector<std::vector<std::pair<double, double>>> my_izoline, my_power_line;
	std::vector<std::pair<double, double>> my_e_line;
	std::vector<double> my_const;
	void CreateIzoline(Gdiplus::Graphics* my_dr);
	void write();
	bool izoline;
	void CreateElime(Gdiplus::Graphics* my_dr);
	double step_e_line;
	double rasstoanie(double my_x, double my_y, double xpt, double ypt);
	double A(double fi1, double fi2);
	double B(double fi1, double fi2);
	void up_down(Gdiplus::Graphics* my_dr, int i, int j, int inext, int jnext, double& x, double& y, int znak);
	std::pair<double, double> point_crossing(double x, double y, int j, int i, double my_A, double my_B, bool triangle);
	std::pair<double, double> next_kletka(int i, int j, double xnew, double ynew);
	bool power_line;
	void SetPower(std::vector<std::vector<std::pair<double, double>>> my);


};

