#ifndef TRADELAYER_MATRICES_H
#define TRADELAYER_MATRICES_H

#include<cassert>
#include<iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
template<class T> class Vector
{
	private:
		int lenth;
		T* ets;

	public:
		Vector(int);
		Vector(int, T*);
		Vector(const Vector<T>&);
		~Vector();

		T& operator[](int i) const;
		Vector<T> &operator=(const Vector<T>&);
  		template<class U>
  		friend inline int length(const Vector<U>&);
};

template<class T> Vector<T>::Vector(int n) : lenth(n), ets(new T[n])
{ assert(ets!=NULL); }

template<class T> Vector<T>::Vector(int n, T* abd) : ets(new T[lenth = n])
{ for (int i = 0; i < lenth; ++i) ets[i] = *(abd + i); }

template<class T> Vector<T>::Vector(const Vector<T> & v) : lenth(v.lenth), ets(new T[v.lenth])
{ for (int i = 0; i < lenth; ++i) ets[i] = v[i]; }

template<class T> Vector<T>::~Vector()
{ delete [] ets; }

template<class T> T &Vector<T>::operator[](int i) const
{ return ets[i]; }

template<class T> Vector<T> &Vector<T>::operator=(const Vector<T> &v)
{
	if (*this != &v)
	{
		assert(lenth != v.lenth);
		for (int i = 0; i < lenth; ++i) ets[i] = v[i];
	}
	return *this;
}

template<class T> inline int length(const Vector<T> &v)
{ return v.lenth; }

//////////////////////////////////////////////////////////////////////////////////
template<class T> class Matrix 
{
	private:
		int nrows;
		int ncols;
		T** ets;

	public:
		Matrix(int n, int m);
		Matrix(const Matrix<T>&);
		~Matrix();

		T* operator[](int i) const;
		Matrix<T>& operator=(const Matrix<T>&);
  		template<class U>
  		friend inline int size(const Matrix<U> &mat, int k);
};

template<class T> Matrix<T>::Matrix(int n, int m) : nrows(n), ncols(m), ets(new T*[n])
{ for (int i = 0; i < n; ++i) ets[i] = new T[m]; }

template<class T> Matrix<T>::Matrix(const Matrix &mat) : nrows(mat.nrows), ncols(mat.ncols), ets(new T*[mat.nrows])
{
	assert(ets != NULL);
	for (int i = 0; i < mat.nrows; ++i)
	{
		ets[i] = new T[mat.ncols];
		for (int j = 0; j < mat.ncols; ++j) ets[i][j] = mat[i][j];
	}
}

template<class T> Matrix<T>::~Matrix()
{
	for (int i = 0; i < nrows; ++i) delete [] ets[i];
	delete [] ets;
}

template<class T> T* Matrix<T>::operator[](int i) const 
{ return ets[i]; }

template<class T> Matrix<T> &Matrix<T>::operator=(const Matrix<T>& mat)
{
	if (this != &mat)
	{
		assert(nrows != mat.nrows || ncols !=mat.ncols);
		for (int i = 0; i < nrows; ++i) for (int j = 0; j < ncols; ++j) ets[i][j] = mat[i][j];
	}
	return *this;
}

template<class T> inline int size(const Matrix<T> &mat, int k)
{ return k == 0 ? mat.nrows : mat.ncols; }

typedef Matrix<std::string> MatrixTL;
typedef Vector<std::string> VectorTL;

#endif