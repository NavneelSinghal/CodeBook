struct Point {
    ld x, y, weight;
    Point(ld x_ = 0, ld y_ = 0, ld weight_ = 1)
        : x(x_), y(y_), weight(weight_) {}
    Point& operator+=(const Point& other) {
        const auto w = weight + other.weight;
        x = (x * weight + other.x * other.weight) / w;
        y = (y * weight + other.y * other.weight) / w;
        weight = w;
        return *this;
    }
};

struct IsotonicRegression {
    vector<Point> points;
    Point centroid;

    IsotonicRegression(vector<Point> points) {
        sort(begin(points), end(points),
             [](const Point& a, const Point& b) { return a.x < b.x; });
        Point sum = accumulate(begin(points), end(points), Point(0, 0, 0),
                               [](const Point& acc, const Point& p) {
                                   return Point{acc.x + p.x * p.weight,
                                                acc.y + p.y * p.weight,
                                                acc.weight + p.weight};
                               });
        sum.x /= sum.weight, sum.y /= sum.weight;
        sum.weight = 1;
        this->points = isotonic_points(points);
        this->centroid = sum;
    }

    ld interpolate(const ld at_x) {
        if (points.size() == 1) return points[0].y;
        auto it = lower_bound(
            begin(points), end(points), Point(at_x, 0, 0),
            [](const Point& x, const Point& y) { return x.x < y.x; });
        auto ix = it - begin(points);
        if ((int)ix == (int)points.size()) {
            return interpolate_two_points(centroid, points.back(), at_x);
        } else {
            if (abs(points[ix].x - at_x) < 1e-6) {
                return points[ix].y;
            } else if (ix == 0) {
                return interpolate_two_points(points.front(), centroid, at_x);
            } else {
                return interpolate_two_points(points[ix - 1], points[ix], at_x);
            }
        }
    }

   private:
    // assume points sorted by x
    auto isotonic_points(const vector<Point>& points) -> vector<Point> {
        vector<Point> isotonic_points;
        for (auto point : points) {
            if (isotonic_points.empty() or point.y > isotonic_points.back().y) {
                isotonic_points.push_back(point);
            } else {
                while (true) {
                    if (isotonic_points.empty() or
                        isotonic_points.back().y < point.y) {
                        isotonic_points.push_back(point);
                        break;
                    } else {
                        point += isotonic_points.back();
                        isotonic_points.pop_back();
                    }
                }
            }
        }
        return isotonic_points;
    }

    ld interpolate_two_points(const Point& a, const Point& b, const ld& at_x) {
        return (b.y - a.y) * ((at_x - a.x) / (b.x - a.x)) + a.y;
    };
};
