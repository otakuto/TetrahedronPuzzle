#include <array>
#include <functional>
#include <algorithm>
#include <iostream>

constexpr std::array<int, 2> rowColumnToIJ(std::array<int, 2> p)
{
	int row = std::get<0>(p);
	int column = std::get<1>(p);

	if (column <= row)
	{
		return {{row, column}};
	}
	else
	{
		return {{(2 * row) - column, row}};
	}
}

constexpr std::array<int, 3> ijToXYZ(std::array<int, 2> p)
{
	int i = std::get<0>(p);
	int j = std::get<1>(p);
	int m = std::max(i, j);

	return {{(m + i - j) / 2, -m, (m - i + j) / 2}};
}

constexpr std::array<int, 2> xyzToIJ(std::array<int, 3> p)
{
	int x = std::get<0>(p);
	int y = std::get<1>(p);
	int z = std::get<2>(p);

	if (x < z)
	{
		return {{x - y - z, -y}};
	}
	else
	{
		return {{-y, - x - y + z}};
	}
}

template<typename T>
T rotateRight(T t)
{
	std::rotate(t.rbegin(), t.rbegin() + 1, t.rend());
	return t;
}

template<typename T>
T rotateLeft(T t)
{
	std::rotate(t.begin(), t.begin() + 1, t.end());
	return t;
}

template<typename F, typename X>
constexpr auto nest(F f, int n, X x) -> decltype(f(x))
{
	for (int i = 0; i < n; ++i)
	{
		x = f(x);
	}
	return x;
};

template<typename T>
constexpr int axisToInt(T axis)
{
	for (int i = 0; i < axis.size(); ++i)
	{
		if (axis[i])
		{
			return i;
		}
	}
};

template <int N>
class TetrahedronPuzzle
{
public:
	constexpr static int const SURFACE_LENGTH = 4;

	constexpr std::array<int, 3> upXYZToCenterXYZ(std::array<int, 3> p) const
	{
		return {{std::get<0>(p), std::get<1>(p) + (N - 1), std::get<2>(p)}};
	};

	constexpr std::array<int, 3> centerXYZToUpXYZ(std::array<int, 3> p) const
	{
		return {{std::get<0>(p), std::get<1>(p) - (N - 1), std::get<2>(p)}};
	};

private:
	std::array<std::array<std::array<int, N>, N>, SURFACE_LENGTH> surface;

public:
	TetrahedronPuzzle()
	{
		for (int i = 0; i < SURFACE_LENGTH; ++i)
		{
			for (int row = 0; row < N; ++row)
			{
				for (int column = 0; column <= (row * 2); ++column)
				{
					auto ij = rowColumnToIJ({{row, column}});
					surface[i][ij[0]][ij[1]] = i;
				}
			}
		}
	}

	constexpr int getTile(int axis, int row, int column) const
	{
		auto ij = rowColumnToIJ({{row, column}});
		return surface[axis][ij[0]][ij[1]];
	}

	void rotate(std::array<int, SURFACE_LENGTH> axis, int index, bool isPrime)
	{
		int rotateDirection = axis[0] - axis[1] + axis[2] - axis[3];
		auto rotateAxis = (rotateDirection > 0) ? [](decltype(axis) e){return rotateLeft(e);} : [](decltype(axis) e){return rotateRight(e);};
		auto rotateSideAxis = isPrime ? [](std::array<int, 3> e){return rotateLeft(e);} : [](std::array<int, 3> e){return rotateRight(e);};
		auto rotateXYZ = isPrime ? [](std::array<int, 3> e){return rotateLeft(e);} : [](std::array<int, 3> e){return rotateRight(e);};

		auto s = surface;
		//side surface rotate
		for (int i = 0; i < SURFACE_LENGTH - 1; ++i)
		{
			int si = axisToInt(nest(rotateAxis, axisToInt(nest(rotateSideAxis, i, std::array<int, 3>{{1,0,0}})) + 1, axis));
			int di = axisToInt(nest(rotateAxis, axisToInt(nest(rotateSideAxis, i + 1, std::array<int, 3>{{1,0,0}})) + 1, axis));
			for (int column = 0; column <= (index * 2); ++column)
			{
				auto ij = rowColumnToIJ({{index, column}});
				auto sij = xyzToIJ(centerXYZToUpXYZ(nest(rotateXYZ, i, upXYZToCenterXYZ(ijToXYZ(ij)))));
				auto dij = xyzToIJ(centerXYZToUpXYZ(nest(rotateXYZ, i + 1, upXYZToCenterXYZ(ijToXYZ(ij)))));
				s[di][dij[0]][dij[1]] = surface[si][sij[0]][sij[1]];
			}
		}

		//bottom surface rotate
		if (index == (N - 1))
		{
			for (int row = 0; row < N; ++row)
			{
				for (int column = 0; column <= (row * 2); ++column)
				{
					auto ij = rowColumnToIJ({{row, column}});
					auto rotij = xyzToIJ(centerXYZToUpXYZ(rotateXYZ(upXYZToCenterXYZ(ijToXYZ(ij)))));
					s[axisToInt(axis)][ij[0]][ij[1]] = surface[axisToInt(axis)][rotij[0]][rotij[1]];
				}
			}
		}
		surface = s;
	}
};

