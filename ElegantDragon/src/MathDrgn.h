#pragma once
#include <vector>
#include <math.h>
#include <iostream>

#include "projectDefines.h"

#define MAX_MAT_SIZE 1000
//#include <stream>



/// <summary>
/// Simple n dimentional vector class (DEPRECATED)
/// </summary>
/// <typeparam name="T">Quantity type</typeparam>
template<typename T>
class MathVec
{
private:
    const unsigned int size;
    T* content;
public:
    MathVec(const unsigned int sizeIn);
    MathVec(const unsigned int sizeIn, const T* contentIn);
    MathVec(const MathVec& src);
    ~MathVec();

    unsigned int getSize() const;

    T& operator[](const unsigned int pos) const;
    MathVec& operator=(const MathVec& rhs);
    
    MathVec& rotate(const T& deg);
    static void rotate2(const float& deg, T& x, T& y);
    static void rotate2(const float& rad, T& x, T& y, const T& x0, const T& y0);

    /*
    * Mirrors along... umm implement later
    */
    //static void mirrorScale2(const float& deg, T& , T& );
};

template<typename T>
inline MathVec<T>::MathVec(const unsigned int sizeIn) : size(sizeIn)
{
    content = new T[sizeIn];
    for (int i = 0; i < sizeIn; ++i)
    {
        content[i] = 0;
    }
}

template<typename T>
inline MathVec<T>::MathVec(const unsigned int sizeIn, const T* contentIn) : size(sizeIn)
{
    content = new T[sizeIn];
    for (int i = 0; i < sizeIn; ++i)
    {
        content[i] = contentIn[i];
    }
}

template<typename T>
MathVec<T>::MathVec(const MathVec& src) : size(src.getSize())
{
    content = new T[src.getSize()];
    for (unsigned int i = 0; i < src.getSize(); ++i)
    {
        content[i] = src[i];
    }
}

template<typename T>
inline MathVec<T>::~MathVec()
{
    delete[] content;
}

template<typename T>
unsigned int MathVec<T>::getSize() const
{
    return size;
}

template<typename T>
T& MathVec<T>::operator[](const unsigned int pos) const
{
    return content[pos];
}

template<typename T>
MathVec<T>& MathVec<T>::operator=(const MathVec& rhs)
{
    //is there some way of using the copy constructor??

    //temporarily make it so you can't change size (change if needed?)
    PROJ_ASSERT_W_MSG(rhs.size == size, "Cannot assign vector of differing sizes");

    delete[] content;
    content = new T[size];
    for (unsigned int i = 0; i < rhs.size; ++i) content[i] = rhs[i];
    return *this;
}

template<typename T>
MathVec<T>& MathVec<T>::rotate(const T& rad)
{
    T origX = content[0];
    T origY = content[1];

    content[0] = origX * std::cos(rad) - origY * std::sin(rad);
    content[1] = origX * std::sin(rad) + origY * std::cos(rad);

    // TODO: insert return statement here
    return *this;
}

template<typename T>
void MathVec<T>::rotate2(const float& rad, T& x, T& y)
{
    T origX = x;
    T origY = y;

    x = origX * std::cos(rad) - origY * std::sin(rad);
    y = origX * std::sin(rad) + origY * std::cos(rad);
}

template<typename T>
void MathVec<T>::rotate2(const float& rad, T& x, T& y, const T& x0, const T& y0)
{
    T origX = x;
    T origY = y;

    x = (origX - x0) * std::cos(rad) - (origY - y0) * std::sin(rad) + x0;
    y = (origX - x0) * std::sin(rad) + (origY - y0) * std::cos(rad) + y0;
}



//Creates a Row major Matrix for math using a single contiguous piece of memory 
//TODO: if i use this for ML or some thing that requires big matrices, try implementing the Strassen algorithm or something similar for matrix mult
template<typename T>
class MathMatRMaj
{
private:
    //maybe switch to short in the future?
    unsigned int numCol;
    unsigned int numRow;
    unsigned int size; // should always be numCol*numRow (here for ez access)
    T* content;
public:
    MathMatRMaj();
    MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn);
    MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, const bool identity);

    MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, T* contentIn);

    MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, T** contentIn);

    MathMatRMaj(const MathMatRMaj<T>& src);
    ~MathMatRMaj();

    inline const unsigned int& getSize() const { return size; };
    inline const unsigned int& getNumCol() const { return numCol; };
    inline const unsigned int& getNumRow() const { return numRow; };
    inline T* getContent() { return content; };//produces the array of the main data content (same as (*this)[0]) 
    inline const T& viewContent(const unsigned int rolIdx, const unsigned int colIdx) const { return content[rolIdx * numCol + colIdx]; };

    T* operator[](const unsigned int pos); //produces an array offset by numCol*pos (pos is the row being selected) (may be used as Matrix[<row#>][<col#>])

    friend inline std::ostream& operator<<(std::ostream& os, const MathMatRMaj<float>& Mat);


    friend inline MathMatRMaj<float> operator*(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs);
    friend inline MathMatRMaj<float> operator*(const float& lhs, const MathMatRMaj<float>& rhs);
    friend inline MathMatRMaj<float> operator*(const MathMatRMaj<float>& lhs, const float& rhs);
    MathMatRMaj<T>& multDirect(const MathMatRMaj<T>& lhs, const MathMatRMaj<T>& rhs);
    MathMatRMaj<T>& multDirect(const float& lhs, const MathMatRMaj<T>& rhs);
    MathMatRMaj<T>& multDirect(const MathMatRMaj<T>& lhs, const float& rhs);

    friend inline MathMatRMaj<float> operator+(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs);
    friend inline MathMatRMaj<float> operator-(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs);

    MathMatRMaj<T>& operator=(const MathMatRMaj<T>& rhs);
    MathMatRMaj<T>& operator*=(const MathMatRMaj<T>& rhs);
    MathMatRMaj<T>& multAssignReverse(const MathMatRMaj<T>& lhs);

    //Helpful functions:
    void setToIdentity();
    void setToZero();

    /*MathMatRMaj& rotate(const T& deg);
    static void rotate2(const float& deg, T& x, T& y);
    static void rotate2(const float& rad, T& x, T& y, const T& x0, const T& y0);*/

    /*
    * Mirrors along... umm implement later
    */
    //static void mirrorScale2(const float& deg, T& , T& );
};

/// <summary>
/// Defaults to 4x4 matrix
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj() :
    numCol(4), numRow(4), size(16)
{
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = 0;
    }
}

/// <summary>
/// Initializes with all 0's
/// </summary>
/// <param name="numColIn">number of columns</param>
/// <param name="numRowIn">number of rows</param>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn) :
    numCol(numColIn), numRow(numRowIn), size(numColIn * numRowIn)
{
    PROJ_ASSERT_W_MSG(size != 0, "Size 0 matrix is not allow");
    PROJ_ASSERT_W_MSG(size < MAX_MAT_SIZE, "Maximum Matrix size reached!");
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = 0;
    }
}

/// <summary>
/// Constructs an identity matrix
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="numColIn">number of columns</param>
/// <param name="numRowIn">number of rows</param>
/// <param name="identity">Unused</param>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, const bool identity) :
    numCol(numColIn), numRow(numRowIn), size(numColIn* numRowIn)
{
    PROJ_ASSERT_W_MSG(size != 0, "Size 0 matrix is not allow");
    PROJ_ASSERT_W_MSG(size < MAX_MAT_SIZE, "Maximum Matrix size reached!");
    content = new T[size];
    setToIdentity();
}


/// <summary>
/// Initializes with a 1d array
/// </summary>
/// <param name="numColIn">number of columns</param>
/// <param name="numRowIn">number of rows</param>
/// <param name="contentIn">1d array used to initialize (must be at least the size of the Matrix being created)</param>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, T* contentIn) :
    numCol(numColIn), numRow(numRowIn), size(numColIn* numRowIn)
{
    PROJ_ASSERT_W_MSG(size != 0, "Size 0 matrix is not allow");
    PROJ_ASSERT_W_MSG(size < MAX_MAT_SIZE, "Maximum Matrix size reached!");
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = contentIn[idx];
    }
}

/// <summary>
/// Initialize with 2d array
/// </summary>
/// <param name="numColIn">number of columns</param>
/// <param name="numRowIn">number of rows</param>
/// <param name="contentIn">2d array used to initialize (must be at least the size of the Matrix being created)</param>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj(const unsigned int numColIn, const unsigned int numRowIn, T** contentIn) :
    numCol(numColIn), numRow(numRowIn), size(numColIn* numRowIn)
{
    PROJ_ASSERT_W_MSG(size != 0, "Size 0 matrix is not allow");
    PROJ_ASSERT_W_MSG(size < MAX_MAT_SIZE, "Maximum Matrix size reached!");
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = contentIn[static_cast<unsigned int>(idx / numColIn)][idx % numColIn];
    }
}

/// <summary>
/// Copy Constructor
/// </summary>
/// <param name="src"></param>
template<typename T>
inline MathMatRMaj<T>::MathMatRMaj(const MathMatRMaj<T>& src) :
    numCol(src.numCol), numRow(src.numRow), size(src.size)
{
    //std::cout << "COPY CONSTRUCTOR" << std::endl;
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = src.content[idx];
    }
}


template<typename T>
inline MathMatRMaj<T>::~MathMatRMaj()
{
    delete[] content;
}

template<typename T>
inline T* MathMatRMaj<T>::operator[](const unsigned int pos)
{
    //add out of bounds check later if need be, but the system should handle it
    return &content[pos * numCol];
}


/// <summary>
/// Overloaded stream output (MathMatRMaj friend)
/// </summary>
/// <param name="os">Output stream</param>
/// <param name="Mat">Matrix</param>
/// <returns>Output stream</returns>
inline std::ostream& operator<<(std::ostream& os, const MathMatRMaj<float>& Mat)
{
    for (unsigned int idx = 0; idx < Mat.size; ++idx)
    {
        os << Mat.content[idx] << "	";
        if (idx % Mat.numCol == Mat.numCol - 1)
            os << std::endl;
    }
    return os;
}

/// <summary>
/// Performs Matrix multiplication
/// </summary>
/// <param name="lhs">matrix</param>
/// <param name="rhs">matrix</param>
/// <returns></returns>
inline MathMatRMaj<float> operator*(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs)
{
    PROJ_ASSERT_W_MSG(lhs.numCol == rhs.numRow, "Matrix Multiplication Column-Row mismatch. lhs col# must match rhs row#");
    MathMatRMaj<float> ResMat(rhs.numCol, lhs.numRow);
    for (unsigned int row = 0; row < ResMat.numRow; ++row)
    {
        for (unsigned int col = 0; col < ResMat.numCol; ++col)
        {
            for (unsigned int idxContGrab = 0; idxContGrab < lhs.numCol; ++idxContGrab)
            {
                ResMat.content[row * ResMat.numCol + col] += lhs.content[row * lhs.numCol + idxContGrab] * rhs.content[idxContGrab * rhs.numCol + col];
            }
        }
    }
    return ResMat;
}

/// <summary>
/// Performs Matrix multiplication
/// </summary>
/// <param name="lhs">scaler</param>
/// <param name="rhs">matrix</param>
/// <returns></returns>
inline MathMatRMaj<float> operator*(const float& lhs, const MathMatRMaj<float>& rhs)
{
    MathMatRMaj<float> ResMat(rhs);
    for (unsigned int contentIdx = 0; contentIdx < rhs.size; ++contentIdx)
    {
        ResMat.content[contentIdx] *= lhs;
    }
    return ResMat;
}

/// <summary>
/// Performs Matrix multiplication
/// </summary>
/// <param name="lhs">matrix</param>
/// <param name="rhs">scaler</param>
/// <returns></returns>
inline MathMatRMaj<float> operator*(const MathMatRMaj<float>& lhs, const float& rhs)
{
    MathMatRMaj<float> ResMat(lhs);
    for (unsigned int contentIdx = 0; contentIdx < lhs.size; ++contentIdx)
    {
        ResMat.content[contentIdx] *= rhs;
    }
    return ResMat;
}

/// <summary>
/// Performs Matrix addition
/// </summary>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
inline MathMatRMaj<float> operator+(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs)
{
    PROJ_ASSERT_W_MSG(lhs.numCol == rhs.numCol, "Matrix Addition Column-Column mismatch. lhs col# must match rhs col#");
    PROJ_ASSERT_W_MSG(lhs.numRow == rhs.numRow, "Matrix Addition Row-Row mismatch. lhs row# must match rhs row#");
    MathMatRMaj<float> ResMat(lhs);
    for (unsigned int contentIdx = 0; contentIdx < lhs.size; ++contentIdx)
    {
        ResMat.content[contentIdx] += rhs.content[contentIdx];
    }

    return ResMat;
}

/// <summary>
/// Performs Matrix subtraction
/// </summary>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
inline MathMatRMaj<float> operator-(const MathMatRMaj<float>& lhs, const MathMatRMaj<float>& rhs)
{
    PROJ_ASSERT_W_MSG(lhs.numCol == rhs.numCol, "Matrix Subtraction Column-Column mismatch. lhs col# must match rhs col#");
    PROJ_ASSERT_W_MSG(lhs.numRow == rhs.numRow, "Matrix Subtraction Row-Row mismatch. lhs row# must match rhs row#");
    MathMatRMaj<float> ResMat(lhs);
    for (unsigned int contentIdx = 0; contentIdx < lhs.size; ++contentIdx)
    {
        ResMat.content[contentIdx] -= rhs.content[contentIdx];
    }

    return ResMat;
}

/// <summary>
/// Slightly faster Matrix multiplication by removing the need for copy constructor in many cases.
/// The Parameter may not be itself
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="ResMat"></param>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::multDirect(const MathMatRMaj<T>& lhs, const MathMatRMaj<T>& rhs)
{
    PROJ_ASSERT_W_MSG(lhs.numCol == rhs.numRow, "Matrix Multiplication Column-Row mismatch. lhs col# must match rhs row#");
    /*PROJ_ASSERT_W_MSG(&lhs != this, "The object itself may not be passed in");
    PROJ_ASSERT_W_MSG(&rhs != this, "The object itself may not be passed in");*/
    //MathMatRMaj<float> ResMat(rhs.numCol, lhs.numRow);
    numCol = rhs.numCol;
    numRow = lhs.numRow;
    delete[] content;
    content = new T[numRow * numCol];

    for (unsigned int row = 0; row < numRow; ++row)
    {
        for (unsigned int col = 0; col < numCol; ++col)
        {
            content[row * numCol + col] = 0;
            for (unsigned int idxContGrab = 0; idxContGrab < lhs.numCol; ++idxContGrab)
            {
                content[row * numCol + col] += lhs.content[row * lhs.numCol + idxContGrab] * rhs.content[idxContGrab * rhs.numCol + col];
            }
        }
    }
    return *this;
}

template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::multDirect(const float& lhs, const MathMatRMaj<T>& rhs)
{
    delete[] content;
    numCol = rhs.numRow;
    numRow = rhs.numRow;
    size = rhs.size;
    content = new T[numRow * numCol];
    for (unsigned int contentIdx = 0; contentIdx < rhs.size; ++contentIdx)
    {
        content[contentIdx] = lhs * rhs.content[contentIdx];
    }
    return *this;
}

template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::multDirect(const MathMatRMaj<T>& lhs, const float& rhs)
{
    delete[] content;
    numCol = lhs.numRow;
    numRow = lhs.numRow;
    size = lhs.size;
    content = new T[numRow * numCol];
    for (unsigned int contentIdx = 0; contentIdx < lhs.size; ++contentIdx)
    {
        content[contentIdx] = rhs * lhs.content[contentIdx];
    }
    return *this;
}

template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::operator=(const MathMatRMaj<T>& rhs)
{
    //std::cout << "ASSIGNMENT OPPERATOR" << std::endl;
    delete[] content;
    size = rhs.size;
    numCol = rhs.numCol;
    numRow = rhs.numRow;
    content = new T[size];
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = rhs.content[idx];
    }

    return *this;
}

template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::operator*=(const MathMatRMaj<T>& rhs)
{
    PROJ_ASSERT_W_MSG(numCol == rhs.numRow, "Matrix Multiplication Column-Row mismatch. object col# must match rhs row#");
    PROJ_ASSERT_W_MSG(&rhs != this, "The object itself may not be passed in");
    MathMatRMaj<T> Orig(*this);
    numCol = rhs.numCol;
    delete[] content;
    content = new T[numRow * numCol];

    for (unsigned int row = 0; row < numRow; ++row)
    {
        for (unsigned int col = 0; col < numCol; ++col)
        {
            content[row * numCol + col] = 0;
            for (unsigned int idxContGrab = 0; idxContGrab < Orig.numCol; ++idxContGrab)
            {
                content[row * numCol + col] += Orig.content[row * Orig.numCol + idxContGrab] * rhs.content[idxContGrab * rhs.numCol + col];
            }
        }
    }
    return *this;
}

template<typename T>
inline MathMatRMaj<T>& MathMatRMaj<T>::multAssignReverse(const MathMatRMaj<T>& lhs)
{
    PROJ_ASSERT_W_MSG(lhs.numCol == numRow, "Matrix Multiplication Column-Row mismatch. object col# must match rhs row#");
    PROJ_ASSERT_W_MSG(&lhs != this, "The object itself may not be passed in");
    MathMatRMaj<T> Orig(*this);
    numRow = lhs.numRow;
    delete[] content;
    content = new T[numRow * numCol];

    for (unsigned int row = 0; row < numRow; ++row)
    {
        for (unsigned int col = 0; col < numCol; ++col)
        {
            content[row * numCol + col] = 0;
            for (unsigned int idxContGrab = 0; idxContGrab < lhs.numCol; ++idxContGrab)
            {
                content[row * numCol + col] += lhs.content[row * lhs.numCol + idxContGrab] * Orig.content[idxContGrab * Orig.numCol + col];
            }
        }
    }
    return *this;
}

template<typename T>
inline void MathMatRMaj<T>::setToIdentity()
{
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = 0;
    }
    for (unsigned int idx = (numCol < numRow ? numCol : numRow); idx != 0; --idx)
    {
        content[numCol*(idx - 1) + (idx - 1)] = 1;
    }
}

template<typename T>
inline void MathMatRMaj<T>::setToZero()
{
    for (unsigned int idx = 0; idx < size; ++idx)
    {
        content[idx] = 0;
    }
}



