#include <cfloat>
#include <cmath>
#include <cstdlib>

#include "linalg.h"

//! Default Constructor
/*! Creates an empty Vector with no dimension. */
Vector::Vector()
{
	vector=0;
	n=0;
}

//! Copy Constructor
/*! Creates a Vector from \a other. Automatically adjusts dimensions as needed.
  \param other the source Vector. */
Vector::Vector(const Vector &other)
{
	try
	{
		vector=new double[other.n];
		for (unsigned int i=0;i<other.n;i++)
			vector[i]=other.vector[i];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	n=other.n;
}

//! Sized Constructor
/*! Creates a zero Vector in \f$\Re^a\f$.
  \param a the dimension of the Vector */
Vector::Vector(unsigned int a)
{
	try
	{
		vector=new double[a];
		for (unsigned int i=0;i<a;i++)
			vector[i]=0.0;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	n=a;
}

//! Sized Constructor With Data
/*! Creates a Vector in \f$\Re^a\f$ populated with data from \a values.
  \param values array of Vector values
  \param a the number of members in \a values */
Vector::Vector(double *values,unsigned int a)
{
	try
	{
		vector=new double[a];
		for (unsigned int i=0;i<a;i++)
			vector[i]=values[i];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	n=a;
}

//! std::vector Constructor
/*! Creates a Vector populated with data from the std::vector \a values.
  \param values the std::vector<double> with the data */
Vector::Vector(std::vector<double> &values)
{
	try
	{
		vector=new double[values.size()];
		for (unsigned int i=0;i<values.size();i++)
			vector[i]=values[i];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	n=values.size();
}

//! Destructor
/*! Deallocates allocated memory */
Vector::~Vector()
{
	delete vector;
}

//! Assignment Operator
/*! Assigns one Vector from another.
  \param other the Vector to assign from
  \return reference to the new Vector */
Vector &Vector::operator=(const Vector &other)
{
	if (vector)
		delete vector;
	try
	{
		vector=new double[other.n];
		for (unsigned int i=0;i<other.n;i++)
			vector[i]=other.vector[i];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	n=other.n;
	return *this;
}

//! Addition Operator
/*! Adds two Vector together.
  \param other the Vector to add
  \throw LinAlgException if the dimensions don't match
  \return the resulting Vector */
Vector Vector::operator+(Vector &other)
{
	if (n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	Vector answer(n);
	for (unsigned int i=0;i<n;i++)
		answer.vector[i]=vector[i]+other.vector[i];
	return answer;
}

//! Subtraction Operator
/*! Subtracts one Vector from the other.
  \param other the Vector to subtract (the subtrahend)
  \throw LinAlgException if the dimensions don't match
  \return the resulting Vector */
Vector Vector::operator-(Vector &other)
{
	if (n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	Vector answer(n);
	for (unsigned int i=0;i<n;i++)
		answer.vector[i]=vector[i]-other.vector[i];
	return answer;
}

//! Dot Product Operator
/*! Takes the dot product of two vectors.
  \param other the Vector to dot with
  \throw LinAlgException if the dimensions don't match
  \return the resulting scalar */
double Vector::operator*(Vector &other)
{
	if (n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	double answer=0.0;
	for (unsigned int i=0;i<n;i++)
		answer+=vector[i]*other.vector[i];
	return answer;
}

//! Cross Product Operator
/*! Takes the cross product of two Vectors in \f$\Re^3\f$.
  \param other the second operand of the cross product
  \throw LinAlgException if both Vectors aren't in \f$\Re^3\f$.
  \return the resulting Vector */
Vector Vector::operator%(Vector &other)
{
	if (n!=3||other.n!=3)
		throw LinAlgException("Cross product is only defined in 3 space");
	Vector answer(3);
	answer.vector[0]=vector[1]*other.vector[2]-vector[2]*other.vector[1];
	answer.vector[1]=vector[2]*other.vector[0]-vector[0]*other.vector[2];
	answer.vector[2]=vector[0]*other.vector[1]-vector[1]*other.vector[0];
	return answer;
}

//! Accumulation Operator
/*! This is a wrapper function for operator+()
  \param other the Vector to add
  \sa operator+()
  \return the resulting Vector */
Vector Vector::operator+=(Vector &other)
{
	*this=this->operator+(other);
	return *this;
}

//! Decumulation Operator
/*! This is a wrapper function for operator-()
  \param other the Vector to subtract (the subtrahend)
  \sa operator-()
  \return the resulting Vector */
Vector Vector::operator-=(Vector &other)
{
	*this=this->operator-(other);
	return *this;
}

//! Scalar Multiplication Operator
/*! This is a wrapper function for operator*()
  \param k the scalar to multiply by
  \sa operator*()
  \return the resulting Vector */
Vector Vector::operator*=(double k)
{
	*this=::operator*(*this,k);
	return *this;
}

//! Scalar Multiplication Operator
/*! This is a wrapper function for operator/()
  \param k the scalar to multiply by
  \sa operator/()
  \return the resulting Vector */
Vector Vector::operator/=(double k)
{
	*this=operator/(*this,k);
	return *this;
}

//! Cross Product Operator
/*! This is a wrapper function for operator%()
  \param other the second operator of the cross product
  \sa operator%()
  \return the resulting Vector */
Vector Vector::operator%=(Vector &other)
{
	*this=this->operator%(other);
	return *this;
}

//! Logical Equivilence Operator
/*! Tests whether two Vectors are the same.
  \param other the Vector to compare
  \return true if equivilent, false if not */
bool Vector::operator==(Vector &other)
{
	bool equal=true;
	for (unsigned int i=0;i<n;i++)
	{
		if (fabs(vector[i]-other.vector[i])>DBL_EPSILON)
		{
			equal=false;
			break;
		}
	}
	return equal;
}

//! Logical Inequivilence Operator
/*! Tests whether two Vectors are different.
  \param other the Vector to compare
  \return true if inequivilent, false if not */
bool Vector::operator!=(Vector &other)
{
	return (!operator==(other));
}

//! Array Subscript Operator
/*! Accesses a particular element in the Vector
  \param a the element to access
  \return the \f$a^{th}\f$ member of the Vector */
double Vector::operator[](unsigned int a)
{
	return vector[a];
}

//! Scalar Multiplication Operator
/*! Friend function that implements \f$k\overrightarrow v\f$.
  \param k the scalar
  \param v the Vector
  \return the resulting Vector */
Vector operator*(double k,Vector &v)
{
	Vector answer(v.n);
	for (unsigned int i=0;i<v.n;i++)
		answer.vector[i]=k*v.vector[i];
	return answer;
}

//! Scalar Multiplication Operator
/*! Friend function that implements \f$\overrightarrow vk\f$.
  \param v the Vector
  \param k the scalar
  \return the resulting Vector */
Vector operator*(Vector &v,double k)
{
	return operator*(k,v);
}

//! Scalar Division Operator
/*! Friend function that implements \f$\frac{\overrightarrow v}k\f$.
  \param v the Vector
  \param k the scalar
  \throw LinAlgException if \f$k=0\f$.
  \return the resulting Vector */
Vector operator/(Vector &v,double k)
{
	if (fabs(k)<DBL_EPSILON)
		throw LinAlgException("Divide by zero");
	Vector answer(v.n);
	for (unsigned int i=0;i<v.n;i++)
		answer.vector[i]=v.vector[i]/k;
	return answer;
}

//! Outdirection Operator
/*! Friend function that prints a Vector \a v to \a os.
  \param os the output stream
  \param v the Vector to output
  \return the output stream */
ostream &operator<<(ostream &os,Vector &v)
{
	os<<"<";
	for (unsigned int i=0;i<v.n;i++)
	{
		if (i!=(v.n-1))
			os<<v.vector[i]<<",";
		else
			os<<v.vector[i];
	}
	os<<">";
	return os;
}

//! Indirection Operator
/*! Friend function that reads a Vector \a v from \a is.
  \param is the input stream to read from
  \param v the Vector to store data in
  \return the input stream */
istream &operator>>(istream &is,Vector &v)
{
	for (unsigned int i=0;i<v.n;i++)
		is>>v.vector[i];
	return is;
}

//! Angle Between Two Vectors
/*! Finds the angle \f$\theta\f$ between two Vectors such that: \f$\theta=\cos^{-1}(\hat u\cdot\hat v)\f$.
  \param other the other Vector to find the angle between
  \return the angle
  \throw LinAlgException if both Vectors aren't the same dimension */
double Vector::angle(Vector &other)
{
	if (n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	Vector u(n),v(n);
	u=*this;
	v=other;
	u.normalize();
	v.normalize();
	return (acos(u*v));
}

//! Accessor Method
/*! Accesses a particular member of the Vector.
  \param a the member to access
  \return the \f$a^{th}\f$ member of the Vector. */
double Vector::at(unsigned int a)
{
	return vector[a];
}

//! Norm
/*! Finds the norm of the Vector such that \f$\left| \overrightarrow v \right|=\sqrt{v_1^2+v_2^2+\cdots+v_n^2}\f$.
  \returns the norm */
double Vector::norm()
{
	double answer=0.0;
	for (unsigned int i=0;i<n;i++)
		answer+=pow(vector[i],2.0);
	return (sqrt(answer));
}

//! Normalize
/*! Normalizes (unitizes) the Vector ``in place.''
  \sa norm() */
void Vector::normalize()
{
	double k=norm();
	for (unsigned int i=0;i<n;i++)
		vector[i]/=k;
}

//! Mutator Method
/*! Load array data into the Vector.
  \param values the array of values containing the data */
void Vector::set(double *values)
{
	for (unsigned int i=0;i<n;i++)
		vector[i]=values[i];
}

//! Mutator Method
/*! Load std::vector data into the Vector.
  \param values the std::vector of values containing the data */
void Vector::set(std::vector<double> &values)
{
	for (unsigned int i=0;i<n;i++)
		vector[i]=values[i];
}

//! Mutator Method
/*! Load \a v in the \f$a^{th}\f$ space in the Vector.
  \param a position to load into
  \param v value to load */
void Vector::set(unsigned int a,double v)
{
	vector[a]=v;
}

//! Clear The Vector
/*! Loads all zeros into the Vector. */
void Vector::zero()
{
	for (unsigned int i=0;i<n;i++)
		vector[i]=0.0;
}
