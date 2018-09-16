#ifndef MATRIZ_H
#define MATRIZ_H

#include<iostream>
#include<cstdlib>
#include<cstring>

using std::cout;
using std::memcpy;

//Matrix 2D

template<class T> class matrizt;
template<class T> int size(const matrizt<T> &x, int k);
template<class T> int length(const matrizt<T> &x);

template<class T> class matrizt{
public:
  matrizt(int m, int n);
  matrizt(const matrizt &x);
  virtual ~matrizt(){delete pt2D;}
  matrizt &operator=(const matrizt &x);
  T &operator()(int i, int j);
  T &operator()(int i);
  friend int size<T>(const matrizt<T> &x, int k);
  friend int length<T>(const matrizt<T> &x);
private:
  int rows, cols;
  T *pt2D;
};

template<class T> matrizt<T>::matrizt(int m, int n)
{
  rows=m;
  cols=n;
  pt2D=new T[rows*cols];
}

template<class T> matrizt<T>::matrizt(const matrizt &x)
{
  rows=x.rows;
  cols=x.cols;
  pt2D=new T[rows*cols];
  memcpy(pt2D, x.pt2D, rows*cols*sizeof(T));
}

template<class T> matrizt<T> &matrizt<T>::operator=(const matrizt &x)
{
  delete pt2D;
  rows=x.rows;
  cols=x.cols;
  pt2D=new T[rows*cols];
  memcpy(pt2D, x.pt2D, rows*cols*sizeof(T));
  return *this;
}

template<class T> T &matrizt<T>::operator()(int i, int j)
{
  return pt2D[i*cols+j];
}

template<class T> T &matrizt<T>::operator()(int i)
{
  return pt2D[i];
}

template<class T> inline int size(const matrizt<T> &x, int k)
{
  return((k == 0) ? x.rows : x.cols);
}

template<class T> inline int length(const matrizt<T> &x)
{
  return((x.cols == 0) ? x.rows : x.cols);
}

//Matrix 3D

template<class T> class matriz3t;
template<class T> int size(const matriz3t<T> &x, int k);

template<class T> class matriz3t{
public:
  matriz3t(int m, int n, int p);
  matriz3t(const matriz3t &x);
  virtual ~matriz3t(){delete pt3D;}
  matriz3t &operator=(const matriz3t &x);
  T &operator()(int i, int j, int k);
  friend int size<T>(const matriz3t<T> &x, int k);
private:
  int rows, cols, alts;
  T *pt3D;
};

template<class T> matriz3t<T>::matriz3t(int m, int n, int p)
{
  rows=m;
  cols=n;
  alts=p;
  pt3D=new T[rows*cols*alts];
}

template<class T> matriz3t<T>::matriz3t(const matriz3t &x)
{
  rows=x.rows;
  cols=x.cols;
  alts=x.alts;
  pt3D=new T[rows*cols*alts];
  memcpy(pt3D, x.pt3D, rows*cols*alts*sizeof(T));
}

template<class T> matriz3t<T> &matriz3t<T>::operator=(const matriz3t &x)
{
  delete pt3D;
  rows=x.rows;
  cols=x.cols;
  alts=x.alts;
  pt3D=new T[rows*cols*alts];
  memcpy(pt3D, x.pt3D, rows*cols*alts*sizeof(T));
  return *this;
}

template<class T> T &matriz3t<T>::operator()(int i, int j, int k)
{
  return pt3D[k*rows*cols+i*cols+j];
}

template<class T> inline int size(const matriz3t<T> &x, int k)
{
  return((k == 0) ? x.rows : ((k == 1) ? x.cols: x.alts));
}

typedef matrizt<double> matriz;
typedef matrizt<std::string> matrizs;
typedef matrizt<int> matrizi;
typedef matriz3t<double> matriz3;
typedef matriz3t<std::string> matriz3s;
typedef matriz3t<int> matriz3i;

#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif
#ifndef MIN
#define MIN(a, b) ((a < b) ? a : b)
#endif
#ifndef sign
#define sign(a) ((a < 0) ? -1 : ((a > 0) ? 1 : 0))
#endif

#endif