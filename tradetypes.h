#ifndef TRADETYPES_H
#define TRADETYPES_H

#include <QString>
#include <QDateTime>
enum LongShortType {
    LONG_ORDER = 0,
    SHORT_ORDER = 1
};

enum OpenOffsetType {
    OPEN = 0,
    OFFSET = 1
};

struct TransactionType {

    QString instr_code = ""; //合约代码
    QString transaction_id = "";	//成交编号
    QDateTime time;	//成交时间
    int client_id = 0;	//客户编号
    double price = 0;	//成交价格
    int amount = 0;	//成交数量
    LongShortType long_short = LONG_ORDER;	//买卖方向
    OpenOffsetType open_offset = OPEN;	//开平方向
    double underlying_price = 0; //标的价格
    QString underlying_code; //标的
    double close_pnl = 0; //平仓盈亏

};

struct PositionType {
    int client_id = -1;	//客户编号
    QString instr_code = ""; //合约代码
    double average_price = 0;	//平均价格
    int total_amount = 0;	//总量
    int available_amount = 0; //可用数量
    int frozen_amount = 0; //冻结数量
    double occupied_margin = 0;
    double underlying_price = 0;
    QString underlying_code;
    QString contractNo;
    LongShortType long_short = LONG_ORDER; //买卖方向
};
#endif // TRADETYPES_H
