#pragma once
#include <math.h>

template<class T>
class Vector3
{
private:
	T *data;

public:
	Vector3();

	Vector3(T *data);

	Vector3(T x, T y, T z);

	Vector3(const Vector3<T>& vec);

	~Vector3()
	{
		delete data;
	}

	float getLength()
	{
		return sqrtf(powf(data[0], 2) + powf(data[1], 2) + powf(data[2], 2));
	}

	double getLengthD()
	{
		return sqrt(pow(data[0], 2) + pow(data[1], 2) + pow(data[2], 2));
	}

	T getX()
	{
		return data[0];
	}

	T getY()
	{
		return data[1];
	}

	T getZ()
	{
		return data[2];
	}

	Vector3<T>& normalize()
	{
		T lenght = getLength();
		for (size_t i = 0; i < 3; i++)
		{
			data[i] /= lenght;
		}
		return (*this);
	}

	bool operator!() const
	{
		return ((data[0] != 0) && (data[1] != 0) && (data[2] != 0));
	}

	Vector3<T>& operator=(const Vector3<T>& other) // copy assignment
	{
		if (this != &other)					// self-assignment check
		{
			for (size_t i = 0; i < 3; i++)	// copy data from rhd
			{
				data[i] = other.data[i];
			}
		}
		return *this;
	}

	Vector3<T>& operator=(Vector3<T>&& other) // move assignment
	{
		assert(this != &other);						// self-assignment check not required
		delete[] data;								// delete this storage
		data = std::exchange(other.data, nullptr);	// leave moved-from in valid state
		return *this;
	}

	const Vector3<T> operator+(const Vector3<T>& other)
	{
		Vector3<T> vec;
		for (size_t i = 0; i < 3; i++)
		{
			vec.data[i] = this->data[i] + other.data[i];
		}
		return vec;
	}

	Vector3<T>& operator+=(const Vector3<T>& other)
	{
		for (size_t i = 0; i < 3; i++)
		{
			data[i] += other.data[i];
		}
		return (*this);
	}

	const float operator*(const Vector3<T>& other)
	{
		float sum;
		for (size_t i = 0; i < 3; i++)
		{
			sum += (this->data[i] * other.data[i]);
		}
		return sum;
	}

	const Vector3<T> operator*(const float& other)
	{
		Vector3<T> vec;
		for (size_t i = 0; i < 3; i++)
		{
			vec.data[i] = this->data[i] * other;
		}
		return vec;
	}

	Vector3<T>& operator*=(const float& other)
	{
		for (size_t i = 0; i < 3; i++)
		{
			data[i] *= other;
		}
		return (*this);
	}

	Vector3<T>& operator*=(const Vector3<T>& other)
	{
		normalize();
		float len = other.getLength();
		for (size_t i = 0; i < 3; i++)
		{
			data[i] *= len;
		}
		return (*this);
	}
};

template<class T>
Vector3<T>::Vector3() : data(new T[3])
{
	
}

template<class T>
Vector3<T>::Vector3(T x, T y, T z) : Vector3()
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
};

template<class T>
Vector3<T>::Vector3(T* data) : Vector3()
{
	for (size_t i = 0; i < 3; i++)
	{
		this->data[i] = data[i];
	}
}

template<class T>
Vector3<T>::Vector3(const Vector3<T>& vec) : Vector3()
{
	for (size_t i = 0; i < 3; i++)
	{
		this->data[i] = vec.data[i];
	}
}

typedef Vector3<float> Vector3f;
#define vec3f Vector3<float>