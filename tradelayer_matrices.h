#ifndef TRADELAYER_MATRICES_H
#define TRADELAYER_MATRICES_H

#include<cassert>
#include<iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
template<class T> class VectorTL 
{
	private:
		int lenth;
		T* ets;

	public:
		VectorTL(int);
		VectorTL(int, T*);
		VectorTL(const VectorTL<T>&);
		~VectorTL();

		T& operator[](int i) const;
		VectorTL<T> &operator=(const VectorTL<T>&);
};

template<class T> VectorTL<T>::VectorTL(int n) 
{
	ets = new T[lenth = n];
}

template<class T> VectorTL<T>::VectorTL(int n, T* abd) 
{
	ets = new T[lenth = n];
	for (int i = 0; i < lenth; ++i)
	{
		ets[i] = *(abd + i);
	}
}

template<class T> VectorTL<T>::VectorTL(const VectorTL<T> & v)
{
	ets = new T[lenth = v.lenth];
	for (int i = 0; i < lenth; ++i)
	{
		ets[i] = v[i];
	}
}

template<class T> VectorTL<T>::~VectorTL()
{ 
	delete [] ets; 
}

template<class T> T &VectorTL<T>::operator[](int i) const
{
	return ets[i];
}

template<class T> VectorTL<T> &VectorTL<T>::operator=(const VectorTL<T> &v)
{
	if (*this != &v)
	{
		if (lenth != v.lenth)
		{
			cout<<"Check vectors: Diferent Sizes!!"<<"\n";
			exit(1);
		}
		for (int i = 0; i < lenth; ++i)
		{
			ets[i] = v[i];
		}
	}
	return *this;
}
//////////////////////////////////////////////////////////////////////////////////
template<class T> class MatrixTL 
{
	private:
		int nrows;
		int ncols;
		T** ets;

	public:
		MatrixTL(int n, int m);
		MatrixTL(const MatrixTL<T>&);
		~MatrixTL();

		T* operator[](int i) const;
		MatrixTL<T>& operator=(const MatrixTL<T>&);
  		template<class U>
  		friend inline int size(const MatrixTL<U> &mat, int k);
};

template<class T> MatrixTL<T>::MatrixTL(int n, int m)
{
	nrows = n;
	ncols = m;
	ets = new T*[nrows];
	for (int i = 0; i < nrows; ++i)
	{
		ets[i] = new T[ncols];
	}
}

template<class T> MatrixTL<T>::MatrixTL(const MatrixTL &mat)
{
	ets = new T*[nrows = mat.nrows];
	ncols = mat.ncols;
	for (int i = 0; i < nrows; ++i)
	{
		ets[i] = new T[ncols];
		for (int j = 0; j < ncols; ++j)
		{
			ets[i][j] = mat[i][j];
		}
	}
}

template<class T> MatrixTL<T>::~MatrixTL()
{
	for (int i = 0; i < nrows; ++i)
	{
		delete [] ets[i];
	}
	delete [] ets;
}

template<class T> T* MatrixTL<T>::operator[](int i) const 
{ 
	return ets[i]; 
}

template<class T> MatrixTL<T> &MatrixTL<T>::operator=(const MatrixTL<T>& mat)
{
	if (this != &mat)
	{
		if (nrows != mat.nrows || ncols !=mat.ncols)
		{
			cout<<"Check matrices: Diferent Sizes!!"<<"\n";
			exit(1);
		}
		for (int i = 0; i < nrows; ++i)
		{
			for (int j = 0; j < ncols; ++j)
			{
				ets[i][j] = mat[i][j];
			}
		}
	}
	return *this;
}

template<class T> inline int size(const MatrixTL<T> &mat, int k)
{
  int dim = k == 0 ? mat.nrows : mat.ncols;
  return dim;
}

#endif