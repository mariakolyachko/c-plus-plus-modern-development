#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

class Figure {
public:
  explicit Figure(const std::string &name) : _name(name) {}
  [[nodiscard]] virtual std::string Name() const = 0;
  [[nodiscard]] virtual double Area() const = 0;
  [[nodiscard]] virtual double Perimeter() const = 0;

  const std::string _name;
};

class Rect : public Figure {
public:
  explicit Rect(double width, double height)
      : Figure("RECT"), _width(width), _height(height) {}

  [[nodiscard]] std::string Name() const override { return _name; }

  [[nodiscard]] double Perimeter() const override {
    return 2 * (_width + _height);
  }

  [[nodiscard]] double Area() const override { return _width * _height; }

  const double _width;
  const double _height;
};

class Triangle : public Figure {
public:
  explicit Triangle(double a, double b, double c)
      : Figure("TRIANGLE"), _a(a), _b(b), _c(c) {}

  [[nodiscard]] std::string Name() const override { return _name; }

  [[nodiscard]] double Perimeter() const override {
    return _a + _b + _c;
  }

  [[nodiscard]] double Area() const override {
    double p = Perimeter() / 2;
    return std::sqrt(p * (p - _a) * (p - _b) * (p - _c));
  }

  const double _a;
  const double _b;
  const double _c;
};

class Circle : public Figure {
public:
  explicit Circle(double r) : Figure("CIRCLE"), _r(r) {}

  [[nodiscard]] std::string Name() const override { return _name; }

  [[nodiscard]] double Perimeter() const override {
    return 2 * 3.14 * _r;
  }

  [[nodiscard]] double Area() const override {
    return 3.14 * _r * _r;
  }

  const double _r;
};


std::shared_ptr<Figure> CreateFigure(std::istringstream &is) {
  std::string type;
  std::shared_ptr<Figure> ptr;
  is >> type;
  if (type == "RECT") {
    double width, height;
    is >> width >> height;
    ptr = std::make_shared<Rect>(width, height);
  } else if (type == "TRIANGLE") {
    double a, b, c;
    is >> a >> b >> c;
    ptr = std::make_shared<Triangle>(a, b, c);
  } else if (type == "CIRCLE") {
    double r;
    is >> r;
    ptr = std::make_shared<Circle>(r);
  }
  return ptr;
}

int main() {
  std::vector<std::shared_ptr<Figure>> figures;
  for (std::string line; getline(std::cin, line);) {
    std::istringstream is(line);

    std::string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto &current_figure : figures) {
        std::cout << std::fixed << std::setprecision(3)
                  << current_figure->Name() << " "
                  << current_figure->Perimeter() << " "
                  << current_figure->Area() << std::endl;
      }
    }
  }
  return 0;
}