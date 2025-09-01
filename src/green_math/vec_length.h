#pragma once

template <typename T>
struct VecLength { using type = float; };

template <>
struct VecLength<double> { using type = double; };
