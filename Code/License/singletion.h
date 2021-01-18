#pragma once

template <typename _Type>
class XSingletion
{
public:
	static _Type * getInstance();
private:
	static _Type * instance;
};

template <typename _Type>
_Type * XSingletion<_Type>::instance = NULL;

template <typename _Type>
_Type * XSingletion<_Type>::getInstance()
{
	if ( instance == NULL )
	{
		instance = new _Type;
	}
	return instance;
}

template <typename _Type, typename _Para1>
class XSingletion1P
{
public:
	static _Type * getInstance(_Para1 para1);
private:
	static _Type * instance;
};

template <typename _Type, typename _Para1>
_Type * XSingletion1P<_Type, _Para1>::instance = NULL;


template <typename _Type, typename _Para1>
_Type * XSingletion1P<_Type, _Para1>::getInstance(_Para1 para1)
{
	if (instance == NULL)
	{
		instance = new _Type(para1);
	}
	return instance;
}

template <typename _Type, typename _Para1, typename _Para2>
class XSingletion2P
{
public:
	static _Type * getInstance(_Para1 para1, _Para2 para2);
private:
	static _Type * instance;
};

template <typename _Type, typename _Para1, typename _Para2>
_Type * XSingletion2P<_Type, _Para1, _Para2>::instance = NULL;

template <typename _Type, typename _Para1, typename _Para2>
_Type * XSingletion2P<_Type, _Para1, _Para2>::getInstance(_Para1 para1, _Para2 para2)
{
	if (instance == NULL)
	{
		instance = new _Type(para1, para2);
	}
	return instance;
}
