// friedmud/variadic_table is licensed under the GNU Lesser General Public License v2.1
// Referenced : https://github.com/friedmud/variadic_table
// Modified by Julien Thibodeau

#ifndef VARIADICTABLE_H
#define VARIADICTABLE_H

#include <iomanip>
#include <ios>
#include <tuple>
#include <type_traits>
#include <cassert>
#include <cmath>

enum class VariadicTableColumnFormat
{
    AUTO,
    SCIENTIFIC,
    FIXED,
    PERCENT
};

template <class... Ts>
class VariadicTable
{
public:
    typedef std::tuple<Ts...> DataTuple;

    VariadicTable(std::vector<std::string> headers,
                  unsigned int static_column_size = 0,
                  unsigned int cell_padding = 1)
        : _headers(headers),
          _num_columns(std::tuple_size<DataTuple>::value),
          _static_column_size(static_column_size),
          _cell_padding(cell_padding)
    {
        assert(headers.size() == _num_columns);
    }

    void addRow(std::tuple<Ts...> data) { _data.push_back(data); }

    template <typename StreamType>
    void print(StreamType &stream)
    {
        size_columns();

        unsigned int total_width = _num_columns + 1;

        for (auto &col_size : _column_sizes)
        {
            total_width += col_size + (2 * _cell_padding);
        }

        stream << std::string(total_width, '-') << std::endl;

        stream << "|";
        for (unsigned int i = 0; i < _num_columns; i++)
        {
            auto half = _column_sizes[i] / 2;
            half -= _headers[i].size() / 2;

            stream << std::string(_cell_padding, ' ') << std::setw(_column_sizes[i]) << std::left
                   << std::string(half, ' ') + _headers[i] << std::string(_cell_padding, ' ') << "|";
        }

        stream << std::endl;

        stream << std::string(total_width, '-') << std::endl;

        for (auto &row : _data)
        {
            stream << "|";
            print_each(row, stream);
            stream << std::endl;
        }

        stream << std::string(total_width, '-') << std::endl;
    }

    void setColumnFormat(const std::vector<VariadicTableColumnFormat> &column_format)
    {
        assert(column_format.size() == std::tuple_size<DataTuple>::value);

        _column_format = column_format;
    }

    void setColumnPrecision(const std::vector<int> &precision)
    {
        assert(precision.size() == std::tuple_size<DataTuple>::value);
        _precision = precision;
    }

protected:
    typedef decltype(&std::right) right_type;
    typedef decltype(&std::left) left_type;

    template <typename T,
              typename = typename std::enable_if<
                  std::is_arithmetic<typename std::remove_reference<T>::type>::value>::type>
    static right_type justify(int)
    {
        return std::right;
    }

    template <typename T>
    static left_type justify(long)
    {
        return std::left;
    }

    template <typename TupleType, typename StreamType>
    void print_each(TupleType &&,
                    StreamType &,
                    std::integral_constant<
                        size_t,
                        std::tuple_size<typename std::remove_reference<TupleType>::type>::value>)
    {
    }

    template <std::size_t I,
              typename TupleType,
              typename StreamType,
              typename = typename std::enable_if<
                  I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type>
    void print_each(TupleType &&t, StreamType &stream, std::integral_constant<size_t, I>)
    {
        auto &val = std::get<I>(t);

        if (!_precision.empty())
        {
            assert(_precision.size() ==
                   std::tuple_size<typename std::remove_reference<TupleType>::type>::value);

            stream << std::setprecision(_precision[I]);
        }

        if (!_column_format.empty())
        {
            assert(_column_format.size() ==
                   std::tuple_size<typename std::remove_reference<TupleType>::type>::value);

            if (_column_format[I] == VariadicTableColumnFormat::SCIENTIFIC)
            {
                stream << std::scientific;
            }

            if (_column_format[I] == VariadicTableColumnFormat::FIXED)
            {
                stream << std::fixed;
            }

            if (_column_format[I] == VariadicTableColumnFormat::PERCENT)
            {
                stream << std::fixed << std::setprecision(2);
            }
        }

        stream << std::string(_cell_padding, ' ') << std::setw(_column_sizes[I])
               << justify<decltype(val)>(0) << val << std::string(_cell_padding, ' ') << "|";

        if (!_column_format.empty())
        {
            stream.unsetf(std::ios_base::floatfield);
        }

        print_each(std::forward<TupleType>(t), stream, std::integral_constant<size_t, I + 1>());
    }

    template <typename TupleType, typename StreamType>
    void print_each(TupleType &&t, StreamType &stream)
    {
        print_each(std::forward<TupleType>(t), stream, std::integral_constant<size_t, 0>());
    }

    template <class T>
    size_t sizeOfData(const T &data, decltype(((T *)nullptr)->size()) * = nullptr)
    {
        return data.size();
    }

    template <class T>
    size_t sizeOfData(const T &data,
                      typename std::enable_if<std::is_integral<T>::value>::type * = nullptr)
    {
        if (data == 0)
        {
            return 1;
        }

        return std::log10(data) + 1;
    }

    size_t sizeOfData(...) { return _static_column_size; }

    template <typename TupleType>
    void size_each(TupleType &&,
                   std::vector<unsigned int> &,
                   std::integral_constant<
                       size_t,
                       std::tuple_size<typename std::remove_reference<TupleType>::type>::value>)
    {
    }

    template <std::size_t I,
              typename TupleType,
              typename = typename std::enable_if<
                  I != std::tuple_size<typename std::remove_reference<TupleType>::type>::value>::type>
    void
    size_each(TupleType &&t, std::vector<unsigned int> &sizes, std::integral_constant<size_t, I>)
    {
        sizes[I] = sizeOfData(std::get<I>(t));

        if (!_column_format.empty())
        {
            if (_column_format[I] == VariadicTableColumnFormat::PERCENT)
            {
                sizes[I] = 6;
            }
        }

        size_each(std::forward<TupleType>(t), sizes, std::integral_constant<size_t, I + 1>());
    }

    template <typename TupleType>
    void size_each(TupleType &&t, std::vector<unsigned int> &sizes)
    {
        size_each(std::forward<TupleType>(t), sizes, std::integral_constant<size_t, 0>());
    }

    void size_columns()
    {
        _column_sizes.resize(_num_columns);

        std::vector<unsigned int> column_sizes(_num_columns);

        for (unsigned int i = 0; i < _num_columns; i++)
        {
            _column_sizes[i] = _headers[i].size();
        }

        for (auto &row : _data)
        {
            size_each(row, column_sizes);

            for (unsigned int i = 0; i < _num_columns; i++)
            {
                _column_sizes[i] = std::max(_column_sizes[i], column_sizes[i]);
            }
        }
    }

    std::vector<std::string> _headers;

    unsigned int _num_columns;

    unsigned int _static_column_size;

    unsigned int _cell_padding;

    std::vector<DataTuple> _data;

    std::vector<unsigned int> _column_sizes;

    std::vector<VariadicTableColumnFormat> _column_format;

    std::vector<int> _precision;
};

#endif