#ifndef VALUATION_CLASS_H_
#define VALUATION_CLASS_H_

#include <string>
#include <vector>
#include "accessredis.h"
#include "base_function.h"
#include <time.h>
#include <thread>
#include <mutex>
#include "tradetypes.h"
#include <map>

#include <QObject>

using namespace std;

struct Contract_Info
{
	string contract_code,product;
	int multiplier;
	map<string, string> OTC_Code;
	map<string,map<string, string>> Quote;
};

class OptionValue
{
public:
    OptionValue(string trade_date_file, const map<string,PricingParam> &parameters);
	~OptionValue();

	void Init();
	void Start();
	void Stop();

	void main_value_process(void * stParam);
	void Auto_Hedger(void *stParam);

    void Parameter_Setting(string param_name, PricingParam param);
    void Parameters_Setting(map<string, PricingParam> & params);
	void Get_Parameters();
	double Maturity_Remain(string maturity_date);
	double Option_Valuation(PricingParam param);
	double Volatility_Adjustment(double basic_vol, double maturity, double strike);
	double Spread_Adjustment(double maturity, double strike, map<string, double> &position);
	double Basis_Spread(map<string, string> &basis);
    double Position_PnL(PositionType Position, bool isMain);
    string getUnderlyingCode(const string &instr_code);
	double getUnderlyingPrice(const string &instr_code);
    int getMultiplier(const string &instr_code);    //获取合约乘数

	int Delta_Hedger(vector<PositionType> &Position, double spot_price);
	int Hedger_Excute(int net_delta, double price);

    double Position_Quote(const PositionType &Position);
    double Settle_Price(const string &instr_code, LongShortType long_short);
	PositionRisk PositionGreeks(const PositionType &Position);

	double Price_Qoute(const string &instr_code);
	vector<string> Get_Main_Contract_Codes(); //Get all main contract code

	vector<string> Trade_Day;
	PricingParam value_parameter;
    map<string,PricingParam> Parameters;
	map<string, string> temp_param;
	Contract_Info main_contract;
	string Param_Key;
    std::thread main_thread, hedge_thread;
	bool value_flag,hedge_flag,param_update;
	mutex param_lock;
    vector<PositionType> Total_Position;
    CAccessRedis my_redis;
private:
	void ParameterToMap();
	void MapToParameter();

};

#endif
