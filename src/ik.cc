#include "ik.h"
#include <cmath>

namespace ik {

  vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3{v1.x+v2.x, v1.y+v2.y, v1.z + v2.z};
  }
  vec3 operator-(const vec3& v1, const vec3& v2) {
    return v1+(-v2);
  }
  vec3 operator-(const vec3& v) {
    return vec3{-v.x, -v.y, -v.z};
  }
  vec3 operator*(double c, const vec3& v) {
    return vec3{c*v.x, c*v.y, c*v.z};
  }

  double dist(const vec3& v1, const vec3& v2) {
    const vec3 diff = v1-v2;
    return sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
  }
  

  chain::chain(const chain& c)
    : joints(c.joints), d(c.d) { }

  chain::chain(std::vector<ik::joint>&& initial_positions)
    : joints{initial_positions} {
    this->calculate_distances();
  }

  std::ostream& operator<<(std::ostream& s, const chain& c) {
    s << "{ ";
    for(size_t i = 0; i < c.size(); i++) {
      const vec3& p = c.joints[i].pos;
      s << "(" << p.x << ", " << p.y << ", " << p.z << "), ";
    }
    s << " }";

    return s;
  }

  const joint& chain::operator[](size_t i) const {
    return this->joints[i];
  }

  void chain::calculate_distances() {
    this->d = std::vector<double>(this->joints.size()-1);
    for(size_t i = 0; i < this->d.size(); i++) {
      this->d[i] = dist(this->joints[i].pos, this->joints[i+1].pos);
    }
  }

  size_t chain::size() const {
    return this->joints.size();
  }
  
  double chain::length() const {
    // for right now we will do the inefficient task of repeatedly
    // recalculating length instead of storing it.
    double length = 0;

    for(size_t i = 0; i < this->d.size(); i++) {
      length += this->d[i];
    }

    return length;
  }

  const joint& chain::get_joint(size_t i) const {
    return this->joints[i];
  }

  void chain::set_joint(size_t i, const joint& j) {
    this->joints[i] = j;

    // we need to recalculate the distances, but in order to do this we need to
    // know if we are at one of the endpoints, to not go out of bounds.
    if(i == 0) {
      if(this->joints.size() > 1) {
        this->d[0] = dist(this->joints[0].pos, this->joints[1].pos);
      }
    }
    else if(i == this->joints.size()-1) {
      if(this->joints.size() > 1) {
        this->d[i-1] = dist(this->joints[i-1].pos, this->joints[i].pos);
      }
    }
    else {
      // we now know that there are at least three elements and that we are at
      // a position between them from the other boundary checks, so no need for
      // any other conditionals.
      this->d[i-1] = dist(this->joints[i-1].pos, this->joints[i].pos);
      this->d[i] = dist(this->joints[i].pos, this->joints[i+1].pos);
    }
  }

  double chain::get_distance(size_t i) const {
    return this->d[i];
  }


  FABRIK::FABRIK(const chain& start, const joint& target)
    : c(start), target(target) {  }

  void FABRIK::iterate() {
    bool reachable = dist(this->c[0].pos, this->target.pos) <= this->c.length();

    if(reachable) {
    
    }
    else {
      joint t = this->target;
      for(size_t i = 0; i < this->c.size()-1; i++) {
        double r = dist(t.pos, this->c.get_joint(i).pos);
        double m = r/this->c.get_distance(i);

        this->c.set_joint(i+1, joint{(1 - m)*this->c.get_joint(i).pos + m*t.pos});
        t = this->c.get_joint(i); // not the greatest since this is a copy, and
                                  // really only needs to be a pointer/ref but
                                  // wtv for now ...
      }
    }
  }
}
