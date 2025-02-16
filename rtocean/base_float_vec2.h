#ifndef  BASE_FLOAT_VEC2_H
#define  BASE_FLOAT_VEC2_H

/****************************************************************
 * Include files
 ****************************************************************/

#include <stdexcept>
//using std::exception;
using std::domain_error;
using std::out_of_range;
//using std::invalid_argument;

/* Own includes */
#include "math_functions.h"
#include "base_int_vec2.h"

/****************************************************************
 * Class definition
 ****************************************************************/

template<typename T>
struct base_float_vec2
{
public:
    T e[2];

    /* Constructors */
    base_float_vec2<T>();                 // Default constructor
    base_float_vec2<T>(const ivec2& rhs); // Conversion constructor
    base_float_vec2<T>(T e0, T e1);       // Other constructor

    base_float_vec2<T>& operator =(const ivec2&); // Conversion
    base_float_vec2<T>& operator+=(const base_float_vec2<T>&);
    base_float_vec2<T>& operator-=(const base_float_vec2<T>&);
    //base_float_vec2<T>& operator&=(const base_float_vec2<T>&); // Yet to define
    base_float_vec2<T>& operator*=(const T                  );
    base_float_vec2<T>& operator/=(const T                  );
    T&                  operator[](const int)      ;
    const T&            operator[](const int) const;

    base_float_vec2<T> operator-() const;

    base_float_vec2<T> operator+(const base_float_vec2<T>&) const;
    base_float_vec2<T> operator-(const base_float_vec2<T>&) const;
    T                  operator*(const base_float_vec2<T>&) const; // Scalar product
    //base_float_vec2<T> operator&(const base_float_vec2<T>&) const; // Crossproduct
    base_float_vec2<T> operator*(const T                  ) const;
    base_float_vec2<T> operator/(const T                  ) const;

    bool              operator==(const base_float_vec2<T>&) const;

    T                  length                              () const;
    T                  sqr_length                          () const;
    void               normalize                           ()      ;
    base_float_vec2<T> normalized                          () const;
    base_float_vec2<T> rotated_clockwise                   () const;
    base_float_vec2<T> rotated_counterclockwise            () const;
    base_float_vec2<T> random_equal_lenth_orthogonal_vector() const;
    base_float_vec2<T> random_normalized_orthogonal_vector () const;
};

//typedef  base_float_vec2<float >  fvec2;
//typedef  base_float_vec2<double>  dvec2;

/****************************************************************
 * Class related functions
 ****************************************************************/

template<typename T, typename T2>
base_float_vec2<T> operator* (const T2&, const base_float_vec2<T>&);

/****************************************************************
 * Public functions
 ****************************************************************/

// Default constructor
template<typename T>
inline
base_float_vec2<T>::base_float_vec2()
{
    e[0] = e[1] = 0.0;
}

template<typename T>
inline
base_float_vec2<T>::base_float_vec2(const ivec2& source)
{
    e[0] = T(source.e[0]);
    e[1] = T(source.e[1]);
}

template<typename T>
inline
base_float_vec2<T>::base_float_vec2(T e0, T e1)
{
    e[0] = e0;
    e[1] = e1;
}

template<typename T>
inline
base_float_vec2<T>& base_float_vec2<T>::operator=(const ivec2& rhs)
{
    for (int i = 0; i < 2; i++) e[i] = T(rhs.e[i]);
    return *this;
}

template<typename T>
inline
base_float_vec2<T>& base_float_vec2<T>::operator+=(const base_float_vec2<T>& rhs)
{
    for (int i = 0; i < 2; i++) e[i] += rhs.e[i];
    return *this;
}

template<typename T>
inline
base_float_vec2<T>& base_float_vec2<T>::operator-=(const base_float_vec2<T>& rhs)
{
    for (int i = 0; i < 2; i++) e[i] -= rhs.e[i];
    return *this;
}

template<typename T>
inline
base_float_vec2<T>& base_float_vec2<T>::operator*=(const T k)
{
    for (int i = 0; i < 2; i++) e[i] *= k;
    return *this;
}

template<typename T>
inline
base_float_vec2<T>& base_float_vec2<T>::operator/=(const T den)
{
#if  DEBUG
    if (!den) throw domain_error("Trying to divide a vector by zero");
#endif
    *this *= 1/den;
    return *this;
}

template<typename T>
inline
T& base_float_vec2<T>::operator[](const int i)
{
#if  DEBUG
    if (i < 0) {
        throw out_of_range("Trying to access a base_float_vec2 element with negative index");
    }
    else if (i >= 2) {
        throw out_of_range("Trying to access a base_float_vec2 element with to high index");
    }
#endif
    return e[i];
}

template<typename T>
inline
const T& base_float_vec2<T>::operator[](const int i) const
{
#if  DEBUG
    if (i < 0) {
        throw out_of_range("Trying to access a base_float_vec2 element with negative index");
    }
    else if (i >= 2) {
        throw out_of_range("Trying to access a base_float_vec2 element with to high index");
    }
#endif
    return e[i];
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::operator-() const
{
    return base_float_vec2<T>(-e[0],
                              -e[1]);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::operator+(const base_float_vec2<T>& rhs) const
{
    return base_float_vec2<T>(e[0] + rhs.e[0],
                              e[1] + rhs.e[1]);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::operator-(const base_float_vec2<T>& rhs) const
{
    return base_float_vec2<T>(e[0] - rhs.e[0],
                              e[1] - rhs.e[1]);
}

template<typename T>
inline
T base_float_vec2<T>::operator*(const base_float_vec2<T>& rhs) const
{
    return e[0]*rhs.e[0] + e[1]*rhs.e[1];
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::operator*(const T k) const
{
    return base_float_vec2<T>(e[0]*k, e[1]*k);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::operator/(const T den) const
{
#if  DEBUG
    if(!den) throw domain_error("Not defined to divide with zero");
#endif
    T k = 1/den;
    return base_float_vec2<T>(e[0]*k, e[1]*k);
}

template<typename T>
inline
bool base_float_vec2<T>::operator==(const base_float_vec2<T>& rhs) const
{
    return e[0] == rhs[0] && e[1] == rhs[1];
}

template<typename T>
inline
T base_float_vec2<T>::length() const
{
    return sqrt(sqr_length());
}

template<typename T>
inline
T base_float_vec2<T>::sqr_length() const
{
    return (e[0]*e[0] + e[1]*e[1]);
}

template<typename T>
inline
void base_float_vec2<T>::normalize()
{
    T len = length();
#if  DEBUG
    if (!len) throw domain_error("Trying to normalize a zero-length base_float_vec2<T>");
#endif
    T k = 1/len;
    for (int i = 0; i < 2; i++) e[i] *= k;
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::normalized() const
{
    T len = length();
#if  DEBUG
    if (!len) throw domain_error("Trying to normalize a zero-length base_float_vec2<T>");
#endif
    T k = 1/len;
    return base_float_vec2<T>(e[0]*k, e[1]*k);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::rotated_clockwise() const
{
    return base_float_vec2<T>(e[1], -e[0]);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::rotated_counterclockwise() const
{
    return base_float_vec2<T>(-e[1], e[0]);
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::random_equal_lenth_orthogonal_vector() const
{
    if (rand() & 1) {
        // Rotate counterclockwise
        return rotated_clockwise();
    }
    else {
        return rotated_counterclockwise();
    }
}

template<typename T>
inline
base_float_vec2<T> base_float_vec2<T>::random_normalized_orthogonal_vector() const
{
    return random_equal_lenth_orthogonal_vector().normalized();
}

/****************************************************************
 * Class related functions
 ****************************************************************/

template<typename T, typename T2>
inline
base_float_vec2<T> operator*(const T2& lhs, const base_float_vec2<T>& rhs)
{
    return base_float_vec2<T>(T(lhs)*rhs.e[0], T(lhs)*rhs.e[1]);
}

#endif  /* BASE_FLOAT_VEC2_H */
