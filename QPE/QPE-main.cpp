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


int out_dBZ(const std::vector<std::string>& elevs, 
	const std::map<std::string, std::pair<unsigned ,unsigned > >& stations)
{
	std::string temp, filename, path = "Z:/swap/radar-luoyang-2015/10/";

	SA_SB_Info radar;

	std::ifstream fin("filenames.txt");

	//读取给定雷达基数据归档文件 输出扫描时间 各站不同仰角dBZ
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


		for (const auto& elev : elevs)//各仰角
		{
			auto elev_data = radar(elev);
			double dBZ = 0;

			if (elev_data.r_valid)
			{
				for (const auto& station : stations)//各站
				{
					auto name = station.first;
					auto position = station.second;

					dBZ = get_dbZ(elev_data.r, position.first, position.second);

					std::ofstream fout(name + "-" + elev + ".txt", std::ios_base::out|std::ios_base::app);
					fout << elev_data.date_begin << ' ' << elev_data.seconds_begin << ' ' << dBZ << ' ' << get_R_from_dBZ(dBZ) << std::endl;
				}

			}
		}

	}


	//std::ofstream fout("log.txt");
	//radar.out_info(fout);

	return 0;
}

int main()
{
	std::vector<std::string> elevs = { "0.50" ,"1.45", "2.40", "3.35" };

	std::map<std::string, std::pair<unsigned, unsigned > > stations = {
		{"57071", std::make_pair(0, 30) },

		{ "57076", std::make_pair(256, 74) }
	};

	out_dBZ(elevs, stations);
}