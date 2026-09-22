class pyramid {
private:
    double square_side;
    double height;

public:
    pyramid(double input_square_side, double input_height);

    void set_side(double input_side);
    void set_height(double input_height);

    double get_side() const;
    double get_height() const;

    double get_base_area() const;
    double get_volume() const;
};