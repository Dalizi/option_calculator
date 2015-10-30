#ifndef POSITION_H
#define POSITION_H

#include <vector>

#include "databaseoperation.h"

class Position : public DatabaseOperation
{
    friend Position &operator<<(Position&, const QSqlQuery&);
    enum class PositionDirection{
        LONG_POSITION, SHORT_POSITION
    };
public:

    Position() = default;
    Position(int client_id, double average_price,
             int total_amount, int available_amount,
             int frozen_amount, double occupied_margin,
             double underlying_price, double knockout_price,
             const QString &underlying_code, const QString &contract_num,
             const QString &instr_code, const QString &long_short);
    ~Position() = default;

    virtual void Add() override;
    virtual void Modify() override;
    virtual void Remove() override;
    virtual void IsExist() override;

private:
    static const QString table_name_;
    static const std::vector<QString> keys_;

    int client_id_ = -1;	//客户编号
    double average_price_ = 0;	//平均价格
    int total_amount_ = 0;	//总量
    int available_amount_ = 0; //可用数量
    int frozen_amount_ = 0; //冻结数量
    double occupied_margin_ = 0;
    double underlying_price_ = 0;
    double knockout_price_ = 0;
    QString underlying_code_;
    QString contract_num_;
    QString instr_code_ = ""; //合约代码
    PositionDirection long_short_ = PositionDirection::LONG_POSITION; //买卖方向
};

Position &operator<<(Position& position, const QSqlQuery& query);

#endif // POSITION_H
