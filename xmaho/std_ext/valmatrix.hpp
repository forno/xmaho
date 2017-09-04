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

}
}

#include "detail/valmatrix.hpp"

#endif
