#include "position.h"

const QString Position::table_name_ = "positions";
const std::vector<QString> Position::keys_ = {"client_id", "instr_code",
                                              "average_price", "total_amount",
                                              "available_amount", "frozen_amount",
                                              "long_short", "underlying_price",
                                              "occupied_margin", "underlying_code",
                                              "knockout_price", "contract_no"};

Position::Position(int client_id, double average_price,
         int total_amount, int available_amount,
         int frozen_amount, double occupied_margin,
         double underlying_price, double knockout_price,
         const QString &underlying_code, const QString &contract_num,
         const QString &instr_code, const QString &long_short):
    client_id_(client_id), average_price_(average_price),
    total_amount_(total_amount), available_amount_(available_amount),
    frozen_amount_(frozen_amount), occupied_margin_(occupied_margin),
    underlying_price_(underlying_price), knockout_price_(knockout_price),
    underlying_code_(underlying_code), contract_num_(contract_num),
    instr_code_(instr_code)
{
    long_short_ = long_short == "long"?PositionDirection::LONG_POSITION:PositionDirection::SHORT_POSITION;
}

Position &operator<<(Position& position, const QSqlQuery& query) {

}
