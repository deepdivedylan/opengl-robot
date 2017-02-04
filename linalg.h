#ifndef LINALG_H
#define LINALG_H

#include <iostream>
#include <vector>
using std::istream;
using std::ostream;
using std::vector;

/*! \file linalg.h
  \brief Generic Purpose Linear Algebra Library

  A fully featured implentation of vectors in \f$\Re^n\f$ and \f$m\times n\f$ matrices. */

//! Vector Library
/*! Represents a vector in \f$\Re^n\f$. */
class Vector
{
public:
	Vector();
	Vector(const Vector &other);
	Vector(unsigned int a);
	Vector(double *values,unsigned int a);
	Vector(std::vector<double> &values);
	~Vector();
	Vector &operator=(const Vector &other);
	Vector operator+(Vector &other);
	Vector operator-(Vector &other);
	double operator*(Vector &other);
	Vector operator%(Vector &other);
	Vector operator+=(Vector &other);
	Vector operator-=(Vector &other);
	Vector operator*=(double k);
	Vector operator/=(double k);
	Vector operator%=(Vector &other);
	bool operator==(Vector &other);
	bool operator!=(Vector &other);
	double operator[](unsigned int a);
	friend Vector operator*(double k,Vector &v);
	friend Vector operator*(Vector &v,double k);
	friend Vector operator/(Vector &v,double k);
	friend ostream &operator<<(ostream &os,Vector &v);
	friend istream &operator>>(istream &is,Vector &v);
	double angle(Vector &other);
	double at(unsigned int a);
	double norm();
	void normalize();
	void set(double *values);
	void set(std::vector<double> &values);
	void set(unsigned int a,double v);
	void zero();
private:
	//! Vector Array
	/*! Array that stores the actual vector. */
	double *vector;
	//! Dimension
	/*! Indicates the dimension of the vector. */
	unsigned int n;
};

//! Matrix Library
/*! Represents a \f$m\times n\f$ matrix. */
class Matrix
{
public:
	Matrix();
	Matrix(const Matrix &other);
	Matrix(unsigned int a,unsigned int b);
	Matrix(double *values,unsigned int a,bool colOrder=true);
	Matrix(double **values,unsigned int a,unsigned int b);
	Matrix(std::vector< std::vector<double> > &values);
	~Matrix();
	Matrix &operator=(const Matrix &other);
	Matrix operator+(Matrix &other);
	Matrix operator-(Matrix &other);
	Matrix operator*(Matrix &other);
	Matrix operator+=(Matrix &other);
	Matrix operator-=(Matrix &other);
	Matrix operator*=(Matrix &other);
	Matrix operator*=(double k);
	Matrix operator/=(double k);
	double *operator[](unsigned int a);
	friend Matrix operator*(double k,Matrix &m);
	friend Matrix operator*(Matrix &m,double k);
	friend Matrix operator/(Matrix &m,double k);
	friend ostream &operator<<(ostream &os,Matrix &m);
	friend istream &operator>>(istream &is,Matrix &m);
	double at(unsigned int a,unsigned int b);
	double det();
	void identity();
	Matrix &inverse();
	void load(double *values,unsigned int a,bool colOrder=true);
	struct LUDecomposition &LU(Matrix &b=*(Matrix *)NULL);
	void pivot(unsigned int a,unsigned int b,bool rowReduce=true);
	void rref();
	void set(double *values,bool colOrder=true);
	void set(double **values);
	void set(std::vector< std::vector <double> > &values);
	void set(unsigned int a,unsigned int b,double v);
	void swapCol(unsigned int a,unsigned int b);
	void swapRow(unsigned int a,unsigned int b);
	Matrix &transpose();
	double *values(bool colOrder=true);
private:
	//! Matrix Array
	/*! Array containing the actual matrix data. */
	double **matrix;
	unsigned int m; /*!< Number Of Rows */
	unsigned int n; /*!< Number Of Columns */
};

//! Linear Algebra Exception
/*! Exception class thrown whenever operator error or floating point errors occur. */
class LinAlgException
{
public:
	//! Default Constructor
	/*! Creates an empty exception. */
	LinAlgException(){message=(char *)"";}
	//! Full Constructor
	/*! Creates an exception with an error message \a msg.
	 \param msg the error message */
	LinAlgException(char *msg){message=msg;}
	//! Destructor
	/*! Currently does nothing. */
	~LinAlgException(){}
	//! Error Message Accessor Method
	/*! Accesses the error message.
	  \return the error message */
	char *what(){return message;}
private:
	char *message; /*!< The Actual Error Message */
};

/* LUDecomposition:
   struct that contains the results of an LU decomposition
   solved is true when the solver attempted to solve the system
   exists is true when the solver found a real non trivial solution */
//! LU Decomposition
/*! A struct that contains the results of an LU decomposition. */
struct LUDecomposition
{
	Matrix A; /*!< Main Matrix */
	Matrix L; /*!< Lower Triangular Matrix */
	Matrix U; /*!< Upper Triangular Matrix */
	Matrix b; /*!< Answer Matrix */
	//! Solve Flag
	/*! This is true when the solver attempted to solve the system. */
	bool solved;
	//! Exists Flag
	/*! This is true when the solver found a real non trivial solution. */
	bool exists;
};

#endif
