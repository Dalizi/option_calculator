#include "valuation_class.h"

#include <sstream>
#include <fstream>
#include <QMessageBox>

using namespace std;

extern string REDIS_ADDR;
extern int REDIS_PORT;
extern string REDIS_PASSWD;

OptionValue::OptionValue(string trade_date_file, CAccessRedis *redis, const map<string,PricingParam> &parameters):my_redis(redis)
{
    ifstream in_file(trade_date_file);

    if (!in_file.is_open()) {
        QMessageBox::about(0, "ERROR", "Error loading trade date info.");
        exit(1);
    }
    string temp;
    while (in_file >> temp)
    {
        Trade_Day.push_back(temp);
    }

    Parameters=parameters;
	return;
}

OptionValue::~OptionValue()
{
	Stop();
}

void OptionValue::ParameterToMap()
{
	char * temp = new char[100];
	temp_param.clear();
	temp_param=value_parameter.other_param;

    sprintf(temp, "%f", value_parameter.strike_price);
	temp_param["Strike"] = temp;
    sprintf(temp, "%f", value_parameter.spot_price);
	temp_param["Underlying"] = temp;
    sprintf(temp, "%f", value_parameter.free_rate);
	temp_param["FreeRate"] = temp;
    sprintf(temp, "%f", value_parameter.yield_rate);
	temp_param["YiledRate"] = temp;
    sprintf(temp, "%f", value_parameter.volatility);
    temp_param["volatility"] = temp;
    sprintf(temp, "%f", value_parameter.time_to_maturity);
	temp_param["Maturity"] = temp;
    sprintf(temp, "%d", value_parameter.option_type);
	temp_param["OptionType"] = temp;
    sprintf(temp, "%d", value_parameter.value_method);
	temp_param["ValueMethod"] = temp;

	delete[] temp;
	return;
}

void OptionValue::MapToParameter()
{
	param_lock.lock();
    value_parameter.free_rate =atof( temp_param["FreeRate"].c_str());
    value_parameter.yield_rate = atof(temp_param["YiledRate"].c_str());
    value_parameter.spot_price = atof(temp_param["Underlying"].c_str());
    value_parameter.strike_price =atof( temp_param["Strike"].c_str());
    value_parameter.volatility = atof(temp_param["volatility"].c_str());
    value_parameter.option_type = atoi(temp_param["OptionType"].c_str());
    value_parameter.time_to_maturity = atof(temp_param["Maturity"].c_str());
    value_parameter.value_method = atoi(temp_param["ValueMethod"].c_str());
	temp_param.erase("FreeRate");
	temp_param.erase("YieldRate");
	temp_param.erase("Underlying");
	temp_param.erase("Strike");
    temp_param.erase("volatility");
	temp_param.erase("OptionType");
	temp_param.erase("Maturity");
	temp_param.erase("ValueMethod");
	value_parameter.other_param = temp_param;
	param_lock.unlock();
	return;
}

void OptionValue::Parameter_Setting(string param_name, PricingParam param)
{
    param_lock.lock();
    Parameters[param_name]=param;
    param_lock.unlock();
	return;
}

void OptionValue::Parameters_Setting(const map<string, PricingParam> & params)
{
	param_lock.lock();
    Parameters=params;
	param_lock.unlock();
	return;
}

void OptionValue::Get_Parameters()
{
	int iRet;
    iRet=my_redis->HGetAll(Param_Key, temp_param);
    //Get All Parameters from DB file
    //Transform to
	return;
}

double OptionValue::Maturity_Remain(string maturity_date)
{
	time_t now;
	time(&now);
	struct tm today;
	today = *localtime(&now);
	int start, end;
	double result = 0;

	char buffer[100];
	strftime(buffer, 100, "%Y-%m-%d", &today);

	string cur_date = buffer;
    for (int i = 0; i < Trade_Day.size(); i++)
	{
		if (strcmp(cur_date.c_str(), Trade_Day[i].c_str()) <= 0){
			start = i;
			break;
		}
	}
    for (int i = 0; i < Trade_Day.size(); i++)
	{
		if (strcmp(maturity_date.c_str(), Trade_Day[i].c_str()) <= 0){
			end = i;
			break;
		}
	}
    result = end - start+1;
	if (end - start <= 5){
		if (today.tm_hour >= 10 && today.tm_hour < 11){
			result = result - 1.25 / 4.5;
		}
		else if (today.tm_hour >= 11 && today.tm_hour < 12){
			result = result - 2 / 4.5;
		}
		else if (today.tm_hour >= 13 && today.tm_hour < 14){
			result = result - 2.5 / 4.5;
		}
		else if (today.tm_hour >= 14 && today.tm_hour < 15){
			result = result - 3.5 / 4.5;
		}
		else if (today.tm_hour >= 15 && today.tm_hour < 16){
			result = result - 1;
		}
		else{}
	}
	return result/256;
}

double OptionValue::Option_Valuation(PricingParam param)
{
    int method = param.value_method;
	double result=0;
	switch (method)
	{
	case 1: result=bls_vanila_option(param); break;
	}
	return result;
}

int OptionValue::Delta_Hedger(vector<PositionType> &Position,double spot_price)
{
	double result=0;
	double maturity,strike,basic_vola;
    PricingParam temp_param;
	
	param_lock.lock();
	temp_param = value_parameter;
	param_lock.unlock();

    basic_vola = temp_param.volatility;
    temp_param.spot_price = spot_price;

	for (int i = 0; i < Position.size(); i++)
	{
		if (strcmp(Position[i].instr_code.toStdString().substr(0, 4).c_str(), "OTC-") == 0){
			int l = Position[i].instr_code.length();
			int pos_first = Position[i].instr_code.toStdString().substr(4, l - 4).find_first_of("-") + 4;
			int pos_last = Position[i].instr_code.toStdString().find_last_of("-");
			string temp = Position[i].instr_code.toStdString().substr(pos_first + 1, 10);
			maturity = Maturity_Remain(temp);
			temp = Position[i].instr_code.toStdString().substr(pos_last + 1, l - pos_last - 1);
			strike = atof(temp.c_str());
			temp = Position[i].instr_code.toStdString().substr(pos_first - 3, 3);

            if (strcmp(temp.c_str(), "C00") == 0){ temp_param.option_type = 1; }
            if (strcmp(temp.c_str(), "P00") == 0){ temp_param.option_type = 2; }

            temp_param.strike_price = strike;
            temp_param.time_to_maturity = maturity;
            temp_param.volatility = Volatility_Adjustment(basic_vola, maturity, strike);

            switch (temp_param.value_method)
			{
			case 1: result = result + 2*(-Position[i].long_short+0.5)*Position[i].total_amount*bls_vanila_delta(temp_param)*main_contract.multiplier; break;
			}
		}
		else if (strcmp(Position[i].instr_code.toStdString().substr(0, 2).c_str(), "IF") == 0)
		{
			result = result + 2 * (-Position[i].long_short + 0.5)*Position[i].total_amount;
		}
		else{}
	}

	return (int)round(result/300);
}

int OptionValue::Hedger_Excute(int net_delta,double price)
{
	double excute_price;
	int amount;
	net_delta = -net_delta;
	if (net_delta < 0)
	{
		excute_price = price + 1;
		amount = net_delta;
	}
	else
	{
		excute_price = price - 1;
		amount = net_delta;
	}

	int i;
	for (i = 0; i < Total_Position.size(); i++)
	{
		if (strcmp(Total_Position[i].instr_code.toStdString().c_str(), main_contract.contract_code.c_str()) == 0) break;
	}

	if (i >= Total_Position.size()){
		PositionType temp;
		memset(&temp, sizeof(temp),0);
		temp.instr_code = QString::fromStdString(main_contract.contract_code);
		Total_Position.push_back(temp);
		i = Total_Position.size() - 1;
	}

	if (Total_Position[i].total_amount == 0)
	{
		Total_Position[i].average_price = price;
		Total_Position[i].total_amount = abs(net_delta);
        Total_Position[i].long_short = (LongShortType)((net_delta > 0) ? 0 : 1);
	}
	else
	{
		if (2*(-Total_Position[i].long_short+0.5)*net_delta > 0)
		{
			Total_Position[i].average_price = Total_Position[i].average_price*Total_Position[i].total_amount + abs(net_delta)*price;
			Total_Position[i].total_amount = Total_Position[i].total_amount + abs(net_delta);
			Total_Position[i].average_price = Total_Position[i].average_price / Total_Position[i].total_amount;
		}
		else
		{
			int new_position = Total_Position[i].total_amount - abs(net_delta);
			if (new_position >= 0){
				Total_Position[i].total_amount = new_position;
				double PnL = (price - Total_Position[i].average_price)*Total_Position[i].long_short/abs(net_delta);
				Total_Position[i].average_price = Total_Position[i].average_price - PnL;
			}
			else{
				Total_Position[i].total_amount = -new_position;
                Total_Position[i].long_short = (LongShortType)(1 - Total_Position[i].long_short);
				double PnL = (price - Total_Position[i].average_price)*Total_Position[i].long_short / Total_Position[i].total_amount;
				Total_Position[i].average_price = Total_Position[i].average_price - PnL;
			}

		}

    }
	
	cout << "Delta Hedged at: " << excute_price << " with amount " << net_delta << endl;

	return 0;
}

double OptionValue::Volatility_Adjustment(double basic_vol, double maturity, double strike)
{

	return basic_vol;
}

double OptionValue::Spread_Adjustment(double maturity, double strike, map<string, double> &position)
{
	return 0;
}

double OptionValue::Basis_Spread( map<string,string> &basis)
{
	double spread=0;
	string type = basis["Spread_Type"];
	if (strcmp(type.c_str(), "Vol") == 0 && basis.count("Basis_Vol_Spread")>0)
	{
		spread = atof(basis["Basis_Vol_Spread"].c_str());
	}
	else if (strcmp(type.c_str(), "Del") == 0 && basis.count("Basis_Delta_Spread")>0)
	{
		spread = atof(basis["Basis_Delta_Spread"].c_str());
	}
	else if (strcmp(type.c_str(), "Prc") == 0 && basis.count("Basis_Price_Spread")>0)
	{
		spread = atof(basis["Basis_Price_Spread"].c_str());
	}
	else
	{
	}
	return spread;
}

double OptionValue::Position_PnL(PositionType Position, bool isMain)
{

    LongShortType ls=Position.long_short;
    double result = 0;
    PricingParam temp_param;
    map<string, string> update_data;
    string underlying_code=Position.underlying_code.toStdString();
    int iRet = my_redis->HGetAll(underlying_code, update_data);
    float last_spot = atof(update_data["LastPrice"].c_str());

    double maturity, strike, basic_vola;
    basic_vola = temp_param.volatility;

    if (strcmp(Position.instr_code.toStdString().substr(0, 4).c_str(), "OTC-") == 0){
        int l = Position.instr_code.toStdString().length();
        int pos_first = Position.instr_code.toStdString().substr(4, l - 4).find_first_of("-") + 4;
        int pos_last = Position.instr_code.toStdString().find_last_of("-");
        string temp = Position.instr_code.toStdString().substr(pos_first + 1, 10);
        maturity = Maturity_Remain(temp);
        temp = Position.instr_code.toStdString().substr(pos_last + 1, l - pos_last - 1);
        strike = atof(temp.c_str());
        temp = Position.instr_code.toStdString().substr(4, 3);

        param_lock.lock();
        temp_param = Parameters[temp];
        param_lock.unlock();

        basic_vola=Position.implied_vol;
        if (Position.vol_type==DEFAULT)
        {
            basic_vola = temp_param.volatility;
        }

        temp_param.spot_price = last_spot;

        temp = Position.instr_code.toStdString().substr(pos_first - 3, 3);
        //European Options
        if (strcmp(temp.c_str(), "C00") == 0){ temp_param.option_type = 1; temp_param.value_method=1;}
        if (strcmp(temp.c_str(), "P00") == 0){ temp_param.option_type = 2; temp_param.value_method=1;}

        //American Options
        if (strcmp(temp.c_str(), "C01") == 0){ temp_param.option_type = 1; temp_param.value_method=2;}
        if (strcmp(temp.c_str(), "P01") == 0){ temp_param.option_type = 2; temp_param.value_method=2;}

        //Barrier Options
        if (strcmp(temp.c_str(), "COO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="1";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "POO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="2";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "COI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="3";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "POI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="4";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "CDO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="5";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "PDO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="6";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "CDI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="7";temp_param.other_param["Barrier"]=Position.knockout_price;}
        if (strcmp(temp.c_str(), "PDI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="8";temp_param.other_param["Barrier"]=Position.knockout_price;}

        temp_param.strike_price = strike;
        temp_param.time_to_maturity = maturity;
        temp_param.volatility = Volatility_Adjustment(basic_vola, maturity, strike);

        switch (temp_param.value_method)
        {
        case 1:
        {
                  result = 2 * (-(int)Position.long_short + 0.5)*Position.total_amount*(bls_vanila_option(temp_param)- Position.average_price); break;
        }
        }
    }
    else{}

    return result;
}

void OptionValue::Init()
{

    string cur_date = QDate::currentDate().toString("%Y-%m-%d").toStdString();
	string mat_date;
	for (int i = 0; i <Trade_Day.max_size(); i++)
	{
		if (strcmp(cur_date.c_str(), Trade_Day[i].c_str()) <= 0){
			mat_date = Trade_Day[i + 64];
			break;
		}
	}

	cout << "Valuation Initialized!" << endl;
	return;
}

void OptionValue::Start()
{
    //value_flag = true;
	//hedge_flag = true;
    //main_thread = std::thread(&OptionValue::main_value_process, this, this);
    //hedge_thread = thread(&OptionValue::Auto_Hedger, this, this);
	return;
}

void OptionValue::Stop()
{
    //value_flag = false;
    //main_thread.join();
	//hedge_flag = false;
	//hedge_thread.join();
	return;
}

void OptionValue::main_value_process(void *stParam)
{
    /*OptionValue *ptr = (OptionValue *)stParam;
    PricingParam temp_param;
	time_t now;
	struct tm current;
	map<string, string>update_data;
	int atm_strike,iRet;
	char *buffer = new char[1000];
	string temp_code;
	double spread;

	ptr->Get_Parameters();
	ptr->MapToParameter();
    atm_strike = value_parameter.strike_price;
	sprintf(buffer, "%d", atm_strike);
	temp_code = buffer;
	ptr->param_update = true;

	cout << "Main Pricing Process Start!" << endl;
	while (ptr->value_flag)
	{
		time(&now);
		current = *localtime(&now);

        iRet = my_redis->HGetAll(ptr->main_contract.contract_code, update_data);
		float last_spot = atof(update_data["LastPrice"].c_str());
        //cout << update_data["UpdateTime"]<<": "<<last_spot << endl;
        if (abs(last_spot - atm_strike)>10)
		{
			atm_strike = round(last_spot / 10) * 10;
			sprintf(buffer, "%d", atm_strike);
			temp_code = buffer;
			ptr->Parameter_Setting("Strike", temp_code);
		}
		if (param_update==true){
			ptr->main_contract.Quote["atm_vanila_call"]["strike"] = temp_code;
			ptr->main_contract.Quote["atm_vanila_put"]["strike"] = temp_code;
			//strftime(buffer, 100, "%H%M%S", &current);
			
			ptr->main_contract.Quote["atm_vanila_call"]["code"] = ptr->main_contract.OTC_Code["atm_vanila_call"]+"-"+temp_code;
			ptr->main_contract.Quote["atm_vanila_put"]["code"] = ptr->main_contract.OTC_Code["atm_vanila_put"] + "-"+temp_code;

			ptr->Get_Parameters();
			ptr->MapToParameter();

			ptr->param_lock.lock();
			temp_param = value_parameter;
			ptr->param_lock.unlock();


            temp_param.spot_price = last_spot;
			spread = Basis_Spread(temp_param.other_param);

            temp_param.option_type = 1;
            temp_param.volatility = temp_param.volatility - spread;
			sprintf(buffer, "%f", ptr->Option_Valuation(temp_param));
			ptr->main_contract.Quote["atm_vanila_call"]["bid"] = buffer;
            temp_param.volatility = temp_param.volatility + 2*spread;
			sprintf(buffer, "%f", ptr->Option_Valuation(temp_param));
			ptr->main_contract.Quote["atm_vanila_call"]["ask"] = buffer;
			ptr->main_contract.Quote["atm_vanila_call"]["bid_volume"] = "500";
			ptr->main_contract.Quote["atm_vanila_call"]["ask_volume"] = "500";

            temp_param.option_type = 2;
            temp_param.volatility = temp_param.volatility - 2 * spread;
			sprintf(buffer, "%f", ptr->Option_Valuation(temp_param));
			ptr->main_contract.Quote["atm_vanila_put"]["bid"] = buffer;
            temp_param.volatility = temp_param.volatility + 2 * spread;
			sprintf(buffer, "%f", ptr->Option_Valuation(temp_param));
			ptr->main_contract.Quote["atm_vanila_put"]["ask"] = buffer;
			ptr->main_contract.Quote["atm_vanila_put"]["bid_volume"] = "500";
			ptr->main_contract.Quote["atm_vanila_put"]["ask_volume"] = "500";

            sprintf(buffer, "%f", temp_param.volatility);
			ptr->main_contract.Quote["atm_vanila_call"]["implied_vol"] = buffer;
			ptr->main_contract.Quote["atm_vanila_put"]["implied_vol"] = buffer;
            my_redis->HMSet(ptr->main_contract.OTC_Code["atm_vanila_call"], ptr->main_contract.Quote["atm_vanila_call"]);
            my_redis->HMSet(ptr->main_contract.OTC_Code["atm_vanila_put"], ptr->main_contract.Quote["atm_vanila_put"]);

			param_lock.lock();
			param_update = false;
			param_lock.unlock();
		}
        sleep(1);
	}

	delete[] buffer;
    cout << "Main Pricing Process Quit!" << endl;*/
	return;
}

void OptionValue::Auto_Hedger(void *stParam)
{
//	OptionValue *ptr = (OptionValue *)stParam;
//	time_t now;
//	struct tm current;
//	map<string, string>update_data;
//	int net_delta, iRet;
//	char *buffer = new char[1000];
//	string temp_code;

//	cout << "Auto Hedger Start!" << endl;
//	while (ptr->hedge_flag)
//	{
//		time(&now);
//		current = *localtime(&now);

//		Total_Position = tm->getAllMainAccountPosition();
//		iRet = my_redis->HGetAll(ptr->main_contract.contract_code, update_data);
//		float last_spot = atof(update_data["LastPrice"].c_str());
//		net_delta=ptr->Delta_Hedger(Total_Position,last_spot);
//		cout << update_data["UpdateTime"] << ": " << last_spot <<"  Position Delta: "<<net_delta<< endl;
//		if (abs(net_delta)>5){
//			ptr->Hedger_Excute(net_delta, last_spot);
//		}
//        sleep(1);
//	}

//	delete[] buffer;

//	cout << "Auto Hedger Stopped!" << endl;
//	return;
}

double OptionValue::Position_Quote(const PositionType &Position)
{
    LongShortType ls=Position.long_short;
    double result = 0;
    PricingParam temp_param;
    map<string, string> update_data;
    string underlying_code=Position.underlying_code.toStdString();
    int iRet = my_redis->HGetAll(underlying_code, update_data);
    float last_spot = atof(update_data["LastPrice"].c_str());

    double maturity, strike, basic_vola;

    if (strcmp(Position.instr_code.toStdString().substr(0, 4).c_str(), "OTC-") == 0){
        int l = Position.instr_code.toStdString().length();
        int pos_first = Position.instr_code.toStdString().substr(4, l - 4).find_first_of("-") + 4;
        int pos_last = Position.instr_code.toStdString().find_last_of("-");
        string temp = Position.instr_code.toStdString().substr(pos_first + 1, 10);
        maturity = Maturity_Remain(temp);
        temp = Position.instr_code.toStdString().substr(pos_last + 1, l - pos_last - 1);
        strike = atof(temp.c_str());
        temp = Position.instr_code.toStdString().substr(4, 3);

        param_lock.lock();
        temp_param = Parameters[temp];
        param_lock.unlock();

        basic_vola=Position.implied_vol;
        if (Position.vol_type==DEFAULT)
        {
            basic_vola=temp_param.volatility;
        }
        temp_param.spot_price = last_spot;

        temp = Position.instr_code.toStdString().substr(pos_first - 3, 3);
        //European Options
        if (strcmp(temp.c_str(), "C00") == 0){ temp_param.option_type = 1; temp_param.value_method=1;}
        if (strcmp(temp.c_str(), "P00") == 0){ temp_param.option_type = 2; temp_param.value_method=1;}

        //American Options
        if (strcmp(temp.c_str(), "C01") == 0){ temp_param.option_type = 1; temp_param.value_method=2;}
        if (strcmp(temp.c_str(), "P01") == 0){ temp_param.option_type = 2; temp_param.value_method=2;}

        //Barrier Options
//        if (strcmp(temp.c_str(), "COO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="1";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "POO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="2";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "COI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="3";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "POI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="4";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "CDO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="5";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "PDO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="6";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "CDI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="7";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "PDI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="8";temp_param.other_param["Barrier"]=Position.Barrier}

        temp_param.strike_price = strike;
        temp_param.time_to_maturity = maturity;
        temp_param.volatility = Volatility_Adjustment(basic_vola, maturity, strike);

        switch (temp_param.value_method)
        {
        case 1:
        {
                  result = bls_vanila_option(temp_param)*Position.total_amount; break;
        }
        }
    }
    else{}

	return result;
}

double OptionValue::Settle_Price(const string &instr_code, LongShortType long_short)
{
	double result = 0;
    double maturity, strike, basic_vola, spot_price;
    PricingParam temp_param;

	param_lock.lock();
	temp_param = value_parameter;
	param_lock.unlock();


    temp_param.spot_price = atof(temp_param.other_param["LastSettlePrice"].c_str());
    temp_param.volatility = atof(temp_param.other_param["LastSettleVola"].c_str());
    spot_price = temp_param.spot_price;
    basic_vola = temp_param.volatility;

    if (strcmp(instr_code.substr(0, 4).c_str(), "OTC-") == 0){
        int l = instr_code.length();
        int pos_first = instr_code.substr(4, l - 4).find_first_of("-") + 4;
        int pos_last = instr_code.find_last_of("-");
        string temp = instr_code.substr(pos_first + 1, 10);
		maturity = Maturity_Remain(temp);
        temp = instr_code.substr(pos_last + 1, l - pos_last - 1);
		strike = atof(temp.c_str());
        temp = instr_code.substr(pos_first - 3, 3);

        if (strcmp(temp.c_str(), "C00") == 0){ temp_param.option_type = 1; }
        if (strcmp(temp.c_str(), "P00") == 0){ temp_param.option_type = 2; }

        temp_param.strike_price = strike;
        temp_param.time_to_maturity = maturity;
		double spread = Basis_Spread(temp_param.other_param);
        temp_param.volatility = (long_short == LONG_ORDER ? -1 : 1) * spread + Volatility_Adjustment(basic_vola, maturity, strike);

        switch (temp_param.value_method)
		{
		case 1: result = bls_vanila_option(temp_param); break;
		}
	}
    else if (strcmp(instr_code.substr(0, 2).c_str(), "IF") == 0)
	{
        result = spot_price;
	}
	else{}

	return result;
}

PositionRisk OptionValue::PositionGreeks(const PositionType &Position)
{
    PricingParam temp_param;
	map<string, string> update_data;
    string underlying_code=Position.underlying_code.toStdString();
    int iRet = my_redis->HGetAll(underlying_code, update_data);
    float last_spot = stof(update_data["LastPrice"]);

    temp_param.spot_price = last_spot;
    PositionRisk result;
    double maturity = 0;
    double strike = 0;
    double basic_vola = 0;


	if (strcmp(Position.instr_code.toStdString().substr(0, 4).c_str(), "OTC-") == 0){
		int l = Position.instr_code.toStdString().length();
		int pos_first = Position.instr_code.toStdString().substr(4, l - 4).find_first_of("-") + 4;
		int pos_last = Position.instr_code.toStdString().find_last_of("-");
		string temp = Position.instr_code.toStdString().substr(pos_first + 1, 10);
		maturity = Maturity_Remain(temp);
		temp = Position.instr_code.toStdString().substr(pos_last + 1, l - pos_last - 1);
		strike = atof(temp.c_str());
        temp = Position.instr_code.toStdString().substr(4, 3);

        param_lock.lock();
        temp_param = Parameters[temp];
        param_lock.unlock();

        basic_vola=Position.implied_vol;
        if (Position.vol_type==DEFAULT)
        {
            basic_vola=temp_param.volatility;
        }

        temp_param.spot_price = last_spot;

        temp = Position.instr_code.toStdString().substr(pos_first - 3, 3);
        //European Options
        if (strcmp(temp.c_str(), "C00") == 0){ temp_param.option_type = 1; temp_param.value_method=1;}
        if (strcmp(temp.c_str(), "P00") == 0){ temp_param.option_type = 2; temp_param.value_method=1;}

        //American Options
        if (strcmp(temp.c_str(), "C01") == 0){ temp_param.option_type = 1; temp_param.value_method=2;}
        if (strcmp(temp.c_str(), "P01") == 0){ temp_param.option_type = 2; temp_param.value_method=2;}

        //Barrier Options
//        if (strcmp(temp.c_str(), "COO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="1";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "POO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="2";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "COI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="3";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "POI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="4";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "CDO") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="5";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "PDO") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="6";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "CDI") == 0){ temp_param.option_type = 1; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="7";temp_param.other_param["Barrier"]=Position.Barrier}
//        if (strcmp(temp.c_str(), "PDI") == 0){ temp_param.option_type = 2; temp_param.value_method=3;temp_param.other_param["Bar_Dir"]="8";temp_param.other_param["Barrier"]=Position.Barrier}

        temp_param.strike_price = strike;
        temp_param.time_to_maturity = maturity;
        temp_param.volatility = Volatility_Adjustment(basic_vola, maturity, strike);

        switch (temp_param.value_method)
		{
		case 1:
		{
                  result.delta = 2 * (-Position.long_short + 0.5)*Position.total_amount*bls_vanila_delta(temp_param);
                  result.delta_f=result.delta/temp_param.multiplier;
                  result.gamma = 2 * (-Position.long_short + 0.5)*Position.total_amount*bls_vanila_gamma(temp_param);
                  result.vega = 2 * (-Position.long_short + 0.5)*Position.total_amount*bls_vanila_vega(temp_param);
                  result.theta = 2 * (-Position.long_short + 0.5)*Position.total_amount*bls_vanila_theta(temp_param)/256;
				  break;
		}
		}
	}
	else{}

	return result;

}

double OptionValue::Price_Qoute(const string &instr_code) {
	string price;
    my_redis->HGet(main_contract.contract_code, "LastPrice", price);
	return stod(price);
}

vector<string> OptionValue::Get_Main_Contract_Codes() {
	vector<string> ret;
	ret.push_back(main_contract.contract_code);
	return ret;
}

string OptionValue::getUnderlyingCode(const string &option_code) {
    string underlying_code;
    my_redis->HGet("PARAM-OTC-IFX03", "Underlying_Code", underlying_code);
    return underlying_code;
}

inline
int OptionValue::getMultiplier(const string &instr_code) {
    /*当前只有一个合约，暂时返回固定乘数*/
    return main_contract.multiplier;
}

double OptionValue::getUnderlyingPrice(const string &instr_code) {
    string underlying_code = getUnderlyingCode(instr_code);
	string str_price;
    my_redis->HGet(underlying_code, "LastPrice", str_price);
	return stod(str_price);
}
