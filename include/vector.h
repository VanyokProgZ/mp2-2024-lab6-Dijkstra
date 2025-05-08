#pragma once
#include <initializer_list>
#include <iostream>
#define _mem_err_check(a) {if(a==nullptr){throw std::exception("Bad alloc!");}}

template<typename T>
class Iterator_ : public std::iterator<std::input_iterator_tag, T>
{
private:
	T* ptr;
public:
	Iterator_(T* ptr) : ptr(ptr) {};
	Iterator_(const Iterator_& iter) : ptr(iter.ptr) {};

	bool operator!=(const Iterator_& it2) const { return ptr != it2.ptr; };
	bool operator==(const Iterator_& it2) const { return ptr == it2.ptr; };
	typename Iterator_::reference operator*() const { return *ptr; };
	Iterator_& operator++() { ptr++; return *this; };
};

template<typename T>
class vector {
private:
	T* pMem;
	size_t sz, cp;
public:
	//<---------------iterators-------------->
	Iterator_<T> begin() { return Iterator_<T>(pMem); }
	Iterator_<T> end() { return Iterator_<T>(pMem+sz); }
	Iterator_<const T> begin()const { return Iterator_(pMem); }
	Iterator_<const T> end()const { return Iterator_(pMem + sz); }
	//<-------------end iterators------------>

	//<-------------constructors------------>
	vector(std::initializer_list<T> list_) {
		sz = list_.size();
		cp = sz+1;
		pMem = new T[cp]();
		_mem_err_check(pMem);
		size_t p = 0;
		for (auto& el : list_) {
			pMem[p++] = el;
		}
	}
	vector() {
		cp = 4;
		sz = 0;
		pMem = new T[cp]();
		_mem_err_check(pMem);
	}
	vector(size_t _sz) {
		sz = _sz;
		cp = sz+1;
		pMem = new T[cp]();
		_mem_err_check(pMem);
	}
	vector(size_t _sz, const T& val_) {
		sz = _sz;
		cp = sz+1;
		pMem = new T[cp];
		_mem_err_check(pMem);
		std::fill(pMem, pMem + sz, val_);
	}
	vector(const vector<T>& v) {
		sz = v.sz;
		cp = v.cp;
		pMem = new T[cp];
		_mem_err_check(pMem);
		std::copy(v.pMem, v.pMem + sz, pMem);
	}
	vector(vector<T>&& v) : pMem(nullptr), sz(0),cp(1) {
		pMem = v.pMem;
		cp = v.cp;
		sz = v.sz;
		v.pMem = nullptr;
		v.cp = v.sz = 0;
	}
	~vector() {
		delete[] pMem;
		cp = sz = 0;
		pMem = nullptr;
	}
	//<------end constructors block--------->
	
	//<------assign.. operators------------->
	vector& operator=(const vector& v) {
		if (&v == this) { return *this; }
		if (sz != v.sz) {
			sz = v.sz;
			cp = v.cp;
			T* p = new T[cp];
			_mem_err_check(p);
			delete[] pMem;
			pMem = p;
		}
		std::copy(v.pMem, v.pMem + sz, pMem);
		return *this;
	}
	vector& operator=(vector&& v) {
		if (this != &v)
		{
			delete[] pMem;
			pMem = v.pMem;
			cp = v.cp;
			sz = v.sz;
			v.pMem = nullptr;
			v.cp = v.sz = 0;
		}
		return *this;
	}
	//<-------assign.. operators end-------->

	//<---------info methods---------------->
	size_t size() { return sz; }
	size_t size() const{ return sz; }
	size_t capacity() { return cp; }
	size_t capacity() const { return cp; }
	//<-------info methods end-------------->


	//<data or size changing/access methods->
		//access-->>
	const T& operator[](size_t ind) const {
		return pMem[ind];
	}
	T& operator[](size_t ind) {
		return pMem[ind];
	}
	const T& at(size_t ind) const {
		if (ind >= sz || ind < 0) throw std::out_of_range("index out of range\n");
		return pMem[ind];
	}
	T& at(size_t ind) {
		if (ind >= sz || ind < 0) throw std::out_of_range("index out of range\n");
		return pMem[ind];
	}
	T& back() { if (!sz)throw std::out_of_range("cant back() from empty vector!"); return pMem[sz - 1]; }
		//end access--<<
		
		//touch data/size-->>
	void push_back(const T& val) {
		if (sz >= cp) {
			T* tmpMem = new T[cp * 2]();
			_mem_err_check(tmpMem);
			std::copy(pMem, pMem + cp, tmpMem);
			delete[] pMem;
			pMem = tmpMem;
			cp *= 2;
		}
		pMem[sz++] = val;
	}
	void pop_back() {
		if (sz == 0)throw std::out_of_range("out of range - cant pop element from empty vector");
		sz--;
	}
	void resize(size_t sz_) {
		size_t mn = 4;
		if (sz_ > mn) mn = sz_;
		if (sz_ > cp) {
			T* tmpMem = new T[mn]();
			_mem_err_check(tmpMem);
			cp = mn;
			std::copy(pMem,pMem+sz,tmpMem);
			delete[] pMem;
			pMem = tmpMem;
			sz = sz_;
		}
		else {
			sz = sz_;
		}
	}
	void clear() { resize(0); }
		//touch data/size end<
	//<data or size changing/access methods end->

	//0)
	friend std::ostream& operator<<(std::ostream& os, vector<T>& v) { for (auto& el : v) os << el << ' '; return os; }//должен быть оператор << у T
	friend std::istream& operator>>(std::istream& is, vector<T>& v) { for (auto& el : v) is >> el; return is; }//вектор должен быть размера вводимых данных
};