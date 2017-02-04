#include <cfloat>
#include <cmath>
#include <cstdlib>

#include "linalg.h"

static int detFactor;

//! Scalar Multiplication Operator
/*! Friend function that multiplies a Matrix \a m by a scalar \a k.
  \param k the scalar to multiply by
  \param m the Matrix
  \return the resulting Matrix */
Matrix operator*(double k,Matrix &m)
{
	Matrix answer(m.m,m.n);
	for (unsigned int i=0;i<m.m;i++)
		for (unsigned int j=0;j<m.n;j++)
			answer[i][j]=k*m.matrix[i][j];
	return answer;
}

//! Scalar Multiplication Operator
/*! Friend function that multiplies a Matrix \a m by a scalar \a k.
  \param m the Matrix
  \param k the scalar to multiply by
  \return the resulting Matrix */
Matrix operator*(Matrix &m,double k)
{
	return operator*(k,m);
}

//! Scalar Division Operator
/*! Friend function that divides a Matrix \a m by a scalar \a k.
  \param m the Matrix
  \param k the scalar
  \throw LinAlgException if \f$k=0\f$.
  \return the resulting Matrix */
Matrix operator/(Matrix &m,double k)
{
	if (k==0)
		throw LinAlgException("Divide by zero");
	Matrix answer(m.m,m.n);
	for (unsigned int i=0;i<m.m;i++)
		for (unsigned int j=0;j<m.n;j++)
			answer[i][j]=m.matrix[i][j]/k;
	return answer;
}

//! Outdirection Operator
/*! Friend function that prints a Matrix \a m to \a os.
  \param os the output stream
  \param m the Matrix to output
  \return the output stream */
ostream &operator<<(ostream &os,Matrix &m)
{
	for (unsigned int i=0;i<m.m;i++)
	{
		for (unsigned int j=0;j<m.n;j++)
		{
			if (j!=(m.n-1))
				os<<m.matrix[i][j]<<" ";
			else
				os<<m.matrix[i][j];
		}
		if (i!=(m.m-1))
			os<<std::endl;
	}
	return os;
}

//! Indirection Operator
/*! Friend function that reads a Matrix \a m from \a is.
  \param is the input stream
  \param m the Matrix to read
  \return the intput stream */
istream &operator>>(istream &is,Matrix &m)
{
	for (unsigned int i=0;i<m.m;i++)
		for (unsigned int j=0;j<m.n;j++)
			is>>m.matrix[i][j];
	return is;
}

//! Accessor Method
/*! Accesses the value at \f$M_{ab}\f$.
  \param a the row of the value
  \param b the column of the value
  \return the value */
double Matrix::at(unsigned int a,unsigned int b)
{
	return matrix[a][b];
}

//! Default Constructor
/*! Creates an empty \f$0\times0\f$ Matrix. */
Matrix::Matrix()
{
	matrix=0;
	m=n=0;
}

//! Copy Constructor
/*! Creates a Matrix from the Matrix \a other.
  \param other the Matrix to copy from */
Matrix::Matrix(const Matrix &other)
{
	try
	{
		matrix=new double*[other.m];
		for (unsigned int i=0;i<other.m;i++)
			matrix[i]=new double[other.n];
		for (unsigned int i=0;i<other.m;i++)
			for (unsigned int j=0;j<other.n;j++)
				matrix[i][j]=other.matrix[i][j];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	m=other.m;
	n=other.n;
}

//! Full Constructor
/*! Creates an empty \f$a\times b\f$ Matrix.
  \param a number of rows
  \param b number of columns */
Matrix::Matrix(unsigned int a,unsigned int b)
{
	try
	{
		matrix=new double*[a];
		for (unsigned int i=0;i<a;i++)
			matrix[i]=new double[b];
		for (unsigned int i=0;i<a;i++)
			for (unsigned int j=0;j<b;j++)
				matrix[i][j]=0.0;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	m=a;
	n=b;
}

//! OpenGL glGetDoublev() Compatible Constructor
/*! Creates a \f$b\times b\f$ Matrix with data from \a values where \f$b=\sqrt a\f$.
  \param values array containg the data to load
  \param a number of elements in \a values
  \param colOrder if true, \a values is in column major order (default); if false, \a values is assumed to be in row major order
  \throw LinAlgException if \a a is not a square number
  \sa load() */
Matrix::Matrix(double *values,unsigned int a,bool colOrder)
{
	/* check to see if this can create a square matrix */
	if ((fabs(pow(sqrt(a),2.0)-a))>DBL_EPSILON)
		throw LinAlgException("Not a square matrix");
	m=n=(unsigned int)sqrt(a);
	try
	{
		matrix=new double*[m];
		for (unsigned int i=0;i<m;i++)
			matrix[i]=new double[n];
		for (unsigned int i=0;i<m;i++)
			for (unsigned int j=0;j<n;j++)
			{
				if (colOrder)
					matrix[i][j]=values[n*j+i];
				else
					matrix[i][j]=values[n*i+j];
			}
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
}

//! Two Dimensional Array Constructor
/*! Creates a \f$a\times b\f$ Matrix from the two dimensional array \a values. \a values is assumed to be in row major order.
  \param values array containing the data to load
  \param a number of rows
  \param b number of columns */
Matrix::Matrix(double **values,unsigned int a,unsigned int b)
{
	try
	{
		matrix=new double*[a];
		for (unsigned int i=0;i<a;i++)
			matrix[i]=new double[b];
		for (unsigned int i=0;i<a;i++)
			for (unsigned int j=0;j<b;j++)
				matrix[i][j]=values[i][j];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	m=a;
	n=b;
}

//! std::vector Constructor
/*! Creates a  Matrix from the std::vector< std::vector<double> > \a values. \a values is assumed to be in row major order.
  \param values std::vector containing the data to load
  \throw LinAlgException if the std::vector is not rectangular */
Matrix::Matrix(std::vector< std::vector<double> > &values)
{
	/* check to make sure the vector is rectangular */
	unsigned int x=values[0].size();
	for (unsigned int i=0;i<values.size();i++)
		if (values[i].size()!=x)
			throw LinAlgException("Incompatible Dimensions");
	try
	{
		matrix=new double*[values.size()];
		for (unsigned int i=0;i<values.size();i++)
			matrix[i]=new double[values[i].size()];
		for (unsigned int i=0;i<values.size();i++)
			for (unsigned int j=0;j<values[i].size();j++)
				matrix[i][j]=values[i][j];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	m=values.size();
	n=values[0].size();
}

//! Destructor
/*! Frees allocated objects needed by Matrix. */
Matrix::~Matrix()
{
	for (unsigned int i=0;i<m;i++)
		delete matrix[i];
}

//! Assignment Operator
/*! Creates a Matrix from another Matrix \a other.
  \param other the Matrix to copy from
  \return a reference to the new Matrix */
Matrix &Matrix::operator=(const Matrix &other)
{
	if (matrix)
		for (unsigned int i=0;i<m;i++)
			delete matrix[i];
	try
	{
		matrix=new double*[other.m];
		for (unsigned int i=0;i<other.m;i++)
			matrix[i]=new double[other.n];
		for (unsigned int i=0;i<other.m;i++)
			for (unsigned int j=0;j<other.n;j++)
				matrix[i][j]=other.matrix[i][j];
	}
	catch (std::bad_alloc &e)
	{
		std::cerr<<"Exception: "<<e.what()<<std::endl;
		abort();
	}
	m=other.m;
	n=other.n;
	return *this;
}

//! Addition Operator
/*! Adds two matrices together.
  \param other Matrix to add
  \throw LinAlgException if the dimensions don't match
  \return the resulting Matrix */
Matrix Matrix::operator+(Matrix &other)
{
	if (m!=other.m||n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	Matrix answer(m,n);
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			answer[i][j]=matrix[i][j]+other.matrix[i][j];
	return answer;
}

//! Subtraction Operator
/*! Subtracts one Matrix from the other.
  \param other Matrix to subtract (the subtrahend)
  \throw LinAlgException if the dimensions don't match
  \return the resulting Matrix */
Matrix Matrix::operator-(Matrix &other)
{
	if (m!=other.m||n!=other.n)
		throw LinAlgException("Incompatible Dimensions");
	Matrix answer(m,n);
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			answer[i][j]=matrix[i][j]-other.matrix[i][j];
	return answer;
}

//! Matrix Multiplication
/*! Multiplies two matrices together.
  \param other Matrix to multiply
  \throw LinAlgException if the dimensions don't match
  \return the resulting Matrix */
Matrix Matrix::operator*(Matrix &other)
{
	if (n!=other.m)
		throw LinAlgException("Incompatible Dimensions");
	Matrix answer(m,other.n);
	for (unsigned int i=0;i<answer.m;i++)
		for (unsigned int j=0;j<answer.n;j++)
			for (unsigned int k=0;k<n;k++)
				answer.matrix[i][j]+=matrix[i][k]*other.matrix[k][j];
	return answer;
}

//! Accumulation Operator
/*! This is a wrapper function for operator+()
  \param other the Matrix to add
  \sa operator+()
  \return the resulting Matrix */
Matrix Matrix::operator+=(Matrix &other)
{
	*this=this->operator+(other);
	return *this;
}

//! Decumulation Operator
/*! This is a wrapper function for operator-()
  \param other the Matrix to subtract (the subtrahend)
  \sa operator-()
  \return the resulting Matrix */
Matrix Matrix::operator-=(Matrix &other)
{
	*this=this->operator-(other);
	return *this;
}

//! Matrix Multiplication Operator
/*! This is a wrapper function for operator*()
  \param other the Matrix to multiply
  \sa operator*()
  \return the resulting Matrix */
Matrix Matrix::operator*=(Matrix &other)
{
	*this=this->operator*(other);
	return *this;
}

//! Scalar Multiplication Operator
/*! This is a wrapper function for operator*()
  \param k scalar to multiply by
  \sa operator*()
  \return the resulting Matrix */
Matrix Matrix::operator*=(double k)
{
	*this=::operator*(k,*this);
	return *this;
}

//! Scalar Division Operator
/*! This is a wrapper function for operator/()
  \param k scalar to divide by
  \return the resulting Matrix */
Matrix Matrix::operator/=(double k)
{
	*this=operator/(*this,k);
	return *this;
}


//! Array Subscript Operator
/*! Accesses a particular element in the Matrix
  \param a the element to access
  \return the \f$a^{th}\f$ element in the Matrix */
double *Matrix::operator[](unsigned int a)
{
	return matrix[a];
}

#if 0
/* find the determinant of a square matrix - runs O(n!) - wow! */
double Matrix::det()
{
	if (n!=m)
		throw "Not a square matrix";
	if (n==2)
		return (matrix[0][0]*matrix[1][1]-matrix[0][1]*matrix[1][0]);
	else
	{
		double c=1.0,sum=0.0;
		for (unsigned int i=0;i<n;i++)
		{
			Matrix temp(n-1,n-1);
			for (unsigned int j=1;j<m;j++)
			{
				bool skip=false;
				for (unsigned int k=0;k<n;k++)
				{
					if (i==k)
					{
						skip=true;
						continue;
					}
					if (skip)
						temp.matrix[j-1][k-1]=matrix[j][k];
					else
						temp.matrix[j-1][k]=matrix[j][k];

				}
			}
			sum+=c*matrix[0][i]*temp.det();
			c*=-1;
		}
		return sum;
	}
}
#endif

//! Determinant
/*! Finds the determinant by LUDecomposition. Runs \f$O(n^3)\f$.
  \throw LinAlgException if the Matrix is not square
  \return the determinant */
double Matrix::det()
{
	if (n!=m)
		throw LinAlgException("Not a square matrix");
	bool singular=false;
	double det=1.0;
	Matrix temp(n,n),L(n,n),U(n,n);
	struct LUDecomposition LU=this->LU();
	temp=LU.A;
	L=LU.L;
	U=LU.A;
	/* if there exists a row of only zeros, the matrix is singular */
	for (unsigned int i=0;i<m;i++)
	{
		for (unsigned int j=0;j<n;j++)
		{
			if (fabs(temp[i][j])>DBL_EPSILON)
				break;
			else if (j==(n-1))
				singular=true;
		}
		if (singular)
			break;
	}
	/* det(A)=0 for all singular matrices A */
	if (singular)
		return 0.0;
	/* otherwise, it's the product of the main diagonal */
	for (unsigned int i=0;i<m;i++)
		det*=L.matrix[i][i];
	det*=detFactor;
	return det;
}

//! Generate Identity
/*! Replaces a Matrix ``in place'' with the idendity Matrix.
  \throw LinAlgException if the Matrix is not square */
void Matrix::identity()
{
	if (n!=m)
		throw LinAlgException("Not a square matrix");
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			if (i!=j)
				matrix[i][j]=0.0;
			else
				matrix[i][j]=1.0;
}

//! Matrix Inversion
/*! Finds the inverse of a Matrix using Gauss Jordan Elimination.
  \throw LinAlgException if the Matrix is not square \b or if the Matrix is singular
  \return the resulting Matrix */
Matrix &Matrix::inverse()
{
	if (n!=m)
		throw LinAlgException("Not a square matrix");
	int largestValue;
	bool singular=false;
	double pivotElement;
	Matrix temp=*this,*inv=new Matrix(n,n);
	inv->identity();

	for (unsigned int i=0;i<m;i++)
	{
		/* there exists a 0 in the main diagonal */
		/* so we have to swap rows */
		if (fabs(temp.matrix[i][i])<DBL_EPSILON)
		{
			largestValue=0;
			for (unsigned int j=0;j<n;j++)
			{
				if (fabs(temp.matrix[j][i])>fabs(temp.matrix[largestValue][i])&&j!=i)
					largestValue=j;
			}
			temp.swapRow(i,largestValue);
			inv->swapRow(i,largestValue);
		}
		/* this essentially does the same as pivot() */
		/* except it works on both temp and inv at the same time */
		pivotElement=temp.matrix[i][i];
		if (fabs(pivotElement)<DBL_EPSILON)
			throw LinAlgException("Divide by zero");
		for (unsigned int j=0;j<n;j++)
		{
			temp.matrix[i][j]/=pivotElement;
			inv->matrix[i][j]/=pivotElement;
		}
		for (unsigned int j=0;j<m;j++)
		{
			if (j!=i&&fabs(temp.matrix[j][i])>DBL_EPSILON)
			{
				double factor=temp.matrix[j][i];
				for (unsigned int k=0;k<n;k++)
				{
					temp.matrix[j][k]=temp.matrix[j][k]-factor*temp.matrix[i][k];
					inv->matrix[j][k]=inv->matrix[j][k]-factor*inv->matrix[i][k];
				}
			}
		}
	}
	/* check if the matrix is singular */
	for (unsigned int i=0;i<m;i++)
	{
		for (unsigned int j=0;j<n;j++)
		{
			if (fabs(temp.matrix[i][j])>DBL_EPSILON)
				break;
			else if (j==(n-1))
				singular=true;
		}
		if (singular)
			throw LinAlgException("Singular matrix");
	}

	return *inv;
}

//! OpenGL glGetDoublev() Compatible Loader
/*! Creates a \f$b\times b\f$ Matrix with data from \a values where \f$b=\sqrt a\f$. If the Matrix is not already \f$b\times b\f$, load() will adjust the Matrix.
  \param values array containg the data to load
  \param a number of elements in \a values
  \param colOrder if true, \a values is in column major order (default); if false, \a values is assumed to be in row major order
  \throw LinAlgException if \a a is not a square number
  \sa set() */
void Matrix::load(double *values,unsigned int a,bool colOrder)
{
	/* check to see if this can create a square matrix */
	if ((fabs(pow(sqrt(a),2.0)-a))>DBL_EPSILON)
		throw LinAlgException("Not a square matrix");
	m=n=(unsigned int)sqrt(a);
	matrix=new double*[m];
	for (unsigned int i=0;i<m;i++)
		matrix[i]=new double[n];
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
		{
			if (colOrder)
				matrix[i][j]=values[n*j+i];
			else
				matrix[i][j]=values[n*i+j];
		}
}

/*! \fn Matrix::LU(Matrix &b)

  \brief LU Decomposition

  Performs an LU Decomposition of a square \f$n\times n\f$ Matrix. It will attempt to solve a system of equations with a \f$n\times 1\f$ Matrix \a b if \a b is provided. If \a b is NULL, the LU Decomposition is performed without attempting to solve a system (default). Solving by LU Decomposition is much faster \f$(O(n^3))\f$ than by Gauss Jordan Elimination \f$O(n^4))\f$.
  \param b \f$n\times 1\f$ answer Matrix
  \return struct LUDecomposition with the results
  \sa inverse()
  \sa struct LUDecomposition */
struct LUDecomposition &Matrix::LU(Matrix &b)
{
	if (n!=m)
		throw LinAlgException("Not a square matrix");
	if (&b&&(b.m!=m||b.n!=1))
		throw LinAlgException("Incompatible dimensions for Matrix b");
	bool solve=false,singular=false;
	int largestValue;
	double c;
	//vector<Matrix> *matrices=new vector<Matrix>;
	LUDecomposition *LU=new LUDecomposition;
	Matrix temp=*this,L(n,n),U(n,n),y(n,1);
	if (&b)
		solve=true;
	U.identity();
	detFactor=1;
	/* travel down the main diagonal and populate L & U */
	for (unsigned int i=0;i<m;i++)
	{
		/* there exists a 0 in the main diagonal */
		/* so we have to swap rows */
		if (fabs(temp.matrix[i][i])<DBL_EPSILON)
		{
			largestValue=0;
			for (unsigned int j=0;j<n;j++)
			{
				if (fabs(temp.matrix[j][i])>fabs(temp.matrix[largestValue][i])&&j!=i)
					largestValue=j;
			}
			detFactor*=-1;
			temp.swapRow(i,largestValue);
			if (solve)
				b.swapRow(i,largestValue);
		}
		/* populate L & U */
		L[i][i]=temp.matrix[i][i];
		for (unsigned int j=i+1;j<n;j++)
			L[j][i]=temp.matrix[j][i];
		temp.pivot(i,i);
		for (unsigned int j=i+1;j<m;j++)
			U[i][j]=temp.matrix[i][j];
	}
	if (solve)
	{
		/* check if the matrix is singular */
		for (unsigned int i=0;i<m;i++)
		{
			for (unsigned int j=0;j<n;j++)
			{
				if (fabs(temp.matrix[i][j])>DBL_EPSILON)
					break;
				else if (j==(n-1))
					singular=true;
			}
			if (singular)
				break;
		}
		if (!singular)
		{
			/* solve y by forward subsitution */
			for (unsigned int i=0;i<m;i++)
			{
				c=0.0;
				for (unsigned int j=0;j<i;j++)
					c+=L.matrix[i][j]*y[j][0];
				y.matrix[i][0]=(b.matrix[i][0]-c)/L.matrix[i][i];
			}
			/* solve x by back substitution */
			for (unsigned int i=m-1;i>=0&&i<m;i--)
			{
				c=0.0;
				for (unsigned int j=i+1;j>=0&&j<m;j++)
					c+=U.matrix[i][j]*b[j][0];
				b.matrix[i][0]=y.matrix[i][0]-c;
			}
		}
		else
			std::cout<<"Matrix is singular. Solution does not exist."<<std::endl;
	}
	LU->A=temp;
	LU->L=L;
	LU->U=U;
	LU->exists=false;
	LU->solved=false;
	if (solve)
	{
		LU->solved=true;
		if (!singular)
			LU->exists=true;
	}
	/* std::cout<<"y: "<<std::endl;
	std::cout<<y<<std::endl;
	std::cout<<"b: "<<std::endl;
	std::cout<<b<<std::endl;
	std::cout<<"L: "<<std::endl;
	std::cout<<L<<std::endl;
	std::cout<<"U: "<<std::endl;
	std::cout<<U<<std::endl; */
	return *LU;
}

//! Pivot Around An Element
/*! Pivots around \f$M_{ab}\f$ ``in place'' and will row reduce if rowReduce is true.
  \param a the row to pivot on
  \param b the column to pivot on
  \param rowReduce if true, row reductions will be performed in addition to pivot operations (default); if false, it will only pivot
  \throw LinAlgException if dimensions are out of bounds
  \sa inverse()
  \sa LU()
  \sa rref() */
void Matrix::pivot(unsigned int a,unsigned int b,bool rowReduce)
{
	if (a>=m||b>=n)
		throw LinAlgException("Dimensions out of bounds");
	double pivotElement=matrix[a][b];
	if (fabs(pivotElement)<DBL_EPSILON)
		throw LinAlgException("Divide by zero");
	for (unsigned int i=0;i<n;i++)
		matrix[a][i]/=pivotElement;
	for (unsigned int i=0;i<m;i++)
	{
		if (rowReduce&&i!=a&&fabs(matrix[i][b])>DBL_EPSILON)
		{
			double factor=matrix[i][b];
			for (unsigned int j=0;j<n;j++)
				matrix[i][j]=matrix[i][j]-factor*matrix[a][j];
		}
	}
}

//! Row Reduced Echelon Form
/*! Transforms a Matrix into row reduced echelon ``in place'' form using Gauss Jordan Elimination using pivot().
  \deprecated This function is deprecated in favor of LU() and inverse()
  \sa inverse()
  \sa LU()
  \sa pivot() */
void Matrix::rref()
{
	double num;
	for (unsigned int i=0;i<m;i++)
	{
		unsigned int col=n;
		for (unsigned int j=0;j<n;j++)
		{
			num=matrix[i][j];
			if (fabs(num)>DBL_EPSILON)
			{
				col=j;
				break;
			}
		}
		if (col!=n)
			pivot(i,col);
	}
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			if (fabs(matrix[i][j])<DBL_EPSILON)
				matrix[i][j]=0.0;
}

//! OpenGL glGetDoublev() Compatible Mutator
/*! This function works like load() except it will not attempt to resize the Matrix.
  \param values array containg the data to load
  \param colOrder if true, \a values is in column major order (default); if false, \a values is assumed to be in row major order
  \sa load() */
void Matrix::set(double *values,bool colOrder)
{
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
		{
			if (colOrder)
				matrix[i][j]=values[n*j+i];
			else
				matrix[i][j]=values[n*i+j];
		}
}

//! Two Dimensional Array Mutator
/*! This function loads data from \a values into the Matrix. \a values is assumed to be in row major order.
  \param values the data to load
  \sa load() */
void Matrix::set(double **values)
{
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			matrix[i][j]=values[i][j];
}

//! std::vector Mutator
/*! This function loads data from \a values into the Matrix. \a values is assumed to be in row major order.
  \param values the data to load
  \sa load() */
void Matrix::set(std::vector< std::vector<double> > &values)
{
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			matrix[i][j]=values[i][j];
}

//! Standard Mutator
/*! This function will assign the value \a v to \f$M_{ab}\f$.
  \param a the row to access
  \param b the column to access
  \param v the data to load */
void Matrix::set(unsigned int a,unsigned int b,double v)
{
	matrix[a][b]=v;
}

//! Swap Columns
/*! This function will swap columns \a a & \a b.
  \param a the first column
  \param b the second column */
void Matrix::swapCol(unsigned int a,unsigned int b)
{
	if (a>=n||b>=n)
		throw LinAlgException("Column out of bounds");
	for (unsigned int i=0;i<m;i++)
	{
		double temp=matrix[i][a];
		matrix[i][a]=matrix[i][b];
		matrix[i][b]=temp;
	}
}

//! Swap Rows
/*! This function will swap rows \a a & \a b.
  \param a the first row
  \param b the second row */
void Matrix::swapRow(unsigned int a,unsigned int b)
{
	if (a>=m||b>=m)
		throw LinAlgException("Row out of bounds");
	for (unsigned int i=0;i<n;i++)
	{
		double temp=matrix[a][i];
		matrix[a][i]=matrix[b][i];
		matrix[b][i]=temp;
	}
}

//! Transpose
/*! This function finds the transpose of the Matrix.
  \return the transposed Matrix */
Matrix &Matrix::transpose()
{
	Matrix *T=new Matrix(n,m);
	for (unsigned int i=0;i<m;i++)
		for (unsigned int j=0;j<n;j++)
			T->matrix[j][i]=matrix[i][j];
	return *T;
}

//! OpenGL glLoadMatrix() Compatible Accessor
/*! This function puts all the values in the Matrix into a one dimensional array.
  \param colOrder the array will be populated in column major order if true (default); if false, it will be populated using row major order
  \return the array of values */
double *Matrix::values(bool colOrder)
{
	double *values=new double[m*n];
	if (colOrder)
		for (unsigned int i=0;i<n;i++)
			for (unsigned int j=0;j<m;j++)
				values[i*m+j]=matrix[j][i];
	else
		for (unsigned int i=0;i<m;i++)
			for (unsigned int j=0;j<n;j++)
				values[i*n+j]=matrix[i][j];
	return values;
}
