#include "pch.h"
#include "Work.h"
using namespace std;

double e0 = 8.854187817e-12;

model::model()
{
}

void model::SetParametr(int my_a, int my_d, int my_l, double my_q, int my_num_const, int my_num_line)
{
	a = my_a;
	d = my_d;
	l = my_l;
	q = my_q;
	//fi = my_fi;
	left_fi = q;
	right_fi = -q;
	//right_fi = q;
	num_fi_const = my_num_const;
	fi_const = vector<double>(num_fi_const);
	for (int i = 0; i < num_fi_const; i++) fi_const[i] = right_fi + fabs(right_fi - left_fi) / (num_fi_const + 1) * (i + 1);
	//fi_const[num_fi_const - 1] = 0;
	num_F_line = my_num_line;
	//E_line = vector<pair<double, double>>(num_F_line);
}

bool model::Initialize(int width, int height)
{
	last_fi = vector<vector<cell>>(height, vector<cell>(width));
	int d_vne = 1;
	int w_cell = (width - 2. * d - a - l - 2. * d_vne) / 2.;
	int h_cell = (height - 2. * d - a - 2. * d_vne) / 2;

	if (!E_line.empty()) E_line.clear();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			last_fi[i][j].material_cell = sreda;
			last_fi[i][j].fi = 0;
		}
	}

	if (w_cell <= 0)
	{
		MessageBoxW(NULL, L"w_cell <= 0", L"Error!", NULL);
		return false;
	}
	if (h_cell <= 0)
	{
		MessageBoxW(NULL, L"h_cell <= 0", L"Error!", NULL);
		return false;
	}

	for (int j = 0; j < width; j++) //верхняя и нижняя внешняя стенка
	{
		last_fi[height - 1][j].material_cell = last_fi[0][j].material_cell = granica_vne;
		last_fi[height - 1][j].fi = last_fi[0][j].fi = 0.;
	}

	for (int i = 0; i < height; i++) //боковые внешние стенки
	{
		last_fi[i][0].material_cell = last_fi[i][width - 1].material_cell = granica_vne;
		last_fi[i][0].fi = last_fi[i][width - 1].fi = 0.;
	}

	for (int i = h_cell + 1; i <= h_cell + d; i++) //горизонтальная верхняя часть конденсатора
	{
		for (int j = w_cell + 1; j <= w_cell + a / 2 + d; j++) //left
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = left_fi;
			E_line.push_back(pair<double, double>({ j, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i }));
			E_line.push_back(pair<double, double>({ j - 0.1, i }));/*
			E_line.push_back(pair<double, double>({ j - 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j - 0.1, i + 0.1 }));*/
		}

		for (int j = w_cell + a / 2 + d + l + 1; j <= w_cell + a + 2. * d + l; j++) //right
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = right_fi;
			E_line.push_back(pair<double, double>({ j, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i }));
			E_line.push_back(pair<double, double>({ j - 0.1, i }));/*
			E_line.push_back(pair<double, double>({ j - 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j - 0.1, i + 0.1 }));*/
		}
	}

	for (int i = h_cell + d + a + 1; i <= h_cell + 2. * d + a; i++) //горизонтальная нижняя верхняя часть конденсатора
	{
		for (int j = w_cell + 1; j <= w_cell + a / 2 + d; j++) //left
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = left_fi;
			E_line.push_back(pair<double, double>({ j, i + 1 }));
			E_line.push_back(pair<double, double>({ j, i - 1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i }));
			E_line.push_back(pair<double, double>({ j - 0.1, i }));/*
			E_line.push_back(pair<double, double>({ j - 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j - 0.1, i + 0.1 }));*/
		}

		for (int j = w_cell + a / 2 + d + l + 1; j <= w_cell + a + 2. * d + l; j++) //right
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = right_fi;
			E_line.push_back(pair<double, double>({ j, i + 1 }));
			E_line.push_back(pair<double, double>({ j, i - 1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i }));
			E_line.push_back(pair<double, double>({ j - 0.1, i }));/*
			E_line.push_back(pair<double, double>({ j - 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i + 0.1 }));
			E_line.push_back(pair<double, double>({ j + 0.1, i - 0.1 }));
			E_line.push_back(pair<double, double>({ j - 0.1, i + 0.1 }));*/
		}
	}

	for (int j = w_cell + 1; j <= w_cell + d; j++) //левая боковая часть конденсатора
	{
		for (int i = h_cell + 1; i <= h_cell + a + 2 * d; i++)
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = left_fi;
			E_line.push_back(pair<double, double>({ j - 0.1, i }));
			E_line.push_back(pair<double, double>({ j + 1, i }));
		}
	}

	for (int j = w_cell + d + a + l + 1; j <= w_cell + d + a + d + l; j++) //правая боковая часть конденсатора
	{
		for (int i = h_cell + 1; i <= h_cell + a + 2 * d; i++)
		{
			last_fi[i][j].material_cell = last_fi[i][j].material_cell = metall;
			last_fi[i][j].fi = last_fi[i][j].fi = right_fi;
			E_line.push_back(pair<double, double>({ j + 1, i }));
			E_line.push_back(pair<double, double>({ j - 0.1, i }));
		}
	}

	//bool need = true;
	//material last;
	//for (int i = 1; i < height - 1; i++) //выставляем границы
	//{
	//	last = sreda;
	//	for (int j = 1; j < width - 1; j++)
	//	{
	//		if (last_fi[i][j].material_cell == metall && last == sreda)
	//		{
	//			last = last_fi[i][j].material_cell;
	//			last_fi[i][j - 1].material_cell = granica_vnut_gor;
	//			if (j < w_cell + d + a / 2 + l) last_fi[i][j - 1].fi = left_fi;
	//			else last_fi[i][j - 1].fi = right_fi;
	//			E_line.push_back(pair<double, double>({ j - 1.1, i }));
	//		}
	//		else
	//		{
	//			if (last_fi[i][j].material_cell == sreda && last == metall)
	//			{
	//				last = last_fi[i][j].material_cell;
	//				last_fi[i][j].material_cell = granica_vnut_gor;
	//				if (j < w_cell + d + a / 2 + l) last_fi[i][j].fi = left_fi;
	//				else last_fi[i][j].fi = right_fi;
	//				E_line.push_back(pair<double, double>({ j - 0.1 , i }));
	//			}
	//		}
	//	}
	//}
	//for (int j = 1; j < width - 1; j++) //выставляем границы
	//{
	//	last = sreda;
	//	for (int i = 1; i < height - 1; i++)
	//	{
	//		if (last_fi[i][j].material_cell == metall && last == sreda)
	//		{
	//			last = last_fi[i][j].material_cell;
	//			last_fi[i - 1][j].material_cell = granica_vnut_ver;
	//			if (j < w_cell + d + a / 2 + l) last_fi[i - 1][j].fi = left_fi;
	//			else last_fi[i - 1][j].fi = right_fi;
	//			E_line.push_back(pair<double, double>({ j - 0.1 , i - 1.1 }));
	//		}
	//		else
	//		{
	//			if (last_fi[i][j].material_cell == sreda && last == metall)
	//			{
	//				last = last_fi[i][j].material_cell;
	//				last_fi[i][j].material_cell = granica_vnut_ver;
	//				if (j < w_cell + d + a / 2 + l) last_fi[i][j].fi = left_fi;
	//				else last_fi[i][j].fi = right_fi;
	//				E_line.push_back(pair<double, double>({ j - 0.1 , i }));
	//			}
	//		}
	//	}
	//}
	cur_fi = last_fi;
	return true;
}

void model::Initialize(std::vector<std::vector<cell>> my_fi)
{
	cur_fi = last_fi = my_fi;
}

void model::Iteration()
{
	for (int i = 1; i < last_fi.size() - 1; i++)
	{
		for (int j = 1; j < last_fi[i].size() - 1; j++)
		{
			if (cur_fi[i][j].material_cell == sreda) cur_fi[i][j].fi = (last_fi[i + 1][j].fi + last_fi[i - 1][j].fi + last_fi[i][j + 1].fi + last_fi[i][j - 1].fi) / 4.;
		}
	}

}

void model::write()
{
	ofstream file;
	file.open(L"my fi.txt");
	file.precision(2);
	for (int i = 0; i < last_fi.size(); i++)
	{
		for (int j = 0; j < last_fi[i].size(); j++)
		{
			file << last_fi[i][j].fi << "\t";
		}
		file << endl;
	}
	file.close();
}

double model::error()
{
	double er = 0;
	for (int i = 1; i < last_fi.size() - 1; i++)
	{
		for (int j = 1; j < last_fi[i].size() - 1; j++) er += fabs(last_fi[i][j].fi - cur_fi[i][j].fi);
	}
	last_fi = cur_fi;
	write();
	return er;
}

std::vector<std::vector<cell>> model::GetFi()
{
	return last_fi;
}

std::vector<std::vector<std::pair<double, double>>> model::GetPowerL()
{
	//return powerL;
	return std::vector<std::vector<std::pair<double, double>>>();
}
