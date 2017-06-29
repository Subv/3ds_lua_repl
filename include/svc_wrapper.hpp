// The MIT License (MIT) 

// Copyright (c) 2017 Subv and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <functional>
#include <inttypes.h>

namespace detail {
template<typename RetT, typename Array, std::size_t... Indices>
auto TupleFromArrayImpl(RetT ret, Array& arr, std::index_sequence<Indices...>) {
    return std::make_tuple(ret, arr[Indices]...);
}
}

template<typename RetT, typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto TupleFromArray(RetT ret, std::array<T, N>& arr) {
    return detail::TupleFromArrayImpl(ret, arr, Indices{});
}

template<typename... InArgs>
struct ArrayApplier {
    template<typename Func, typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
    auto Apply(Func func, std::array<T, N>& arr, InArgs... in_args) {
        return ApplyImpl(func, arr, Indices{}, in_args...);
    }

private:
    template<typename Func, typename Array, std::size_t... Indices>
    auto ApplyImpl(Func func, Array& arr, std::index_sequence<Indices...>, InArgs... in_args) {
        return func(&arr[Indices]..., in_args...);
    }
};

template<typename Func, int NumOutArgs, typename... InArgs>
auto Wrap(Func fun) {
    return [=](InArgs... in_args) {
        std::array<uint32_t, NumOutArgs> out_args;

        ArrayApplier<InArgs...> applier;
        auto ret = applier.Apply(fun, out_args, in_args...);
        return TupleFromArray(ret, out_args);
    };
}
