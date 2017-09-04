#ifndef XMAHO_STD_EXT_VALMATRIX_H
#define XMAHO_STD_EXT_VALMATRIX_H
/*
BSD 2-Clause License

Copyright (c) 2017, Doi Yusuke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file std_ext/valmatrix.hpp
 * @brief The valmatrix composit valarray with dimention data.
 */

#include <cstddef>
#include <utility>
#include <valarray>

namespace xmaho
{
namespace std_ext
{

/**
 * @brief The adapter class for matrix valarray.
 *
 * valarray class are useful for multiple dimension vector.
 * But it haven't dimension values.
 * This class hold dimension value and present some API.
 *
 * @invariant size() == row_size() * col_size()
 *
 * @tparam T Value type of valarray.
 */
template<typename T>
class valmatrix : std::valarray<T>
{
public:
  //! @brief Value type of valarray.
  using value_type = typename std::valarray<T>::value_type;
  //! @brief Size type for access to values.
  using size_type = std::size_t;
  //! @brief Specific position type for two dimention.
  using position_type = std::pair<size_type, size_type>;

  /**
   * @brief Default constructor for empty valmatrix.
   *
   * @post row_size() == 0
   * @post col_size() == 0
   */
  valmatrix() = default;

  /**
   * @brief Construct by matrix size.
   *
   * Create valarray that size is row size * column size.
   *
   * @param[in] row_size Row size.
   * @param[in] col_size Column size.
   */
  valmatrix(size_type row_size, size_type col_size);

  //! @brief Default copy constructor for overload.
  valmatrix(const valmatrix&) = default;
  //! @brief Default move constructor for overload.
  valmatrix(valmatrix&&) noexcept = default;
  //! @brief Default copy assign for overload.
  valmatrix& operator=(const valmatrix&) = default;
  //! @brief Default move assign for overload.
  valmatrix& operator=(valmatrix&&) noexcept = default;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator=(const std::valarray<T>& rhs) &;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator=(std::valarray<T>&& rhs) &;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @param rhs Value.
   * @return This reference.
   */
  valmatrix& operator=(const T& rhs) &;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @param rhs Value.
   * @return This reference.
   */
  valmatrix& operator=(T&& rhs) & noexcept;

  /**
   * @brief Access by position.
   *
   * @pre position.first < ros_size()
   * @pre position.second < col_size()
   * @param position Access point.
   * @return Const reference of access point.
   */
  const T& operator[](position_type position) const;

  /**
   * @brief Access by position.
   *
   * @pre position.first < ros_size()
   * @pre position.second < col_size()
   * @param position Access point.
   * @return Reference of access point.
   */
  T& operator[](position_type position);

  /**
   * @brief Add assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator+=(const valmatrix& rhs) &;

  /**
   * @brief Add assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator+=(const std::valarray<T>& rhs) &;

  /**
   * @brief Add assign to each element in the matrix.
   *
   * @param rhs Value.
   * @return This reference.
   */
  valmatrix& operator+=(const T& rhs) &;

  /**
   * @brief Get begin iterator.
   *
   * @return Const begin iterator.
   */
  const T* begin() const noexcept;

  /**
   * @brief Get begin iterator.
   *
   * @return Begin iterator.
   */
  T* begin() noexcept;

  /**
   * @brief Get end iterator.
   *
   * @return Const end iterator.
   */
  const T* end() const noexcept;

  /**
   * @brief Get end iterator.
   *
   * @return End iterator.
   */
  T* end() noexcept;

  /**
   * @brief Swap objects.
   *
   * @param other Swap target.
   */
  void swap(valmatrix& other) noexcept;

  using std::valarray<T>::operator[];
  using std::valarray<T>::operator+;
  using std::valarray<T>::operator-;
  using std::valarray<T>::operator~;
  using std::valarray<T>::operator!;
  using std::valarray<T>::size;
  using std::valarray<T>::sum;
  using std::valarray<T>::min;
  using std::valarray<T>::max;
  using std::valarray<T>::shift;
  using std::valarray<T>::cshift;
  using std::valarray<T>::apply;

private:
  position_type size_;
};

/**
 * @brief Get begin iterator.
 *
 * @param v Target matrix.
 * @return Const begin iterator.
 */
template<typename T>
const T* begin(const valmatrix<T>& v) noexcept
{
  return v.begin();
}

/**
 * @brief Get begin iterator.
 *
 * @param v Target matrix.
 * @return Begin iterator.
 */
template<typename T>
T* begin(valmatrix<T>& v) noexcept
{
  return v.begin();
}

/**
 * @brief Get end iterator.
 *
 * @param v Target matrix.
 * @return Const end iterator.
 */
template<typename T>
const T* end(const valmatrix<T>& v) noexcept
{
  return v.end();
}

/**
 * @brief Get end iterator.
 *
 * @param v Target matrix.
 * @return End iterator.
 */
template<typename T>
T* end(valmatrix<T>& v) noexcept
{
  return v.end();
}

/**
 * @brief Swap objects.
 *
 * @param a Swap target.
 * @param b Swap target.
 */
template<typename T>
void swap(valmatrix<T>& a, valmatrix<T>& b) noexcept
{
  a.swap(b);
}

}
}

#include "detail/valmatrix.hpp"

#endif
