#include <vector>
#include <cstddef>
#include <iostream>

namespace ik {
  struct vec3 {
    double x;
    double y;
    double z;
  };

  vec3 operator+(const vec3& v1, const vec3& v2);
  vec3 operator-(const vec3& v1, const vec3& v2);
  vec3 operator-(const vec3& v);
  vec3 operator*(double c, const vec3& v);

  
  struct joint {
    vec3 pos;
  };

  
  class chain {
    std::vector<joint> joints;
    std::vector<double> d;

    void calculate_distances();

  public:
    chain(const chain& c);
    chain(std::vector<joint>&& initial_positions);

    friend std::ostream& operator<<(std::ostream&, const chain&);
    const joint& operator[](size_t i) const;
    
    size_t size() const;
    double length() const;

    const joint& get_joint(size_t i) const;
    void set_joint(size_t i, const joint& j);

    double get_distance(size_t i) const;
  };


  struct FABRIK {
    chain c;
    joint target;

    FABRIK(const chain& start, const joint& end);

    void iterate();
  };
}


