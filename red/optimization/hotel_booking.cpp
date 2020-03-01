//
// Created by mariakolyachko on 24.01.20.
//

#include <iostream>
#include <map>
#include <queue>

struct RoomBooking {
  int room_count_;
  int64_t time_;
  int client_id_;

  RoomBooking(int room_count, int64_t time, int client_id)
      : room_count_(room_count), time_(time), client_id_(client_id) {}
};

class BookingManager {
public:
  void Book(int64_t time, const std::string &hotel_name, int client_id,
            int room_count) {
    current_time_ = time;
    Adjust(hotel_name);
    hotel_reservations_[hotel_name].push(
        RoomBooking(room_count, time, client_id));
    ++hotel_clients_[hotel_name][client_id];
    room_count_[hotel_name] += room_count;
  }

  size_t Clients(const std::string &hotel_name) {
    Adjust(hotel_name);
    return hotel_clients_[hotel_name].size();
  }

  int Rooms(const std::string &hotel_name) {
    Adjust(hotel_name);
    return room_count_[hotel_name];
  }

private:
  std::map<std::string, std::queue<RoomBooking>> hotel_reservations_;
  std::map<std::string, int> room_count_;
  std::map<std::string, std::map<int, int>> hotel_clients_;
  int64_t current_time_;

  static const int64_t DAY = 86400;

  void Adjust(const std::string &hotel_name) {
    while (!hotel_reservations_[hotel_name].empty() &&
           hotel_reservations_[hotel_name].front().time_ <= current_time_ - DAY) {
      auto reservation = hotel_reservations_[hotel_name].front();
      room_count_[hotel_name] -= reservation.room_count_;
      --hotel_clients_[hotel_name][reservation.client_id_];
      if (hotel_clients_[hotel_name][reservation.client_id_] <= 0) {
        hotel_clients_[hotel_name].erase(reservation.client_id_);
      }
      hotel_reservations_[hotel_name].pop();
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  BookingManager manager;

  int query_count;
  std::cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    std::string query_type;
    std::cin >> query_type;
    if (query_type == "BOOK") {
      int64_t time;
      std::string hotel_name;
      int client_id;
      int room_count;
      std::cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else {
      std::string hotel_name;
      std::cin >> hotel_name;
      if (query_type == "CLIENTS") {
        std::cout << manager.Clients(hotel_name) << '\n';
      } else if (query_type == "ROOMS") {
        std::cout << manager.Rooms(hotel_name) << '\n';
      }
    }
  }
  return 0;
}