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
   * @note If either value is 0, set 0 to both values.
   *
   * @param[in] row_size Row size.
   * @param[in] col_size Column size.
   */
  valmatrix(size_type row_size, size_type col_size);

  /**
   * @brief Constrcut by matrix size with default value.
   *
   * Create valarray with default value with matrix size.
   *
   * @note If either value is 0, set 0 to both values.
   *
   * @pre row_size * col_size == values.size()
   * @param[in] value Default value.
   * @param[in] row_size Row size.
   * @param[in] col_size Column size.
   */
  valmatrix(const T& value, size_type row_size, size_type col_size);

  /**
   * @brief Construct by matrix size with default values.
   *
   * Create valarray with default values.
   *
   * @note If either value is 0, set 0 to both values.
   *
   * @pre row_size * col_size == values.size()
   * @param[in] values Default values.
   * @param[in] row_size Row size.
   * @param[in] col_size Column size.
   */
  valmatrix(const std::valarray<T>& values, size_type row_size, size_type col_size);

  /**
   * @brief Construct by matrix size with default values.
   *
   * Create valarray with default values.
   *
   * @note If either value is 0, set 0 to both values.
   *
   * @pre row_size * col_size == values.size()
   * @param[in] values Default values.
   * @param[in] row_size Row size.
   * @param[in] col_size Column size.
   */
  valmatrix(std::valarray<T>&& values, size_type row_size, size_type col_size);

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
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator=(const std::valarray<T>& rhs) &;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator=(std::valarray<T>&& rhs) &;

  /**
   * @brief Assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator=(const T& rhs) &;

  /**
   * @brief Access by position.
   *
   * @pre position.first < ros_size()
   * @pre position.second < col_size()
   * @param[in] position Access point.
   * @return Const reference of access point.
   */
  const T& operator[](position_type position) const;

  /**
   * @brief Access by position.
   *
   * @pre position.first < ros_size()
   * @pre position.second < col_size()
   * @param[in] position Access point.
   * @return Reference of access point.
   */
  T& operator[](position_type position);

  /**
   * @brief Apply + operator to each elements in the matrix.
   */
  valmatrix operator+() const noexcept;

  /**
   * @brief Apply - operator to each elements in the matrix.
   */
  valmatrix operator-() const noexcept;

  /**
   * @brief Apply ~ operator to each elements in the matrix.
   */
  valmatrix operator~() const noexcept;

  /**
   * @brief Addition assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator+=(const valmatrix& rhs) &;

  /**
   * @brief Addition assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator+=(const std::valarray<T>& rhs) &;

  /**
   * @brief Addition assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator+=(const T& rhs) &;

  /**
   * @brief Subtraction assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator-=(const valmatrix& rhs) &;

  /**
   * @brief Subtraction assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator-=(const std::valarray<T>& rhs) &;

  /**
   * @brief Subtraction assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator-=(const T& rhs) &;

  /**
   * @brief Multiplication assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator*=(const valmatrix& rhs) &;

  /**
   * @brief Multiplication assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator*=(const std::valarray<T>& rhs) &;

  /**
   * @brief Multiplication assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator*=(const T& rhs) &;

  /**
   * @brief Divition assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator/=(const valmatrix& rhs) &;

  /**
   * @brief Divition assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator/=(const std::valarray<T>& rhs) &;

  /**
   * @brief Divition assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator/=(const T& rhs) &;

  /**
   * @brief Residue assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator%=(const valmatrix& rhs) &;

  /**
   * @brief Residue assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator%=(const std::valarray<T>& rhs) &;

  /**
   * @brief Residue assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator%=(const T& rhs) &;

  /**
   * @brief Bitwise and assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator&=(const valmatrix& rhs) &;

  /**
   * @brief Bitwise and assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator&=(const std::valarray<T>& rhs) &;

  /**
   * @brief Bitwise and assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator&=(const T& rhs) &;

  /**
   * @brief Bitwise or assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator|=(const valmatrix& rhs) &;

  /**
   * @brief Bitwise or assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator|=(const std::valarray<T>& rhs) &;

  /**
   * @brief Bitwise or assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator|=(const T& rhs) &;

  /**
   * @brief Xor assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator^=(const valmatrix& rhs) &;

  /**
   * @brief Xor assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator^=(const std::valarray<T>& rhs) &;

  /**
   * @brief Xor assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator^=(const T& rhs) &;

  /**
   * @brief Shift assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator<<=(const valmatrix& rhs) &;

  /**
   * @brief Shift assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator<<=(const std::valarray<T>& rhs) &;

  /**
   * @brief Shift assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator<<=(const T& rhs) &;

  /**
   * @brief Counter shift assign to each element in the matrix.
   *
   * @pre row_size() == rhs.row_size()
   * @pre col_size() == rhs.col_size()
   * @param[in] rhs Same dimention matrix.
   * @return This reference.
   */
  valmatrix& operator>>=(const valmatrix& rhs) &;

  /**
   * @brief Counter shift assign to each element in the matrix.
   *
   * @pre size() == rhs.size()
   * @param[in] rhs Same size array.
   * @return This reference.
   */
  valmatrix& operator>>=(const std::valarray<T>& rhs) &;

  /**
   * @brief Counter shift assign to each element in the matrix.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  valmatrix& operator>>=(const T& rhs) &;

  /**
   * @brief Get row size.
   *
   * @return Count of column.
   */
  std::size_t row_size() const noexcept;

  /**
   * @brief Get column size.
   *
   * @return Count of row.
   */
  std::size_t col_size() const noexcept;

  /**
   * @brief Get copy of row.
   *
   * @pre index < col_size()
   * @param[in] index Row index.
   * @return Copy of row array.
   */
  std::valarray<T> row(std::size_t index) const;

  /**
   * @brief Get access to row.
   *
   * @pre index < col_size()
   * @param[in] index Row index.
   * @return Row array reference.
   */
  std::slice_array<T> row(std::size_t index);

  /**
   * @brief Get copy of column.
   *
   * @pre index < row_size()
   * @param[in] index Column index.
   * @return Copy of column array.
   */
  std::valarray<T> col(std::size_t index) const;

  /**
   * @brief Get access to column.
   *
   * @pre index < row_size()
   * @param[in] index Column index.
   * @return Column array reference.
   */
  std::slice_array<T> col(std::size_t index);

  /**
   * @brief Get copy of block.
   *
   * @pre index.first + size.first < row_size()
   * @pre index.second + size.second < col_size()
   * @param[in] pos Block's top left index.
   * @param[in] size Block's size.
   * @return Copy of block.
   */
  valmatrix block(position_type pos, position_type size) const;

  /*
   * @brief Get access to block.
   *
   * @note This function's return type are no-stable.
   *       It must have demention data.
   *       I will maybe change return type.
   *
   * @pre index.first + size.first < row_size()
   * @pre index.second + size.second < col_size()
   * @param[in] pos Block's top left index.
   * @param[in] size Block's size.
   * @return Block reference.
   */
  //std::gslice_array<T> block(position_type pos, position_type size);

  /**
   * @brief Get begin iterator.
   *
   * @return Const begin iterator.
   */
  const auto begin() const noexcept;

  /**
   * @brief Get begin iterator.
   *
   * @return Begin iterator.
   */
  auto begin() noexcept;

  /**
   * @brief Get end iterator.
   *
   * @return Const end iterator.
   */
  const auto end() const noexcept;

  /**
   * @brief Get end iterator.
   *
   * @return End iterator.
   */
  auto end() noexcept;

  /**
   * @brief Swap objects.
   *
   * @param[in,out] other Swap target.
   */
  void swap(valmatrix& other) noexcept;

  using std::valarray<T>::operator[];
  using std::valarray<T>::operator!;
  using std::valarray<T>::size;
  using std::valarray<T>::sum;
  using std::valarray<T>::min;
  using std::valarray<T>::max;

private:
  position_type size_;
};

/**
 * @brief Addition operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of addition.
 */
template<typename T>
valmatrix<T> operator+(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Addition operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of addition.
 */
template<typename T>
valmatrix<T> operator+(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Addition operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of addition.
 */
template<typename T>
valmatrix<T> operator+(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Subtraction operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of subtraction.
 */
template<typename T>
valmatrix<T> operator-(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Subtraction operator for valmatrix with valarray.
 *
 * @pre lsh.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of subtraction.
 */
template<typename T>
valmatrix<T> operator-(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Subtraction operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of subtraction.
 */
template<typename T>
valmatrix<T> operator-(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Multiplication operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of multiplication.
 */
template<typename T>
valmatrix<T> operator*(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Multiplication operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of multiplication.
 */
template<typename T>
valmatrix<T> operator*(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Multiplication operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of multiplication.
 */
template<typename T>
valmatrix<T> operator*(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Divition operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of divition.
 */
template<typename T>
valmatrix<T> operator/(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Divition operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of divition.
 */
template<typename T>
valmatrix<T> operator/(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Divition operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of divition.
 */
template<typename T>
valmatrix<T> operator/(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Residue operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of residue.
 */
template<typename T>
valmatrix<T> operator%(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Residue operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of residue.
 */
template<typename T>
valmatrix<T> operator%(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Residue operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of residue.
 */
template<typename T>
valmatrix<T> operator%(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Bitwise and operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of bitwise and.
 */
template<typename T>
valmatrix<T> operator&(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Bitwise and operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of bitwise and.
 */
template<typename T>
valmatrix<T> operator&(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Bitwise and operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of bitwise and.
 */
template<typename T>
valmatrix<T> operator&(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Bitwise or operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of bitwise or.
 */
template<typename T>
valmatrix<T> operator|(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Bitwise or operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of bitwise or.
 */
template<typename T>
valmatrix<T> operator|(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Bitwise or operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of bitwise or.
 */
template<typename T>
valmatrix<T> operator|(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Xor operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of xor.
 */
template<typename T>
valmatrix<T> operator^(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Xor operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of xor.
 */
template<typename T>
valmatrix<T> operator^(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Xor operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of xor.
 */
template<typename T>
valmatrix<T> operator^(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Shift operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of shift.
 */
template<typename T>
valmatrix<T> operator<<(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Shift operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of shift.
 */
template<typename T>
valmatrix<T> operator<<(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Shift operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of shift.
 */
template<typename T>
valmatrix<T> operator<<(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Counter shift operator for valmatrix.
 *
 * @pre lhs.row_size() == rhs.row_size()
 * @pre lhs.col_size() == rhs.col_size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value.
 * @return Result of counter shift.
 */
template<typename T>
valmatrix<T> operator>>(valmatrix<T> lhs, const valmatrix<T>& rhs);

/**
 * @brief Counter shift operator for valmatrix with valarray.
 *
 * @pre lhs.size() == rhs.size()
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is valarray.
 * @return Result of counter shift.
 */
template<typename T>
valmatrix<T> operator>>(valmatrix<T> lhs, const std::valarray<T>& rhs);

/**
 * @brief Counter shift operator for valmatrix with value.
 *
 * @param[in] lhs Left hand side value.
 * @param[in] rhs Right hand side value that is value.
 * @return Result of counter shift.
 */
template<typename T>
valmatrix<T> operator>>(valmatrix<T> lhs, const T& rhs);

/**
 * @brief Get begin iterator.
 *
 * @param[in] v Target matrix.
 * @return Const begin iterator.
 */
template<typename T>
const auto begin(const valmatrix<T>& v) noexcept;

/**
 * @brief Get begin iterator.
 *
 * @param[in] v Target matrix.
 * @return Begin iterator.
 */
template<typename T>
auto begin(valmatrix<T>& v) noexcept;

/**
 * @brief Get end iterator.
 *
 * @param[in] v Target matrix.
 * @return Const end iterator.
 */
template<typename T>
const auto end(const valmatrix<T>& v) noexcept;

/**
 * @brief Get end iterator.
 *
 * @param[in] v Target matrix.
 * @return End iterator.
 */
template<typename T>
auto end(valmatrix<T>& v) noexcept;

/**
 * @brief Swap objects.
 *
 * @param[in,out] a Swap target.
 * @param[in,out] b Swap target.
 */
template<typename T>
void swap(valmatrix<T>& a, valmatrix<T>& b) noexcept;

}
}

#include "detail/valmatrix.hpp"

#endif
