#include "SA-SBDataStruct.h"

#include <string>
#include <fstream>
#include <iostream>

//使用给定的Z-R关系 从dBZ获取降水率
double get_R_from_dBZ(double dBZ, double A = 300, double b = 1.4)
{
	return pow(10, (dBZ - 10 * log10(A)) / (10 * b));
}

//给定某一仰角数据 获取某个方位角和距离上的反射率因子
double get_dbZ(const std::vector<std::vector<double> > & r, unsigned az, unsigned gate)
{
	using point = std::pair<unsigned, unsigned>;
	std::vector< point > points;

	points.push_back(point(az, gate));
	points.push_back(point(az, gate - 1));
	points.push_back(point(az, gate + 1));

	unsigned az_next = (az + 359) % 360;
	points.push_back(point(az_next, gate));
	points.push_back(point(az_next, gate - 1));
	points.push_back(point(az_next, gate + 1));

	az_next = (az + 1) % 360;
	points.push_back(point(az_next, gate));
	points.push_back(point(az_next, gate - 1));
	points.push_back(point(az_next, gate + 1));

	double dbz = VALUE_INVALID;
	for (auto& point : points)
	{
		dbz = r[point.first][point.second];
		if (dbz != VALUE_INVALID && dbz != VALUE_RANFOLD)
		{
			return dbz;
		}
	}
	return 0; //or return VALUE_INVALID ?
}


int main()
{
	std::string temp, filename, path = "Z:/swap/radar-luoyang-2015/10/";

	//std::cout << "输入雷达基数据文件名(输入0使用测试文件Z_RADR_I_Z9398_20120717041200_O_DOR_SB_CAP.bin)：\n";
	//std::cin >> filename;

	//if (filename == "0") filename = "2015102506.57A";

	SA_SB_Info radar;

	std::ifstream fin("filenames.txt");
	std::ofstream f050("0.5.txt"),
		f145("1.45.txt"),
		f240("2.40.txt"),
		f335("3.35.txt");

	//读取给定雷达基数据归档文件 输出扫描时间 某一点不同仰角dBZ
	while (fin >> temp)
	{
		fin >> temp >> temp;
		fin >> filename;
		std::cout << filename << '\n';

		radar.read_base_data(path + filename);

		std::cout << "sizeof(RadarBaseData):" << sizeof(SA_SB_Info::SB_Base) << '\n';
		std::cout << "date_begin:" << radar.date_begin << " date_end:" << radar.date_end << '\n';
		std::cout << "seconds_begin:" << radar.seconds_begin << " seconds_end:" << radar.seconds_end << '\n';

		std::cout << "radar.vol_model:" << radar.vol_model << '\n';

		for (auto &elev : radar.elevations)
		{
			std::cout << elev.first << ' ';
		}
		std::cout << std::endl;



		auto elev = radar("0.50");
		double dBZ = 0;

		if (elev.r_valid)
		{
			dBZ = get_dbZ(elev.r, 0, 30);
			f050 << elev.date_begin << ' ' << elev.seconds_begin << ' ' << dBZ << ' ' << get_R_from_dBZ(dBZ) << std::endl;
		}

		elev = radar("1.45");

		if (elev.r_valid)
		{
			dBZ = get_dbZ(elev.r, 0, 30);
			f145 << elev.date_begin << ' ' << elev.seconds_begin << ' ' << dBZ << ' ' << get_R_from_dBZ(dBZ) << std::endl;
		}

		elev = radar("2.40");

		if (elev.r_valid)
		{
			dBZ = get_dbZ(elev.r, 0, 30);
			f240 << elev.date_begin << ' ' << elev.seconds_begin << ' ' << dBZ << ' ' << get_R_from_dBZ(dBZ) << std::endl;
		}

		elev = radar("3.35");

		if (elev.r_valid)
		{
			dBZ = get_dbZ(elev.r, 0, 30);
			f335 << elev.date_begin << ' ' << elev.seconds_begin << ' ' << dBZ << ' ' << get_R_from_dBZ(dBZ) << std::endl;
		}
	}

	f050.close(); f145.close(); f240.close(); f335.close();

	//std::ofstream fout("log.txt");
	//radar.out_info(fout);
}