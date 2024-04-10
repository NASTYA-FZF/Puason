#pragma once
#include <vector>
#include <fstream>

enum material
{
	metall,
	sreda,
	granica_vne,
	granica_vnut_ver,
	granica_vnut_gor,
	none
};

struct cell
{
	material material_cell;
	double fi;
};

class model
{
	std::vector<std::vector<cell>> last_fi, cur_fi;
	int a, d, l, num_fi_const, num_F_line;
	double q, left_fi, right_fi;
	//std::vector<std::vector<std::pair<double, double>>> powerL;
public:
	std::vector<double> fi_const;
	std::vector<std::pair<double, double>> E_line;
	
	model();

	void SetParametr(int a, int d, int l, double q, int my_num_const, int my_num_line);

	bool Initialize(int width, int height);

	void Initialize(std::vector<std::vector<cell>> my_fi);

	void Iteration();

	void write();

	double error();

	std::vector<std::vector<cell>> GetFi();

	//void OnePowerL(double x, double y);

	//void ManyPowerL(int kol);

	std::vector<std::vector<std::pair<double, double>>> GetPowerL();
};