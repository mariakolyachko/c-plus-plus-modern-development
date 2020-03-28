//
// Created by mariakolyachko on 16.03.20.
//

#ifndef SECOND_GAME_OBJECT_H
#define SECOND_GAME_OBJECT_H

class Unit;
class Building;
class Tower;
class Fence;

struct GameObject {
  virtual ~GameObject() = default;

  [[nodiscard]] virtual bool Collide(const GameObject& that) const = 0;
  [[nodiscard]] virtual bool CollideWith(const Unit& that) const = 0;
  [[nodiscard]] virtual bool CollideWith(const Building& that) const = 0;
  [[nodiscard]] virtual bool CollideWith(const Tower& that) const = 0;
  [[nodiscard]] virtual bool CollideWith(const Fence& that) const = 0;
};

bool Collide(const GameObject& first, const GameObject& second);

#endif // SECOND_GAME_OBJECT_H
