// my_puasson.cpp: файл реализации
//

#include "pch.h"
#include "Puason.h"
#include "my_puasson.h"
using namespace std;
using namespace Gdiplus;

// my_puasson

IMPLEMENT_DYNAMIC(my_puasson, CStatic)

my_puasson::my_puasson()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"Не удалось нарисовать", L"Ошибка", NULL);
	}
	draw = false;
	izoline = false;
	power_line = false;
}

my_puasson::~my_puasson()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(my_puasson, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений my_puasson



double MYsign(double x)
{
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

void my_puasson::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (!draw) return;

	if (setka.empty()) return;

	//if (my_izoline.empty()) izoline = false;

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);
	draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);

	draw_in_buffer.Clear(Color::LightGreen);

	SolidBrush b_metall(Color::Gray), b_sreda(Color::LightGreen), b_granica_vne(Color::RosyBrown), b_granica_vnut(Color::LightGoldenrodYellow);
	Pen white_pen(Color::White, 3), black_pen(Color::Black, 2);

	double cur_width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	double cur_height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	int fi_width = setka[0].size();
	int fi_height = setka.size();

	xScale = cur_width / fi_width;
	yScale = cur_height / fi_height;

	Matrix matr;
	matr.Scale(xScale, yScale);

	PointF pt[4];

	if (!izoline)
	{
		for (int i = 0; i < fi_height; i++)
		{
			for (int j = 0; j < fi_width; j++)
			{
				pt[0] = PointF((double)j, (double)i);
				pt[1] = PointF((double)(j + 1), (double)i);
				pt[2] = PointF((double)(j + 1), (double)(i + 1));
				pt[3] = PointF((double)j, (double)(i + 1));
				matr.TransformPoints(pt, 4);
				draw_in_buffer.DrawRectangle(&white_pen, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y);

				switch (setka[i][j].material_cell)
				{
				case sreda: draw_in_buffer.FillRectangle(&b_sreda, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y); break;
				case metall: draw_in_buffer.FillRectangle(&b_metall, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y); break;
				case granica_vne: draw_in_buffer.FillRectangle(&b_granica_vne, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y); break;
				case granica_vnut_gor: draw_in_buffer.FillRectangle(&b_granica_vnut, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y); break;
				case granica_vnut_ver: draw_in_buffer.FillRectangle(&b_granica_vnut, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y); break;
				}
			}
		}
	}
	else
	{
		double radius = 1;
		for (int i = 0; i < fi_height; i++)
		{
			for (int j = 0; j < fi_width; j++)
			{
				//if (setka[i][j].material_cell == metall && setka[i][j + 1].material_cell != metall) continue;

				if (setka[i][j].material_cell != metall) continue;
				pt[0] = PointF((double)j, (double)i);
				pt[1] = PointF((double)j + radius, (double)i);
				pt[2] = PointF((double)j + radius, (double)i + radius);
				pt[3] = PointF((double)j, (double)i + radius);
				matr.TransformPoints(pt, 4);

				draw_in_buffer.DrawRectangle(&Pen(&b_metall), pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y);
				draw_in_buffer.FillRectangle(&b_metall, pt[0].X, pt[0].Y, pt[1].X - pt[0].X, pt[3].Y - pt[0].Y);
			}
		}
		draw_in_buffer.DrawRectangle(&Pen(&b_granica_vne, 10), 0, 0, cur_width, cur_height);
		CreateIzoline(&draw_in_buffer);
		CreateElime(&draw_in_buffer);
	}
	//if (power_line)
	//{
	//CreateElime(&draw_in_buffer);
	//}
	/*if (!my_power_line.empty())
	{
		for (int i = 0; i < my_power_line.size(); i++)
		{
			for (int j = 0; j < my_power_line[i].size() - 1; j++)
			{
				draw_in_buffer.DrawLine(&Pen(Color::LightPink), PointF(my_power_line[i][j].first, my_power_line[i][j].second),
					PointF(my_power_line[i][j + 1].first, my_power_line[i][j + 1].second));
			}
		}
	}*/
	//}

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void my_puasson::Convert()
{
	cells = std::vector<std::vector<Cell>>(setka[0].size() - 1, vector<Cell>(setka.size() - 1));
	//инициализация клеток
	for (int x = 0; x < cells[0].size(); x++)
	{
		for (int y = 0; y < cells.size(); y++)
		{
			cells[x][y].pt1.x = x;
			cells[x][y].pt1.y = y;
			cells[x][y].pt1.phi = setka[y][x].fi;

			cells[x][y].pt2.x = x + 1;
			cells[x][y].pt2.y = y;
			cells[x][y].pt2.phi = setka[y][x + 1].fi;

			cells[x][y].pt3.x = x + 1;
			cells[x][y].pt3.y = y + 1;
			cells[x][y].pt3.phi = setka[y + 1][x + 1].fi;

			cells[x][y].pt4.x = x;
			cells[x][y].pt4.y = y + 1;
			cells[x][y].pt4.phi = setka[y + 1][x].fi;

		}
	}
}

void my_puasson::CreateIzoline(Gdiplus::Graphics* my_dr)
{
	int size = my_const.size();
	my_izoline = vector<vector<pair<double, double>>>(size);

	bool first = true;
	
	Pen white_pen(Color::White, 3), black_pen(Color::Black, 2);

	Matrix matr;
	matr.Scale(xScale, yScale);

	PointF pt[4];

	for (int i = 0; i < setka.size() - 1; i++)
	{
		for (int j = 0; j < setka[i].size() - 1; j++)
		{
			for (int k = 0; k < size; k++)
			{
				if ((my_const[k] <= setka[i][j].fi && my_const[k] >= setka[i][j + 1].fi)
					|| (my_const[k] >= setka[i][j].fi && my_const[k] <= setka[i][j + 1].fi))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1) - fabs(setka[i][j + 1].fi - my_const[k]) / fabs(setka[i][j].fi - setka[i][j + 1].fi), (double)i));

				if ((my_const[k] <= setka[i][j + 1].fi && my_const[k] >= setka[i + 1][j + 1].fi)
					|| (my_const[k] >= setka[i][j + 1].fi && my_const[k] <= setka[i + 1][j + 1].fi))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1), (double)(i + 1) - fabs(setka[i + 1][j + 1].fi - my_const[k]) / fabs(setka[i][j + 1].fi - setka[i + 1][j + 1].fi)));

				if ((my_const[k] <= setka[i + 1][j].fi && my_const[k] >= setka[i + 1][j + 1].fi)
					|| (my_const[k] >= setka[i + 1][j].fi && my_const[k] <= setka[i + 1][j + 1].fi))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1) - fabs(setka[i + 1][j + 1].fi - my_const[k]) / fabs(setka[i + 1][j].fi - setka[i + 1][j + 1].fi), (double)(i + 1)));

				if ((my_const[k] <= setka[i][j].fi && my_const[k] >= setka[i + 1][j].fi)
					|| (my_const[k] >= setka[i][j].fi && my_const[k] <= setka[i + 1][j].fi))
					my_izoline[k].push_back(pair<double, double>((double)(j), (double)(i + 1) - fabs(setka[i + 1][j].fi - my_const[k]) / fabs(setka[i][j].fi - setka[i + 1][j].fi)));
				
				/*if (!my_izoline[k].empty())
				{
					pt[0] = PointF(my_izoline[k][my_izoline[k].size() - 2].first, my_izoline[k][my_izoline[k].size() - 2].second);
					pt[1] = PointF(my_izoline[k][my_izoline[k].size() - 1].first, my_izoline[k][my_izoline[k].size() - 1].second);
					matr.TransformPoints(pt, 2);
				}*/

				if (my_izoline[k].size() == 2)
				{
					pt[0] = PointF(my_izoline[k][my_izoline[k].size() - 2].first, my_izoline[k][my_izoline[k].size() - 2].second);
					pt[1] = PointF(my_izoline[k][my_izoline[k].size() - 1].first, my_izoline[k][my_izoline[k].size() - 1].second);
					matr.TransformPoints(pt, 2);
				}

				my_dr->DrawLine(&black_pen, pt[0], pt[1]);
				my_izoline[k].clear();
			}
		}
	}
	write();
}

void my_puasson::write()
{
	ofstream file;
	file.open(L"izoline.txt");
	file.precision(2);
	for (int i = 0; i < my_izoline.size(); i++)
	{
		file << i + 1 << endl;
		for (int j = 0; j < my_izoline[i].size(); j++)
		{
			file << my_izoline[i][j].first << "\t" << my_izoline[i][j].second << endl;
		}
		//file << endl;
	}
	file.close();
}

void my_puasson::CreateElime(Gdiplus::Graphics* my_dr)
{
	//material last;
	//int x, y;
	//double ye, xe,lastx, lasty, sign;
	//PointF pt[2];
	//Matrix matr;
	//matr.Scale(xScale, yScale);
	//Pen red_pen(Color::Pink, 3);
	//for (double i = 0; i < setka.size() - 1; i += step_e_line)
	//{
	//	for (double j = 0; j < setka[i].size() - 1; j += step_e_line)
	//	{
	//		//if (setka[i][j].material_cell == metall || setka[i][j].material_cell == granica_vne) continue;
	//		if (setka[i][j].material_cell != sreda) continue;
	//		if (setka[i][j].fi >= 0) sign = 1;
	//		else sign = -1;
	//		lastx = j;
	//		lasty = i;
	//		while (lastx < (int)(j + 1) && lasty < (int)(i + 1))
	//		{
	//			if (rasstoanie(lastx, lasty, (int)lastx, (int)lasty) < rasstoanie(lastx, lasty, (int)(lastx + 1), (int)(lasty + 1))) //верхний треугольник
	//			{
	//				pt[0] = PointF(lastx, lasty);
	//				pt[1] = PointF(sign * A(setka[i][j].fi, setka[i][j + 1].fi) + lastx, sign * B(setka[i][j].fi, setka[i + 1][j].fi) + lasty);
	//				matr.TransformPoints(pt, 2);
	//				my_dr->DrawLine(&red_pen, pt[0], pt[1]);
	//			}
	//			else
	//			{
	//				pt[0] = PointF(lastx, lasty);
	//				pt[1] = PointF(sign * A(setka[i + 1][j + 1].fi, setka[i + 1][j].fi) + lastx, sign * B(setka[i + 1][j + 1].fi, setka[i][j + 1].fi) + lasty);
	//				matr.TransformPoints(pt, 2);
	//				my_dr->DrawLine(&red_pen, pt[0], pt[1]);
	//			}
	//			lastx = pt[1].X;
	//			lasty = pt[1].Y;
	//		}
	//	}
	//}

	PointF pt[2];
	Matrix matr;
	matr.Scale(xScale, yScale);
	Pen red_pen(Color::Red, 2);
	double x = 1.5, y = 1.5, xlast, ylast, cury = y, curx = x;
	int i = 0, j = 0;
	double znak;
	/*vector<pair<double, double>> power({ pair<double, double>({1, 1}), pair<double, double>({53.5, 53.5}),
		pair<double, double>({1.5, 53.5}), pair<double, double>({53.5, 1.5}),
		pair<double, double>({15.5, 1.5}), pair<double, double>({1.5, 15.5}),
		pair<double, double>({1.5, 25.5}), pair<double, double>({25.5, 1.5}),
		pair<double, double>({28.5, 28.5}), pair<double, double>({23.5, 28.5}) });*/
	//while (y < setka.size() - 1)
	//{
	for (int o = 0; o < my_e_line.size(); o += step_e_line)
	{
		x = my_e_line[o].first;
		y = my_e_line[o].second;
		//while (x < setka[i].size() - 1)
		//{
		//power.push_back(pair<double, double>({ x, y }));
		znak = (x > (int)setka[0].size() / 2 ? -1 : 1);
		do
		{
			xlast = x;
			ylast = y;/*
			if ((int)x == setka[0].size() / 2) 
				znak = (x >= setka[0].size() / 2 ? 1 : -1);*/
			i = (int)y;
			j = (int)x;
			if (i >= setka.size() - 1 || j >= setka[0].size() - 1) break;
			up_down(my_dr, i, j, i + 1, j + 1, x, y, znak);
			//power.push_back(pair<double, double>({ x, y }));
		} while (x - xlast != 0 || y - ylast != 0);
		//x += step_e_line;

		/*for (int mypair = 0; mypair < power.size() - 1; mypair++)
		{
			pt[0] = PointF(power[mypair].first, power[mypair].second);
			pt[1] = PointF(power[mypair + 1].first, power[mypair + 1].second);
			matr.TransformPoints(pt, 2);
			my_dr->DrawLine(&red_pen, pt[0], pt[1]);
		}*/
		//power.clear();
		//power.erase(power.begin(), power.begin() + 1);
	//}
		//cury += step_e_line;
		//y = cury;
		//i = (int)y;
	//}
	}

	/*PointF pt[2];
	Matrix matr;
	matr.Scale(xScale, yScale);

	double x = 0.5, y = 0.5, xnext = x, ynext = y, a, b, curx = x, cury = y, ab;
	int i = 15, j = 1;
	bool trian = true;
	int sign;
	//while (cury < setka.size() - 1)
	//{
		//cury += step_e_line;
		ynext = 1.5;
		//while (curx < setka[0].size() / 2)
		//{
			curx += step_e_line;
			xnext = 5;
			i = (int)ynext;
			j = (int)xnext;
			do
			{
				sign = setka[i][j].fi >= 0 ? 1 : -1;
				x = xnext;
				y = ynext;
				if (trian)
				{
					a = A(setka[i][j].fi, setka[i][j + 1].fi);
					b = B(setka[i][j].fi, setka[i + 1][j].fi);
				}
				else
				{
					a = A(setka[i + 1][j + 1].fi, setka[i + 1][j].fi);
					b = B(setka[i + 1][j + 1].fi, setka[i][j + 1].fi);
				}

				ab = sqrt(a * a + b * b);

				if (a == 0 && b == 0) break;
				a /= ab;
				b /= ab;

				auto point = point_crossing(x, y, j, i, a, b, trian);
				xnext = point.first;
				ynext = point.second;

				pt[0] = PointF(x, y);
				pt[1] = PointF(xnext, ynext);
				matr.TransformPoints(pt, 2);
				my_dr->DrawLine(&Pen(Color::Red), pt[0], pt[1]);

				point = next_kletka(i, j, xnext, ynext);
				i = point.second;
				j = point.first;

				trian = !trian;
			} while (xnext - x != 0 || ynext - y != 0);
		//}
		//xnext = 1.;
	//}*/
}

double my_puasson::rasstoanie(double my_x, double my_y, double xpt, double ypt)
{
	return (xpt - my_x) * (xpt - my_x) + (ypt - my_y) * (ypt - my_y);
}

double my_puasson::A(double fi1, double fi2)
{
	return fi1 - fi2;
}

double my_puasson::B(double fi1, double fi3)
{
	return -(fi3 - fi1);
}

void my_puasson::up_down(Gdiplus::Graphics* my_dr, int i, int j, int inext, int jnext, double& x, double& y, int znak)
{
	PointF pt[2];
	Matrix matr;
	matr.Scale(xScale, yScale);
	Pen red_pen(Color::Red, 2);

	//double sign = -1;
	double sign = znak;
	double ugol, step = 0.3, mya, myb;
	double xnext = x - 1, ynext = y - 1;
	xnext = x;
	ynext = y;
	i = (int)y;
	j = (int)x;
	inext = (int)y + 1;
	jnext = (int)x + 1;
	double ab;
	if (rasstoanie(x, y, j, i) <= rasstoanie(x, y, jnext, inext))
	{
		x = xnext;
		y = ynext;
		pt[0] = PointF(x, y);
		mya = A(setka[i][j].fi, setka[i][j + 1].fi);
		myb = B(setka[i][j].fi, setka[i + 1][j].fi);
		if (mya == 0 || myb == 0) return;
		//while (xnext >= j && ynext >= i && rasstoanie(xnext, ynext, j, i) < rasstoanie(xnext, ynext, jnext, inext))
		//{
		//	xnext = sign * mya * step + x;
		//	ynext = sign * myb * step + y;
		//	step += 0.01;
		//}
		ab = sqrt(mya * mya + myb * myb);
		mya /= ab;
		myb /= ab;
		xnext = sign * mya * step + x;
		ynext = sign * myb * step + y;
		pt[1] = PointF(xnext, ynext);
		matr.TransformPoints(pt, 2);
		my_dr->DrawLine(&red_pen, pt[0], pt[1]);
	}
	if (rasstoanie(x, y, j, i) > rasstoanie(x, y, jnext, inext))
	{
		x = xnext;
		y = ynext;
		pt[0] = PointF(x, y);
		mya = A(setka[i][j].fi, setka[i][j + 1].fi);
		myb = B(setka[i][j].fi, setka[i + 1][j].fi);
		if (mya == 0 || myb == 0) return;
		ab = sqrt(mya * mya + myb * myb);
		mya /= ab;
		myb /= ab;
		xnext = sign * mya * step + x;
		ynext = sign * myb * step + y;
		//while (xnext <= jnext && ynext <= inext && rasstoanie(xnext, ynext, j, i) > rasstoanie(xnext, ynext, jnext, inext))
		//{
		//	xnext = sign * mya * step + x;
		//	ynext = sign * myb * step + y;
		//	step += 0.01;
		//}
		pt[1] = PointF(xnext, ynext);
		matr.TransformPoints(pt, 2);
		my_dr->DrawLine(&red_pen, pt[0], pt[1]);
	}
	x = xnext;
	y = ynext;
}

std::pair<double, double> my_puasson::point_crossing(double x, double y, int j, int i, double my_A, double my_B, bool triangle)
{
	if (my_A == 0) 
		if (triangle) return pair<double, double>({ x, (j + 1 - x) + y });
		else return pair<double, double>({ x, i + 1 });

	if (my_B == 0)
		if (triangle) return pair<double, double>({ (j + 1 - x) / 2 + x, y });
		else return pair<double, double>({ j + 1, y });

	double k = tan(my_B / my_A);

	if (x == j)
		if (k < (y - i)) return pair<double, double>({ (j + 1 - x) / (k + 1) / 2. + x, (j + 1 - x) * k / (k + 1) / 2. + y });
		else return pair<double, double>({ (i - y) / k + x, i });

	if (x == j + 1)
		if (k < (i + 1 - y)) return pair<double, double>({ (j + 1 - x) / (k + 1) / 2. + x, (j + 1 - x) * k / (k + 1) / 2. + y });
		else return pair<double, double>({ (i + 1 - y) / k + x, i + 1 });

	if (y == i)
		if (k < (x - j) ) return pair<double, double>({ (j + 1 - x) / (k + 1) / 2. + x, (j + 1 - x) * k / (k + 1) / 2. + y });
		else return pair<double, double>({ j, (j - x) * k + y });

	if (y == i + 1)
		if (k < (j + 1 - x)) return pair<double, double>({ (j + 1 - x) / (k + 1) / 2. + x, (j + 1 - x) * k / (k + 1) / 2. + y });
		else return pair<double, double>({ j + 1, (j + 1 - x) * k + y });

	//true - пересечения в верхнем треугольнике, fasle - пересечения в нижнем треугольнике
	if (triangle)
	{
		if (x >= 0) return pair<double, double>({ (i - y) / k + x, i });
		if (y >= 0) return pair<double, double>({ j, (j - x) * k + y });
		if (k < (j - x) / (i - y)) return pair<double, double>({ (i - y) / k + x, i });
		else return pair<double, double>({ j, (j - x) * k + y });
	}
	else
	{
		if (x >= 0 && y <= 0) return pair<double, double>({ j + 1, (j + 1 - x) * k + y });
		if (y >= 0 && x <= 0) return pair<double, double>({ (i + 1 - y) / k + x, i + 1 });
		if (k < (j + 1 - x) / (i + 1 - y)) return pair<double, double>({ (i + 1 - y) / k + x, i + 1 });
		else return pair<double, double>({ j + 1, (j + 1 - x) * k + y });
	}
	/*if (triangle)
		if (k >= (j - x) / (i - y)) return pair<double, double>({ (i - y) / k + x, i });
		else return pair<double, double>({ j, (j - x) * k + y });
	else
		if (k >= (j + 1 - x) / (i + 1 - y)) return pair<double, double>({ (i + 1 - y) / k + x, i + 1 });
		else return pair<double, double>({ j + 1, (j + 1 - x) * k + y });*/
}

std::pair<double, double> my_puasson::next_kletka(int i, int j, double xnew, double ynew)
{
	/*if (xnew == j) return pair<double, double>({ j - 1, i });
	if (xnew == j + 1) return std::pair<double, double>({ j + 1, i });
	if (ynew == i) return std::pair<double, double>({ j, i - 1 });
	if (ynew == i + 1) return std::pair<double, double>({ j, i + 1 });
	return pair<double, double>({ j, i });*/
	return pair<double, double>({ (int)xnew, (int)ynew});
}

void my_puasson::SetPower(std::vector<std::vector<std::pair<double, double>>> my)
{
	my_power_line = my;
}
