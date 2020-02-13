#ifndef SMOOTHSPARSEVECTOR_H
#define SMOOTHSPARSEVECTOR_H
/***********************************************************//**
* @file 	sparse.h
*
* @brief 	稀疏向量类模板
*
* @details 	稀疏向量类的实现文件
*
* @version	1.0
* @author 	段博文
* @date		2012-04-17 周二
*
***************************************************************/
#include <map>
#include <limits.h>

/**
* @brief 	稀疏向量
*
* @details	通过仅保存与默认值不同的元素,来减少存储空间的占用
*/
template <class T>
class Smooth_Sparse_vector
{
public:
	/**
	* @brief 构造函数
	*
	* @details 不带参数的构造函数
	*/
    Smooth_Sparse_vector();
	/**
	* @brief 			构造函数
	*
	* @details 			带有参数的构造函数
	*
	* @param[in]		def_val 指定元素的默认值
	* @param[in]		size	指定该结构的最大元素数目,注意该数目仅仅是一个标志,便于传递参数,
	*							并不限制实际的元素数目,但是当访问元素时指定的下标超过该值时会
	*							抛出异常,参见 operator[]()
	* @return
	*
	* @note
	* @par 示例:
	* @code
    *					Smooth_Sparse_vector<double> A(0.0,15);
	*					A[4] = 1.0;		// 正确
	*					A[20] = 10.0;	// 会抛出异常,但该值仍然会写入结构中
	* @endcode
    * @see				Smooth_Sparse_vector<T>::operator[]()
	*/
    Smooth_Sparse_vector(const T& def_val, unsigned int size = UINT_MAX);
	/**
	* @brief 			访问元素
	*
	* @details 			访问结构中元素,当下标超过结构的 size() 时,将抛出异常,但数据仍将正常写入或读取
	*
	* @param[in]		i 下标
	* @return 			T&	下标所指示的元素的引用
	* @throws			i 当i超过 size() 时抛出异常
	*
	* @note
	* @par 示例:
	* @code
    *					Smooth_Sparse_vector<double> A;
	*					A.set_default(0.0);
	*					A[4] = 3.0;
	* @endcode
	* @see
	*/
	T& operator[](unsigned int i);
	/**
	* @brief 			判断两端值是否相等
	*
	* @details
	*
    * @param[in]		v 欲与之比较的 Smooth_Sparse_vector<T> 类型数据
	* @return 			bool
	*					- true 两值不等
	*					- false 两值所包含数据,默认值,上次数据,上次下标都相等
	*
	* @note
	* @par 示例:
	* @code
	*
	* @endcode
	* @see
	*/
    bool operator!=(const Smooth_Sparse_vector<T>& v);
	/**
	* @brief 			设置元素默认值
	*
	* @details 			设置所包含元素的默认值,当访问并未保存的数据时,将返回此默认值
	*
	* @param[in]		def_val 要作为默认值的数据
	* @return 			bool	设置是否成功
	*					- true 设置默认值成功
	*					- false 设置默认值失败,这是由于为了避免数据混乱,默认值只允许设置一次.
	*							如果您一定要强制修改默认值并承担由此引发的风险,请使用
    *							Smooth_Sparse_vector<T>::I_desperately_want_to_change_default_value_to
	*
	* @note
	* @par 示例:
	* @code
	*
	* @endcode
    * @see				Smooth_Sparse_vector<T>::I_desperately_want_to_change_default_value_to
	*/
	bool set_default(const T& def_val);
	/**
	* @brief 			赋值
	*
	* @details 			赋值完成后,二者的所有元素及默认值等完全相同
	*
	* @param[in]		c
    * @return 			Smooth_Sparse_vector<T>&
	*
	* @note
	* @par 示例:
	* @code
	*
	* @endcode
	* @see
	*/
    Smooth_Sparse_vector<T>& operator=(const Smooth_Sparse_vector<T>& c);
	/**
	* @brief 			获取大小
	*
	* @details 			获取结构的名义大小,即最多所<b>应当</b>包含的元素个数,注意这
	*					并不是一个强制的限制,元素数目可能超过此值
	*
	* @return 			unsigned int	结构大小
	*
	* @note
	* @par 示例:
	* @code
	*
	* @endcode
	* @see
	*/
	unsigned int size() {return isize;}
	/**
	* @brief 			重新调整大小
	*
	* @details
	*
	* @param[in]		n 元素数目上限
	* @return 			void
	*
	* @note 			关于大小的说明见size()
	* @par 示例:
	* @code
	*
	* @endcode
	* @see				size()
	*/
	void resize(unsigned int n) {isize = n;}
	/**
	* @brief 			清空数据
	*
	* @details 			删除所有元素,重置默认值设置状态,但并不重置默认值
	*
	* @return 			void
	*
	* @note
	* @par 示例:
	* @code
	*
	* @endcode
	* @see
	*/
	void clear();
	/**
	* @brief 			强制更新默认值
	*
	* @details
	*
	* @param[in]		def_val
	* @return 			void
	*
	* @note 			该操作可能引发不可预料的错误,由于在写入元素值时,与默认值相同的值
	*					并不进行保存,当您强制更改默认值后,这些值将会丢失,当您重新访问这些
	*					元素时,它们将变为新的默认值
	* @par 示例:
	* @code
    *					Smooth_Sparse_vector<int> A;
	*					A.set_default(0);		// 默认值为0
	*					A[9] = 0;
	*					cout << A[9] <<endl;	// 将输出 0
	*					A.I_desperately_want_to_change_default_value_to(1);
	*					cout << A[9] <<endl;	// 将输出 1
	* @endcode
	* @see
	*/
	void I_desperately_want_to_change_default_value_to(const T& def_val);
protected:
    /**
    * @brief 			保存之前被赋值的元素
    *
    * @details 			如果之前对一个没有保存在结构中的下标进行了赋值操作,
	*					并且所赋的值不等于默认值,则执行该函数将保存该值,否则
	*					不执行任何操作
    *
    * @return 			void
    *
    * @note
    * @par 示例:
    * @code
    *
    * @endcode
    * @see
    */
    void write_last();
private:
	std::map<unsigned int,T> data;/**< 元素 */
	T default_val;/**< 默认值 */
	unsigned last_i;/**< 上次访问的下标 */
	T last_val;/**< 上次访问的无记录的值 */
	unsigned int isize;/**< 数目限制 */
	bool default_set;/**< 标志,指示默认值是否已经设置 */
};

template <class T>
void Smooth_Sparse_vector<T>::write_last()
{
	if ((last_val != default_val))
	{
		data.insert(std::pair<unsigned int,T>(last_i,last_val));
		last_val = default_val;
	}
}

template <class T>
void Smooth_Sparse_vector<T>::I_desperately_want_to_change_default_value_to( const T& def_val )
{
	write_last();
	last_val = default_val = def_val;
	last_i = 0;
	default_set = true;
}

template <class T>
void Smooth_Sparse_vector<T>::clear()
{
	data.clear();
	default_set = false;
	isize = 0;
	last_i = 0;
}

template <class T>
Smooth_Sparse_vector<T>& Smooth_Sparse_vector<T>::operator=( const Smooth_Sparse_vector<T>& c )
{
	data = c.data;
	default_val = c.default_val;
	last_i = c.last_i;
	last_val = c.last_val;
	isize = c.isize;
	return *this;
}

template <class T>
bool Smooth_Sparse_vector<T>::set_default( const T& def_val )
{
	if(default_set) return false;
	write_last();
	last_val = default_val = def_val;
	last_i = 0;
	default_set = true;
	return true;
}

template <class T>
T& Smooth_Sparse_vector<T>::operator[]( unsigned int i )
{
	//if(i >= isize) throw(i);
	write_last();
	last_i = i;
	if(0 == data.count(i)) return last_val;
	else if(data[i] != default_val) return data[i];
	else {data.erase(i);return last_val;}
}

template <class T>
bool Smooth_Sparse_vector<T>::operator!=( const Smooth_Sparse_vector<T>& v )
{
	if((data != v.data)||(last_val != v.last_val)||(last_i != v.last_i)) return true;
	else return false;
}

template <class T>
Smooth_Sparse_vector<T>::Smooth_Sparse_vector() :default_set(false)
{
	last_val = default_val = T();
	last_i = 0;
}

template <class T>
Smooth_Sparse_vector<T>::Smooth_Sparse_vector( const T& def_val, unsigned int size /*= 0*/ ) :default_set(true)
{
	last_val = default_val = def_val;
	last_i = 0;
	isize = size;
}
#endif
