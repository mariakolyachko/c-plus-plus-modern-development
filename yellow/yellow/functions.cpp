////
//// Created by mariakolyachko on 19.12.19.
////
//
//#include <stdexcept>
//#include "bus_manager.h"
//
//std::string getNextSubstr(int &start, int &finish,
//        const std::string &str) {
//    if (finish != std::string::npos) {
//        std::string substr = str.substr(start, finish - 1 - start + 1);
//        start = finish + 1;
//        finish = str.find('-', start);
//        return substr;
//    } else {
//        throw std::invalid_argument(str);
//    }
//}
//
//PhoneNumber::PhoneNumber(const string &international_number) {
//    if (international_number.empty() || international_number[0] != '+') {
//        throw std::invalid_argument(international_number);
//    }
//    int start = 1;
//    int finish = international_number.find('-', start);
//    country_code_ = getNextSubstr(start, finish, international_number);
//    city_code_ = getNextSubstr(start, finish, international_number);
//    local_number_ = international_number.substr(start,
//            international_number.size() - 1);
//    if (country_code_.empty() || city_code_.empty() || local_number_.empty()) {
//        throw std::invalid_argument(international_number);
//    }
//}
//
//string PhoneNumber::GetCountryCode() const {
//    return country_code_;
//}
//
//string PhoneNumber::GetCityCode() const {
//    return city_code_;
//}
//
//string PhoneNumber::GetLocalNumber() const {
//    return local_number_;
//}
//
//string PhoneNumber::GetInternationalNumber() const {
//    return "+" + country_code_ + "-" + city_code_
//        + "-" + local_number_;
//}
